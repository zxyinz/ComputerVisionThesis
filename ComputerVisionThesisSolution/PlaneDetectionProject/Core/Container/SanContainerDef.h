#include"list"
#include"vector"
#include"map"
#include"queue"
#include"stack"
#include"functional"
#include"bitset"
#include"../../RenderSystem.h"
#include"../SanTypes.h"
#pragma once
using namespace std;
namespace San
{
#ifndef __STDSANCONTAINERDEF_H__
#define __STDSANCONTAINERDEF_H__
	struct stSANUNKNOWN
	{
	public:
		SString	description;
		SVALUE	value;
	public:
		stSANUNKNOWN(const SString strDesc = _SSTR(""), const SVALUE &Value = 0);
		~stSANUNKNOWN();

		bool operator==(const stSANUNKNOWN &Variable);
		bool operator!=(const stSANUNKNOWN &Variable);
	};
	typedef stSANUNKNOWN	SANUNKNOWN;
	typedef stSANUNKNOWN*	lpSANUNKNOWN;

	template<typename ST1, typename ST2>
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