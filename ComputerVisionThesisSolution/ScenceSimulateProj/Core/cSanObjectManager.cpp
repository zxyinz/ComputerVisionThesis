#include"cSanObjectManager.h"
using namespace std;
using namespace San;
void cSanObjectManager::iSetObjectTime(uint32 Time)
{
	this->m_Time=Time;
}
void cSanObjectManager::iObjectTimeAdd(uint32 Time)
{
	this->m_Time=this->m_Time+Time;
}
void cSanObjectManager::iObjectTimeSub(uint32 Time)
{
	this->m_Time=this->m_Time-Time;
}
DWORD cSanObjectManager::iGetObjectTime()
{
	return this->m_Time;
}