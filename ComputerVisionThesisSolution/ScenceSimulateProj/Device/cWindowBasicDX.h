#include"iostream"
#include"../RenderSystem.h"
#include"cWindowBasic.h"
using namespace std;
namespace San
{
#ifndef __CWINDOWCREATERDX_H__
#define __CWINDOWCREATERDX_H__
#ifdef _DIRECTX
	class cWindowCreaterDX :public cWindowCreater
	{
	protected:
		LPDIRECT3D9 m_pDX;
		LPDIRECT3DDEVICE9 m_pDXDevice;
		bool _OnInitEnvironments();
		bool _FreeImage();
	public:
		cWindowCreaterDX(LPCWSTR pWindowName,bool EdgeType,bool ScreenType=true,int Weight=200,int Height=100,int FullWeight=1024,int FullHeight=768,LPCWSTR pClassName=_SSTR("WindowClass"))
			:cWindowCreater(pWindowName,EdgeType,ScreenType,Weight,Height,FullWeight,FullHeight,pClassName)
		{
			this->m_pDX=NULL;
			this->m_pDXDevice=NULL;
		}
		cWindowCreaterDX(LPCTSTR pIniRoad)
			:cWindowCreater(pIniRoad)
		{
			this->m_pDX=NULL;
			this->m_pDXDevice=NULL;
		}
		~cWindowCreaterDX()
		{
			this->_FreeImage();
		};
		bool iShowWindow();
		virtual void iBasicDraw();
	};
#endif
#endif
}