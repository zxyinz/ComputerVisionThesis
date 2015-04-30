#include"cMessageManager.h"
using namespace std;
using namespace San;
bool cMessageManager::iCreateMessageManager()
{
	this->m_MsgTreeRoot.strNodeName=_SSTR("MessageTreeRoot");
	this->m_MsgTreeRoot.Type=MT_MSGNODE;
	this->m_MsgTreeRoot.pMsgNodeList=nullptr;
	this->m_SearchList.clear();
	this->m_SearchList.insert(this->m_SearchList.begin(),&(this->m_MsgTreeRoot));
	this->m_WaitMsgList.clear();
	if(this->m_pKeyBoardDevice==nullptr)
	{
		this->m_pKeyBoardDevice=new Device::cSanKeyBoardDevice();
	}
	if(this->m_pMouseDevice==nullptr)
	{
		this->m_pMouseDevice=new Device::cSanMouseDevice();
	}
	try
	{
		this->m_pKeyBoardDevice->iCreateDevice();
		this->m_pKeyBoardDevice->iClear();
		this->m_pMouseDevice->iCreateDevice();
		this->m_pMouseDevice->iClear();
		POINT MousePos;
		::GetCursorPos(&MousePos);
		this->m_pMouseDevice->iSetMousePos(MousePos.x,MousePos.y);
	}
	catch(SString strError)
	{
		throw strError;
		if((int)strError.find(_SSTR("error"))>=0)
		{
			this->iReleaseMessageManager();
			return false;
		}
	}
	cVectorManager Manager;
	try
	{
		if(!Manager.iFound(this->m_strName))
		{
			Manager.iRegisterVector(this->m_strName,VT_SYS|VT_VAR,(SHANDLE)this);
		}
		else
		{
			SString strError=_SSTR("error: manager already exits");
			throw strError;
		}
	}
	catch(SString strError)
	{
		throw strError;
		if((int)strError.find(_SSTR("error"))>=0)
		{
			this->iReleaseMessageManager();
			return false;
		}
	}
	this->m_TTL=100;
	return true;
}
void cMessageManager::iReleaseMessageManager()
{
	try
	{
		if(this->m_pKeyBoardDevice!=nullptr)
		{
			this->m_pKeyBoardDevice->iReleaseDevice();
			this->m_pKeyBoardDevice=nullptr;
		}
		if(this->m_pMouseDevice!=nullptr)
		{
			this->m_pMouseDevice->iReleaseDevice();
			this->m_pMouseDevice=nullptr;
		}
	}
	catch(SString strError)
	{
		throw strError;
	}
	this->_ReleaseMessageList(this->m_MsgTreeRoot);
	this->m_SearchList.clear();
	this->m_TTL=0;
	::gloDestoryVector(this->m_strName,VT_SYS|VT_VAR);
}
void cMessageManager::_Update()
{
	if(this->m_pKeyBoardDevice!=nullptr)
	{
		this->m_pKeyBoardDevice->iUpdate();
	}
	if(this->m_pMouseDevice!=nullptr)
	{
		this->m_pMouseDevice->iUpdate();
	}
	this->_CheckMessage(&(this->m_MsgTreeRoot),MT_KEYBOARD);
	this->_CheckMessage(&(this->m_MsgTreeRoot),MT_MOUSE);
	this->_CheckMessage(&(this->m_MsgTreeRoot),MT_NETWORK);
	this->_CheckMessage(&(this->m_MsgTreeRoot),MT_SYSTEM);
	::Sleep(this->m_TTL);
}
void cMessageManager::_ReleaseMessageList(SANMSGNODE &MsgList)
{
	if(MsgList.Type==MT_MSGNODE)
	{
		if(MsgList.pMsgNodeList==nullptr)
		{
			return;
		}
		list<SANMSGNODE>::iterator pNode=MsgList.pMsgNodeList->begin();
		while(pNode!=MsgList.pMsgNodeList->end())
		{
			this->_ReleaseMessageList((*pNode));
			pNode++;
		}
		MsgList.pMsgNodeList->clear();
		delete (MsgList.pMsgNodeList);
		list<lpSANMSGNODE>::iterator pSearchNode=this->m_SearchList.begin();
		while(pSearchNode!=this->m_SearchList.end())
		{
			if((*pSearchNode)->strNodeName==MsgList.strNodeName)
			{
				this->m_SearchList.erase(pSearchNode);
				break;
			}
			pSearchNode++;
		}
		MsgList.strNodeName.clear();
	}
	else
	{
		if(MsgList.pMsgList!=nullptr)
		{
			MsgList.pMsgList->clear();
			delete (MsgList.pMsgList);
		}
	}
}
lpSANMSGNODE cMessageManager::_FindMessageList(SString strMessageListName)
{
	if(strMessageListName.empty())
	{
		return nullptr;
	}
	list<lpSANMSGNODE>::iterator pNode=this->m_SearchList.begin();
	while(pNode!=this->m_SearchList.end())
	{
		if((*pNode)->strNodeName==strMessageListName)
		{
			return (*pNode);
		}
		pNode++;
	}
	return nullptr;
}
bool cMessageManager::_FindMessage(SANMSGNODE MsgNode,uint32 Message,SVALUE wParam,SVALUE lParam,bool bCheckParams,bool bIterat)
{
	bool bFound=false;
	if(MsgNode.Type==MT_MSGNODE)
	{
		if(MsgNode.pMsgNodeList==nullptr)
		{
			return false;
		}
		list<SANMSGNODE>::iterator pNode=MsgNode.pMsgNodeList->begin();
		while(pNode!=MsgNode.pMsgNodeList->end())
		{
			if(bIterat)
			{
				bFound=this->_FindMessage(*pNode,Message,wParam,lParam,bCheckParams,bIterat);
				if(bFound)
				{
					return true;
				}
			}
			else
			{
				if(pNode->Type!=MT_MSGNODE)
				{
					bFound=this->_FindMessage(*pNode,Message,wParam,lParam,bCheckParams,bIterat);
					if(bFound)
					{
						return true;
					}
				}
			}
			pNode++;
		}
	}
	else
	{
		if(MsgNode.pMsgList==nullptr)
		{
			return false;
		}
		list<MESSAGEELEMENT>::iterator pMsg=MsgNode.pMsgList->begin();
		while(pMsg!=MsgNode.pMsgList->end())
		{
			if(bCheckParams)
			{
				bFound=(pMsg->msg==Message)&&(pMsg->wparam==wParam)&&(pMsg->lparam==lParam);
			}
			else
			{
				bFound=(pMsg->msg==Message);
			}
			if(bFound)
			{
				return true;
			}
			pMsg++;
		}
	}
	return false;
}
SRESULT cMessageManager::_CheckMessage(lpSANMSGNODE pMsgList,uint32 MessageType,uint32 Message,SVALUE wParam,SVALUE lParam)
{
	MessageType=MessageType&(!MT_MASK);
	Message=Message&MT_MASK;
	if(pMsgList->Type==MT_MSGNODE)
	{
		if(pMsgList->pMsgNodeList==nullptr)
		{
			return 0;
		}
		SRESULT MsgResult=0;
		list<SANMSGNODE>::iterator pMsgListSeek=pMsgList->pMsgNodeList->begin();
		while(pMsgListSeek!=pMsgList->pMsgNodeList->end())
		{
			if(pMsgListSeek->Type!=MT_MSGNODE)
			{
				if(pMsgListSeek->Type==MessageType)
				{
					MsgResult=this->_CheckMessage(&(*pMsgListSeek),MessageType,Message,wParam,lParam);
				}
			}
			else
			{
				MsgResult=this->_CheckMessage(&(*pMsgListSeek),MessageType,Message,wParam,lParam);
			}
			if(MsgResult!=0)
			{
				return MsgResult;
			}
			pMsgListSeek++;
		}
		return 0;
	}
	if(pMsgList->Type!=MessageType)
	{
		return 0;
	}
	if(pMsgList->pMsgList==nullptr)
	{
		return 0;
	}
	bool (*pCheckFunPtr)(uint32,SVALUE,SVALUE,uint32,SVALUE,SVALUE);
	if(pMsgList->pCheckFun==nullptr)
	{
		switch(pMsgList->Type)
		{
		case MT_KEYBOARD:
			pCheckFunPtr=San::cMessageManager::_DefaultKeyboardCheck;
			break;
		case MT_MOUSE:
			pCheckFunPtr=San::cMessageManager::_DefaultMouseCheck;
			break;
		case MT_NETWORK:
			pCheckFunPtr=San::cMessageManager::_DefaultNetworkCheck;
			break;
		case MT_SYSTEM:
			pCheckFunPtr=San::cMessageManager::_DefaultSystemCheck;
			break;
		default:
			pCheckFunPtr=nullptr;
			break;
		}
	}
	else
	{
		pCheckFunPtr=pMsgList->pCheckFun;
	}
	if(pCheckFunPtr==nullptr)
	{
		return 0;
	}
	list<MESSAGEELEMENT>::iterator pMsg=pMsgList->pMsgList->begin();
	while(pMsg!=pMsgList->pMsgList->end())
	{
		if(pCheckFunPtr(pMsg->msg,pMsg->wparam,pMsg->lparam,Message,wParam,lParam))
		{
			return pMsg->p_ptr(wParam,lParam);
		}
		pMsg++;
	}
	if(pMsgList->pMsgList->end()->level==San::DEFAULTMSGLEVEL)
	{
		return pMsgList->pMsgList->end()->p_ptr(wParam,lParam);
	}
	return 0;
}
bool cMessageManager::_DefaultKeyboardCheck(uint32 CheckMsg,SVALUE wCheckParam,SVALUE lCheckParam,uint32 Msg,SVALUE wMsgParam,SVALUE lMsgParam)
{
	if(CheckMsg!=Msg)
	{
		return false;
	}
	cMessageManager *pManager=(cMessageManager*)lCheckParam;
	switch(Msg)
	{
	case MSG_KEYUP:
		return pManager->m_pKeyBoardDevice->iCheck(wMsgParam,KBT_BUTTONUP);
		break;
	case MSG_KEYDOWN:
		return pManager->m_pKeyBoardDevice->iCheck(wMsgParam,KBT_BUTTONDOWN);
		break;
	default:
		return false;
		break;
	}
	return false;
}
bool cMessageManager::_DefaultMouseCheck(uint32 CheckMsg,SVALUE wCheckParam,SVALUE lCheckParam,uint32 Msg,SVALUE wMsgParam,SVALUE lMsgParam)
{
	return false;
}
bool cMessageManager::_DefaultNetworkCheck(uint32 CheckMsg,SVALUE wCheckParam,SVALUE lCheckParam,uint32 Msg,SVALUE wMsgParam,SVALUE lMsgParam)
{
	return false;
}
bool cMessageManager::_DefaultSystemCheck(uint32 CheckMsg,SVALUE wCheckParam,SVALUE lCheckParam,uint32 Msg,SVALUE wMsgParam,SVALUE lMsgParam)
{
	return (CheckMsg==Msg);
}
bool cMessageManager::iCreateMessageList(SString strMessageListName,SString strUpLevelList,uint32 Level)
{
	if(!this->iFindMessageList(strMessageListName))
	{
		return false;
	}
	if(strUpLevelList.empty())
	{
		strUpLevelList=_SSTR("MessageTreeRoot");
	}
	list<lpSANMSGNODE>::iterator pSeek=this->m_SearchList.begin();
	while(pSeek!=this->m_SearchList.end())
	{
		if(*pSeek=nullptr)
		{
			return false;
		}
		if((*pSeek)->strNodeName==strUpLevelList)
		{
			break;
		}
		pSeek++;
	}
	if(pSeek==this->m_SearchList.end())
	{
		return false;
	}
	if((*pSeek)->Type!=MT_MSGNODE)
	{
		return false;
	}
	if((*pSeek)->pMsgNodeList==nullptr)
	{
		return false;
	}
	list<SANMSGNODE>::iterator pNode=(*pSeek)->pMsgNodeList->begin();
	while(pNode!=(*pSeek)->pMsgNodeList->end())
	{
		if(pNode->Level>Level)
		{
			break;
		}
		pNode++;
	}
	SANMSGNODE MsgNode;
	MsgNode.strNodeName=strMessageListName;
	MsgNode.Type=MT_MSGNODE;
	MsgNode.pMsgNodeList=new list<SANMSGNODE>;
	MsgNode.pMsgNodeList->clear();
	MsgNode.Level=Level;
	MsgNode.pPNode=(*pSeek);
	MsgNode.pCheckFun=nullptr;
	(*pSeek)->pMsgNodeList->insert(pNode,MsgNode);
	this->m_SearchList.insert(this->m_SearchList.end(),&(*pNode));
	return true;
}
bool cMessageManager::iReleaseMessageList(SString strMessageListName)
{
	if(strMessageListName==_SSTR("MessageTreeList"))
	{
		return false;
	}
	list<lpSANMSGNODE>::iterator pSeek=this->m_SearchList.begin();
	while(pSeek!=this->m_SearchList.end())
	{
		if((*pSeek==nullptr))
		{
			return false;
		}
		if((*pSeek)->strNodeName==strMessageListName)
		{
			break;
		}
		pSeek++;
	}
	if(pSeek==this->m_SearchList.end())
	{
		return false;
	}
	lpSANMSGNODE pMsgList=(*pSeek)->pPNode;
	if(pMsgList==nullptr)
	{
		return false;
	}
	if(pMsgList->pMsgNodeList==nullptr)
	{
		return false;
	}
	list<SANMSGNODE>::iterator pMsgNode=pMsgList->pMsgNodeList->begin();
	while(pMsgNode!=pMsgList->pMsgNodeList->end())
	{
		if(pMsgNode->strNodeName==strMessageListName)
		{
			break;
		}
		pMsgNode++;
	}
	if(pMsgNode==pMsgList->pMsgNodeList->end())
	{
		return false;
	}
	this->_ReleaseMessageList(*(*pSeek));
	pMsgList->pMsgNodeList->erase(pMsgNode);
	return true;
}
bool cMessageManager::iCreateMessage(SString strMsgListName,uint32 Message,SRESULT (*pPtr)(SVALUE,SVALUE),uint32 wParam,uint32 lParam,uint32 Level)
{
	if(Level>::MAX_AVAILABLEMSGLEVEL)
	{
		return false;
	}
	lpSANMSGNODE pMsgList=this->_FindMessageList(strMsgListName);
	if(pMsgList==nullptr)
	{
		return false;
	}
	if(pMsgList->Type!=MT_MSGNODE)
	{
		return false;
	}
	if(this->_FindMessage(*pMsgList,Message))
	{
		return false;
	}
	uint32 Type=Message&(!MT_MASK);
	Message=Message&MT_MASK;
	switch(Type)
	{
	case MT_KEYBOARD:
	case MT_MOUSE:
	case MT_NETWORK:
	case MT_SYSTEM:
		break;
	default:
		if(Type&MT_USERDEFMASK)
		{
			break;
		}
		return false;
	}
	if(pMsgList->pMsgNodeList==nullptr)
	{
		pMsgList->pMsgNodeList=new list<SANMSGNODE>;
		pMsgList->pMsgNodeList->clear();
	}
	list<SANMSGNODE>::iterator pNode=pMsgList->pMsgNodeList->begin();
	while(pNode!=pMsgList->pMsgNodeList->end())
	{
		if((pNode->Type==Type)||(pNode->Type==MT_MSGNODE))
		{
			break;
		}
		pNode++;
	}
	if(pNode->Type!=Type)
	{
		SANMSGNODE MsgList;
		MsgList.Type=Type;
		MsgList.pMsgList=new list<MESSAGEELEMENT>;
		MsgList.pMsgList->clear();
		MsgList.Level=0;
		MsgList.pPNode=pMsgList;
		pMsgList->pMsgNodeList->insert(pNode,MsgList);
		if(pNode==pMsgList->pMsgNodeList->end())
		{
			pNode--;
		}
	}
	list<MESSAGEELEMENT>::iterator pMsg=pNode->pMsgList->begin();
	while(pMsg!=pNode->pMsgList->end())
	{
		if(pMsg->level>Level)
		{
			break;
		}
		pMsg++;
	}
	MESSAGEELEMENT Msg;
	Msg.msg=Message;
	Msg.p_ptr=pPtr;
	Msg.wparam=wParam;
	Msg.lparam=lParam;
	Msg.level=Level;
	pNode->pMsgList->insert(pMsg,Msg);
	return true;
}
bool cMessageManager::iCreateMessageEx(SString strMsgListName,uint32 Message,SRESULT (*pPtr)(SVALUE,SVALUE),uint32 wParam,uint32 lParam,uint32 Level)
{
	if(Level>::MAX_AVAILABLEMSGLEVEL)
	{
		return false;
	}
	lpSANMSGNODE pMsgList=this->_FindMessageList(strMsgListName);
	if(pMsgList==nullptr)
	{
		return false;
	}
	if(pMsgList->Type!=MT_MSGNODE)
	{
		return false;
	}
	if(this->_FindMessage(*pMsgList,Message,wParam,lParam,true))
	{
		return false;
	}
	uint32 Type=Message&(!MT_MASK);
	Message=Message&MT_MASK;
	switch(Type)
	{
	case MT_KEYBOARD:
	case MT_MOUSE:
	case MT_NETWORK:
	case MT_SYSTEM:
		break;
	default:
		if(Type&MT_USERDEFMASK)
		{
			break;
		}
		return false;
	}
	if(pMsgList->pMsgNodeList==nullptr)
	{
		pMsgList->pMsgNodeList=new list<SANMSGNODE>;
		pMsgList->pMsgNodeList->clear();
	}
	list<SANMSGNODE>::iterator pNode=pMsgList->pMsgNodeList->begin();
	while(pNode!=pMsgList->pMsgNodeList->end())
	{
		if((pNode->Type==Type)||(pNode->Type==MT_MSGNODE))
		{
			break;
		}
		pNode++;
	}
	if(pNode->Type!=Type)
	{
		SANMSGNODE MsgList;
		MsgList.Type=Type;
		MsgList.pMsgList=new list<MESSAGEELEMENT>;
		MsgList.pMsgList->clear();
		MsgList.Level=0;
		MsgList.pPNode=pMsgList;
		pMsgList->pMsgNodeList->insert(pNode,MsgList);
		if(pNode==pMsgList->pMsgNodeList->end())
		{
			pNode--;
		}
	}
	list<MESSAGEELEMENT>::iterator pMsg=pNode->pMsgList->begin();
	while(pMsg!=pNode->pMsgList->end())
	{
		if(pMsg->level>Level)
		{
			break;
		}
		pMsg++;
	}
	MESSAGEELEMENT Msg;
	Msg.msg=Message;
	Msg.p_ptr=pPtr;
	Msg.wparam=wParam;
	Msg.lparam=lParam;
	Msg.level=Level;
	pNode->pMsgList->insert(pMsg,Msg);
	return true;
}
bool cMessageManager::iReleaseMessage(SString strMsgListName,uint32 Message)
{
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return false;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return false;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return false;
	}
	uint32 MsgList=Message&(!MT_MASK);
	Message=Message&MT_MASK;
	list<SANMSGNODE>::iterator pMsgListSeek=pNode->pMsgNodeList->begin();
	while(pMsgListSeek!=pNode->pMsgNodeList->end())
	{
		if(pMsgListSeek->Type==MsgList)
		{
			bool bReleased=false;
			list<MESSAGEELEMENT>::iterator pMsg=pMsgListSeek->pMsgList->begin();
			while(pMsg!=pMsgListSeek->pMsgList->end())
			{
				if(pMsg->msg==Message)
				{
					pMsgListSeek->pMsgList->erase(pMsg);
					bReleased=true;
					if(pMsgListSeek->pMsgList->empty())
					{
						break;
					}
				}
				else
				{
					pMsg++;
				}
			}
			return bReleased;
		}
		pMsgListSeek++;
	}
	return false;
}
bool cMessageManager::iReleaseMessageEx(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam)
{
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return false;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return false;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return false;
	}
	uint32 MsgList=Message&(!MT_MASK);
	Message=Message&MT_MASK;
	list<SANMSGNODE>::iterator pMsgListSeek=pNode->pMsgNodeList->begin();
	while(pMsgListSeek!=pNode->pMsgNodeList->end())
	{
		if(pMsgListSeek->Type==MsgList)
		{
			list<MESSAGEELEMENT>::iterator pMsg=pMsgListSeek->pMsgList->begin();
			while(pMsg!=pMsgListSeek->pMsgList->end())
			{
				if((pMsg->msg==Message)&&(pMsg->wparam==wParam)&&(pMsg->lparam==lParam))
				{
					pMsgListSeek->pMsgList->erase(pMsg);
					return true;
				}
				pMsg++;
			}
			return false;
		}
		pMsgListSeek++;
	}
	return false;
}
void cMessageManager::iMsgLevelUp(SString strMsgListName,uint32 Message)
{
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return;
	}
	uint32 MsgList=Message&(!MT_MASK);
	Message=Message&MT_MASK;
	list<SANMSGNODE>::iterator pMsgListSeek=pNode->pMsgNodeList->begin();
	while(pMsgListSeek!=pNode->pMsgNodeList->end())
	{
		if(pMsgListSeek->Type==MsgList)
		{
			list<MESSAGEELEMENT>::iterator pMsg=pMsgListSeek->pMsgList->begin();
			while(pMsg!=pMsgListSeek->pMsgList->end())
			{
				if(pMsg->msg==Message)
				{
					if((pMsg->level!=::DEFAULTMSGLEVEL)&&(pMsg->level<::MAX_AVAILABLEMSGLEVEL))
					{
						pMsg->level=pMsg->level+1;
					}
				}
				pMsg++;
			}
			this->_MessageListSort(pMsgListSeek->pMsgList);
			return;
		}
		pMsgListSeek++;
	}
}
void cMessageManager::iMsgLevelDown(SString strMsgListName,uint32 Message)
{
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return;
	}
	uint32 MsgList=Message&(!MT_MASK);
	Message=Message&MT_MASK;
	list<SANMSGNODE>::iterator pMsgListSeek=pNode->pMsgNodeList->begin();
	while(pMsgListSeek!=pNode->pMsgNodeList->end())
	{
		if(pMsgListSeek->Type==MsgList)
		{
			list<MESSAGEELEMENT>::iterator pMsg=pMsgListSeek->pMsgList->begin();
			while(pMsg!=pMsgListSeek->pMsgList->end())
			{
				if(pMsg->msg==Message)
				{
					if((pMsg->level!=::DEFAULTMSGLEVEL)&&(pMsg->level>::MIN_AVAILABLEMSGLEVEL))
					{
						pMsg->level=pMsg->level-1;
					}
				}
				pMsg++;
			}
			this->_MessageListSort(pMsgListSeek->pMsgList);
			return;
		}
		pMsgListSeek++;
	}
}
void cMessageManager::iMsgLevelUpEx(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam)
{
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return;
	}
	uint32 MsgList=Message&(!MT_MASK);
	Message=Message&MT_MASK;
	list<SANMSGNODE>::iterator pMsgListSeek=pNode->pMsgNodeList->begin();
	while(pMsgListSeek!=pNode->pMsgNodeList->end())
	{
		if(pMsgListSeek->Type==MsgList)
		{
			list<MESSAGEELEMENT>::iterator pMsg=pMsgListSeek->pMsgList->begin();
			while(pMsg!=pMsgListSeek->pMsgList->end())
			{
				if((pMsg->msg==Message)&&(pMsg->wparam==wParam)&&(pMsg->lparam==lParam))
				{
					if(pMsg->level==::DEFAULTMSGLEVEL)
					{
						return;
					}
					if(pMsg->level>=::MAX_AVAILABLEMSGLEVEL)
					{
						return;
					}
					MESSAGEELEMENT Msg=*pMsg;
					Msg.level=Msg.level+1;
					pMsgListSeek->pMsgList->erase(pMsg);
					pMsg=pMsgListSeek->pMsgList->begin();
					while(pMsg!=pMsgListSeek->pMsgList->end())
					{
						if(pMsg->level>Msg.level)
						{
							break;
						}
						pMsg++;
					}
					pMsgListSeek->pMsgList->insert(pMsg,Msg);
					return;
				}
				pMsg++;
			}
			return;
		}
		pMsgListSeek++;
	}
}
void cMessageManager::iMsgLevelDownEx(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam)
{
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return;
	}
	uint32 MsgList=Message&(!MT_MASK);
	Message=Message&MT_MASK;
	list<SANMSGNODE>::iterator pMsgListSeek=pNode->pMsgNodeList->begin();
	while(pMsgListSeek!=pNode->pMsgNodeList->end())
	{
		if(pMsgListSeek->Type==MsgList)
		{
			list<MESSAGEELEMENT>::iterator pMsg=pMsgListSeek->pMsgList->begin();
			while(pMsg!=pMsgListSeek->pMsgList->end())
			{
				if((pMsg->msg==Message)&&(pMsg->wparam==wParam)&&(pMsg->lparam==lParam))
				{
					if(pMsg->level==::DEFAULTMSGLEVEL)
					{
						return;
					}
					if(pMsg->level<=::MIN_AVAILABLEMSGLEVEL)
					{
						return;
					}
					MESSAGEELEMENT Msg=*pMsg;
					Msg.level=Msg.level-1;
					pMsgListSeek->pMsgList->erase(pMsg);
					pMsg=pMsgListSeek->pMsgList->begin();
					while(pMsg!=pMsgListSeek->pMsgList->end())
					{
						if(pMsg->level>Msg.level)
						{
							break;
						}
						pMsg++;
					}
					pMsgListSeek->pMsgList->insert(pMsg,Msg);
					return;
				}
				pMsg++;
			}
			return;
		}
		pMsgListSeek++;
	}
}
void cMessageManager::iSetMsgLevel(SString strMsgListName,uint32 Message,int Level)
{
	if(Level<::MIN_AVAILABLEMSGLEVEL)
	{
		return;
	}
	if(Level>::MAX_AVAILABLEMSGLEVEL)
	{
		return;
	}
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return;
	}
	uint32 MsgList=Message&(!MT_MASK);
	Message=Message&MT_MASK;
	list<SANMSGNODE>::iterator pMsgListSeek=pNode->pMsgNodeList->begin();
	while(pMsgListSeek!=pNode->pMsgNodeList->end())
	{
		if(pMsgListSeek->Type==MsgList)
		{
			list<MESSAGEELEMENT>::iterator pMsg=pMsgListSeek->pMsgList->begin();
			while(pMsg!=pMsgListSeek->pMsgList->end())
			{
				if(pMsg->msg==Message)
				{
					if(pMsg->level!=::DEFAULTMSGLEVEL)
					{
						pMsg->level=Level;
					}
				}
				pMsg++;
			}
			this->_MessageListSort(pMsgListSeek->pMsgList);
			return;
		}
		pMsgListSeek++;
	}
}
void cMessageManager::iSetMsgToTop(SString strMsgListName,uint32 Message)
{
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return;
	}
	uint32 MsgList=Message&(!MT_MASK);
	Message=Message&MT_MASK;
	list<SANMSGNODE>::iterator pMsgListSeek=pNode->pMsgNodeList->begin();
	while(pMsgListSeek!=pNode->pMsgNodeList->end())
	{
		if(pMsgListSeek->Type==MsgList)
		{
			list<MESSAGEELEMENT>::iterator pMsg=pMsgListSeek->pMsgList->begin();
			while(pMsg!=pMsgListSeek->pMsgList->end())
			{
				if(pMsg->msg==Message)
				{
					MESSAGEELEMENT Msg=*pMsg;
					pMsgListSeek->pMsgList->erase(pMsg);
					while(pMsg!=pMsgListSeek->pMsgList->begin())
					{
						if(pMsg->level!=Msg.level)
						{
							break;
						}
						pMsg--;
					}
					if(pMsg==pMsgListSeek->pMsgList->begin())
					{
						if(pMsg->level!=Msg.level)
						{
							pMsg++;
						}
					}
					else
					{
						pMsg++;
					}
					pMsgListSeek->pMsgList->insert(pMsg,Msg);
					return;
				}
				pMsg++;
			}
			return;
		}
		pMsgListSeek++;
	}
}
void cMessageManager::iSetMsgToBottom(SString strMsgListName,uint32 Message)
{
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return;
	}
	uint32 MsgList=Message&(!MT_MASK);
	Message=Message&MT_MASK;
	list<SANMSGNODE>::iterator pMsgListSeek=pNode->pMsgNodeList->begin();
	while(pMsgListSeek!=pNode->pMsgNodeList->end())
	{
		if(pMsgListSeek->Type==MsgList)
		{
			list<MESSAGEELEMENT>::iterator pMsg=pMsgListSeek->pMsgList->begin();
			while(pMsg!=pMsgListSeek->pMsgList->end())
			{
				if(pMsg->msg==Message)
				{
					MESSAGEELEMENT Msg=*pMsg;
					pMsgListSeek->pMsgList->erase(pMsg);
					while(pMsg!=pMsgListSeek->pMsgList->end())
					{
						if(pMsg->level!=Msg.level)
						{
							break;
						}
						pMsg++;
					}
					pMsgListSeek->pMsgList->insert(pMsg,Msg);
					return;
				}
				pMsg++;
			}
			return;
		}
		pMsgListSeek++;
	}
}
bool cMessageManager::iSetDefaultMessage(SString strMsgListName,uint32 Message)
{
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return false;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return false;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return false;
	}
	uint32 MsgList=Message&(!MT_MASK);
	Message=Message&MT_MASK;
	list<SANMSGNODE>::iterator pMsgListSeek=pNode->pMsgNodeList->begin();
	while(pMsgListSeek!=pNode->pMsgNodeList->end())
	{
		if(pMsgListSeek->Type==MsgList)
		{
			uint32 MsgElementCount=0;
			list<MESSAGEELEMENT>::iterator pMsg=pMsgListSeek->pMsgList->begin();
			list<MESSAGEELEMENT>::iterator pDefMsg=pMsgListSeek->pMsgList->end();
			while(pMsg!=pMsgListSeek->pMsgList->end())
			{
				if(pMsg->msg==Message)
				{
					if(MsgElementCount==0)
					{
						pDefMsg=pMsg;
					}
					MsgElementCount=MsgElementCount+1;
				}
				pMsg++;
			}
			if(MsgElementCount==1)
			{
				MESSAGEELEMENT Msg=*pDefMsg;
				Msg.level=::DEFAULTMSGLEVEL;
				pMsgListSeek->pMsgList->erase(pDefMsg);
				pMsgListSeek->pMsgList->insert(pMsgListSeek->pMsgList->end(),Msg);
				return true;
			}
			return false;
		}
		pMsgListSeek++;
	}
	return false;
}
void cMessageManager::iSetMsgLevelEx(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam,int Level)
{
	if(Level<::MIN_AVAILABLEMSGLEVEL)
	{
		return;
	}
	if(Level>::MAX_AVAILABLEMSGLEVEL)
	{
		return;
	}
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return;
	}
	uint32 MsgList=Message&(!MT_MASK);
	Message=Message&MT_MASK;
	list<SANMSGNODE>::iterator pMsgListSeek=pNode->pMsgNodeList->begin();
	while(pMsgListSeek!=pNode->pMsgNodeList->end())
	{
		if(pMsgListSeek->Type==MsgList)
		{
			list<MESSAGEELEMENT>::iterator pMsg=pMsgListSeek->pMsgList->begin();
			while(pMsg!=pMsgListSeek->pMsgList->end())
			{
				if((pMsg->msg==Message)&&(pMsg->wparam==wParam)&&(pMsg->lparam==lParam))
				{
					if(pMsg->level==::DEFAULTMSGLEVEL)
					{
						return;
					}
					MESSAGEELEMENT Msg=*pMsg;
					Msg.level=Level;
					pMsgListSeek->pMsgList->erase(pMsg);
					pMsg=pMsgListSeek->pMsgList->begin();
					while(pMsg!=pMsgListSeek->pMsgList->end())
					{
						if(pMsg->level>Msg.level)
						{
							break;
						}
						pMsg++;
					}
					pMsgListSeek->pMsgList->insert(pMsg,Msg);
					return;
				}
				pMsg++;
			}
			return;
		}
		pMsgListSeek++;
	}
}
void cMessageManager::iSetMsgToTopEx(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam)
{
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return;
	}
	uint32 MsgList=Message&(!MT_MASK);
	Message=Message&MT_MASK;
	list<SANMSGNODE>::iterator pMsgListSeek=pNode->pMsgNodeList->begin();
	while(pMsgListSeek!=pNode->pMsgNodeList->end())
	{
		if(pMsgListSeek->Type==MsgList)
		{
			list<MESSAGEELEMENT>::iterator pMsg=pMsgListSeek->pMsgList->begin();
			while(pMsg!=pMsgListSeek->pMsgList->end())
			{
				if((pMsg->msg==Message)&&(pMsg->wparam=wParam)&&(pMsg->lparam==lParam))
				{
					MESSAGEELEMENT Msg=*pMsg;
					pMsgListSeek->pMsgList->erase(pMsg);
					while(pMsg!=pMsgListSeek->pMsgList->begin())
					{
						if(pMsg->level!=Msg.level)
						{
							break;
						}
						pMsg--;
					}
					if(pMsg==pMsgListSeek->pMsgList->begin())
					{
						if(pMsg->level!=Msg.level)
						{
							pMsg++;
						}
					}
					else
					{
						pMsg++;
					}
					pMsgListSeek->pMsgList->insert(pMsg,Msg);
					return;
				}
				pMsg++;
			}
			return;
		}
		pMsgListSeek++;
	}
}
void cMessageManager::iSetMsgToBottomEx(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam)
{
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return;
	}
	uint32 MsgList=Message&(!MT_MASK);
	Message=Message&MT_MASK;
	list<SANMSGNODE>::iterator pMsgListSeek=pNode->pMsgNodeList->begin();
	while(pMsgListSeek!=pNode->pMsgNodeList->end())
	{
		if(pMsgListSeek->Type==MsgList)
		{
			list<MESSAGEELEMENT>::iterator pMsg=pMsgListSeek->pMsgList->begin();
			while(pMsg!=pMsgListSeek->pMsgList->end())
			{
				if((pMsg->msg==Message)&&(pMsg->wparam=wParam)&&(pMsg->lparam==lParam))
				{
					MESSAGEELEMENT Msg=*pMsg;
					pMsgListSeek->pMsgList->erase(pMsg);
					while(pMsg!=pMsgListSeek->pMsgList->end())
					{
						if(pMsg->level!=Msg.level)
						{
							break;
						}
						pMsg++;
					}
					pMsgListSeek->pMsgList->insert(pMsg,Msg);
					return;
				}
				pMsg++;
			}
			return;
		}
		pMsgListSeek++;
	}
}
bool cMessageManager::iSetDefaultMessageEx(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam)
{
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return false;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return false;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return false;
	}
	uint32 MsgList=Message&(!MT_MASK);
	Message=Message&MT_MASK;
	list<SANMSGNODE>::iterator pMsgListSeek=pNode->pMsgNodeList->begin();
	while(pMsgListSeek!=pNode->pMsgNodeList->end())
	{
		if(pMsgListSeek->Type==MsgList)
		{
			list<MESSAGEELEMENT>::iterator pMsg=pMsgListSeek->pMsgList->begin();
			while(pMsg!=pMsgListSeek->pMsgList->end())
			{
				if((pMsg->msg==Message)&&(pMsg->wparam==wParam)&&(pMsg->lparam==lParam))
				{
					MESSAGEELEMENT Msg=*pMsg;
					Msg.level=::DEFAULTMSGLEVEL;
					pMsgListSeek->pMsgList->erase(pMsg);
					pMsgListSeek->pMsgList->insert(pMsgListSeek->pMsgList->end(),Msg);
					return true;
				}
				pMsg++;
			}
			return false;
		}
		pMsgListSeek++;
	}
	return false;
}
bool cMessageManager::iCreateMessageType(SString strMsgListName,uint32 MessageType)
{
	MessageType=MessageType&(!MT_MASK);
	switch(MessageType)
	{
	case MT_KEYBOARD:
	case MT_MOUSE:
	case MT_NETWORK:
	case MT_SYSTEM:
		break;
	default:
		if(!(MessageType&MT_USERDEFMASK))
		{
			return false;
		}
	}
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return false;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return false;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return false;
	}
	list<SANMSGNODE>::iterator pMsgList=pNode->pMsgNodeList->begin();
	while(pMsgList!=pNode->pMsgNodeList->end())
	{
		if(pMsgList->Type==MessageType)
		{
			return false;
		}
		pMsgList++;
	}
	SANMSGNODE MessageList;
	MessageList.Type=MessageType;
	MessageList.pMsgList=new list<MESSAGEELEMENT>;
	MessageList.pMsgList->clear();
	MessageList.pPNode=&(*pNode);
	MessageList.Level=::MIN_AVAILABLEMSGLEVEL;
	MessageList.pCheckFun=nullptr;
	pMsgList=pNode->pMsgNodeList->begin();
	while(pMsgList!=pNode->pMsgNodeList->end())
	{
		if(pMsgList->Level>MessageList.Level)
		{
			break;
		}
		pMsgList++;
	}
	pNode->pMsgNodeList->insert(pMsgList,MessageList);
	return true;
}
bool cMessageManager::iReleaseMessageType(SString strMsgListName,uint32 MessageType)
{
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return false;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return false;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return false;
	}
	list<SANMSGNODE>::iterator pMsgList=pNode->pMsgNodeList->begin();
	while(pMsgList!=pNode->pMsgNodeList->end())
	{
		if(pMsgList->Type==MessageType)
		{
			pMsgList->pMsgList->clear();
			pNode->pMsgNodeList->erase(pMsgList);
			pMsgList--;
		}
		pMsgList++;
	}
	return false;
}
bool cMessageManager::iSetMessageTypeCheckFun(SString strMsgListName,uint32 MessageType,bool(*pCheck)(uint32,SVALUE,SVALUE,uint32,SVALUE,SVALUE))
{
	MessageType=MessageType&(!MT_MASK);
	switch(MessageType)
	{
	case MT_KEYBOARD:
	case MT_MOUSE:
	case MT_NETWORK:
	case MT_SYSTEM:
		break;
	default:
		if(!(MessageType&MT_USERDEFMASK))
		{
			return false;
		}
	}
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return false;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return false;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return false;
	}
	list<SANMSGNODE>::iterator pMsgList=pNode->pMsgNodeList->begin();
	while(pMsgList!=pNode->pMsgNodeList->end())
	{
		if(pMsgList->Type==MessageType)
		{
			pMsgList->pCheckFun=pCheck;
			return true;
		}
		pMsgList++;
	}
	return false;
}
bool cMessageManager::iPushMessageType(SString strMsgListName,uint32 MessageType)
{
	MessageType=MessageType&(!MT_MASK);
	switch(MessageType)
	{
	case MT_KEYBOARD:
	case MT_MOUSE:
	case MT_NETWORK:
	case MT_SYSTEM:
		break;
	default:
		if(!(MessageType&MT_USERDEFMASK))
		{
			return false;
		}
	}
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return false;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return false;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return false;
	}
	list<SANMSGNODE>::iterator pMsgList=pNode->pMsgNodeList->begin();
	while(pMsgList!=pNode->pMsgNodeList->end())
	{
		if(pMsgList->Type==MessageType)
		{
			break;
		}
		pMsgList++;
	}
	SANMSGNODE MessageList;
	MessageList.Type=MessageType;
	MessageList.pMsgList=new list<MESSAGEELEMENT>;
	MessageList.pMsgList->clear();
	MessageList.pPNode=&(*pNode);
	MessageList.Level=::MIN_AVAILABLEMSGLEVEL;
	pMsgList=pNode->pMsgNodeList->begin();
	pNode->pMsgNodeList->insert(pMsgList,MessageList);
	return true;
}
bool cMessageManager::iPopMessageType(SString strMsgListName,uint32 MessageType)
{
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return false;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return false;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return false;
	}
	list<SANMSGNODE>::iterator pMsgList=pNode->pMsgNodeList->begin();
	while(pMsgList!=pNode->pMsgNodeList->end())
	{
		if(pMsgList->Type==MessageType)
		{
			pMsgList->pMsgList->clear();
			pNode->pMsgNodeList->erase(pMsgList);
			return true;
		}
		pMsgList++;
	}
	return false;
}
void cMessageManager::iClearMessageList(SString strMsgListName)
{
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return;
	}
	list<SANMSGNODE>::iterator pMsgList=pNode->pMsgNodeList->begin();
	while(pMsgList!=pNode->pMsgNodeList->end())
	{
		if(pMsgList->Type!=MT_MSGNODE)
		{
			pMsgList->pMsgList->clear();
		}
		else
		{
			this->_ReleaseMessageList((*pMsgList));
		}
		pMsgList++;
	}
	pNode->pMsgNodeList->clear();
}
void cMessageManager::iClearWaitMessageList()
{
	this->m_WaitMsgList.clear();
}
void cMessageManager::iSetUpdateTime(uint32 time)
{
	if(time==0)
	{
		SString strError=_SSTR("error: the ttl must greater than 0");
		throw strError;
	}
	this->m_TTL=time;
}
bool cMessageManager::iFindMessageList(SString strMsgListName)
{
	if(this->_FindMessageList(strMsgListName)==nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool cMessageManager::iFindMessage(SString strMsgListName,uint32 Message,bool bIterat)
{
	return this->_FindMessage(*(this->_FindMessageList(strMsgListName)),Message,0,0,false,bIterat);
}
bool cMessageManager::iFindMessageEx(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam,bool bIterat)
{
	return this->_FindMessage(*(this->_FindMessageList(strMsgListName)),Message,wParam,lParam,true,bIterat);
}
uint32 cMessageManager::iGetUpdateTime() const
{
	return this->m_TTL;
}
int cMessageManager::iGetMinLevel(SString strMsgListName,uint32 MessageType)
{
	MessageType=MessageType&(!MT_MASK);
	switch(MessageType)
	{
	case MT_KEYBOARD:
	case MT_MOUSE:
	case MT_NETWORK:
	case MT_SYSTEM:
		break;
	default:
		if(!(MessageType&MT_USERDEFMASK))
		{
			return -1;
		}
	}
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return -1;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return -1;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return -1;
	}
	uint32 MinLevel=::MAX_AVAILABLEMSGLEVEL;
	list<SANMSGNODE>::iterator pMsgList=pNode->pMsgNodeList->begin();
	while(pMsgList!=pNode->pMsgNodeList->end())
	{
		if(pMsgList->Level<MinLevel)
		{
			MinLevel=pMsgList->Level;
		}
		pMsgList++;
	}
	return MinLevel;
}
int cMessageManager::iGetMaxLevel(SString strMsgListName,uint32 MessageType)
{
	MessageType=MessageType&(!MT_MASK);
	switch(MessageType)
	{
	case MT_KEYBOARD:
	case MT_MOUSE:
	case MT_NETWORK:
	case MT_SYSTEM:
		break;
	default:
		if(!(MessageType&MT_USERDEFMASK))
		{
			return -1;
		}
	}
	lpSANMSGNODE pNode=this->_FindMessageList(strMsgListName);
	if(pNode==nullptr)
	{
		return -1;
	}
	if(pNode->Type!=MT_MSGNODE)
	{
		return -1;
	}
	if(pNode->pMsgNodeList==nullptr)
	{
		return -1;
	}
	uint32 MaxLevel=::MIN_AVAILABLEMSGLEVEL;
	list<SANMSGNODE>::iterator pMsgList=pNode->pMsgNodeList->begin();
	while(pMsgList!=pNode->pMsgNodeList->end())
	{
		if(pMsgList->Level>MaxLevel)
		{
			MaxLevel=pMsgList->Level;
		}
		pMsgList++;
	}
	return MaxLevel;
}
San::Device::cSanKeyBoardDevice* cMessageManager::iGetKeyBoardDevicePtr()
{
	return this->m_pKeyBoardDevice;
}
San::Device::cSanMouseDevice* cMessageManager::iGetMouseDevicePtr()
{
	return this->m_pMouseDevice;
}
void cMessageManager::iUpdate()
{
	this->_Update();
}
void cMessageManager::iSendMessage(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam,bool bUrgent)
{
	stSANUNKNOWNEX<SString,MESSAGEELEMENT> Msg;
	Msg.description=strMsgListName;
	Msg.value.msg=Message;
	Msg.value.wparam=wParam;
	Msg.value.lparam=lParam;
	if(bUrgent)
	{
		this->m_WaitMsgList.insert(this->m_WaitMsgList.begin(),Msg);
	}
	else
	{
		this->m_WaitMsgList.insert(this->m_WaitMsgList.end(),Msg);
	}
}
SRESULT cMessageManager::iPostMessage(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam)
{
	return this->_CheckMessage((this->_FindMessageList(strMsgListName)),Message&(!MT_MASK),Message&MT_MASK,wParam,lParam);
}

/*void San::gloSendMessage(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam,bool bUrgent)
{
	cMessageManager *pMsgManager=(cMessageManager*)::gloFoundVector(_SSTR("Sys_MessageManager"));
	if(pMsgManager==nullptr)
	{
		return;
	}
	pMsgManager->iSendMessage(strMsgListName,Message,wParam,lParam,bUrgent);
}
void San::gloSendMessage(uint32 Message,SVALUE wParam,SVALUE lParam,bool bUrgent)
{
	cMessageManager *pMsgManager=(cMessageManager*)::gloFoundVector(_SSTR("Sys_MessageManager"));
	if(pMsgManager==nullptr)
	{
		return;
	}
	pMsgManager->iSendMessage(_SSTR("MessageTreeRoot"),Message,wParam,lParam,bUrgent);
}
SRESULT San::gloPostMessage(SString strMsgListName,uint32 Message,SVALUE wParam,SVALUE lParam)
{
	cMessageManager *pMsgManager=(cMessageManager*)::gloFoundVector(_SSTR("Sys_MessageManager"));
	if(pMsgManager==nullptr)
	{
		return 0;
	}
	return pMsgManager->iPostMessage(strMsgListName,Message,wParam,lParam);
}
SRESULT San::gloPostMessage(uint32 Message,SVALUE wParam,SVALUE lParam)
{
	cMessageManager *pMsgManager=(cMessageManager*)::gloFoundVector(_SSTR("Sys_MessageManager"));
	if(pMsgManager==nullptr)
	{
		return 0;
	}
	return pMsgManager->iPostMessage(_SSTR("MessageTreeRoot"),Message,wParam,lParam);
}*/