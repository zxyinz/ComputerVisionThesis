#include"time.h"
#include"../FileIO/SanFileIO.h"
#include"cSanLogManager.h"
using namespace std;
using namespace San;
using namespace San::FileIOStream;
SANLOG::SANLOG()
	:Type(SLT_LOG), Code(0), strLog(""), strCodeFile(""), CodeLine(0), MemLocation(0), UserData()
{
	this->TimeStamp = ::time(nullptr);
	this->ClockStamp = ::clock();
}
SANLOG::SANLOG(const eSANLOGTYPE Type, const uint32 Code, const SString &strLog, const SString &strCodeFile, const uint32 CodeLine, const SHANDLE MemLocation, const _sstream<uint8> &UserData)
	:Type(Type), Code(Code), strLog(strLog), strCodeFile(strCodeFile), CodeLine(CodeLine), MemLocation(MemLocation), UserData(UserData)
{
	this->TimeStamp = ::time(nullptr);
	this->ClockStamp = ::clock();
}
SANLOG::~SANLOG()
{
}
cSanLogAgent::cSanLogAgent()
{
}
cSanLogAgent::cSanLogAgent(const vector<SANLOGFILITER> &FiliterArray)
	:m_LogFiliterArray(FiliterArray)
{
}
cSanLogAgent::~cSanLogAgent()
{
	this->_ReleaseAgent();
}
cSanConsoleLogAgent::cSanConsoleLogAgent(cSanConsolePort* pConsole)
	:m_pConsole(pConsole)
{
}
cSanConsoleLogAgent::cSanConsoleLogAgent(cSanConsolePort* pConsole, const vector<SANLOGFILITER> &FiliterArray)
	:m_pConsole(pConsole), cSanLogAgent(FiliterArray)
{
}
cSanConsoleLogAgent::~cSanConsoleLogAgent()
{
	this->_ReleaseAgent();
}
void cSanConsoleLogAgent::_ReleaseAgent()
{
	this->m_pConsole = nullptr;
}
bool cSanConsoleLogAgent::_LogAcceptable() const
{
	return true;
}
bool cSanConsoleLogAgent::iPushLog(const SANLOG &Log)
{
	const SString LogTypeString[4] = { "***LOG***", "***ERROR***", "***WARNING***", "***MESSAGE***" };

	SString strOutputString;

	sachar Buffer[16];

	const uint32 Hour = Log.ClockStamp / (1000 * 60 * 60);
	const uint32 Min = (Log.ClockStamp / (1000 * 60)) % 60;
	const uint32 Sec = (Log.ClockStamp / 1000) % 60;
	const uint32 MS = Log.ClockStamp % 1000;

	sprintf(Buffer, "[ %02i:%02i:%02i:%03i ] \0", Hour, Min, Sec, MS);

	//Write clock stamp
	strOutputString << Buffer;

	//Write log type, code and log information
	strOutputString << LogTypeString[Log.Type] << ": Code - " << (uint64)Log.Code << " Info - " << Log.strLog << "\r\n";

	//Write debug information
	if (!Log.strCodeFile.empty())
	{
		strOutputString << "\t\tDebug code file       : " << Log.strCodeFile << "\r\n";
		strOutputString << "\t\tDebug code line       : " << (uint64) Log.CodeLine << "\r\n";
		strOutputString << "\t\tDebug memory location : 0x" << ::gloTToA(::gloIToS((uint64) Log.MemLocation, 16)) << "\r\n";
	}

	//Write to console

	return true;
}
cSanFileLogAgent::cSanFileLogAgent(const SString &strFilePath)
{
	this->m_OutputFile.open(strFilePath.c_str(), ios_base::out);
}
cSanFileLogAgent::cSanFileLogAgent(const SString &strFilePath, const vector<SANLOGFILITER> &FiliterArray)
	:cSanLogAgent(FiliterArray)
{
	this->m_OutputFile.open(strFilePath.c_str(), ios_base::out);
}
cSanFileLogAgent::~cSanFileLogAgent()
{
	this->_ReleaseAgent();
}
void cSanFileLogAgent::_ReleaseAgent()
{
	this->m_OutputFile.close();
}
bool cSanFileLogAgent::_LogAcceptable() const
{
	return true;
}
bool cSanFileLogAgent::iPushLog(const SANLOG &Log)
{
	const SString LogTypeString[4] = { "***LOG***", "***ERROR***", "***WARNING***", "***MESSAGE***" };

	sachar Buffer[16];

	const uint32 Hour = Log.ClockStamp / (1000 * 60 * 60);
	const uint32 Min = (Log.ClockStamp / (1000 * 60)) % 60;
	const uint32 Sec = (Log.ClockStamp / 1000) % 60;
	const uint32 MS = Log.ClockStamp % 1000;

	sprintf(Buffer, "[ %02i:%02i:%02i:%03i ] \0", Hour, Min, Sec, MS);

	//Write time stramp and clock stamp
	this->m_OutputFile << "[ " << ctime(&Log.TimeStamp) << "] " << Buffer;

	//Write log type, code and log information
	this->m_OutputFile << LogTypeString[Log.Type] << ": Code - " << Log.Code << " Info - " << Log.strLog << "\r\n";

	//Write debug information
	if (!Log.strCodeFile.empty())
	{
		this->m_OutputFile << "\t\tDebug code file       : " << Log.strCodeFile << "\r\n";
		this->m_OutputFile << "\t\tDebug code line       : " << Log.CodeLine << "\r\n";
		this->m_OutputFile << "\t\tDebug memory location : 0x" << (void*) Log.MemLocation << "\r\n";
	}

	return true;
}
cSanLogManager::cSanLogManager()
{
}
cSanLogManager::~cSanLogManager()
{
	this->iReleaseLogManager();
}
uint32 cSanLogManager::_GenerateResponseCode(const eSANLOGTYPE Type, const uint32 Code) const
{
	return static_cast<uint32>(Type) << 12 | (Code & 0x0fff);
}
uint32 cSanLogManager::_RegisteLogAgent(cSanLogAgent* pAgent)
{
	if (pAgent == nullptr){ return 0; }

	this->m_LogAgentList.push_back(pAgent);

	return this->m_LogAgentList.size();
}
void cSanLogManager::_ReleaseLogAgent(const uint32 AgentID)
{
	if (AgentID > this->m_LogAgentList.size()){ return; }

	list<cSanLogAgent*>::iterator pAgent = this->m_LogAgentList.begin();
	for (int32 seek = 0; seek < AgentID; seek = seek + 1)
	{
		pAgent++;
	}

	/*Delete agent, keep the ptr with nullptr*/
	delete *pAgent;
	*pAgent = nullptr;
}
void cSanLogManager::_WriteLog(const SANLOG &Log) const
{
	for (auto AgentPtr : this->m_LogAgentList)
	{ 
		if (AgentPtr != nullptr){ AgentPtr->iPushLog(Log); } 
	}
}
void cSanLogManager::_ReplyResponse(const SANLOG &Log) const
{
	uint32 ResponseCode = this->_GenerateResponseCode(Log.Type, Log.Code);

	map<uint32, SANLOGRESPONSEFUNC>::const_iterator pResItem = this->m_ResponseFuncMap.find(ResponseCode);

	if (pResItem != this->m_ResponseFuncMap.end()){ pResItem->second(Log);; }
}
bool cSanLogManager::iCreateLogManager(const SANSYSTEMID &strManagerName)
{
	if (::gloFoundVector(strManagerName)){ return false; }

	if (::gloRegisterVector(strManagerName, VT_SYS | VT_VAR, (SHANDLE)this))
	{
		this->m_strManagerName = strManagerName;
		return true;
	}

	return false;
}
void cSanLogManager::iReleaseLogManager()
{
	if (!this->m_strManagerName.empty()){ ::gloDestoryVector(this->m_strManagerName, VT_SYS | VT_VAR); }

	/*list<cSanLogAgent*>::iterator pAgent = this->m_LogAgentList.begin();
	while (pAgent != this->m_LogAgentList.end())
	{
		delete *pAgent;
		pAgent++;
	}*/

	//C++11, need test
	for (auto Agent : this->m_LogAgentList){ delete Agent; }

	this->m_LogAgentList.clear();
	this->m_ResponseFuncMap.clear();
	this->m_LogWaitingList.clear();
}
void cSanLogManager::iReleaseLogAgent(const uint32 AgentID)
{
	this->_ReleaseLogAgent(AgentID);
}
bool cSanLogManager::iRegisteLogResponseFunc(const eSANLOGTYPE Type, const uint32 Code, SANLOGRESPONSEFUNC pResponseFunc)
{
	if ((Type == SLT_COUNT) || (pResponseFunc == nullptr)){ return false; }

	uint32 ResponseCode = this->_GenerateResponseCode(Type, Code);

	if (this->m_ResponseFuncMap.find(Code) != this->m_ResponseFuncMap.end())
	{
		this->m_ResponseFuncMap.insert(make_pair(ResponseCode, pResponseFunc));
	}

	return true;
}
void cSanLogManager::iReleaseLogResponseFunc(const eSANLOGTYPE Type, const uint32 Code, SANLOGRESPONSEFUNC pResponseFunc)
{
	if (Type == SLT_COUNT){ return; }

	uint32 ResponseCode = this->_GenerateResponseCode(Type, Code);

	map<uint32, SANLOGRESPONSEFUNC>::iterator pItem = this->m_ResponseFuncMap.find(ResponseCode);

	if (pItem != this->m_ResponseFuncMap.end()){ this->m_ResponseFuncMap.erase(pItem); }
}
void cSanLogManager::iPushLog(const SANLOG &Log)
{
	if (Log.Type >= SLT_COUNT){ return; }

	this->m_LogWaitingList.push_back(Log);
}
void cSanLogManager::iWriteLog(const SANLOG &Log)
{
	if (Log.Type >= SLT_COUNT){ return; }

	this->m_LogWaitingList.push_back(Log);

	this->iUpdate();
}
void cSanLogManager::iUpdate()
{
	this->m_WaitingListLock.lock();

	const uint32 LogCount = this->m_LogWaitingList.size();

	list<SANLOG>::iterator pLog = this->m_LogWaitingList.begin();
	for (int32 seek = 0; seek < LogCount; seek = seek + 1)
	{
		this->_WriteLog(*pLog);
		this->_ReplyResponse(*pLog);
		pLog++;
	}

	for (int32 seek = 0; seek < LogCount; seek = seek + 1)
	{
		this->m_LogWaitingList.pop_front();
	}

	this->m_WaitingListLock.unlock();
}