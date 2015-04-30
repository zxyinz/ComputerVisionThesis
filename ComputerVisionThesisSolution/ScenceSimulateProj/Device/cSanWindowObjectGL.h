//Project: San Lab Game Engine
//Version: 2.1.1
//Debug State: Add functions, need test [render FPS]
#include"../Core/SanContainer.h"
#include"../Core/MsgDef.h"
#include"cWindowBasic.h"
#pragma once
using namespace std;
namespace San
{
	namespace Device
	{
#ifndef __CSANWINDOWOBJECTGL_H__
#define __CSANWINDOWOBJECTGL_H__
#ifdef _OPENGL
		struct stSANWINDOWDEVDESC
		{
		public:
			SString		str_title;
			uint32		win_height;
			uint32		win_width;
			uint32		win_fullheight;
			uint32		win_fullwidth;
			SString		win_classname;
			bool		b_win_fullscreen;
			bool		b_win_has_edge;
			bool		b_use_inifile;//Extened
			SString		str_inifile_path;//Extened
			sfloat		render_fps;
		public:
			stSANWINDOWDEVDESC()
				:str_title(_SSTR("san window object")),
				win_height(600),
				win_width(800),
				win_fullheight(768),
				win_fullwidth(1024),
				win_classname(_SSTR("SanWindowDev")),
				b_win_fullscreen(false),
				b_win_has_edge(true),
				b_use_inifile(false),
				str_inifile_path(_SSTR("")),
				render_fps(30.0)
			{};
			~stSANWINDOWDEVDESC(){};
		};
		typedef stSANWINDOWDEVDESC	SANWINDEVDESC;
		typedef stSANWINDOWDEVDESC*	lpSANWINDEVDESC;

		class cSanWindowObjectGL :protected cWindowCreater
		{
		private:
			SString m_strObjName;
			uint32	m_ResourceID;
			SHANDLE m_WindowHandle;
			HGLRC	m_GLHandle;
			SHANDLE m_WindowBufferPtr;
			uint8*	m_WindowPaintBufferPtr;
			uint32	m_BufferWidth;
			uint32	m_BufferHeight;
			uint32	m_BufferPixWidth;
			bool	m_bBufferLock;
			bool	m_bUseGLPaintOrder;
			bool	m_bSubWindow;
			bool	m_bPeekSystemMessage;///////////////////////////////////////////
			SRESULT	(*m_RenderFuncPtr)(SVALUE,SVALUE);
			SRESULT	(*m_MessageFuncPtr)(SHANDLE,uint32,SVALUE,SVALUE);
		protected:
			bool _OnInitEnvironments();
			bool _OnInitGLEnvironment();
			void _ReleaseGLEnvironment();
			void _UpdateGLWindowSize();
			void _DefaultRenderFun();
			LRESULT iBasicProc(HWND hWnd,uint32 uMsg,WPARAM wParam,LPARAM lParam);
			void iBasicDraw();
		public:
			cSanWindowObjectGL(const SString &strWindowName, const SANWINDEVDESC &Desc = SANWINDEVDESC(), const bool bSubWindow = false);
			cSanWindowObjectGL(const SString &strWindowName, const SString &strIniFilePath, const bool bSubWindow = false);
			cSanWindowObjectGL(const SString &strWindowName, const SHANDLE WindowHandle, const bool bPeekSystemMessage = true, const bool bSubWindow = false);
			~cSanWindowObjectGL();
			bool iCreateWindowObject(const uint32 ResID);
			void iReleaseWindowObject();
			void iShowWindow(const bool bShow = true);
			//void iSetRenderFocus(bool bFucus=true);
			bool iRegisterRenderFunc(SRESULT(*pRenderFunc)(SVALUE, SVALUE));// void RenderFun(SVALUE lParam,SVALUE wParam)
			void iReleaseRenderFunc();
			bool iRegisterMessageFunc(SRESULT(*pMesFunc)(SHANDLE, uint32, SVALUE, SVALUE));//SVALUE MessageFun(SHANDLE hWnd,uint32 Msg,SVALUE lParam,SVALUE wParam)
			void iReleaseMessageFunc();
			bool iMountWindowRenderBuffer(SHANDLE pBuffer, const uint32 Width, const uint32 Height, const uint32 PixWidthInByte = 4, const bool bUseGLPaintOrder = true);
			void iUnMountWindowRenderBuffer();
			int iWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
			SHANDLE iDoSomething(SHANDLE lParam, SHANDLE wParam);
		};
		typedef cSanWindowObjectGL cSanWindowObject;
#endif
#endif
	}
}