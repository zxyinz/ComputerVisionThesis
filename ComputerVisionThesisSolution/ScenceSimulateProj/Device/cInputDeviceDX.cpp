#include"cInputDeviceDX.h"
using namespace San;
using namespace San::Device;
#ifdef _DXDEVICE
LPDIRECTINPUT8 cInputDeviceDX::m_pInputDevice;
SString cInputDeviceDX::m_Name;
bool cInputDeviceDX::m_bOnInit=true;
bool cInputDeviceDX::_CreateInputDevice(SString Name)
{
	cVectorManager *pVManager=new cVectorManager();
	HINSTANCE hInstance=(HINSTANCE)(pVManager->iFound(_SSTR("Sys_hInstance")));
	if(hInstance==NULL)
	{
		SString strError=_SSTR("error: can not find hInstance");
		throw strError;
		delete pVManager;
		return false;
	}
	if(this->m_pInputDevice!=NULL)
	{
		this->m_pInputDevice->Release();
	}
	if(FAILED(::DirectInput8Create(hInstance,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&(this->m_pInputDevice),NULL)))
	{
		SString strError=_SSTR("error: can not create direct input interface");
		throw strError;
		this->m_pInputDevice=NULL;
		this->m_Name.clear();
		delete pVManager;
		return false;
	}
	this->m_Name=Name;
	delete pVManager;
	return true;
}
bool cInputDeviceDX::_CreateInputDevice(HINSTANCE hInstance,SString Name)
{
	if(this->m_pInputDevice!=NULL)
	{
		this->m_pInputDevice->Release();
	}
	if(FAILED(::DirectInput8Create(hInstance,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&(this->m_pInputDevice),NULL)))
	{
		SString strError=_SSTR("error: can not create direct input interface");
		throw strError;
		this->m_pInputDevice=NULL;
		this->m_Name.clear();
		return false;
	}
	this->m_Name=Name;
	return true;
}
LPDIRECTINPUT8 cInputDeviceDX::_GetInputDevice()
{
	return this->m_pInputDevice;
}
#endif