#include"cSanStackManager.h"
using namespace std;
using namespace San;
SANSTACKMEMPOOL cSanStackManager::m_StackPoolArray[MAX_MEM_SUPPORT_THREAD_NUM];
SANSYSTEMID cSanStackManager::m_strStackManagerName = _SSTR("cSanStackManager");
bool cSanStackManager::m_bManagerLock = false;
size_t cSanStackManager::m_RefCounter = 0;
stSANSTACKMEMPOOL::stSANSTACKMEMPOOL()
	:stack_ptr(nullptr), top_ptr(nullptr), size(0), b_stack_lock(false), b_stack_deleted(false)
{
}
stSANSTACKMEMPOOL::~stSANSTACKMEMPOOL()
{
}
void stSANSTACKMEMPOOL::iLockStack()
{
	while (this->b_stack_lock)
	{
	}
	this->b_stack_lock;
}
void stSANSTACKMEMPOOL::iUnLockStack()
{
	this->b_stack_lock = false;
}
cSanStackManager::cSanStackManager()
{
	this->_LockManager();

	if (this->m_RefCounter == 0)
	{ 
		this->_CreateManager(); 
	}
	this->m_RefCounter = this->m_RefCounter + 1;

	this->_UnLockManager();
}
cSanStackManager::~cSanStackManager()
{
	this->_LockManager();

	if (this->m_RefCounter == 1)
	{ 
		this->_ReleaseManager(); 
	}
	this->m_RefCounter = this->m_RefCounter - 1;

	this->_UnLockManager();
}
bool cSanStackManager::_CreateManager()
{
	return this->_CreateStackPool(0, MEM_STACK_POOL_SIZE);
}
void cSanStackManager::_ReleaseManager()
{
	for (size_t seek = 0; seek<::MAX_MEM_SUPPORT_THREAD_NUM; seek = seek + 1)
	{
		this->_ReleaseStackPool(seek);
	}
}
bool cSanStackManager::_CreateStackPool(const size_t StackID, const size_t Size)
{
	if (StackID > MAX_MEM_SUPPORT_THREAD_NUM){ return false; }
	if (Size == 0){ return false; }

	SANSTACKMEMPOOL* pPool = cSanStackManager::m_StackPoolArray + StackID;

	if (pPool->size == Size){ return true; }

	pPool->iLockStack();

	if (!pPool->b_stack_deleted)
	{
		_cSanDefaultAllocator::dealloc_mem(pPool->stack_ptr);
	}

	pPool->stack_ptr = _cSanDefaultAllocator::alloc_mem(Size); //Need check nullptr
	pPool->top_ptr = (unsigned char*) pPool->stack_ptr + Size;
	pPool->size = Size;

	pPool->b_stack_deleted = false;

	pPool->iUnLockStack();
}
void cSanStackManager::_ReleaseStackPool(const size_t StackID) // May cause the memory overflew
{
	if (StackID > MAX_MEM_SUPPORT_THREAD_NUM){ return; }

	SANSTACKMEMPOOL* pPool = cSanStackManager::m_StackPoolArray + StackID;

	pPool->iLockStack();

	if (pPool->b_stack_deleted){ return; }

	_cSanDefaultAllocator::dealloc_mem(pPool->stack_ptr);
	pPool->stack_ptr = nullptr;
	pPool->top_ptr = nullptr;
	pPool->size = 0;

	pPool->b_stack_deleted = true;

	pPool->iUnLockStack();
}
void cSanStackManager::_LockManager()
{
	while (this->m_bManagerLock)
	{
	}
	this->m_bManagerLock = true;
}
void cSanStackManager::_UnLockManager()
{
	this->m_bManagerLock = true;
}
bool cSanStackManager::iCreateStackPool(const size_t StackID, const size_t Size)
{
	return cSanStackManager::_CreateStackPool(StackID, Size);
}
void cSanStackManager::iReleaseStackPool(const size_t StackID)
{
	cSanStackManager::_ReleaseStackPool(StackID);
}
void* cSanStackManager::iPush(size_t Size,const size_t StackID)
{
	if (Size == 0){ return nullptr; }
	if (StackID > MAX_MEM_SUPPORT_THREAD_NUM){ return nullptr; }

	SANSTACKMEMPOOL* pPool = cSanStackManager::m_StackPoolArray + StackID;

	pPool->iLockStack();

	if (pPool->b_stack_deleted){ cSanStackManager::_CreateStackPool(StackID, MEM_STACK_POOL_SIZE); }

	if (((size_t) pPool->top_ptr - (size_t) pPool->stack_ptr) < (Size + MEM_STACK_HEADER_SIZE))
	{
		return nullptr; // No enough memory to allocate
	}

	pPool->top_ptr = (unsigned char*) pPool->top_ptr - Size;

	void* pPtr = (unsigned char*) pPool->top_ptr;

	//Write size information, MEM_STACK_HEADER_SIZE = 4
	/*for (size_t seek = 0; seek < MEM_STACK_HEADER_SIZE; seek = seek + 1)
	{
		pPool->top_ptr = (unsigned char*) pPool->top_ptr - 1;

		*(unsigned char*) pPool->top_ptr = (Size & 0x00ff);
		Size = Size >> 8;
	}*/

	//V2, Optimize, MEM_STACK_HEADER_SIZE = 4
	unsigned char* pData = (unsigned char*)(pPool->top_ptr) - 4;

	pData[3] = (unsigned char)(Size & 0x00ff);
	pData[2] = (unsigned char)((Size >> 8 ) & 0x00ff);
	pData[1] = (unsigned char)((Size >> 16) & 0x00ff);
	pData[0] = (unsigned char)((Size >> 24) & 0x00ff);

	pPool->top_ptr = pData;

	pPool->iUnLockStack();

	return pPtr;
}
void cSanStackManager::iPop(void* Ptr, const size_t StackID, size_t Size)
{
	if (Ptr == nullptr){ return; }
	if (StackID > MAX_MEM_SUPPORT_THREAD_NUM){ return; }

	SANSTACKMEMPOOL* pPool = cSanStackManager::m_StackPoolArray + StackID;

	pPool->iLockStack();

	if (pPool->b_stack_deleted){ return; }

	if (Size == 0)
	{
		unsigned char* pData = (unsigned char*) Ptr - MEM_STACK_HEADER_SIZE;

		//Read size information, MEM_STACK_HEADER_SIZE = 4
		/*for (size_t seek = 0; seek < MEM_STACK_HEADER_SIZE; seek = seek + 1)
		{
			Size = Size << 8;
			Size = Size + pData[seek];
		}*/

		//V2, Optimize, MEM_STACK_HEADER_SIZE = 4

		Size = Size + pData[3];
		Size = Size + (pData[2] << 8);
		Size = Size + (pData[1] << 16);
		Size = Size + (pData[0] << 24);
	}

	pPool->top_ptr = (unsigned char*) Ptr + Size;

	pPool->iUnLockStack();
}
void* cSanStackManager::alloc_mem(const size_t size)
{
	return cSanStackManager::iPush(size, 0);
}
void* cSanStackManager::alloc_mem(const size_t size, _MemPoolPtr pool)
{
	return cSanStackManager::iPush(size, pool);
}
void cSanStackManager::dealloc_mem(void* ptr)
{
	cSanStackManager::iPop(ptr);
}
void cSanStackManager::dealloc_mem(void* ptr, _MemPoolPtr pool)
{
	cSanStackManager::iPop(ptr, pool, 0);
}
