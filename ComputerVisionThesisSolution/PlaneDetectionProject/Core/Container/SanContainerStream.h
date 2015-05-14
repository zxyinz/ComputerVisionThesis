//Project: San Lab Game Engine
//Version: 2.1.1
//Debug State: Need test
#include"SanContainerDef.h"
#include"SanContainerStreamT.h"
#pragma once
using namespace std;
namespace San
{
#ifndef __STDCONTAINERSTREAM_H__
#define __STDCONTAINERSTREAM_H__
	struct stSANSTREAM
	{
	public:
		uint8* pStream;
		union
		{
			uint32 Size;
			uint32 SizeInByte;
		};
	protected:
		void _ReleaseStream();
	public:
		stSANSTREAM(const uint32 Size = 0);
		stSANSTREAM(const stSANSTREAM& Stream);
		stSANSTREAM(const uint8* pStream, const uint32 BufSize);
		~stSANSTREAM();

		//operators overload
		stSANSTREAM& operator=(const stSANSTREAM& Stream);
		stSANSTREAM operator+(const stSANSTREAM& Stream);
		stSANSTREAM operator-(const uint32 Size);
		bool operator==(const stSANSTREAM& Stream);
		bool operator!=(const stSANSTREAM& Stream);
		const uint8& operator[](const size_t Position) const;
		uint8& operator[](const size_t Position);

		//Stream IO Function
		uint32 iSetStream(const uint32 Offset, const stSANSTREAM& Stream);
		uint32 iSetStream(const uint32 Offset, const uint8* pBuffer, const uint32 BufSize);
		uint32 iSetStream(const uint32 Offset, const uint8 Data);
		uint32 iSetStream(const uint32 Offset, const uint16 Data);
		uint32 iSetStream(const uint32 Offset, const uint32 Data);
		uint32 iSetStream(const uint32 Offset, const uint64 Data);
		uint32 iGetStream(const uint32 Offset, uint8* pBuffer, const uint32 BufSize) const;
		uint32 iGetStream(const uint32 Offset, uint8 &Data) const;
		uint32 iGetStream(const uint32 Offset, uint16 &Data) const;
		uint32 iGetStream(const uint32 Offset, uint32 &Data) const;
		uint32 iGetStream(const uint32 Offset, uint64 &Data) const;
		uint32 iPopBegin(const uint32 Size);
		uint32 iPopEnd(const uint32 Size);
		uint32 iReSizeStream(const uint32 Size);
		uint32 iClear(const uint8 Data = 0);
	};
	typedef stSANSTREAM	SANSTREAM;
	typedef stSANSTREAM* lpSANSTREAM;

	struct SANBITSTREAM : public _SANSTREAMEX<bool>
	{
	public:
		SANBITSTREAM(const uint32 BitStreamSize = 8);
		SANBITSTREAM(const SANBITSTREAM& BitStream);
		SANBITSTREAM(const SANSTREAM& Stream, const uint32 Begin, const uint32 BitStreamSize);
		SANBITSTREAM(const uint8* pStream, const uint32 BufSize, const uint32 Begin, const uint32 BitStreamSize);
		~SANBITSTREAM();

		//operators overload
		//SANBITSTREAM& operator=(const SANBITSTREAM& BitStream);
		//SANBITSTREAM operator+(const SANBITSTREAM& BitStream);
		//SANBITSTREAM operator-(const uint32 SizeInBit);

		//Stream IO Function
		uint32 iSetBitStream(const uint32 Offset, const SANBITSTREAM& Stream, uint32 Begin = 0, uint32 Length = 0);
		uint32 iSetBitStream(const uint32 Offset, const uint8* pBuffer, const uint32 BufSize, uint32 Begin = 0, uint32 Length = 0);
		uint32 iSetBitStream(const uint32 Offset, const uint8 Data, uint32 Begin = 0, uint32 Length = 0);
		uint32 iSetBitStream(const uint32 Offset, const uint16 Data, uint32 Begin = 0, uint32 Length = 0);
		uint32 iSetBitStream(const uint32 Offset, const uint32 Data, uint32 Begin = 0, uint32 Length = 0);
		uint32 iSetBitStream(const uint32 Offset, const uint64 Data, uint32 Begin = 0, uint32 Length = 0);
		uint32 iGetBitStream(const uint32 Offset, uint8* pBuffer, const uint32 BufSize, uint32 Begin = 0, uint32 Length = 0) const;
		uint32 iGetBitStream(const uint32 Offset, uint8 &Data, uint32 Begin = 0, uint32 Length = 0) const;
		uint32 iGetBitStream(const uint32 Offset, uint16 &Data, uint32 Begin = 0, uint32 Length = 0) const;
		uint32 iGetBitStream(const uint32 Offset, uint32 &Data, uint32 Begin = 0, uint32 Length = 0) const;
		uint32 iGetBitStream(const uint32 Offset, uint64 &Data, uint32 Begin = 0, uint32 Length = 0) const;
	};
	typedef SANBITSTREAM* lpSANBITSTREAM;
#endif
}