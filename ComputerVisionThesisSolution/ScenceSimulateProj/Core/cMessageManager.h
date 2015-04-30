#include"cMessageManagerDef.h"
#pragma once
using namespace std;
using namespace San::Device;
namespace San
{
#ifndef __CMESSAGEMANAGER_H__
#define __CMESSAGEMANAGER_H__
	class cMessageManager
	{
	private:
		SANMSGNODE	m_MsgTreeRoot;
		list<lpSANMSGNODE>	m_SearchList;
		cSanKeyBoardDevice*	m_pKeyBoardDevice;
		cSanMouseDevice*	m_pMouseDevice;
		list<stSANUNKNOWNEX<SString,MESSAGEELEMENT>>	m_WaitMsgList;
		uint32	m_TTL;
		SString	m_strName;
	protected:
		void _Update();
		void _ReleaseMessageList(SANMSGNODE &MsgList);////////////////////Can redefine same message but must have different params
		lpSANMSGNODE _FindMessageList(SString strMessageListName);
		bool _FindMessage(SANMSGNODE MsgNode,uint32 Message,SVALUE wParam=0,SVALUE lParam=0,bool bCheckParam=false,bool bIterat=false);
		SRESULT _CheckMessage(lpSANMSGNODE pMsgList,uint32 MessageType,uint32 Message=0,SVALUE wParam=0,SVALUE lParam=0);
		void _MessageListSort(list<MESSAGEELEMENT>* pMsgList){};/////////////////////////////////////////////////////////////////////////////////
		static bool _DefaultKeyboardCheck(uint32 CheckMsg,SVALUE wCheckParam,SVALUE lCheckParam,uint32 Msg,SVALUE wMsgParam,SVALUE lMsgParam);
		static bool _DefaultMouseCheck(uint32 CheckMsg,SVALUE wCheckParam,SVALUE lCheckParam,uint32 Msg,SVALUE wMsgParam,SVALUE lMsgParam);
		static bool _DefaultNetworkCheck(uint32 CheckMsg,SVALUE wCheckParam,SVALUE lCheckParam,uint32 Msg,SVALUE wMsgParam,SVALUE lMsgParam);
		static bool _DefaultSystemCheck(uint32 CheckMsg,SVALUE wCheckParam,SVALUE lCheckParam,uint32 Msg,SVALUE wMsgParam,SVALUE lMsgParam);
		static SRESULT _DefultPointer(SVALUE wParam,SVALUE lParam)
		{
			return 0;
		};
	public:
		cMessageManager()
			:m_pKeyBoardDevice(nullptr),
			m_pMouseDevice(nullptr),
			m_strName(_SSTR("Sys_MessageManager"))
		{
		};
		~cMessageManager(){};
		bool iCreateMessageManager();
		void iReleaseMessageManager();
		bool iCreateMessageList(SString strMessageListName,SString strUpLevelList=_SSTR(""),uint32 Level=0);
		bool iReleaseMessageList(SString strMessageListName);
		bool iCreateMessage(SString strMsgListName,uint32 Message,SRESULT (*Ptr)(SVALUE,SVALUE),uint32 wParam=0,uint32 lParam=0,uint32 Level=0);
		bool iCreateMessageEx(SString strMsgListName,uint32 Message,SRESULT (*Ptr)(SVALUE,SVALUE),uint32 wParam=0,uint32 lParam=0,uint32 Level=0);
		bool iReleaseMessage(SString strMsgListName,uint32 Message);
		bool iReleaseMessageEx(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam);
		void iMsgLevelUp(SString strMsgListName,uint32 Message);
		void iMsgLevelDown(SString strMsgListName,uint32 Message);
		void iMsgLevelUpEx(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam);
		void iMsgLevelDownEx(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam);
		void iSetMsgLevel(SString strMsgListName,uint32 Message,int Level);
		void iSetMsgToTop(SString strMsgListName,uint32 Message);//------------------------------------------------------------------//
		void iSetMsgToBottom(SString strMsgListName,uint32 Message);//---------------------------------------------------------------//
		bool iSetDefaultMessage(SString strMsgListName,uint32 Message);//--------------may cause problem: multi def msg--------------//
		void iSetMsgLevelEx(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam,int Level);
		void iSetMsgToTopEx(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam);
		void iSetMsgToBottomEx(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam);
		bool iSetDefaultMessageEx(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam);
		bool iCreateMessageType(SString strMsgListName,uint32 MessageType);
		bool iReleaseMessageType(SString strMsgListName,uint32 MessageType);
		bool iSetMessageTypeCheckFun(SString strMsgListName,uint32 MessageType,bool(*pCheck)(uint32,SVALUE,SVALUE,uint32,SVALUE,SVALUE));
		bool iPushMessageType(SString strMsgListName,uint32 MessageType);
		bool iPopMessageType(SString strMsgListName,uint32 MessageType);
		void iClearMessageList(SString strMsgListName);
		void iClearWaitMessageList();
		void iSetUpdateTime(uint32 time);
		bool iFindMessageList(SString strMsgListName);
		bool iFindMessageType(uint32 Type);
		bool iFindMessage(SString strMsgListName,uint32 Message,bool bIterat=false);
		bool iFindMessageEx(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam,bool bIterat=false);
		uint32 iGetUpdateTime() const;
		int	iGetMinLevel(SString strMsgListName,uint32 MessageType);
		int	iGetMaxLevel(SString strMsgListName,uint32 MessageType);
		San::Device::cSanKeyBoardDevice* iGetKeyBoardDevicePtr();
		San::Device::cSanMouseDevice* iGetMouseDevicePtr();
		void iUpdate();
		void iSendMessage(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam,bool bUrgent=false);
		SRESULT iPostMessage(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam);
		SRESULT iCommandLine(SVALUE lParam,SVALUE wParam);
	};

	inline void gloSendMessage(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam,bool bUrgent=false)
	{
		cMessageManager *pMsgManager=(cMessageManager*)San::gloFoundVector(_SSTR("Sys_MessageManager"));
		if(pMsgManager==nullptr)
		{
			return;
		}
		pMsgManager->iSendMessage(strMsgListName,Message,wParam,lParam,bUrgent);
	};

	inline void gloSendMessage(uint32 Message,SVALUE wParam,SVALUE lParam,bool bUrgent=false)
	{
		cMessageManager *pMsgManager=(cMessageManager*)San::gloFoundVector(_SSTR("Sys_MessageManager"));
		if(pMsgManager==nullptr)
		{
			return;
		}
		pMsgManager->iSendMessage(_SSTR("MessageTreeRoot"),Message,wParam,lParam,bUrgent);
	};

	inline SRESULT gloPostMessage(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam)
	{
		cMessageManager *pMsgManager=(cMessageManager*)San::gloFoundVector(_SSTR("Sys_MessageManager"));
		if(pMsgManager==nullptr)
		{
			return 0;
		}
		return pMsgManager->iPostMessage(strMsgListName,Message,wParam,lParam);
	};

	inline SRESULT gloPostMessage(uint32 Message,SVALUE wParam,SVALUE lParam)
	{
		cMessageManager *pMsgManager=(cMessageManager*)San::gloFoundVector(_SSTR("Sys_MessageManager"));
		if(pMsgManager==nullptr)
		{
			return 0;
		}
		return pMsgManager->iPostMessage(_SSTR("MessageTreeRoot"),Message,wParam,lParam);
	};

#endif
}