//Project: San Lab Game Engine
//Version: 2.1.1
//Debug State: Add functions [render FPS]
#include"time.h"
#include"../Core/SanTypes.h"
//#include"../Core/SanMathematics.h"
#include"../Core/cVectorManager.h"
using namespace std;
namespace San
{
	namespace Device
	{
#ifndef __CWINDOWCREATER_H__
#define __CWINDOWCREATER_H__
		struct stWINDOWSIZESETTINGS
		{
			int32 WindowHeight;
			int32 WindowWidth;
			int32 FullHeight;
			int32 FullWidth;
		};

		class cWindowCreater
		{
		protected:
			HWND m_hWnd;
			HDC m_hDC;
			HINSTANCE m_hInstance;
			WNDCLASS m_WindowClass;
			int32 m_Paintx;
			int32 m_Painty;
			RECT m_WindowRect;
			stWINDOWSIZESETTINGS m_WindowSize;
			bool m_bOnActive;
			bool m_bFullScreen;
			SString m_strWindowTitle;
			SString m_strClassName;
			bool m_bShowEdge;
			DWORD m_dwExStyle;
			DWORD m_dwStyle;
			bool m_bUseIniFile;
			SString m_strIniFilePath;
			sfloat m_FPS;
			uint32 m_PerFrameWaitTime;
		protected:
			bool _RegisterClass();
			bool _SetPixelEnvironment();
			bool _SetDisplayMode();
			void _SwapBuffer();
			bool _INIFileRead(const SString &pIniFile);
			void _SetWindow(const SString &strWindowTitle, const SString &pClassName, const bool EdgeType, const bool ScreenType, const int32 Width, const int32 Height, const int32 FullWidth, const int32 FullHeight, const int32 Paintx, const int32 Painty, const DWORD EXStyle, const DWORD Style);
			virtual bool _OnInitEnvironments();
			virtual bool _FreeImage();
		public:
			cWindowCreater(const SString &strWindowTitle, const bool EdgeType, const bool ScreenType = true, const int32 Width = 200, const int32 Height = 100, const int32 FullWidth = 1024, const int32 FullHeight = 768, const SString &strClassName = _SSTR("WindowClass"), const sfloat RenderFPS = 30.0);
			cWindowCreater(const SString &strIniRoad);
			cWindowCreater(const SString &strWindowTitle, const HWND &hWnd);//cSanWIndowObject WindowHandle
			~cWindowCreater();
			bool iCreateWindows(LPARAM lParam = NULL);
			void iSetPaint(const int32 Paintx, const int32 Painty);
			void iSetFullSize(const int32 Width, const int32 Heigh);
			void iSetFullScreen(const bool bFullScreen);
			void iSetInstance(const HINSTANCE &hInstance);
			void iSetFPS(const sfloat FPS = 30.0);
			int32 iGetWindowHeight() const;
			int32 iGetWindowWidth() const;
			HWND iGetWnd() const;
			HINSTANCE iGetInstance() const;
			bool iGetEdgeType() const;
			sfloat iGetFPS() const;
			virtual LRESULT iBasicProc(HWND hWnd, uint32 uMsg, WPARAM wParam, LPARAM lParam);
			virtual int32 iBasicWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nCmdShow);
			virtual bool iShowWindow();
			virtual void iBasicDraw();
		};
#endif
	}
}