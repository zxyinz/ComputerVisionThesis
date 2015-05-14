//Project: San Lab Game Engine
//Version: 2.1.1
//Debug State: Add allocator
#include"cSanMemoryManagerDef.h"
#include"cSanStackManager.h"
#pragma once
using namespace std;
namespace San
{
#ifndef __CSANMEMORYMANAGER_H__
#define __CSANMEMORYMANAGER_H__

	class cSanMemoryManager
	{
	private:
		static SANSYSTEMID m_strManagerName;
		static cSanStackManager m_StackManager;
		static bool m_bInited;
	public:
		cSanMemoryManager()
		{
		};
		~cSanMemoryManager(){ if (m_bInited){ this->iReleaseMemoryManager(); } };
		bool iInitMemoryManager();
		void iReleaseMemoryManager();
		const cSanStackManager* iGetStackManagerPtr();
		void iPrintCurrentMemoryState();
	};

#endif
}
