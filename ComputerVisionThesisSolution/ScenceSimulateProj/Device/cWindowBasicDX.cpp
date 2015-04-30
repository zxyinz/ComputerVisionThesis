#include"iostream"
#include"cWindowBasicDX.h"
using namespace std;
using namespace San;
#ifdef _DIRECTX
bool cWindowCreaterDX::_OnInitEnvironments()
{
	D3DDISPLAYMODE DXDisplayMode;
	D3DCAPS9 DXCaps;
	this->m_pDX=::Direct3DCreate9(D3D_SDK_VERSION);
	if(this->m_pDX==NULL)
	{
		return false;
	}
	if(FAILED(this->m_pDX->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&DXDisplayMode)))
	{
		this->m_pDX->Release();
		this->m_pDX=NULL;
		return false;
	}
	if(FAILED(this->m_pDX->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&DXCaps)))
	{
		this->m_pDX->Release();
		this->m_pDX=NULL;
		return false;
	}
	UINT DXPROCESSING;
	if(DXCaps.VertexProcessingCaps!=0)
	{
		DXPROCESSING=D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_PUREDEVICE;
	}
	else
	{
		DXPROCESSING=D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	D3DMULTISAMPLE_TYPE DXMultiSampleLv=D3DMULTISAMPLE_NONE;
	if(this->m_pDX->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,DXDisplayMode.Format,!(this->m_bFullScreen),D3DMULTISAMPLE_2_SAMPLES,NULL)==D3D_OK)
	{
		DXMultiSampleLv=D3DMULTISAMPLE_2_SAMPLES;
	}
	if(this->m_pDX->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,DXDisplayMode.Format,!(this->m_bFullScreen),D3DMULTISAMPLE_4_SAMPLES,NULL)==D3D_OK)
	{
		DXMultiSampleLv=D3DMULTISAMPLE_4_SAMPLES;
	}
	if(this->m_pDX->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,DXDisplayMode.Format,!(this->m_bFullScreen),D3DMULTISAMPLE_8_SAMPLES,NULL)==D3D_OK)
	{
		DXMultiSampleLv=D3DMULTISAMPLE_8_SAMPLES;
	}
	if(this->m_pDX->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,DXDisplayMode.Format,!(this->m_bFullScreen),D3DMULTISAMPLE_16_SAMPLES,NULL)==D3D_OK)
	{
		DXMultiSampleLv=D3DMULTISAMPLE_16_SAMPLES;
	}
	D3DPRESENT_PARAMETERS D3DParam;
	::memset(&D3DParam,0,sizeof(D3DParam));
	D3DParam.Windowed=!(this->m_bFullScreen);
	D3DParam.PresentationInterval=D3DPRESENT_INTERVAL_DEFAULT;
	D3DParam.BackBufferWidth=this->iGetWindowWeight();
	D3DParam.BackBufferHeight=this->iGetWindowHeight();
	D3DParam.hDeviceWindow=this->iGetWnd();
	D3DParam.SwapEffect=D3DSWAPEFFECT_DISCARD;
	D3DParam.BackBufferFormat=DXDisplayMode.Format;
	D3DParam.BackBufferCount=1;
	D3DParam.EnableAutoDepthStencil=TRUE;
	D3DParam.AutoDepthStencilFormat=D3DFMT_D16;
	D3DParam.MultiSampleType=DXMultiSampleLv;
	if(this->m_pDX->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,this->m_hWnd,DXPROCESSING,&D3DParam,&(this->m_pDXDevice)))
	{
		this->m_pDX->Release();
		this->m_pDX=NULL;
		return false;
	}
	return true;
}
bool cWindowCreaterDX::_FreeImage()
{
	if(this->m_pDXDevice!=NULL)
	{
		this->m_pDXDevice->Release();
		this->m_pDXDevice=NULL;
	}
	if(this->m_pDX!=NULL)
	{
		this->m_pDX->Release();
		this->m_pDX=NULL;
	}
	return true;
}
bool cWindowCreaterDX::iShowWindow()
{
	::ShowWindow(this->m_hWnd,SW_SHOW);
	::SetForegroundWindow(this->m_hWnd);
	::SetFocus(this->m_hWnd);
	this->iBasicDraw();
	return true;
}
void cWindowCreaterDX::iBasicDraw()
{
	this->m_pDXDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0,0);
	this->m_pDXDevice->BeginScene();
	this->m_pDXDevice->EndScene();
	this->m_pDXDevice->Present(NULL,NULL,NULL,NULL);
}
#endif