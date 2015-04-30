#include"../RenderSystem.h"
using namespace std;
#pragma once
namespace San
{
#ifndef __STDSANMEMORY_H__
#define __STDSANMEMORY_H__

#ifdef _WINDOWS
	inline __int16 gBtoL16(__int16 data){return (data<<8)|(data>>8);};
	inline __int16 gLtoB16(__int16 data){return (data<<8)|(data>>8);};
	inline __int32 gBtoL32(__int32 data){return (data<<24)|((data&0x00ff0000)>>8)|((data&0x0000ff00)<<8)|(data<<24);};
	inline __int32 gLtoB32(__int32 data){return (data<<24)|((data&0x00ff0000)>>8)|((data&0x0000ff00)<<8)|(data<<24);};
	inline __int64 gBtoL64(__int64 data){return (((__int64)(gBtoL32((__int32)data)))<<32)|((__int64)(gBtoL32((__int32)data>>32)));};
	inline __int64 gLtoB64(__int64 data){return (((__int64)(gLtoB32((__int32)data)))<<32)|((__int64)(gLtoB32((__int32)data>>32)));};
#endif
#ifdef _UNIX
	inline unsigned short gBtoL16(unsigned short data){return (data<<8)|(data>>8);};
	inline unsigned short gLtoB16(unsigned short data){return (data<<8)|(data>>8);};
	inline unsigned long gBtoL32(unsigned long data){return (data<<24)|((data&0x00ff0000)>>8)|((data&0x0000ff00)<<8)|(data<<24);};
	inline unsigned long gLtoB32(unsigned long data){return (data<<24)|((data&0x00ff0000)>>8)|((data&0x0000ff00)<<8)|(data<<24);};
	inline unsigned long long gBtoL64(__int64 data){return (((unsigned long long)(gBtoL32((unsigned long)data)))<<32)|((unsigned long long)(gBtoL32((unsigned long)data>>32)));};
	inline unsigned long long gLtoB64(__int64 data){return (((unsigned long long)(gLtoB32((unsigned long)data)))<<32)|((unsigned long long)(gLtoB32((unsigned long)data>>32)));};
#endif

	void* gloMemCopy(void* _Dest,const void* _Src,size_t _Size);
	void* gloMemSet(void* _Dest,unsigned int _Set,size_t _Size);
#endif
}