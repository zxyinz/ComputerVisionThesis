#include"cSanObject.h"
using namespace std;
namespace San
{
#ifndef __CSANOBJECTMANAGER_H__
#define __CSANOBJECTMANAGER_H__
	class cSanObjectManager :public cSanObject
	{
	//private:
	public:
		cSanObjectManager()
			:cSanObject()
		{
		};
		~cSanObjectManager(){};
	public:
		bool iCreateManager();
		void iReleaseManager();
		void iSetObjectTime(uint32 Time);
		void iObjectTimeAdd(uint32 Time=1);
		void iObjectTimeSub(uint32 Time=1);
		DWORD iGetObjectTime();
	};
#endif
}