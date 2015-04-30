#include"cSanVisualKeyBoard.h"
using namespace std;
using namespace San;
using namespace San::Device;
#ifndef _DIRECTX
bool cSanVisualKeyBoard::iUpdate()
{
	::memcpy(this->m_KBHistory,this->m_KBBuffer,sizeof(this->m_KBBuffer));
	//::memset(this->m_KBBuffer,0,sizeof(this->m_KBBuffer));////////////////////////////////////////////////////////
	this->_AddToString();
	return true;
}
void cSanVisualKeyBoard::_AddToString()
{
	string strVisualKey="??1234567890-=\b\tqwertyuiop[]\n?asdfghjkl;'???zxcvbnm,./??? ";
	string strVisualKeyShift="??!@#$%^&*()_+?\tQWERTYUIOP{}\n?ASDFGHJKL:\"???ZXCVBNM<>???? ";
	for(int seek=0;seek<KBD_SIZE;seek=seek+1)
	{
		if(this->m_KBBuffer[seek]!=0)
		{
			if(this->m_KBState[seek]==0)
			{
				switch(seek)
				{
				case 0:
					break;
				case SVK_CAPSLOCK:
					this->m_bSetCapsLock=!(this->m_bSetCapsLock);
#ifdef _DEBUG
					::cout<<"Set CapsLock Value:"<<this->m_bSetCapsLock<<"\n";
#endif
					break;
				case SVK_BACKSPACE:
					if(!this->m_strString.empty())
					{
						this->m_strString=this->m_strString.substr(0,this->m_strString.length()-1);
						this->m_strString=this->m_strString+"\b \b";
					}
					break;
				case SVK_SHIFT_L:
				case SVK_SHIFT_R:
					this->m_bShiftDown=this->m_KBBuffer[seek];
					break;
				default:
					if((!this->m_bSetCapsLock)||(!this->m_bShiftDown))
					{
						if(seek<strVisualKey.length())
						{
							this->m_strString=this->m_strString+strVisualKey[seek];
							//::cout<<seek;
						}
					}
					else
					{
						if(seek<strVisualKeyShift.length())
						{
							this->m_strString=this->m_strString+strVisualKeyShift[seek];
						}
					}
					break;
				}
				this->m_KBState[seek]=this->m_KBState[seek]+1;
				::cout<<this->m_strString<<"\n";
			}
			else
			{
				if(this->m_KBState[seek]<this->m_TTL)
				{
					this->m_KBState[seek]=this->m_KBState[seek]+1;
				}
				else
				{
					this->m_KBState[seek]=0;
				}
			}
		}
		else
		{
			this->m_KBState[seek]=0;
		}
	}
}
void cSanVisualKeyBoard::iClear()
{
	::memset((this->m_KBBuffer),0,sizeof(this->m_KBBuffer));
	::memset((this->m_KBHistory),0,sizeof(this->m_KBHistory));
	::memset((this->m_KBState),0,sizeof(this->m_KBState));
	this->m_strString.clear();
}
void cSanVisualKeyBoard::iClearString()
{
	this->m_strString.clear();
}
void cSanVisualKeyBoard::iSetKeyState(UINT Key,bool bState)
{
	if(Key>=KBD_SIZE)
	{
		return;
	}
	this->m_KBBuffer[Key]=bState;
}
void cSanVisualKeyBoard::iSetTTL(UINT TTL)
{
	this->m_TTL=TTL;
}
bool cSanVisualKeyBoard::iCheck(UINT Key,UINT Type)
{
	if(Key>KBD_SIZE)
	{
		SString strError=_SSTR("error: the key id is overflow");
		throw strError;
		return false;
	}
	if(Type==KBT_BUTTONUP)
	{
		if((this->m_KBBuffer[Key]==false)&&(this->m_KBHistory[Key]==true))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if(Type==KBT_BUTTONDOWN)
	{
		if((this->m_KBBuffer[Key]==true)&&(this->m_KBHistory[Key]==false))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
SString cSanVisualKeyBoard::iGetString()
{
	unsigned int size=::MultiByteToWideChar(CP_ACP,0,this->m_strString.c_str(),-1,NULL,0);
	SString strWString;
	if(size!=0)
	{
		wchar_t *pstrWString=new wchar_t[size];
		::MultiByteToWideChar(CP_ACP,0,this->m_strString.c_str(),-1,pstrWString,size);
		strWString=pstrWString;
		delete [] pstrWString;
	}
	else
	{
		strWString=_SSTR("");
	}
	return strWString;
}
#endif