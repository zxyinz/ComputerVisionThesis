//Project: San Lab Game Engine
//Version: 2.1.1
//Debug State: Need test
#include"cstdlib"
#include"cSanMemoryManagerAlgorithms.h"
#pragma once
using namespace std;
namespace San
{
#ifndef __CSANMEMORYMANAGERDEF_H__
#define __CSANMEMORYMANAGERDEF_H__

	enum eSANMEMPOOLTYPE// Currently not use
	{
		SMPT_DYNAMIC	= 0x00,
		SMPT_STATIC		= 0x01,
		SMPT_STACK		= 0x02,
	};

	class _cSanDefaultAllocator
	{
	public:
		typedef void* _MemPoolPtr;
	public:
		static void* alloc_mem(const size_t size){ return ::malloc(size); };
		static void* alloc_mem(const size_t size, _MemPoolPtr pool){ return pool; };

		static void dealloc_mem(void* ptr){ ::free(ptr); };
		static void dealloc_mem(void* ptr, _MemPoolPtr pool){};
	};

	//class _cSanDefaultAllocator
	//{
	//public:
	//	typedef void* _MemPoolPtr;
	//public:
	//	void* operator()(const size_t size){ return ::malloc(size); };
	//	void* operator()(const size_t size, _MemPoolPtr pool){ return pool; };
	//};

	//class _cSanDefaultDeallocator
	//{
	//public:
	//	typedef void* _MemPoolPtr;
	//public:
	//	void operator()(void* ptr){ ::free(ptr); };
	//	void operator()(void* ptr, _MemPoolPtr pool){ /*::free(ptr);*/ };
	//};

	typedef _cSanDefaultAllocator cSanSystemAllocator;

	template<class _Allocator = cSanSystemAllocator/*, class _Deallocator*/>
	struct _SANMEMOBJ
	{
	public:
		void* operator new(const size_t size){ return _Allocator::alloc_mem(size); };
		void* operator new(const size_t size, typename _Allocator::_MemPoolPtr pool){ return _Allocator::alloc_mem(size, pool); };
		void* operator new[](const size_t size){ return _Allocator::alloc_mem(size); };
		void* operator new[](const size_t size, typename _Allocator::_MemPoolPtr pool){ return _Allocator::alloc_mem(size, pool); };

		void operator delete(void* ptr){ return _Allocator::dealloc_mem(ptr); };
		void operator delete(void* ptr, typename _Allocator::_MemPoolPtr pool){ return _Allocator::dealloc_mem(ptr, pool); };
		void operator delete[](void* ptr){return _Allocator::dealloc_mem(ptr); };
		void operator delete[](void* ptr, typename _Allocator::_MemPoolPtr pool){return _Allocator::dealloc_mem(ptr, pool); };
	};

	template<class _Allocator = cSanSystemAllocator/*, class _Deallocator*/>
	using _smemobj = _SANMEMOBJ<_Allocator/*, _Deallocator*/>;

	template<class ST, class _Allocator = cSanSystemAllocator>
	class cSanSTLAllocator : public std::allocator<ST> // Need test
	{
	public:
		typedef typename std::allocator<ST>::size_type			size_type;
		typedef typename std::allocator<ST>::difference_type	difference_type;
		typedef typename std::allocator<ST>::pointer			pointer;
		typedef typename std::allocator<ST>::const_pointer		const_pointer;
		typedef typename std::allocator<ST>::reference			reference;
		typedef typename std::allocator<ST>::const_reference	const_reference;
		typedef typename std::allocator<ST>::value_type			value_type;

		template<class U> struct rebind{ typedef cSanSTLAllocator<U> Other; };
	public:
		cSanSTLAllocator(){};
		template<class U> cSanSTLAllocator(const cSanSTLAllocator<U>&){};

		pointer allocate(size_type n, allocator<void>::const_pointer p = 0)
		{
			return _Allocator::alloc_mem(sizeof(ST) * n);
		};
		void deallocate(pointer p, size_type n)
		{
			_Allocator::dealloc_mem(p);
		};
		void construct(pointer p, const_reference t)
		{
			new((void*) p) ST(t);
		};
		void destory(pointer p)
		{
			((ST*) p)->~ST();
		};
	};

	//void*	gmem_push(const size_t size=1U,const uint32 thread_id=0U);
	//void	gmem_pop(const void** ptr,const size_t size=1U,const uint32 thread_id=0U);

	static const eSANMEMORYALGORITHMTYPE SAN_MEM_ALGORITHM_TYPE=SMAT_SSM;

//#ifdef _DEBUG
	void*	_san_new(const size_t size,void* buffer,const size_t buf_size,const int index,const char* FILE,const size_t LINE);
	void*	_san_delete(void* ptr,const size_t size,const char* FILE,const size_t LINE);
//#define san_new(size,buffer,buf_size) _san_new(size,buffer,buf_size,-1,__FILE__,__LINE__)
//#define san_new(size,buffer,buf_size,index) _san_new(size,buffer,buf_size,index,__FILE__,__LINE__)
//#define san_delete(ptr,size) _san_delete(ptr,size,__FILE__,__LINE__)
//#else
	//void* san_new(const size_t size,void* buffer,const size_t buf_size,const int index=-1);
	//void* san_delete(void* ptr,const size_t size);
//#endif
#endif
}
