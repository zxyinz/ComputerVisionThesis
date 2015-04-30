#include"SanContainer.h"
using namespace std;
using namespace San;
San::stSANSTREAM::stSANSTREAM(const uint32 Size)
	:pStream(nullptr),
	Size(Size)
{
	if (Size != 0)
	{
		this->pStream = new uint8[this->Size];
		gloMemSet(this->pStream, 0, this->Size);
	}
}
San::stSANSTREAM::stSANSTREAM(const stSANSTREAM& Stream)
	:pStream(nullptr),
	Size(Size)
{
	if (Size != 0)
	{
		this->pStream = new uint8[this->Size];
		gloMemSet(this->pStream, 0, this->Size);
		gloMemCopy(this->pStream, Stream.pStream, Stream.Size);
	}
}
San::stSANSTREAM::stSANSTREAM(const uint8* pStream, const uint32 BufSize)
	:pStream(nullptr),
	Size(BufSize)
{
	if (Size != 0)
	{
		this->pStream = new uint8[this->Size];
		gloMemSet(this->pStream, 0, this->Size);
		gloMemCopy(this->pStream, pStream, BufSize);
	}
}
San::stSANSTREAM::~stSANSTREAM()
{
	this->_ReleaseStream();
}
void San::stSANSTREAM::_ReleaseStream()
{
	if (this->pStream != nullptr)
	{
		if (this->Size == 1)
		{
			delete this->pStream;
		}
		else
		{
			delete[] this->pStream;
		}
	}
	this->pStream = nullptr;
	this->Size = 0;
}
San::stSANSTREAM& San::stSANSTREAM::operator=(const stSANSTREAM& Stream)
{
	if ((Stream.pStream == nullptr) || (Stream.Size == 0))
	{
		this->_ReleaseStream();
		return *this;
	}
	uint32 Size = Stream.Size;
	uint8* pStream = new uint8[Size];
	gloMemSet(pStream, 0, Size);
	gloMemCopy(pStream, Stream.pStream, Stream.Size);
	this->_ReleaseStream();
	this->pStream = pStream;
	this->Size = Size;
	return *this;
}
San::stSANSTREAM San::stSANSTREAM::operator+(const stSANSTREAM& Stream)
{
	if ((Stream.pStream == nullptr) || (Stream.Size == 0))
	{
		return *this;
	}
	uint32 Size = this->Size + Stream.Size;
	SANSTREAM stm(Size);
	gloMemSet(stm.pStream, 0, Size);
	gloMemCopy(stm.pStream, this->pStream, this->Size);
	gloMemCopy(stm.pStream + this->Size, Stream.pStream, Stream.Size);
	return stm;
}
San::stSANSTREAM San::stSANSTREAM::operator-(const uint32 Size)
{
	this->iPopBegin(Size);
	return *this;
}
bool San::stSANSTREAM::operator==(const stSANSTREAM& Stream)
{
	if (this->Size != Stream.Size)
	{
		return false;
	}
	for (uint32 seek = 0; seek < this->Size; seek = seek + 1)
	{
		if (this->pStream[seek] != Stream.pStream[seek])
		{
			return false;
		}
	}
	return true;
}
bool San::stSANSTREAM::operator!=(const stSANSTREAM& Stream)
{
	if (this->Size != Stream.Size)
	{
		return true;
	}
	for (uint32 seek = 0; seek < this->Size; seek = seek + 1)
	{
		if (this->pStream[seek] == Stream.pStream[seek])
		{
			return false;
		}
	}
	return true;
}
const uint8& San::stSANSTREAM::operator[](const size_t Position) const
{
	return this->pStream[Position];
}
uint8& San::stSANSTREAM::operator[](const size_t Position)
{
	return this->pStream[Position];
}
uint32 San::stSANSTREAM::iSetStream(const uint32 Offset, const stSANSTREAM& Stream)
{
	uint32 StreamSize = Stream.Size;
	if (StreamSize == 0)
	{
		return 0;
	}
	this->iSetStream(Offset, Stream.pStream, StreamSize);
}
uint32 San::stSANSTREAM::iSetStream(const uint32 Offset, const uint8* pBuffer, const uint32 BufSize)
{
	if ((pBuffer == nullptr) || (BufSize == 0))
	{
		return 0;
	}
	if (Offset >= this->Size)
	{
		return 0;
	}
	uint32 EndPos = Offset + BufSize;
	EndPos = EndPos < this->Size ? EndPos : this->Size;
	uint32 Index = 0;
	for (uint32 seek = Offset; seek < EndPos; seek = seek + 1)
	{
		this->pStream[seek] = pBuffer[Index];
		Index = Index + 1;
	}
	return EndPos - Offset;
}
uint32 San::stSANSTREAM::iSetStream(const uint32 Offset, const uint8 Data)
{
	return this->iSetStream(Offset, (uint8*) &Data, sizeof(Data));
}
uint32 San::stSANSTREAM::iSetStream(const uint32 Offset, const uint16 Data)
{
	return this->iSetStream(Offset, (uint8*) &Data, sizeof(Data));
}
uint32 San::stSANSTREAM::iSetStream(const uint32 Offset, const uint32 Data)
{
	return this->iSetStream(Offset, (uint8*) &Data, sizeof(Data));
}
uint32 San::stSANSTREAM::iSetStream(const uint32 Offset, const uint64 Data)
{
	return this->iSetStream(Offset, (uint8*) &Data, sizeof(Data));
}
uint32 San::stSANSTREAM::iGetStream(const uint32 Offset, uint8* pBuffer, const uint32 BufSize) const
{
	if ((pBuffer == nullptr) || (BufSize == 0))
	{
		return 0;
	}
	if (Offset >= this->Size)
	{
		return 0;
	}
	uint32 EndPos = Offset + BufSize;
	EndPos = EndPos < this->Size ? EndPos : this->Size;
	uint32 Index = 0;
	for (uint32 seek = Offset; seek < EndPos; seek = seek + 1)
	{
		pBuffer[Index] = this->pStream[seek];
		Index = Index + 1;
	}
	return EndPos - Offset;
}
uint32 San::stSANSTREAM::iGetStream(const uint32 Offset, uint8 &Data) const
{
	return this->iGetStream(Offset, (uint8*) &Data, sizeof(Data));
}
uint32 San::stSANSTREAM::iGetStream(const uint32 Offset, uint16 &Data) const
{
	return this->iGetStream(Offset, (uint8*) &Data, sizeof(Data));
}
uint32 San::stSANSTREAM::iGetStream(const uint32 Offset, uint32 &Data) const
{
	return this->iGetStream(Offset, (uint8*) &Data, sizeof(Data));
}
uint32 San::stSANSTREAM::iGetStream(const uint32 Offset, uint64 &Data) const
{
	return this->iGetStream(Offset, (uint8*) &Data, sizeof(Data));
}
uint32 San::stSANSTREAM::iPopBegin(const uint32 Size)
{
	if (Size >= this->Size)
	{
		this->_ReleaseStream();
		return this->Size;
	}
	uint32 StreamSize = this->Size - Size;
	uint8* pStream = new uint8[StreamSize];
	gloMemSet(pStream, 0, StreamSize);
	gloMemCopy(pStream, this->pStream + Size, this->Size - Size);
	this->_ReleaseStream();
	this->pStream = pStream;
	this->Size = StreamSize;
	return this->Size;
}
uint32 San::stSANSTREAM::iPopEnd(const uint32 Size)
{
	if (Size >= this->Size)
	{
		this->_ReleaseStream();
		return this->Size;
	}
	uint32 StreamSize = this->Size - Size;
	uint8* pStream = new uint8[StreamSize];
	gloMemSet(pStream, 0, StreamSize);
	gloMemCopy(pStream, this->pStream, this->Size - Size);
	this->_ReleaseStream();
	this->pStream = pStream;
	this->Size = StreamSize;
	return this->Size;
}
uint32 San::stSANSTREAM::iReSizeStream(const uint32 Size)
{
	if (Size == 0)
	{
		this->_ReleaseStream();
		return this->Size;
	}
	uint32 StreamSize = Size;
	uint8* p_stream = new uint8[StreamSize];
	gloMemSet(p_stream, 0, StreamSize);
	if (this->pStream != nullptr)
	{
		this->Size = StreamSize > this->Size ? this->Size : StreamSize;
		gloMemCopy(p_stream, this->pStream, this->Size);
		delete[] this->pStream;
		this->pStream = nullptr;
	}
	this->pStream = p_stream;
	this->Size = StreamSize;
	return this->Size;
}
uint32 San::stSANSTREAM::iClear(const uint8 Data)
{
	gloMemSet(this->pStream, Data, this->Size);
	return this->Size;
}