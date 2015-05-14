//Project: San Lab Game Engine
//Version: 2.1.1
//Debug State: Add log agent, log input/output
#include"SanContainer.h"
#include"cSanConsolePort.h"
using namespace std;
using namespace San::Device;
namespace San
{
#ifndef __CSANLOGMANAGER_H__
#define __CSANLOGMANAGER_H__

	enum eSANLOGTYPE
	{
		SLT_LOG,
		SLT_ERROR,
		SLT_WARNING,
		SLT_MESSAGE,
		SLT_COUNT,
	};

	struct SANLOG
	{
	public:
		eSANLOGTYPE	Type;
		//uint32	Level;
		uint32	Code;
		SString	strLog;
		time_t	TimeStamp;
		clock_t ClockStamp;

		/*For debug*/
		SString	strCodeFile;
		uint32	CodeLine;
		SHANDLE	MemLocation;

		/*User data, for response func*/
		_sstream<uint8>	UserData;
	public:
		SANLOG();
		SANLOG(const eSANLOGTYPE Type, const uint32 Code, const SString &strLog, const SString &strCodeFile = _SSTR("null"), const uint32 CodeLine = 0, const SHANDLE MemLocation = nullptr, const _sstream<uint8> &UserData = _sstream<uint8>());
		~SANLOG();
	};
	typedef SANLOG	SANLOG;
	typedef SANLOG*	lpSANLOG;

	typedef _spair<uint32, uint32> SANLOGFILITER; // current: level range, need extend

	class cSanLogAgent
	{
	protected:
		vector<SANLOGFILITER> m_LogFiliterArray;
	protected:
		virtual void _ReleaseAgent() = 0;
		virtual bool _LogAcceptable() const;
	public:
		cSanLogAgent();
		cSanLogAgent(const vector<SANLOGFILITER> &FiliterArray);
		~cSanLogAgent();

		virtual bool iPushLog(const SANLOG &Log) = 0;
	};

	class cSanConsoleLogAgent : public cSanLogAgent
	{
	private:
		cSanConsolePort* m_pConsole;
	protected:
		void _ReleaseAgent() override final;
		bool _LogAcceptable() const override final;
	public:
		cSanConsoleLogAgent(cSanConsolePort* pConsole);
		cSanConsoleLogAgent(cSanConsolePort* pConsole, const vector<SANLOGFILITER> &FiliterArray);
		~cSanConsoleLogAgent();

		bool iPushLog(const SANLOG &Log) override final;
	};

	class cSanFileLogAgent : public cSanLogAgent
	{
	private:
		ofstream m_OutputFile;
	protected:
		void _ReleaseAgent() override final;
		bool _LogAcceptable() const override final;
	public:
		cSanFileLogAgent(const SString &strFilePath);
		cSanFileLogAgent(const SString &strFilePath, const vector<SANLOGFILITER> &FiliterArray);
		~cSanFileLogAgent();

		bool iPushLog(const SANLOG &Log) override final;
	};

	typedef SRESULT (*SANLOGRESPONSEFUNC)(const SANLOG&);

	class cSanLogManager // Current output only
	{
	private:
		SANSYSTEMID	m_strManagerName;
		list<cSanLogAgent*> m_LogAgentList;
		map<uint32, SANLOGRESPONSEFUNC> m_ResponseFuncMap;
		list<SANLOG> m_LogWaitingList;
		SANLOCK m_WaitingListLock;
	protected:
		uint32 _GenerateResponseCode(const eSANLOGTYPE Type, const uint32 Code) const;
		uint32 _RegisteLogAgent(cSanLogAgent* pAgent);
		void _ReleaseLogAgent(const uint32 AgentID);
		void _WriteLog(const SANLOG &Log) const;
		void _ReplyResponse(const SANLOG &Log) const;
	public:
		cSanLogManager();
		~cSanLogManager();

		bool iCreateLogManager(const SANSYSTEMID &strManagerName = _SSTR("SanLogManager"));
		void iReleaseLogManager();

		template<class _AgentType> uint32 iRegisteLogAgent(const _AgentType &Agent)
		{ 
			_AgentType* pAgent = new _AgentType(Agent);
			return this->_RegisteLogAgent(pAgent); 
		};
		void iReleaseLogAgent(const uint32 AgentID);

		bool iRegisteLogResponseFunc(const eSANLOGTYPE Type, const uint32 Code, SANLOGRESPONSEFUNC pResponseFunc);
		void iReleaseLogResponseFunc(const eSANLOGTYPE Type, const uint32 Code, SANLOGRESPONSEFUNC pResponseFunc);

		/*Push log to the wait list*/
		void iPushLog(const SANLOG &Log);

		/*Write log to all valid agent*/
		void iWriteLog(const SANLOG &Log);

		void iUpdate();
	};
#endif
}