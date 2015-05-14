//Project: San Lab Game Engine
//Version: 2.1.1
//Debug State: Add pair
#include"list"
#include"vector"
#include"map"
#include"queue"
#include"stack"
#include"functional"
#include"bitset"
#include"mutex"
#include"atomic"
#include"../SanMemory.h"
#include"../SanTypes.h"
#pragma once
using namespace std;
namespace San
{
#ifndef __STDSANCONTAINERDEF_H__
#define __STDSANCONTAINERDEF_H__
	struct stSANUNKNOWN : public _smemobj<>
	{
	public:
		SString	description;
		SVALUE	value;
	public:
		stSANUNKNOWN(const SString &strDesc = SString(""), const SVALUE &Value = 0);
		~stSANUNKNOWN();

		bool operator==(const stSANUNKNOWN &Variable) const;
		bool operator!=(const stSANUNKNOWN &Variable) const;
	};
	typedef stSANUNKNOWN	SANUNKNOWN;
	typedef stSANUNKNOWN*	lpSANUNKNOWN;

	template<class ST1, class ST2, class _Allocator = cSanSystemAllocator>
	struct _SANUNKNOWN : public _smemobj<_Allocator>
	{
	public:
		/*ST1 first;
		ST2 second;*/
		union{ ST1 description; ST1 first; };
		union{ ST2 value; ST2 second; };
	public:
		_SANUNKNOWN();
		_SANUNKNOWN(const ST1 &First, const ST2 &Second);
		~_SANUNKNOWN();

		bool operator==(const _SANUNKNOWN<ST1, ST2> &Variable) const;
		bool operator!=(const _SANUNKNOWN<ST1, ST2> &Variable) const;
	};

#pragma region _SANUNKNOWN CPP file
	template<class ST1, class ST2, class _Allocator>
	_SANUNKNOWN<ST1, ST2, _Allocator>::_SANUNKNOWN()
	{
	}
	template<class ST1, class ST2, class _Allocator>
	_SANUNKNOWN<ST1, ST2, _Allocator>::_SANUNKNOWN(const ST1 &First, const ST2 &Second)
		:first(First), second(Second)
	{
	}
	template<class ST1, class ST2, class _Allocator>
	_SANUNKNOWN<ST1, ST2, _Allocator>::~_SANUNKNOWN()
	{
	}
	template<class ST1, class ST2, class _Allocator>
	bool _SANUNKNOWN<ST1, ST2, _Allocator>::operator==(const _SANUNKNOWN<ST1, ST2> &Variable) const
	{
		return (this->first == Variable.first) && (this->second == Variable.second);
	}
	template<class ST1, class ST2, class _Allocator>
	bool _SANUNKNOWN<ST1, ST2, _Allocator>::operator!=(const _SANUNKNOWN<ST1, ST2> &Variable) const
	{
		return (this->first != Variable.first) || (this->second != Variable.second);
	}
#pragma endregion

	template<class _First, class _Second, class _Allocator = cSanSystemAllocator>
	using _spair = _SANUNKNOWN<_First, _Second, _Allocator>;

	struct stSANSIMPLELOCK : public _smemobj<>
	{
	private:
		bool bLock;
	public:
		stSANSIMPLELOCK(const bool LockState = false);
		~stSANSIMPLELOCK();

		void iLock();
		void iUnLock();

		void lock();
		void unlock();
	};
	typedef stSANSIMPLELOCK		SANSIMPLELOCK;
	typedef stSANSIMPLELOCK*	lpSANSIMPLELOCK;

	typedef std::mutex SANLOCK;

	template<class ST,class _Allocator = cSanSystemAllocator>
	struct _SANATOMIC : public _smemobj<_Allocator>
	{
	private:
		ST Obj;
		SANLOCK Lock;
	public:
		_SANATOMIC(const ST &Obj);
		_SANATOMIC(const _SANATOMIC<ST, _Allocator> &LockObj) = delete;
		~_SANLOCKOBJ();

		_SANATOMIC& operator=(const _SANATOMIC<ST, _Allocator> &LockObj) = delete;

		_SANATOMIC& operator=(const ST &Obj);

		_SANATOMIC operator+(const ST &Obj);
		_SANATOMIC operator-(const ST &Obj);
		_SANATOMIC operator/(const ST &Obj);

		ST& operator*();
		ST* operator->();
	};

#pragma region _SANATOMIC CPP file
	template<class ST, class _Allocator>
	_SANATOMIC<ST, _Allocator>::_SANATOMIC(const ST &Obj)
		:Obj(Obj)
	{
	}
	template<class ST, class _Allocator>
	_SANATOMIC<ST, _Allocator>::~_SANATOMIC()
	{
	}
	template<class ST, class _Allocator>
	_SANATOMIC<ST, _Allocator>& _SANATOMIC<ST, _Allocator>::operator=(const ST &Obj)
	{
		this->Obj = Obj;
		return *this;
	}
	template<class ST, class _Allocator>
	_SANATOMIC<ST, _Allocator>& _SANATOMIC<ST, _Allocator>::operator+(const ST &Obj)
	{
		this->Obj = this->Obj + Obj;
		return *this;
	}
	template<class ST, class _Allocator>
	_SANATOMIC<ST, _Allocator>& _SANATOMIC<ST, _Allocator>::operator-(const ST &Obj)
	{
		this->Obj = this->Obj - Obj;
		return *this;
	}
	template<class ST, class _Allocator>
	_SANATOMIC<ST, _Allocator>& _SANATOMIC<ST, _Allocator>::operator/(const ST &Obj)
	{
		this->Obj = this->Obj / Obj;
		return *this;
	}
	template<class ST, class _Allocator>
	ST& _SANATOMIC<ST, _Allocator>::operator*()
	{
		return this->Obj;
	}
	template<class ST, class _Allocator>
	ST* _SANATOMIC<ST, _Allocator>::operator->()
	{
		return &this->Obj;
	}
#pragma endregion

	template<class ST,class _Allocator = cSanSystemAllocator>
	using _satomic = atomic<ST>;// _SANATOMIC<ST, _Allocator>;

	struct stSANBYTECONTAINER
	{
		union
		{
			uint8 byte;
			struct
			{
				uint8 b0 : 1;
				uint8 b1 : 1;
				uint8 b2 : 1;
				uint8 b3 : 1;
				uint8 b4 : 1;
				uint8 b5 : 1;
				uint8 b6 : 1;
				uint8 b7 : 1;
			};
		};
	};
	typedef stSANBYTECONTAINER	SANBITCONTAINER;
	typedef stSANBYTECONTAINER*	lpSANBITCONTAINER;
#endif
}