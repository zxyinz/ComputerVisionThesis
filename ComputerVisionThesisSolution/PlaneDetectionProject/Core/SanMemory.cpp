#include"SanMemory.h"
using namespace std;
using namespace San;
void* San::gloMemCopy(void* _Dest,const void* _Src,size_t _Size)
{
#ifndef _WINDOWS
	if((_Dest==nullptr)||(_Src==nullptr)||(_Size==0))
	{
		return 0;
	}
	unsigned char* pDest=(unsigned char*)_Dest;
	const unsigned char* pSrc=(const unsigned char*)_Src;
	for(size_t seek=0;seek<_Size;seek=seek+1)
	{
		*pDest=*(pSrc+seek);
		pDest=pDest+1;
	}
	return (void*)_Size;
#else
	return ::memcpy(_Dest,_Src,_Size);
#endif
};
void* San::gloMemSet(void* _Dest,unsigned int _Set,size_t _Size)
{
#ifndef _WINDOWS
	if((_Dest==nullptr)||(_Size==NULL))
	{
		return 0;
	}
	unsigned int* pDest=(unsigned int*)_Dest;
	for(size_t seek=0;seek<_Size;seek=seek+1)
	{
		*pDest=_Set;
		pDest=pDest+1;
	}
	return (void*)_Size;
#else
	return ::memset(_Dest,_Set,_Size);
#endif
};