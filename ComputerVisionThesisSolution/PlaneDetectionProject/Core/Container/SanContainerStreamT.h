//Project: San Lab Game Engine
//Version: 2.1.1
//Debug State: Need test [_sstream iterator]
#include"SanContainerDef.h"
#pragma once
using namespace std;
namespace San
{
#ifndef __STDCONTAINERSTREAMT_H__
#define __STDCONTAINERSTREAMT_H__
	template<class ST, class _Ref, class _Ptr>
	struct _SANSTREAMEX_ITERATOR
	{
	private:
		const _Ptr stream_start_ptr;
		const _Ptr stream_end_ptr;
		_Ptr p_current;
	public:
		_SANSTREAMEX_ITERATOR(const _Ptr pStreamStart = nullptr, const _Ptr pStreamEnd = nullptr, const _Ptr pCurrentElement = nullptr);
		_SANSTREAMEX_ITERATOR(const _SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr> &Iterator);
		~_SANSTREAMEX_ITERATOR();

		_SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr>& operator=(const _SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr> &Iterator);

		bool operator==(const _SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr> &Iterator) const;
		bool operator!=(const _SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr> &Iterator) const;

		_Ref operator*();
		_Ptr operator->();

		_SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr>& operator++();
		_SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr> operator++(int Val);
		_SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr>& operator--();
		_SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr> operator--(int Val);
	};

#pragma region _SANSTREAMEX_ITERATOR CPP file
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr>::_SANSTREAMEX_ITERATOR(const _Ptr pStreamStart, const _Ptr pStreamEnd, const _Ptr pCurrentElement)
		:stream_start_ptr(pStreamStart),
		stream_end_ptr(pStreamEnd),
		p_current(pCurrentElement)
	{
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr>::_SANSTREAMEX_ITERATOR(const _SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr> &Iterator)
		:stream_start_ptr(Iterator.stream_start_ptr),
		stream_end_ptr(Iterator.stream_end_ptr),
		p_current(Iterator.p_current)
	{
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr>::~_SANSTREAMEX_ITERATOR()
	{
		this->stream_start_ptr = nullptr;
		this->stream_end_ptr = nullptr;
		this->p_current = nullptr;
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr>& _SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr>::operator=(const _SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr> &Iterator)
	{
		this->stream_start_ptr = Iterator.stream_start_ptr;
		this->stream_end_ptr = Iterator.stream_end_ptr;
		this->p_current = Iterator.p_current;

		return *this;
	}
	template<class ST, class _Ref, class _Ptr>
	bool _SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr>::operator==(const _SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr> &Iterator) const
	{
		if (this->p_current != Iterator.p_current){ return false; }

		if (this->stream_start_ptr != Iterator.stream_start_ptr){ return false; }
		if (this->stream_end_ptr != Iterator.stream_end_ptr){ return false; }

		return true;
	}
	template<class ST, class _Ref, class _Ptr>
	bool _SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr>::operator!=(const _SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr> &Iterator) const
	{
		if (this->p_current != Iterator.p_current){ return true; }

		if (this->stream_start_ptr != Iterator.stream_start_ptr){ return true; }
		if (this->stream_end_ptr != Iterator.stream_end_ptr){ return true; }

		return false;
	}
	template<class ST, class _Ref, class _Ptr>
	_Ref _SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr>::operator*()
	{
		return *this->p_current;
	}
	template<class ST, class _Ref, class _Ptr>
	_Ptr _SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr>::operator->()
	{
		return this->p_current;
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr>& _SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr>::operator++()
	{
		if ((this->p_current == nullptr) || (this->stream_start_ptr == nullptr) || (this->stream_end_ptr == nullptr))
		{
			return *this;
		}

		this->p_current = this->p_current + 1;
		this->p_current = this->p_current >= this->stream_end_ptr ? nullptr : this->p_current;

		return *this;
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr> _SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr>::operator++(int Val)
	{
		_SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr> OrigionalIterator(*this);

		this->operator++();

		return OrigionalIterator;
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr>& _SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr>::operator--()
	{
		if ((this->p_current == nullptr) || (this->stream_start_ptr == nullptr) || (this->stream_end_ptr == nullptr))
		{
			return *this;
		}

		if (this->p_current < sizeof(ST))
		{
			this->p_current = nullptr;
			return *this;
		}

		this->p_current = this->p_current - 1;
		this->p_current = this->p_current < this->stream_start_ptr ? nullptr : this->p_current;

		return *this;
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr> _SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr>::operator--(int Val)
	{
		_SANSTREAMEX_ITERATOR<ST, _Ref, _Ptr> OrigionalIterator(*this);

		this->operator--();

		return OrigionalIterator;
	}
#pragma endregion

	template<class ST, class _Ref, class _Ptr>
	struct _SANSTREAMEX_ITERATOR_S
	{
	private:
		const _Ptr* p_stream_start_ptr;
		const uint32* stream_size_ptr;
		//const ST* stream_end_ptr;
		_Ptr p_current;
	public:
		_SANSTREAMEX_ITERATOR_S(const _Ptr* pStreamStartPtr = nullptr, const uint32* pStreamSizePtr = nullptr, const _Ptr pCurrentElement = nullptr);
		_SANSTREAMEX_ITERATOR_S(const _SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr> &Iterator);
		~_SANSTREAMEX_ITERATOR_S();

		_SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr>& operator=(const _SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr> &Iterator);

		bool operator==(const _SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr> &Iterator) const;
		bool operator!=(const _SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr> &Iterator) const;

		_Ref operator*();
		_Ptr operator->();

		_SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr>& operator++();
		_SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr> operator++(int Val);
		_SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr>& operator--();
		_SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr> operator--(int Val);
	};

#pragma region _SANSTREAMEX_ITERATOR CPP file
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr>::_SANSTREAMEX_ITERATOR_S(const _Ptr* pStreamStartPtr, const uint32* pStreamSizePtr, const _Ptr pCurrentElement)
		:p_stream_start_ptr(pStreamStartPtr),
		//stream_end_ptr(pStreamSizePtr),
		p_current(pCurrentElement)
	{
		//this->stream_end_ptr = this->p_stream_start_ptr + (*pStreamSizePtr);
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr>::_SANSTREAMEX_ITERATOR_S(const _SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr> &Iterator)
		:p_stream_start_ptr(Iterator.p_stream_start_ptr),
		stream_size_ptr(Iterator.stream_size_ptr),
		//stream_end_ptr(Iterator.stream_end_ptr),
		p_current(Iterator.p_current)
	{
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr>::~_SANSTREAMEX_ITERATOR_S()
	{
		this->p_stream_start_ptr = nullptr;
		this->stream_size_ptr = nullptr;
		//this->stream_end_ptr = nullptr;
		this->p_current = nullptr;
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr>& _SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr>::operator=(const _SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr> &Iterator)
	{
		this->p_stream_start_ptr = Iterator.p_stream_start_ptr;
		this->stream_size_ptr = Iterator.stream_size_ptr;
		//this->stream_end_ptr = Iterator.stream_end_ptr;
		this->p_current = Iterator.p_current;

		return *this;
	}
	template<class ST, class _Ref, class _Ptr>
	bool _SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr>::operator==(const _SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr> &Iterator) const
	{
		if (this->p_current != Iterator.p_current){ return false; }

		if (this->p_stream_start_ptr != Iterator.p_stream_start_ptr){ return false; }
		if (this->stream_size_ptr != Iterator.stream_size_ptr){ return false; }

		return true;
	}
	template<class ST, class _Ref, class _Ptr>
	bool _SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr>::operator!=(const _SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr> &Iterator) const
	{
		if (this->p_current != Iterator.p_current){ return true; }

		if (this->p_stream_start_ptr != Iterator.p_stream_start_ptr){ return true; }
		if (this->stream_size_ptr != Iterator.stream_size_ptr){ return true; }

		return false;
	}
	template<class ST, class _Ref, class _Ptr>
	_Ref _SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr>::operator*()
	{
		return *this->p_current;
	}
	template<class ST, class _Ref, class _Ptr>
	_Ptr _SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr>::operator->()
	{
		return this->p_current;
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr>& _SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr>::operator++()
	{
		if ((this->p_current == nullptr) || (this->p_stream_start_ptr == nullptr) || (this->stream_size_ptr == nullptr))
		{
			return *this;
		}

		_Ptr stream_end_ptr = (*this->p_stream_start_ptr) + (*this->stream_size_ptr);

		this->p_current = this->p_current + 1;
		this->p_current = this->p_current >= stream_end_ptr ? nullptr : this->p_current;

		return *this;
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr> _SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr>::operator++(int Val)
	{
		_SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr> OrigionalIterator(*this);

		this->operator++();

		return OrigionalIterator;
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr>& _SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr>::operator--()
	{
		if ((this->p_current == nullptr) || (this->p_stream_start_ptr == nullptr) || (this->stream_size_ptr == nullptr))
		{
			return *this;
		}

		if (this->p_current < sizeof(ST))
		{
			this->p_current = nullptr;
			return *this;
		}

		this->p_current = this->p_current - 1;
		this->p_current = this->p_current < this->p_stream_start_ptr ? nullptr : this->p_current;

		return *this;
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr> _SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr>::operator--(int Val)
	{
		_SANSTREAMEX_ITERATOR_S<ST, _Ref, _Ptr> OrigionalIterator(*this);

		this->operator--();

		return OrigionalIterator;
	}
#pragma endregion

	template<class ST, class _Ref, class _Ptr>
	struct _SANSTREAMEX_ITERATOR_B
	{
	private:
		//const ST* stream_start_ptr;
		//const ST* stream_end_ptr;
		const uint8* byte_stream_start_ptr;
		const uint8* byte_stream_end_ptr;
		//ST* p_current;
		uint8* p_current_byte;
	public:
		_SANSTREAMEX_ITERATOR_B(const _Ptr pStreamStart = nullptr, const uint32 StreamSize = 0, const uint8* pCurrentByte = nullptr);
		_SANSTREAMEX_ITERATOR_B(const _SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr> &Iterator);
		~_SANSTREAMEX_ITERATOR_B();

		_SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr>& operator=(const _SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr> &Iterator);

		bool operator==(const _SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr> &Iterator) const;
		bool operator!=(const _SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr> &Iterator) const;

		uint8& operator*();
		uint8* operator->();

		_SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr>& operator++();
		_SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr> operator++(int Val);
		_SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr>& operator--();
		_SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr> operator--(int Val);
	};

#pragma region _SANSTREAMEX_ITERATOR_B CPP file
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr>::_SANSTREAMEX_ITERATOR_B(const _Ptr pStreamStart, const uint32 StreamSize, const uint8* pCurrentByte)
		:byte_stream_start_ptr((uint8*) pStreamStart),
		byte_stream_end_ptr(((uint8*) pStreamStart) + (StreamSize*sizeof(ST))),
		p_current_byte(pCurrentByte)
	{
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr>::_SANSTREAMEX_ITERATOR_B(const _SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr> &Iterator)
		:byte_stream_start_ptr(Iterator.byte_stream_start_ptr),
		byte_stream_end_ptr(Iterator.byte_stream_end_ptr),
		p_current_byte(Iterator.p_current_byte)
	{
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr>::~_SANSTREAMEX_ITERATOR_B()
	{
		this->byte_stream_start_ptr = nullptr;
		this->byte_stream_end_ptr = nullptr;
		this->p_current_byte = nullptr;
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr>& _SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr>::operator=(const _SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr> &Iterator)
	{
		this->byte_stream_start_ptr = Iterator.byte_stream_start_ptr;
		this->byte_stream_end_ptr = Iterator.byte_stream_end_ptr;
		this->p_current_byte = Iterator.p_current_byte;

		return *this;
	}
	template<class ST, class _Ref, class _Ptr>
	bool _SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr>::operator==(const _SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr> &Iterator) const
	{
		if (this->p_current_byte != Iterator.p_current_byte){ return false; }

		if (this->byte_stream_start_ptr != Iterator.byte_stream_start_ptr){ return false; }
		if (this->byte_stream_end_ptr != Iterator.byte_stream_end_ptr){ return false; }

		return true;
	}
	template<class ST, class _Ref, class _Ptr>
	bool _SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr>::operator!=(const _SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr> &Iterator) const
	{
		if (this->p_current_byte != Iterator.p_current_byte){ return true; }

		if (this->byte_stream_start_ptr != Iterator.byte_stream_start_ptr){ return true; }
		if (this->byte_stream_end_ptr != Iterator.byte_stream_end_ptr){ return true; }

		return false;
	}
	template<class ST, class _Ref, class _Ptr>
	uint8& _SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr>::operator*()
	{
		return *this->p_current_byte;
	}
	template<class ST, class _Ref, class _Ptr>
	uint8* _SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr>::operator->()
	{
		return this->p_current_byte;
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr>& _SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr>::operator++()
	{
		if ((this->p_current_byte == nullptr) || (this->byte_stream_start_ptr == nullptr) || (this->byte_stream_end_ptr == nullptr))
		{
			return *this;
		}

		this->p_current_byte = this->p_current_byte + 1;
		this->p_current_byte = this->p_current_byte >= this->byte_stream_end_ptr ? nullptr : this->p_current_byte;

		return *this;
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr> _SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr>::operator++(int Val)
	{
		_SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr> OrigionalIterator(*this);

		this->operator++();

		return OrigionalIterator;
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr>& _SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr>::operator--()
	{
		if ((this->p_current_byte == nullptr) || (this->byte_stream_start_ptr == nullptr) || (this->byte_stream_end_ptr == nullptr))
		{
			return *this;
		}

		this->p_current_byte = this->p_current_byte - 1;
		this->p_current_byte = this->p_current_byte < this->byte_stream_start_ptr ? nullptr : this->p_current_byte;

		return *this;
	}
	template<class ST, class _Ref, class _Ptr>
	_SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr> _SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr>::operator--(int Val)
	{
		_SANSTREAMEX_ITERATOR_B<ST, _Ref, _Ptr> OrigionalIterator(*this);

		this->operator--();

		return OrigionalIterator;
	}
#pragma endregion

	template<class ST>
	struct _stSANSTREAMEX
	{
	public:
		ST* pStream;
		uint32 Size;
		uint32 SizeInByte;
	public:
		typedef _SANSTREAMEX_ITERATOR<ST, ST&, ST*> iterator;
		typedef _SANSTREAMEX_ITERATOR_S<ST, ST&, ST*> safe_iterator;
		typedef _SANSTREAMEX_ITERATOR_B<ST, ST&, ST*> byte_iterator;
	protected:
		void _ReleaseStream();
	public:
		_stSANSTREAMEX(const uint32 Size = 2);
		_stSANSTREAMEX(const uint32 Size, const ST &InitializeValue);
		_stSANSTREAMEX(const _stSANSTREAMEX<ST>& Stream);
		_stSANSTREAMEX(const ST* pStream, const uint32 BufSize);
		~_stSANSTREAMEX();

		//operators overload
		_stSANSTREAMEX& operator=(const _stSANSTREAMEX<ST>& Stream);
		_stSANSTREAMEX operator+(const _stSANSTREAMEX<ST>& Stream);
		_stSANSTREAMEX operator-(const uint32 Size);
		bool operator==(const _stSANSTREAMEX<ST>& Stream);
		bool operator!=(const _stSANSTREAMEX<ST>& Stream);
		const ST& operator[](const size_t Position) const;
		ST& operator[](const size_t Position);

		//Stream IO Function
		uint32 iSetStream(const uint32 Offset, const _stSANSTREAMEX<ST>& Stream);
		uint32 iSetStream(const uint32 Offset, const ST* pBuffer, const uint32 BufSize);
		uint32 iSetStream(const uint32 Offset, const ST &Data);
		uint32 iGetStream(const uint32 Offset, ST* pBuffer, const uint32 BufSize) const;
		uint32 iGetStream(const uint32 Offset, ST &Data) const;
		uint32 iPopBegin(const uint32 Size);
		uint32 iPopEnd(const uint32 Size);
		uint32 iReSizeStream(const uint32 Size);
		uint32 iGetSizeInByte() const;
		uint32 iClear(const ST &Data);

		//C++ STL Style Iterator Function
		typename _stSANSTREAMEX<ST>::iterator begin() const;
		typename _stSANSTREAMEX<ST>::iterator end() const;
		typename _stSANSTREAMEX<ST>::iterator find(const ST &Target) const;
		uint32 size() const;

		typename _stSANSTREAMEX<ST>::safe_iterator begin_s() const;
		typename _stSANSTREAMEX<ST>::safe_iterator end_s() const;
		typename _stSANSTREAMEX<ST>::safe_iterator find_s(const ST &Target) const;
		uint32 size_s() const;

		typename _stSANSTREAMEX<ST>::byte_iterator begin_b() const;
		typename _stSANSTREAMEX<ST>::byte_iterator end_b() const;
		typename _stSANSTREAMEX<ST>::byte_iterator find_b(const uint8 &TargetByte) const;
		uint32 size_b() const;
	};

#pragma region _stSANSTREAMEX CPP file
	template<class ST>
	_stSANSTREAMEX<ST>::_stSANSTREAMEX(const uint32 Size)
		:pStream(nullptr), Size(Size), SizeInByte(sizeof(ST) *Size)
	{
		if (Size != 0){ this->pStream = new ST[this->Size]; }
	}
	template<class ST>
	_stSANSTREAMEX<ST>::_stSANSTREAMEX(const uint32 Size, const ST &InitializeValue)
		:pStream(nullptr), Size(Size), SizeInByte(sizeof(ST) *Size)
	{
		if (Size != 0){ this->pStream = new ST[this->Size]; }

		for (uint32 seek = 0; seek < this->Size; seek = seek + 1)
		{
			this->pStream[seek] = InitializeValue;
		}
	}
	template<class ST>
	_stSANSTREAMEX<ST>::_stSANSTREAMEX(const _stSANSTREAMEX<ST>& Stream)
		:pStream(nullptr),
		Size(Stream.Size),
		SizeInByte(sizeof(ST) *Size)
	{
		if (this->Size != 0)
		{
			this->pStream = new ST[this->Size];
			for (uint32 seek = 0; seek < this->Size; seek = seek + 1)
			{
				this->pStream[seek] = Stream.pStream[seek];
			}
		}
	}
	template<class ST>
	_stSANSTREAMEX<ST>::_stSANSTREAMEX(const ST* pStream, const uint32 BufSize)
		:pStream(nullptr),
		Size(BufSize),
		SizeInByte(sizeof(ST) *Size)
	{
		if (this->Size != 0)
		{
			this->pStream = new ST[this->Size];
			for (uint32 seek = 0; seek < this->Size; seek = seek + 1)
			{
				this->pStream[seek] = pStream[seek];
			}
		}
	}
	template<class ST>
	_stSANSTREAMEX<ST>::~_stSANSTREAMEX()
	{
		this->_ReleaseStream();
	}
	template<class ST>
	void _stSANSTREAMEX<ST>::_ReleaseStream()
	{
		if (this->pStream != nullptr)
		{
			this->Size == 1 ? (delete this->pStream) : (delete[] this->pStream);
		}

		this->pStream = nullptr;
		this->Size = 0;
		this->SizeInByte = 0;
	}
	template<class ST>
	_stSANSTREAMEX<ST>& _stSANSTREAMEX<ST>::operator=(const _stSANSTREAMEX<ST>& Stream)
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
		this->SizeInByte = sizeof(ST) *this->Size;
		return *this;
	}
	template<class ST>
	_stSANSTREAMEX<ST> _stSANSTREAMEX<ST>::operator+(const _stSANSTREAMEX<ST>& Stream)
	{
		if ((Stream.pStream == nullptr) || (Stream.Size == 0)){ return *this; }

		uint32 Size = this->Size + Stream.Size;
		_stSANSTREAMEX<ST> stm(Size);

		for (uint32 seek = 0; seek < this->Size; seek = seek + 1)
		{
			stm.pStream[seek] = this->pStream[seek];
		}
		for (uint32 seek = 0; seek < Stream.Size; seek = seek + 1)
		{
			stm.pStream[seek + this->Size] = Stream.pStream[seek];
		}
		return stm;
	}
	template<class ST>
	_stSANSTREAMEX<ST> _stSANSTREAMEX<ST>::operator-(const uint32 Size)
	{
		this->iPopBegin(Size);
		return *this;
	}
	template<class ST>
	bool _stSANSTREAMEX<ST>::operator==(const _stSANSTREAMEX<ST>& Stream)
	{
		if (this->Size != Stream->Size){ return false; }

		for (uint32 seek = 0; seek < this->Size; seek = seek + 1)
		{
			if (this->pStream[seek] != Stream->pStream[seek])
			{
				return false;
			}
		}
		return true;
	}
	template<class ST>
	bool _stSANSTREAMEX<ST>::operator!=(const _stSANSTREAMEX<ST>& Stream)
	{
		if (this->Size != Stream->Size){ return true; }

		for (uint32 seek = 0; seek < this->Size; seek = seek + 1)
		{
			if (this->pStream[seek] == Stream->pStream[seek])
			{
				return false;
			}
		}
		return true;
	}
	template<class ST>
	const ST& _stSANSTREAMEX<ST>::operator[](const size_t Position) const
	{
		return this->pStream[Position];
	}
	template<class ST>
	ST& _stSANSTREAMEX<ST>::operator[](const size_t Position)
	{
		return this->pStream[Position];
	}
	template<class ST>
	uint32 _stSANSTREAMEX<ST>::iSetStream(const uint32 Offset, const _stSANSTREAMEX<ST>& Stream)
	{
		uint32 StreamSize = Stream.Size;

		if (StreamSize == 0){ return 0; }
		this->iSetStream(Offset, Stream.pStream, StreamSize);
	}
	template<class ST>
	uint32 _stSANSTREAMEX<ST>::iSetStream(const uint32 Offset, const ST* pBuffer, const uint32 BufSize)
	{
		if ((pBuffer == nullptr) || (BufSize == 0)){ return 0; }
		if (Offset >= this->Size){ return 0; }

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
	template<class ST>
	uint32 _stSANSTREAMEX<ST>::iSetStream(const uint32 Offset, const ST &Data)
	{
		return this->iSetStream(Offset, (ST*) &Data, sizeof(Data));
	}
	template<class ST>
	uint32 _stSANSTREAMEX<ST>::iGetStream(const uint32 Offset, ST* pBuffer, const uint32 BufSize) const
	{
		if ((pBuffer == nullptr) || (BufSize == 0)){ return 0; }
		if (Offset >= this->Size){ return 0; }

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
	template<class ST>
	uint32 _stSANSTREAMEX<ST>::iGetStream(const uint32 Offset, ST &Data) const
	{
		return this->iGetStream(Offset, (ST*) &Data, sizeof(Data));
	}
	template<class ST>
	uint32 _stSANSTREAMEX<ST>::iPopBegin(const uint32 Size)
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
		for (uint32 seek = 0; seek < Length; seek = seek + 1)
		{
			pStream[seek] = this->pStream[seek + Size];
		}
		this->_ReleaseStream();
		this->pStream = pStream;
		this->Size = StreamSize;
		this->SizeInByte = sizeof(ST) *this->Size;
		return this->Size;
	}
	template<class ST>
	uint32 _stSANSTREAMEX<ST>::iPopEnd(const uint32 Size)
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
		for (uint32 seek = 0; seek < Length; seek = seek + 1)
		{
			pStream[seek] = this->pStream[seek + Size];
		}
		this->_ReleaseStream();
		this->pStream = pStream;
		this->Size = StreamSize;
		this->SizeInByte = sizeof(ST) *this->Size;
		return this->Size;
	}
	template<class ST>
	uint32 _stSANSTREAMEX<ST>::iReSizeStream(const uint32 Size)
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
		this->SizeInByte = sizeof(ST) *this->Size;
		return this->Size;
	}
	template<class ST>
	uint32 _stSANSTREAMEX<ST>::iGetSizeInByte() const
	{
		return this->SizeInByte;
	}
	template<class ST>
	uint32 _stSANSTREAMEX<ST>::iClear(const ST &Data)
	{
		for (uint32 seek = 0; seek < this->Size; seek = seek + 1)
		{
			this->pStream[seek] = Data;
		}
		return this->Size;
	}
	template<class ST>
	typename _stSANSTREAMEX<ST>::iterator _stSANSTREAMEX<ST>::begin() const
	{
		return _stSANSTREAMEX<ST>::iterator(this->pStream, this->pStream + this->Size, this->pStream);
	}
	template<class ST>
	typename _stSANSTREAMEX<ST>::iterator _stSANSTREAMEX<ST>::end() const
	{
		return _stSANSTREAMEX<ST>::iterator(this->pStream, this->pStream + this->Size, nullptr);
	}
	template<class ST>
	typename _stSANSTREAMEX<ST>::iterator _stSANSTREAMEX<ST>::find(const ST &Target) const
	{
		for (int32 seek = 0; seek < this->Size; seek = seek + 1)
		{
			if (this->pStream[seek] == Target)
			{
				return _stSANSTREAMEX<ST>::iterator(this->pStream, this->pStream + this->Size, this->pStream + seek);
			}
		}
		return _stSANSTREAMEX<ST>::iterator(this->pStream, this->pStream + this->Size, nullptr);
	}
	template<class ST>
	uint32 _stSANSTREAMEX<ST>::size() const
	{
		return this->size;
	}
	template<class ST>
	typename _stSANSTREAMEX<ST>::safe_iterator _stSANSTREAMEX<ST>::begin_s() const
	{
		return _stSANSTREAMEX<ST>::safe_iterator(this->pStream, this->Size, this->pStream);
	}
	template<class ST>
	typename _stSANSTREAMEX<ST>::safe_iterator _stSANSTREAMEX<ST>::end_s() const
	{
		return _stSANSTREAMEX<ST>::safe_iterator(this->pStream, this->Size, nullptr);
	}
	template<class ST>
	typename _stSANSTREAMEX<ST>::safe_iterator _stSANSTREAMEX<ST>::find_s(const ST &Target) const
	{
		for (int32 seek = 0; seek < this->Size; seek = seek + 1)
		{
			if (this->pStream[seek] == Target)
			{
				return _stSANSTREAMEX<ST>::safe_iterator(this->pStream, this->Size, this->pStream + seek);
			}
		}
		return _stSANSTREAMEX<ST>::safe_iterator(this->pStream, this->Size, nullptr);
	}
	template<class ST>
	uint32 _stSANSTREAMEX<ST>::size_s() const
	{
		return this->Size;
	}
	template<class ST>
	typename _stSANSTREAMEX<ST>::byte_iterator _stSANSTREAMEX<ST>::begin_b() const
	{
		return _stSANSTREAMEX<ST>::byte_iterator(this->pStream, this->Size, this->pStream);
	}
	template<class ST>
	typename _stSANSTREAMEX<ST>::byte_iterator _stSANSTREAMEX<ST>::end_b() const
	{
		return _stSANSTREAMEX<ST>::byte_iterator(this->pStream, this->Size, nullptr);
	}
	template<class ST>
	typename _stSANSTREAMEX<ST>::byte_iterator _stSANSTREAMEX<ST>::find_b(const uint8 &TargetByte) const
	{
		const uint8* pByteStream = this->pStream;

		for (int32 seek = 0; seek < this->SizeInByte; seek = seek + 1)
		{
			if (pByteStream[seek] == TargetByte)
			{
				return _stSANSTREAMEX<ST>::byte_iterator(this->pStream, this->Size, pByteStream + seek);
			}
		}
		return _stSANSTREAMEX<ST>::byte_iterator(this->pStream, this->Size, nullptr);
	}
	template<class ST>
	uint32 _stSANSTREAMEX<ST>::size_b() const
	{
		return this->SizeInByte;
	}
#pragma endregion

	template<class ST>
	using _SANSTREAMEX = _stSANSTREAMEX<ST>;
	template<class ST>
	using _sstream = _stSANSTREAMEX<ST>;
#endif
}