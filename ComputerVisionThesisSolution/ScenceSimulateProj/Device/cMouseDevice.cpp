#include"iostream"
#include"cMouseDevice.h"
using namespace std;
using namespace San;
using namespace San::Device;
#ifdef _DXDEVICE
bool cMouseDevice::iCreateDevice(UINT Type)
{
	cVectorManager *pVManager;
	if(this->m_pMouseDevice==NULL)
	{
		if(FAILED(this->_GetInputDevice()->CreateDevice(GUID_SysMouse,&(this->m_pMouseDevice),NULL)))
		{
			SString strError=_SSTR("error: can not create direct mouse device");
			throw strError;
			return false;
		}
	}
	else
	{
		SString strError=_SSTR("error: the direct mouse device already exist");
		throw strError;
		return true;
	}
	if(FAILED(this->m_pMouseDevice->SetDataFormat(&c_dfDIMouse)))
	{
		SString strError=_SSTR("error: can not set data format");
		throw strError;
		this->iDeleteDevice();
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
	}
	if(FAILED(this->m_pMouseDevice->SetCooperativeLevel(hWnd,Type)))
	{
		SString strError=_SSTR("error: can not set copper active level");
		throw strError;
		this->iDeleteDevice();
		delete pVManager;
		return false;
	}
	DIPROPDWORD dipMouseState;
	dipMouseState.diph.dwSize=sizeof(dipMouseState);
	dipMouseState.diph.dwHeaderSize=sizeof(DIPROPHEADER);
	dipMouseState.diph.dwObj=NULL;
	dipMouseState.diph.dwHow=DIPH_DEVICE;
	dipMouseState.dwData=MD_SIZE;
	if(FAILED(this->m_pMouseDevice->SetProperty(DIPROP_BUFFERSIZE,&(dipMouseState.diph))))
	{
		SString strError=_SSTR("error: can not set property");
		throw strError;
		this->iDeleteDevice();
		delete pVManager;
	}
	if(FAILED(this->m_pMouseDevice->Acquire()))
	{
		SString strError=_SSTR("error: can not acquire mouse device");
		throw strError;
	}
	delete pVManager;
	return true;
}
void cMouseDevice::iReleaseDevice()
{
	if(this->m_pMouseDevice!=NULL)
	{
		this->m_pMouseDevice->Release();
		this->m_pMouseDevice=NULL;
	}
	::memset((this->m_MDBuffer),0,sizeof(this->m_MDBuffer));
}
bool cMouseDevice::iUpdate()
{
	DWORD DataNumber=1;
	if(this->m_pMouseDevice==NULL)
	{
		SString strError=_SSTR("error: the mouse device was undecleared");
		throw strError;
		return false;
	}
	::memset((this->m_MDBuffer),0,sizeof(this->m_MDBuffer));
	for(int seek=0;seek<MD_SIZE;seek=seek+1)
	{
		if(this->m_pMouseDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA),&(this->m_MDBuffer[seek]),&DataNumber,0)==DIERR_INPUTLOST)
		{
			if(FAILED(this->m_pMouseDevice->Acquire()))
			{
				SString strError=_SSTR("error: can not acquire mouse device");
				throw strError;
				return false;
			}
			if(FAILED(this->m_pMouseDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA),&(this->m_MDBuffer[seek]),&DataNumber,0)))
			{
				SString strError=_SSTR("error: can not get device data");
				throw strError;
				return false;
			}
		}
		stSPOINTEX<long> PosOffset;
		::memset(&(PosOffset),0,sizeof(PosOffset));
		if(this->m_MDBuffer[seek].dwOfs==DIMOFS_X)
		{
			PosOffset.x=PosOffset.x+this->m_MDBuffer[seek].dwData;
		}
		if(this->m_MDBuffer[seek].dwOfs==DIMOFS_Y)
		{
			PosOffset.y=PosOffset.y+this->m_MDBuffer[seek].dwData;
		}
		if(this->m_MDBuffer[seek].dwOfs==DIMOFS_Z)
		{
			PosOffset.z=PosOffset.z+this->m_MDBuffer[seek].dwData;
		}
		if((PosOffset.x!=0)||(PosOffset.y!=0)||(PosOffset.z!=0))
		{
			this->m_bMouseMove=true;
		}
		this->m_MousePos.x=this->m_MousePos.x+PosOffset.x;
		this->m_MousePos.y=this->m_MousePos.y+PosOffset.y;
		this->m_MousePos.z=this->m_MousePos.z+PosOffset.z;
	}
	::SetCursorPos(this->m_MousePos.x,this->m_MousePos.y);
	return true;
}
void cMouseDevice::iClear()
{
	::memset((this->m_MDBuffer),0,sizeof(this->m_MDBuffer));
	::memset(&(this->m_MousePos),0,sizeof(this->m_MousePos));
}
void cMouseDevice::iSetMousePos(stSPOINTEX<long> Pos)
{
	this->m_MousePos.x=Pos.x;
	this->m_MousePos.y=Pos.y;
	this->m_MousePos.z=Pos.z;
	::SetCursorPos(this->m_MousePos.x,this->m_MousePos.y);
}
void cMouseDevice::iSetMousePos(long x,long y,long z)
{
	this->m_MousePos.x=x;
	this->m_MousePos.y=y;
	this->m_MousePos.z=z;
	::SetCursorPos(this->m_MousePos.x,this->m_MousePos.y);
}
stSPOINTEX<long> cMouseDevice::iGetMousePos()
{
	return this->m_MousePos;
}
DWORD cMouseDevice::iGetMouseState()
{
	DWORD State=NULL;
	for(int seek=0;seek<MD_SIZE;seek=seek+1)
	{
		if((this->m_MDBuffer[seek].dwOfs==DIMOFS_BUTTON0)&&(this->m_MDBuffer[seek].dwData&0x80))
		{
			State=State|MDS_LBUTTON|MDS_BUTTONDOWN;
		}
		if((this->m_MDBuffer[seek].dwOfs==DIMOFS_BUTTON0)&&!(this->m_MDBuffer[seek].dwData&0x80))
		{
			State=State|MDS_LBUTTON|MDS_BUTTONUP;
		}
		if((this->m_MDBuffer[seek].dwOfs==DIMOFS_BUTTON1)&&(this->m_MDBuffer[seek].dwData&0x80))
		{
			State=State|MDS_RBUTTON|MDS_BUTTONDOWN;
		}
		if((this->m_MDBuffer[seek].dwOfs==DIMOFS_BUTTON1)&&!(this->m_MDBuffer[seek].dwData&0x80))
		{
			State=State|MDS_RBUTTON|MDS_BUTTONUP;
		}
		if((this->m_MDBuffer[seek].dwOfs==DIMOFS_BUTTON2)&&(this->m_MDBuffer[seek].dwData&0x80))
		{
			State=State|MDS_MBUTTON|MDS_BUTTONUP;
		}
		if((this->m_MDBuffer[seek].dwOfs==DIMOFS_BUTTON2)&&!(this->m_MDBuffer[seek].dwData&0x80))
		{
			State=State|MDS_MBUTTON|MDS_BUTTONDOWN;
		}
	}
	if(this->m_bMouseMove)
	{
		State=State|MDS_MOVE;
	}
	this->m_bMouseMove=false;
	return State;
}
#endif