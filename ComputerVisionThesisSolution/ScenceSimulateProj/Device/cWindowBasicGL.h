#include"iostream"
#include"../RenderSystem.h"
#include"cWindowBasic.h"
using namespace std;
using namespace San::Device;
namespace San
{
#ifndef __CWINDOWCREATERGL_H__
#define __CWINDOWCREATERGL_H__
#ifdef _OPENGL
	class cWindowCreaterGL :public cWindowCreater
	{
	private:
		HGLRC m_hRC;
	protected:
		bool _SetDisplayMode();
		bool _OnInitEnvironments();
		bool _OnInitGlScene();
		virtual void _SetGlWindow(int Weight,int Height);
		bool _FreeImage();
	public:
		cWindowCreaterGL(LPCWSTR pWindowName,bool EdgeType,bool ScreenType=true,int Weight=200,int Height=100,int FullWeight=1024,int FullHeight=768,LPCWSTR pClassName=_SSTR("WindowClass"))
			:cWindowCreater(pWindowName,EdgeType,ScreenType,Weight,Height,FullWeight,FullHeight,pClassName)
		{
			this->m_hRC=NULL;
		}
		cWindowCreaterGL(LPCWSTR pIniRoad)
			:cWindowCreater(pIniRoad)
		{
			this->m_hRC=NULL;
		}
		~cWindowCreaterGL()
		{
			this->_FreeImage();
		};
		bool iShowWindow();
		virtual void iBasicDraw();
	};
#endif
#endif
}