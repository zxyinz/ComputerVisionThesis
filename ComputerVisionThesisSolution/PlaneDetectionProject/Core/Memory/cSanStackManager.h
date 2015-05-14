//Project: San Lab Game Engine
//Version: 2.1.1
//Debug State: Need add log and multi-thread support
#include"cSanMemoryManagerDef.h"
#include"../SanTypes.h"
#pragma once
using namespace std;
namespace San
{
#ifndef __CSANSTACKMANAGER_H__
#define __CSANSTACKMANAGER_H__

	static const size_t MIN_MEM_STACK_UNIT_SIZE		= 1;				//	4	Byte, not use
	static const size_t MAX_MEM_STACK_UNIT_SIZE		= 256;				//	265	Byte, not use
	static const size_t MEM_STACK_UNIT_SIZE			= 1;				//	1	Byte Current version stack pool unit size, not use
	static const size_t MEM_STACK_LIST_SIZE			= 1024;				//	1024 MIN UNIT  Stack Segment total: 1*1024 1KB, not use
	static const size_t MEM_STACK_POOL_SIZE			= 1024 * 1024 * 16;	//	1024 Byte (16M), Default Stack Pool Size
	static const size_t MEM_STACK_HEADER_SIZE		= 4;					//	2 Byte, contain object size
	static const size_t MAX_MEM_SUPPORT_THREAD_NUM	= 32;				//	32 thread stack support

	struct stSANSTACKMEMPOOL
	{
	public:
		void* stack_ptr;
		void* top_ptr;
		size_t size;
		bool b_stack_lock;
		bool b_stack_deleted;
	public:
		stSANSTACKMEMPOOL();
		~stSANSTACKMEMPOOL();

		void iLockStack();
		void iUnLockStack();
	};
	typedef stSANSTACKMEMPOOL	SANSTACKMEMPOOL;
	typedef stSANSTACKMEMPOOL*	lpSANSTACKMEMPOOL;

	class cSanStackManager : public _smemobj<_cSanDefaultAllocator>
	{
	private:
		static SANSTACKMEMPOOL m_StackPoolArray[MAX_MEM_SUPPORT_THREAD_NUM];
		static SANSYSTEMID m_strStackManagerName;
		static bool m_bManagerLock;
		static size_t m_RefCounter;
	public:
		typedef size_t _MemPoolPtr; // Stack pool ID
	private:
		bool _CreateManager();
		void _ReleaseManager();
		static bool _CreateStackPool(const size_t StackID, const size_t Size);
		static void _ReleaseStackPool(const size_t StackID);
		void _LockManager();
		void _UnLockManager();
	public:
		cSanStackManager();
		cSanStackManager(const cSanStackManager&) = delete;
		~cSanStackManager();

		cSanStackManager& operator=(const cSanStackManager&) = delete;

		static bool iCreateStackPool(const size_t StackID, const size_t Size = MEM_STACK_POOL_SIZE);
		static void iReleaseStackPool(const size_t StackID);

		static void* iPush(size_t Size = 1, const size_t StackID = 0);
		static void iPop(void* Ptr, const size_t StackID = 0, size_t Size = 0);

		static void* alloc_mem(const size_t size);
		static void* alloc_mem(const size_t size, _MemPoolPtr pool);

		static void dealloc_mem(void* ptr);
		static void dealloc_mem(void* ptr, _MemPoolPtr pool);
	};
#endif
}
