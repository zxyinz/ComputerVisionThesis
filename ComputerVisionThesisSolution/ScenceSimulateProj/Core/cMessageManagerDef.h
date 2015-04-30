#include"SanMemory.h"
#include"SanTypes.h"
#include"SanContainer.h"
#include"cVectorManager.h"
#include"../Device/cKeyBoardDevice.h"
#include"../Device/cMouseDevice.h"
#include"../Device/cSanVisualKeyBoard.h"
#include"../Device/cSanVisualMouse.h"
//#include"../Network/cSanNetManager.h"
#include"MsgDef.h"
#pragma once
using namespace std;
namespace San
{
#ifndef __CSANMESSAGEMANAGERDEF_H__
#define __CSANMESSAGEMANAGERDEF_H__

	enum eSANMESSAGETYPE
	{
		MT_KEYBOARD		= 0x01000000,
		MT_MOUSE		= 0x02000000,
		MT_NETWORK		= 0x04000000,
		MT_SYSTEM		= 0x08000000,
		MT_MSGNODE		= 0x10000000,
		MT_USERDEFMASK	= 0x80000000,
		MT_MASK			= 0x00ffffff,
	};

	static const uint32 MIN_AVAILABLEMSGLEVEL	= 0x00000000;
	static const uint32 MAX_AVAILABLEMSGLEVEL	= 0xfffffffe;
	static const uint32 DEFAULTMSGLEVEL		= 0xffffffff;

	typedef SRESULT32	(*SANFUNCPTR32)(uint32,uint32);
	typedef SRESULT64	(*SANFUNCPTR64)(uint64,uint64);
	typedef SRESULT		(*SANFUNCPTRH)(SHANDLE,SHANDLE);
#ifndef _X64
	typedef SANFUNCPTR32 SANFUNCPTR;
#else
	typedef SANFUNCPTR64 SANFUNCPTR;
#endif

	typedef SANFUNCPTR SANMSGFUNPTR;

	struct stMESSAGEELEMENT
	{
		uint32		msg;
		SVALUE		wparam;
		SVALUE		lparam;
		uint32		level;
		(SRESULT)	(*p_ptr)(SVALUE,SVALUE);
	};
	typedef stMESSAGEELEMENT	MESSAGEELEMENT;
	typedef stMESSAGEELEMENT*	lpMESSAGEELEMENT;

	struct stSANMSGNODE
	{
		SString strNodeName;
		uint32	Type;
		uint32	Level;
		union
		{
			list<MESSAGEELEMENT>*	pMsgList;
			list<stSANMSGNODE>*		pMsgNodeList;
		};
		bool	(*pCheckFun)(uint32,SVALUE,SVALUE,uint32,SVALUE,SVALUE);
		stSANMSGNODE*	pPNode;
	};
	typedef stSANMSGNODE	SANMSGNODE;
	typedef stSANMSGNODE*	lpSANMSGNODE;
	//typedef stSANUNKNOWNEX<uint32,MESSAGEELEMENT>		SANMSGNODE; // MessageListType MessageList
	//typedef stSANUNKNOWNEX<uint32,MESSAGEELEMENT>*	lpSANMSGNODE;

#endif
}