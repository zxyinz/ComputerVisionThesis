//Project: San Lab Game Engine
//Version: 2.1.1
//Debug State: Add local memory manager [stack, pool]
#include"../RenderSystem.h"
#include"Memory/cSanMemoryManagerDef.h"
using namespace std;
#pragma once
namespace San
{
#ifndef __STDSANMEMORY_H__
#define __STDSANMEMORY_H__

#ifdef _WINDOWS
	inline __int16 gBtoL16(__int16 data){ return (data << 8) | (data >> 8); };
	inline __int16 gLtoB16(__int16 data){ return (data << 8) | (data >> 8); };
	inline __int32 gBtoL32(__int32 data){ return (data << 24) | ((data & 0x00ff0000) >> 8) | ((data & 0x0000ff00) << 8) | (data << 24); };
	inline __int32 gLtoB32(__int32 data){ return (data << 24) | ((data & 0x00ff0000) >> 8) | ((data & 0x0000ff00) << 8) | (data << 24); };
	inline __int64 gBtoL64(__int64 data){ return (((__int64) (gBtoL32((__int32) data))) << 32) | ((__int64) (gBtoL32((__int32) data >> 32))); };
	inline __int64 gLtoB64(__int64 data){ return (((__int64) (gLtoB32((__int32) data))) << 32) | ((__int64) (gLtoB32((__int32) data >> 32))); };
#endif
#ifdef _UNIX
	inline unsigned short gBtoL16(unsigned short data){ return (data << 8) | (data >> 8); };
	inline unsigned short gLtoB16(unsigned short data){ return (data << 8) | (data >> 8); };
	inline unsigned long gBtoL32(unsigned long data){ return (data << 24) | ((data & 0x00ff0000) >> 8) | ((data & 0x0000ff00) << 8) | (data << 24); };
	inline unsigned long gLtoB32(unsigned long data){ return (data << 24) | ((data & 0x00ff0000) >> 8) | ((data & 0x0000ff00) << 8) | (data << 24); };
	inline unsigned long long gBtoL64(__int64 data){ return (((unsigned long long)(gBtoL32((unsigned long) data))) << 32) | ((unsigned long long)(gBtoL32((unsigned long) data >> 32))); };
	inline unsigned long long gLtoB64(__int64 data){ return (((unsigned long long)(gLtoB32((unsigned long) data))) << 32) | ((unsigned long long)(gLtoB32((unsigned long) data >> 32))); };
#endif

	inline void* gloMemCopy(void* _Dest, const void* _Src, const size_t _Size)
	{
#ifndef _WINDOWS
		if ((_Dest == nullptr) || (_Src == nullptr) || (_Size == 0))
		{
			return 0;
		}
		unsigned char* pDest = (unsigned char*) _Dest;
		const unsigned char* pSrc = (const unsigned char*) _Src;
		for (size_t seek = 0; seek<_Size; seek = seek + 1)
		{
			*pDest = *(pSrc + seek);
			pDest = pDest + 1;
		}
		return (void*) _Size;
#else
		return ::memcpy(_Dest, _Src, _Size);
#endif
	};
	inline void* gloMemSet(void* _Dest, unsigned int _Set, const size_t _Size)
	{
#ifndef _WINDOWS
		if ((_Dest == nullptr) || (_Size == NULL))
		{
			return 0;
		}
		unsigned int* pDest = (unsigned int*) _Dest;
		for (size_t seek = 0; seek<_Size; seek = seek + 1)
		{
			*pDest = _Set;
			pDest = pDest + 1;
		}
		return (void*) _Size;
#else
		return ::memset(_Dest, _Set, _Size);
#endif
	};
#endif
}