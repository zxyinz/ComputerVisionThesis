#include"iostream"
#include"cKeyBoardDevice.h"
using namespace std;
using namespace San;
using namespace San::Device;
#ifdef _DXDEVICE
bool cKeyBoardDevice::iCreateDevice(UINT Type)
{
	cVectorManager *pVManager;
	if(this->m_pKeyBoardDevice==NULL)
	{
		if(FAILED(this->_GetInputDevice()->CreateDevice(GUID_SysKeyboard,&(this->m_pKeyBoardDevice),NULL)))
		{
			SString strError=_SSTR("error: can not create direct keyboard device");
			throw strError;
			return false;
		}
	}
	else
	{
		SString strError=_SSTR("error: the direct keyboard device already exist");
		throw strError;
		return true;
	}
	if(FAILED(this->m_pKeyBoardDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		SString strError=_SSTR("error: can not set data format");
		throw strError;
		return false;
	}
	pVManager=new cVectorManager();
	HWND hWnd=(HWND)(pVManager->iFound(_SSTR("Sys_hWnd")));
	if(hWnd==NULL)
	{
		SString strError=_SSTR("error: can not find hWnd");
		throw strError;
		this->iDeleteDevice();
		delete pVManager;
		return false;
	}
	if(FAILED(this->m_pKeyBoardDevice->SetCooperativeLevel(hWnd,Type)))
	{
		SString strError=_SSTR("error: can not set copper active level");
		throw strError;
		this->iDeleteDevice();
		delete pVManager;
		return false;
	}
	if(FAILED(this->m_pKeyBoardDevice->Acquire()))
	{
		SString strError=_SSTR("error: can not acquire device");
		throw strError;
	}
	delete pVManager;
	return true;
}
void cKeyBoardDevice::iReleaseDevice()
{
	if(this->m_pKeyBoardDevice!=NULL)
	{
		this->m_pKeyBoardDevice->Release();
		this->m_pKeyBoardDevice=NULL;
	}
	this->iClear();
}
bool cKeyBoardDevice::iUpdate()
{
	::memcpy(this->m_KBHistory,this->m_KBBuffer,sizeof(this->m_KBBuffer));
	if(this->m_pKeyBoardDevice==NULL)
	{
		SString strError=_SSTR("error: the keyboard device was undecleared");
		throw strError;
		return false;
	}
	if(FAILED(this->m_pKeyBoardDevice->GetDeviceState(sizeof(this->m_KBBuffer),(LPVOID)(this->m_KBBuffer))))
	{
		if(FAILED(this->m_pKeyBoardDevice->Acquire()))
		{
			SString strError=_SSTR("error: can not acauire device");
			throw strError;
			return false;
		}
		if(FAILED(this->m_pKeyBoardDevice->GetDeviceState(sizeof(this->m_KBBuffer),(LPVOID)(this->m_KBBuffer))))
		{
			SString strError=_SSTR("error: can not get decive state");
			throw strError;
			return false;
		}
	}
	this->_AddToString();
	return true;
}
void cKeyBoardDevice::_AddToString()
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
						//this->m_strString=this->m_strString.substr(0,this->m_strString.length()-1);
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
				//::cout<<this->m_strString<<"\n";
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
void cKeyBoardDevice::iClear()
{
	::memset((this->m_KBBuffer),0,sizeof(this->m_KBBuffer));
	::memset((this->m_KBHistory),0,sizeof(this->m_KBHistory));
	::memset((this->m_KBState),0,sizeof(this->m_KBState));
	this->m_strString.clear();
}
void cKeyBoardDevice::iClearString()
{
	this->m_strString.clear();
}
void cKeyBoardDevice::iSetTTL(UINT TTL)
{
	this->m_TTL=TTL;
}
bool cKeyBoardDevice::iCheck(UINT Key,UINT Type)
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
SString cKeyBoardDevice::iGetString()
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