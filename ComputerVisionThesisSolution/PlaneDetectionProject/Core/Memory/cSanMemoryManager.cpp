#include"cSanMemoryManager.h"
using namespace San;
SANSYSTEMID cSanMemoryManager::m_strManagerName=_SSTR("cSanMemoryManager");
cSanStackManager cSanMemoryManager::m_StackManager;
bool cSanMemoryManager::m_bInited=false;
bool cSanMemoryManager::iInitMemoryManager()
{
	if(this->m_bInited)
	{
		return true;
	}
	switch(San::SAN_MEM_ALGORITHM_TYPE)
	{
	case SMAT_STD:
		break;
	case SMAT_SSM:
		if(_ssm_init()!=0)
		{
			return true;
		}
		return false;
		break;
	default:
		break;
	}
	return true;
}
void cSanMemoryManager::iReleaseMemoryManager()
{
	if(this->m_bInited)
	{
		switch(San::SAN_MEM_ALGORITHM_TYPE)
		{
		case SMAT_STD:
			break;
		case SMAT_SSM:
			_ssm_release();
		default:
			break;
		}
	}
}
const cSanStackManager* cSanMemoryManager::iGetStackManagerPtr()
{
	return (const cSanStackManager*)&(this->m_StackManager);
}
void cSanMemoryManager::iPrintCurrentMemoryState()
{
	switch(San::SAN_MEM_ALGORITHM_TYPE)
	{
	case SMAT_STD:
		break;
	case SMAT_SSM:
		_ssm_print_mem_state();
		break;
	default:
		break;
	}
}
//#ifdef _DEBUG
void* san_new(const size_t size,void* buffer,const size_t buf_size,const int index,const char* FILE,const size_t LINE)
//#else
//void* san_new(const size_t size,void* buffer,const size_t buf_size,const int index)
//#endif
{
	void* ptr=nullptr;
	switch(San::SAN_MEM_ALGORITHM_TYPE)
	{
	case SMAT_STD:
		return malloc(size);
		break;
	case SMAT_SSM:
		if(index<0)
		{
			std::srand(std::time(0));
			ptr=_ssm_new(size,buffer,buf_size,rand()%SSM_PAGE_LIST_SIZE);
			if(ptr==nullptr)
			{
				ptr=_ssm_new(size,buffer,buf_size,-1);
				return ptr;
			}
		}
		return _ssm_new(size,buffer,buf_size,buf_size);
		break;
	default:
		return nullptr;
	}
}
//#ifdef _DEBUG
void* san_delete(void* ptr,size_t size,const char* FILE,const size_t LINT)
//#else
//void* san_delete(void* ptr,size_t size)
//#endif
{
	switch(San::SAN_MEM_ALGORITHM_TYPE)
	{
	case SMAT_STD:
		free(ptr);
		return nullptr;
		break;
	case SMAT_SSM:
		return _ssm_delete(ptr,size);
		break;
	default:
		return nullptr;
		break;
	}
	return nullptr;
}