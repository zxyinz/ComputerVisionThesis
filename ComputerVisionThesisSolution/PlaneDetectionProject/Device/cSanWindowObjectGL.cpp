#include"cSanWindowObjectGL.h"
using namespace std;
using namespace San;
using namespace San::Device;
cSanWindowObjectGL::cSanWindowObjectGL(const SString &strWindowName, const SANWINDEVDESC &Desc, const bool bSubWindow)
	:cWindowCreater(Desc.str_title.c_str(), Desc.b_win_has_edge, Desc.b_win_fullscreen,
	Desc.win_width, Desc.win_height, Desc.win_fullwidth, Desc.win_fullheight, Desc.win_classname.c_str(),Desc.render_fps),
	m_strObjName(strWindowName),
	m_ResourceID(0),
	m_WindowHandle(nullptr),
	m_GLHandle(nullptr),
	m_WindowBufferPtr(nullptr),
	m_WindowPaintBufferPtr(nullptr),
	m_BufferWidth(0),
	m_BufferHeight(0),
	m_BufferPixWidth(4),
	m_bBufferLock(false),
	m_bUseGLPaintOrder(true),
	m_bSubWindow(bSubWindow),
	m_bPeekSystemMessage(true),
	m_RenderFuncPtr(nullptr),
	m_MessageFuncPtr(nullptr)
{
	if (Desc.b_win_has_edge)
	{
		this->m_WindowSize.WindowWidth = this->m_WindowSize.WindowWidth + 8;
		this->m_WindowSize.WindowHeight = this->m_WindowSize.WindowHeight + 30;
	}
}
cSanWindowObjectGL::cSanWindowObjectGL(const SString &strWindowName, const SString &strIniFilePath, const bool bSubWindow)
	:cWindowCreater(strIniFilePath.c_str()),
	m_strObjName(strWindowName),
	m_ResourceID(0),
	m_WindowHandle(nullptr),
	m_GLHandle(nullptr),
	m_WindowBufferPtr(nullptr),
	m_WindowPaintBufferPtr(nullptr),
	m_BufferWidth(0),
	m_BufferHeight(0),
	m_BufferPixWidth(4),
	m_bBufferLock(false),
	m_bUseGLPaintOrder(true),
	m_bSubWindow(bSubWindow),
	m_bPeekSystemMessage(true),
	m_RenderFuncPtr(nullptr),
	m_MessageFuncPtr(nullptr)
{
	if (this->m_bShowEdge)
	{
		this->m_WindowSize.WindowWidth = this->m_WindowSize.WindowWidth + 8;
		this->m_WindowSize.WindowHeight = this->m_WindowSize.WindowHeight + 30;
	}
}
cSanWindowObjectGL::cSanWindowObjectGL(const SString &strWindowName, const SHANDLE WindowHandle, const bool bPeekSystemMessage, const bool bSubWindow)
	:cWindowCreater(strWindowName, (HWND) WindowHandle),
	m_strObjName(strWindowName),
	m_ResourceID(0),
	m_WindowHandle(WindowHandle),
	m_GLHandle(nullptr),
	m_WindowBufferPtr(nullptr),
	m_WindowPaintBufferPtr(nullptr),
	m_BufferWidth(0),
	m_BufferHeight(0),
	m_BufferPixWidth(4),
	m_bBufferLock(false),
	m_bUseGLPaintOrder(true),
	m_bSubWindow(bSubWindow),
	m_bPeekSystemMessage(bPeekSystemMessage),
	m_RenderFuncPtr(nullptr),
	m_MessageFuncPtr(nullptr)
{
}
cSanWindowObjectGL::~cSanWindowObjectGL()
{
	this->_ReleaseGLEnvironment();
	this->_FreeImage();
}
bool cSanWindowObjectGL::_OnInitEnvironments()
{ 
	return this->_OnInitGLEnvironment(); 
}
bool cSanWindowObjectGL::_OnInitGLEnvironment()
{
	if ((this->m_ResourceID == 0))
	{
		return false;
	}
	if (this->m_bSubWindow)
	{
		this->m_hDC = ::GetWindowDC(this->m_hWnd);
		if (this->m_hDC == nullptr)
		{
			return false;
		}
		return true;
	}
	if (this->m_hWnd == nullptr)
	{
		return false;
	}
	if (this->m_GLHandle != nullptr)
	{
		this->_ReleaseGLEnvironment();
	}
	if ((this->m_ResourceID != 0) && (this->m_GLHandle == nullptr))
	{
		int PixelFormat;
		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			32,
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			16,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};
		if (!(this->m_hDC = GetDC(this->m_hWnd)))
		{
			this->_FreeImage();
			MessageBox(NULL, _SSTR("error: Create DC filed"), _SSTR("error"), MB_OK);
			return false;
		}
		if (!(PixelFormat = ChoosePixelFormat(this->m_hDC, &pfd)))
		{
			this->_FreeImage();
			MessageBox(NULL, _SSTR("error: Find pixel format filed"), _SSTR("error"), MB_OK);
			return false;
		}
		if (!SetPixelFormat(this->m_hDC, PixelFormat, &pfd))
		{
			this->_FreeImage();
			MessageBox(NULL, _SSTR("error: Set pixel format filed"), _SSTR("error"), MB_OK);
			return false;
		}
		if (!(this->m_GLHandle = wglCreateContext(this->m_hDC)))
		{
			this->_FreeImage();
			MessageBox(NULL, _SSTR("error: Set openGL RC filed"), _SSTR("error"), MB_OK);
			return false;
		}
		if (!wglMakeCurrent(this->m_hDC, this->m_GLHandle))
		{
			this->_FreeImage();
			MessageBox(NULL, _SSTR("error: Current openGL environment filed"), _SSTR("error"), MB_OK);
			return false;
		}
		/*Init GL Environment*/
		::glewInit();
		::glShadeModel(GL_SMOOTH);
		::glClearColor(0, 0, 0, 0);
		::glClearDepth(1);
		::glEnable(GL_DEPTH_TEST);
		::glDepthFunc(GL_LEQUAL);
		::glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		/*Init GL View Port*/
		this->_UpdateGLWindowSize();
		return true;
	}
	return false;
}
void cSanWindowObjectGL::_ReleaseGLEnvironment()
{
	if (this->m_ResourceID == 0)
	{
		return;
	}
	if (!this->m_bSubWindow)
	{
		if (this->m_GLHandle != nullptr)
		{
			wglMakeCurrent(nullptr, nullptr);
			wglDeleteContext(this->m_GLHandle);
			this->m_GLHandle = nullptr;
		}
	}
}
void cSanWindowObjectGL::_UpdateGLWindowSize()
{
	if ((this->m_ResourceID == 0) || (this->m_GLHandle == nullptr) || (this->m_bSubWindow))
	{
		return;
	}
	uint32 win_height;
	uint32 win_width;
	if (this->m_bFullScreen)
	{
		win_height = this->m_WindowSize.FullHeight;
		win_width = this->m_WindowSize.FullWidth;
	}
	else
	{
		win_height = this->m_WindowSize.WindowHeight;
		win_width = this->m_WindowSize.WindowWidth;
	}
	if (win_width < 1)
	{
		win_width = 1;
	}
	::glViewport(0, 0, win_width, win_height);
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();
	::gluPerspective(45, win_height / win_width, 0, 100);
	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
}
void cSanWindowObjectGL::_DefaultRenderFun()
{
	if (this->m_ResourceID == 0)
	{
		return;
	}
	clock_t StartTime = ::clock();

	SVALUE wParam = 0;
	SVALUE lParam = 0;

	if (this->m_RenderFuncPtr != nullptr)
	{
		wParam = (SVALUE) (this);
		if (this->m_RenderFuncPtr(wParam, lParam) == 0)
		{
			this->_SwapBuffer();
		}
	}
	else
	{
		if (!this->m_bSubWindow)
		{
			::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			TextOut(this->m_hDC, 0, 0, _SSTR("WindowBasic"), ::strnlen("WindowBasic", 100));
			::glColor3f((0.92549), (0.91372), (0.84705));
			::glBegin(GL_POLYGON);
			::glVertex2f(-1, -1);
			::glVertex2f(-1, 1);
			::glVertex2f(1, 1);
			::glVertex2f(1, -1);
			::glEnd();
			::glFlush();
			this->_SwapBuffer();
		}
		else
		{
			if ((this->m_WindowBufferPtr != nullptr) && (!this->m_bBufferLock))
			{
				HDC BufferDC = ::CreateCompatibleDC(NULL);
				HBITMAP Buffer = ::CreateCompatibleBitmap(this->m_hDC, this->m_BufferWidth, this->m_BufferHeight);
				if (this->m_bUseGLPaintOrder)
				{
					uint32 index_src = 0;
					uint32 index_dest = 0;
					uint32 buf_width = this->m_BufferWidth*this->m_BufferPixWidth;
					for (uint32 seeky = 0; seeky < this->m_BufferHeight; seeky = seeky + 1)
					{
						index_src = (this->m_BufferHeight - 1 - seeky)*buf_width;
						index_dest = seeky*buf_width;
						for (uint32 seekx = 0; seekx < buf_width; seekx = seekx + 1)
						{
							this->m_WindowPaintBufferPtr[index_dest + seekx] = ((uint8*)this->m_WindowBufferPtr)[index_src + seekx];
						}
					}
				}
				::SetBitmapBits(Buffer, this->m_BufferWidth*this->m_BufferHeight*this->m_BufferPixWidth, this->m_WindowPaintBufferPtr);
				::SelectObject(BufferDC, Buffer);
				if (this->m_bShowEdge)
				{
					::BitBlt(this->m_hDC, 8, 30, this->m_BufferWidth, this->m_BufferHeight, BufferDC, 0, 0, SRCCOPY);
				}
				else
				{
					::BitBlt(this->m_hDC, 0, 0, this->m_BufferWidth, this->m_BufferHeight, BufferDC, 0, 0, SRCCOPY);
				}
				::DeleteObject(Buffer);
				::DeleteDC(BufferDC);
			}
		}
	}

	int32 CurrentFrameWaitTime = this->m_PerFrameWaitTime == 0 ? 0 : this->m_PerFrameWaitTime - (::clock() - StartTime);

	if (CurrentFrameWaitTime > 0){ ::Sleep(CurrentFrameWaitTime); }
}
LRESULT cSanWindowObjectGL::iBasicProc(HWND hWnd, uint32 uMsg, WPARAM wParam, LPARAM lParam)
{
	if (this->m_ResourceID == 0)
	{
		return 0;
	}
	if (this->m_MessageFuncPtr != 0)
	{
		return this->m_MessageFuncPtr(hWnd, uMsg, wParam, lParam);
	}
	switch (uMsg)
	{
	case WM_SYSCOMMAND:
		break;
	case WM_CLOSE:
		PostMessage(this->m_hWnd, WM_QUIT, 0, 0);
		return 0;
		break;
	case WM_PAINT:
		//this->_DefaultRenderFun();
		this->iBasicDraw();
		break;
	case WM_MOVE:
		return 1;
		break;
	case WM_SIZING:
		break;
	case WM_KEYDOWN:
		if (wParam == VK_F1)
		{
			PostMessage(this->m_hWnd, WM_QUIT, 0, 0);
		}
		return 0;
		break;
	case WM_KEYUP:
		break;
	case WM_ERASEBKGND:
		return 1;
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
void cSanWindowObjectGL::iBasicDraw()
{ 
	this->_DefaultRenderFun(); 
}
bool cSanWindowObjectGL::iCreateWindowObject(const uint32 ResID)
{
	if ((ResID != 0) && (this->m_ResourceID == 0))
	{
		this->m_ResourceID = ResID;
		return true;
	}
	return false;
}
void cSanWindowObjectGL::iReleaseWindowObject()
{
	this->m_RenderFuncPtr = nullptr;
	this->m_MessageFuncPtr = nullptr;
	this->m_WindowBufferPtr = nullptr;
	if ((this->m_bUseGLPaintOrder) && (this->m_WindowPaintBufferPtr != nullptr))
	{
		delete[] this->m_WindowPaintBufferPtr;
		this->m_WindowPaintBufferPtr = nullptr;
	}
	this->m_BufferWidth = 0;
	this->m_BufferHeight = 0;
	this->m_BufferPixWidth = 0;
	this->m_bUseGLPaintOrder = false;
	this->m_WindowHandle = nullptr;
	if (!this->m_bSubWindow)
	{
		this->_ReleaseGLEnvironment();
	}
	this->m_ResourceID = 0;
}
void cSanWindowObjectGL::iShowWindow(const bool bShow)
{
	//uint32 win_height;
	//uint32 win_width;
	if (bShow)
	{
		::ShowWindow(this->m_hWnd, SW_SHOW);
		SetForegroundWindow(this->m_hWnd);
		SetFocus(this->m_hWnd);
		/*if(this->m_bFullScreen)
		{
		win_height=this->m_WindowSize.FullHeight;
		win_width=this->m_WindowSize.FullWidth;
		}
		else
		{
		win_height=this->m_WindowSize.WindowHeight;
		win_width=this->m_WindowSize.WindowWidth;
		}
		::glViewport(0,0,win_height,win_width);
		::glMatrixMode(GL_PROJECTION);
		::glLoadIdentity();
		::gluPerspective(45,win_height/win_width,0,100);
		::glMatrixMode(GL_MODELVIEW);
		::glLoadIdentity();
		if(!this->_OnInitGlScene())
		{
		MessageBox(NULL,_SSTR("error: On Init openGl environments filed"),_SSTR("error"),MB_OK);
		return false;
		}*/
	}
	else
	{
		::ShowWindow(this->m_hWnd, SW_HIDE);
	}
}
bool cSanWindowObjectGL::iRegisterRenderFunc(SRESULT(*pRenderFunc)(SVALUE, SVALUE))
{
	if (this->m_ResourceID == 0)
	{
		return false;
	}
	if (pRenderFunc != nullptr)
	{
		this->m_RenderFuncPtr = pRenderFunc;
		return true;
	}
	return false;
}
void cSanWindowObjectGL::iReleaseRenderFunc()
{
	if (this->m_ResourceID == 0)
	{
		return;
	}
	this->m_RenderFuncPtr = nullptr;
}
bool cSanWindowObjectGL::iRegisterMessageFunc(SVALUE(*pMsgFunc)(SHANDLE, uint32, SVALUE, SVALUE))
{
	if (this->m_ResourceID == 0)
	{
		return false;
	}
	/*if(this->m_WindowHandle!=nullptr)
	{
	return false;
	}*/
	if (pMsgFunc != nullptr)
	{
		this->m_MessageFuncPtr = pMsgFunc;
		return true;
	}
	return false;
}
void cSanWindowObjectGL::iReleaseMessageFunc()
{
	if ((this->m_ResourceID == 0))
	{
		return;
	}
	/*if(this->m_WindowHandle!=nullptr)
	{
	return;
	}*/
	this->m_MessageFuncPtr = nullptr;
}
bool cSanWindowObjectGL::iMountWindowRenderBuffer(SHANDLE pBuffer, const uint32 Width, const uint32 Height, const uint32 PixWidthInByte, const bool bUseGLPaintOrder)
{
	if ((pBuffer == nullptr) || (Width == 0) || (Height == 0) || ((PixWidthInByte != 3) && (PixWidthInByte != 4)))
	{
		return false;
	}
	if (this->m_bSubWindow)
	{
		this->iUnMountWindowRenderBuffer();
		while (this->m_bBufferLock)
		{
		}
		this->m_bBufferLock = true;
		this->m_WindowBufferPtr = pBuffer;
		this->m_BufferWidth = Width;
		this->m_BufferHeight = Height;
		this->m_BufferPixWidth = PixWidthInByte;
		this->m_bUseGLPaintOrder = bUseGLPaintOrder;
		if (!bUseGLPaintOrder)
		{
			this->m_WindowPaintBufferPtr = (uint8*)this->m_WindowBufferPtr;
		}
		else
		{
			this->m_WindowPaintBufferPtr = new uint8[this->m_BufferWidth*this->m_BufferHeight*this->m_BufferPixWidth];
			::gloMemSet(this->m_WindowPaintBufferPtr, 0, this->m_BufferWidth*this->m_BufferHeight*this->m_BufferPixWidth);
		}
		this->m_bBufferLock = false;
		return true;
	}
	return false;
}
void cSanWindowObjectGL::iUnMountWindowRenderBuffer()
{
	if (this->m_bSubWindow)
	{
		while (this->m_bBufferLock)
		{
		}
		this->m_bBufferLock = true;
		if ((this->m_bUseGLPaintOrder) && (this->m_WindowPaintBufferPtr != nullptr))
		{
			delete[] this->m_WindowPaintBufferPtr;
			this->m_WindowPaintBufferPtr = nullptr;
		}
		this->m_WindowBufferPtr = nullptr;
		this->m_BufferWidth = 0;
		this->m_BufferHeight = 0;
		this->m_BufferPixWidth = 0;
		this->m_bUseGLPaintOrder = false;
		this->m_bBufferLock = false;
	}
}
int cSanWindowObjectGL::iWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (this->m_WindowHandle != nullptr)
	{
		this->m_hInstance = hInstance;
		this->_OnInitGLEnvironment();
		while (this->m_bPeekSystemMessage)////////////////////////////////////////////////////
		{
			MSG Message;
			if (::PeekMessage(&Message, this->m_hWnd, 0, 0, PM_REMOVE))
			{
				if (Message.message == WM_QUIT)
				{
					break;
				}
				this->iBasicProc(this->m_hWnd, Message.message, Message.wParam, Message.lParam);
				//::DispatchMessage(&Message);
			}
			else
			{
				this->_DefaultRenderFun();
			}
		}
		return 0;
	}
	return this->iBasicWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
SHANDLE cSanWindowObjectGL::iDoSomething(SHANDLE wParam, SHANDLE lParam)
{
	switch ((uint32) wParam)
	{
	case MSG_RENDER:
		this->_DefaultRenderFun();
		break;
	case MSG_CHECKMESSAGE:
		this->iBasicProc(nullptr, 0, (WPARAM) wParam, (LPARAM) lParam);
		break;
	default:
		break;
	}
	return 0;
}