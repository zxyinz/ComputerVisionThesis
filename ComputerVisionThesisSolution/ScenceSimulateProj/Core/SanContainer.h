#include"list"
#include"vector"
#include"map"
#include"stack"
#include"bitset"
#include"../RenderSystem.h"
#include"SanTypes.h"
#pragma once
namespace San
{
#ifndef __STDSANCONTAINER_H__
#define __STDSANCONTAINER_H__
	/*struct stSANUNKNOWN
	{
		SANVARIABLE	description;
		SANVARIABLE	value;
	};
	typedef stSANUNKNOWN	SANUNKNOWN;
	typedef stSANUNKNOWN*	lpSANUNKNOWN;*/
	struct stSANUNKNOWN
	{
		SString	description;
		SVALUE	value;
	};
	typedef stSANUNKNOWN	SANUNKNOWN;
	typedef stSANUNKNOWN*	lpSANUNKNOWN;

	template<typename ST1,typename ST2>
	struct stSANUNKNOWNEX
	{
	public:
		ST1 description;
		ST2 value;
	public:
		stSANUNKNOWNEX(){};
		stSANUNKNOWNEX(const ST1 &Description, const ST2 &Value)
			:description(Description),
			value(Value)
		{};
		~stSANUNKNOWNEX(){};
	};

	//----------------------------C Container Definition---------------------------//

	struct st_san_array
	{
		uint32*	p_data_buffer;
		uint32*	p_weight_buffer;
		uint32	array_size;
	};

	struct st_san_list
	{
		st_san_list*	p_prev;
		st_san_list*	p_next;
		uint32	weight;
		SHANDLE	data_ptr;
	};

	typedef st_san_list	st_san_queue;
	typedef st_san_list	st_san_stack;

	struct st_san_tree
	{
		st_san_tree* p_trunk;
		st_san_tree* p_offshoot_list;
		uint32	list_size;
		uint32	index;
		uint32	weight;
		SHANDLE	data_ptr;
	};
	//-----------------------------------------------------------------------------//

	template<class ST>
	struct stSANOCTREE
	{
		stSANOCTREE<ST>*	p_trunk;
		stSANOCTREE<ST>*	p_offshootarray[8];
		ST		data;
		uint32	weight;
		uint32	identify;
	};

	template<class ST>
	struct stSANTREEEX
	{
		stSANTREEEX<ST>*		p_trunk;
		list<stSANTREEEX<ST>*>	p_offshootlist;
		ST		data;
		uint32	weight;
		uint32	identify;
		uint32	listsize;
	};

	struct stSANSTREAM
	{
	public:
		uint8* pStream;
		uint32 Size;
	protected:
		void _ReleaseStream();
	public:
		stSANSTREAM(const uint32 Size = 2);
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

	template<typename ST>
	struct stSANSTREAMEX
	{
	public:
		ST* pStream;
		uint32 Size;
	protected:
		void _ReleaseStream()
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
		};
	public:
		stSANSTREAMEX(const uint32 Size = 2)
			:pStream(nullptr),
			Size(Size)
		{
			if (Size != 0)
			{
				this->pStream = new ST[this->Size];
			}
		};
		stSANSTREAMEX(const stSANSTREAMEX<ST>& Stream)
			:pStream(nullptr),
			Size(Stream.Size)
		{
			if (this->Size != 0)
			{
				this->pStream = new ST[this->Size];
				for (uint32 seek = 0; seek < this->Size; seek = seek + 1)
				{
					this->pStream[seek] = Stream.pStream[seek];
				}
			}
		};
		stSANSTREAMEX(const ST* pStream, const uint32 BufSize)
			:pStream(nullptr),
			Size(BufSize)
		{
			if (this->Size != 0)
			{
				this->pStream = new ST[this->Size];
				for (uint32 seek = 0; seek < this->Size; seek = seek + 1)
				{
					this->pStream[seek] = pStream[seek];
				}
			}
		};
		~stSANSTREAMEX()
		{
			this->_ReleaseStream();
		};

		//operators overload
		stSANSTREAMEX& operator=(const stSANSTREAMEX<ST>& Stream)
		{
			if ((Stream.pStream == nullptr) || (Stream.Size == 0))
			{
				this->_ReleaseStream();
				return *this;
			}
			uint32 Size = Stream.Size;
			ST* pStream = new ST[Size];
			if (this->pStream != nullptr)
			{
				for (uint32 seek = 0; seek < Stream.Size; seek = seek + 1)
				{
					pStream[seek] = Stream.pStream[seek];
				}
				this->_ReleaseStream();
			}
			this->pStream = pStream;
			this->Size = Size;
			return *this;
		};
		stSANSTREAMEX operator+(const stSANSTREAMEX<ST>& Stream)
		{
			if ((Stream.pStream == nullptr) || (Stream.Size == 0))
			{
				return *this;
			}
			uint32 Size = this->Size + Stream.Size;
			stSANSTREAMEX<ST> stm(Size);
			for (uint32 seek = 0; seek < this->Size; seek = seek + 1)
			{
				stm.pStream[seek] = this->pStream[seek];
			}
			for (uint32 seek = 0; seek < Stream.Size; seek = seek + 1)
			{
				stm.pStream[seek + this->Size] = Stream.pStream[seek];
			}
			return stm;
		};
		stSANSTREAMEX operator-(const uint32 Size)
		{
			this->iPopBegin(Size);
			return *this;
		};
		bool operator==(const stSANSTREAMEX<ST>& Stream)
		{
			if (this->Size != Stream->Size)
			{
				return false;
			}
			for (uint32 seek = 0; seek < this->Size; seek = seek + 1)
			{
				if (this->pStream[seek] != Stream->pStream[seek])
				{
					return false;
				}
			}
			return true;
		};
		bool operator!=(const stSANSTREAMEX<ST>& Stream)
		{
			if (this->Size != Stream->Size)
			{
				return true;
			}
			for (uint32 seek = 0; seek < this->Size; seek = seek + 1)
			{
				if (this->pStream[seek] == Stream->pStream[seek])
				{
					return false;
				}
			}
			return true;
		};
		const ST& operator[](const size_t Position) const
		{
			return this->pStream[Position];
		};
		ST& operator[](const size_t Position)
		{
			return this->pStream[Position];
		};

		//Stream IO Function
		uint32 iSetStream(const uint32 Offset, const stSANSTREAMEX<ST>& Stream)
		{
			uint32 StreamSize = Stream.Size;
			if (StreamSize == 0)
			{
				return 0;
			}
			this->iSetStream(Offset, Stream.pStream, StreamSize);
		};
		uint32 iSetStream(const uint32 Offset, const ST* pBuffer, const uint32 BufSize)
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
		};
		uint32 iSetStream(const uint32 Offset, const ST &Data)
		{
			return this->iSetStream(Offset, (ST*) &Data, sizeof(Data));
		};
		uint32 iGetStream(const uint32 Offset, ST* pBuffer, const uint32 BufSize) const
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
		};
		uint32 iGetStream(const uint32 Offset, ST &Data) const
		{
			return this->iGetStream(Offset, (ST*) &Data, sizeof(Data));
		};
		uint32 iPopBegin(const uint32 Size)
		{
			if (Size >= this->Size)
			{
				this->_ReleaseStream();
				return this->Size;
			}
			uint32 StreamSize = this->Size - Size;
			StreamSize = StreamSize < 2 ? 2 : StreamSize;
			ST* pStream = new ST[StreamSize];
			uint32 Length = this->Size - Size;
			for (uint32 seek = 0; seek < Length, seek = seek + 1)
			{
				pStream[seek] = this->pStream[seek + Size];
			}
			this->_ReleaseStream();
			this->pStream = pStream;
			this->Size = StreamSize;
			return this->Size;
		};
		uint32 iPopEnd(const uint32 Size)
		{
			if (Size >= this->Size)
			{
				this->_ReleaseStream();
				return this->Size;
			}
			uint32 StreamSize = this->Size - Size;
			StreamSize = StreamSize < 2 ? 2 : StreamSize;
			ST* pStream = new ST[StreamSize];
			uint32 Length = this->Size - Size;
			for (uint32 seek = 0; seek < Length, seek = seek + 1)
			{
				pStream[seek] = this->pStream[seek + Size];
			}
			this->_ReleaseStream();
			this->pStream = pStream;
			this->Size = StreamSize;
			return this->Size;
		};
		uint32 iReSizeStream(const uint32 Size)
		{
			if (Size == 0)
			{
				if (this->pStream != nullptr)
				{
					this->_ReleaseStream();
					return this->Size;
				}
			}
			uint32 StreamSize = Size;
			ST* p_stream = new ST[StreamSize];
			if (this->pStream != nullptr)
			{
				this->Size = StreamSize > this->Size ? this->Size : StreamSize;
				for (uint32 seek = 0; seek < this->Size; seek = seek + 1)
				{
					p_stream[seek] = this->pStream[seek];
				}
				delete[] this->pStream;
				this->pStream = nullptr;
			}
			this->pStream = p_stream;
			this->Size = StreamSize;
			return this->Size;
		};
		uint32 iGetSizeInByte() const
		{
			return sizeof(ST) *this->Size;
		};
		uint32 iClear(const ST &Data)
		{
			for (uint32 seek = 0; seek < this->Size; seek = seek + 1)
			{
				this->pStream[seek] = Data;
			}
			return this->Size;
		};
	};
	//typedef stSANSTREAMEX	SANSTREAMEX;
	//typedef stSANSTREAMEX* lpSANSTREAMEX;

#endif
}