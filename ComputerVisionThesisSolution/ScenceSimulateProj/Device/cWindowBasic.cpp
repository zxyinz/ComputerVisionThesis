#include"cWindowBasic.h"
using namespace std;
using namespace San;
//using namespace San::Mathematics;
using namespace San::Device;
LRESULT CALLBACK WindowProc(HWND hWnd, uint32 uMsg, WPARAM wParam, LPARAM lParam);
cWindowCreater::cWindowCreater(const SString &strWindowTitle, const bool EdgeType, const bool ScreenType, const int32 Width, const int32 Height, const int32 FullWidth, const int32 FullHeight, const SString &strClassName, const sfloat RenderFPS)
	:m_FPS(RenderFPS),
	m_PerFrameWaitTime(RenderFPS <= 0.0 ? 0 : 1000.0 / RenderFPS)
{
	this->_SetWindow(strWindowTitle, strClassName, EdgeType, ScreenType, Width, Height, FullWidth, FullHeight, 0, 0, NULL, NULL);
	this->m_bUseIniFile = false;
	this->m_strIniFilePath.clear();
}
cWindowCreater::cWindowCreater(const SString &strIniRoad)
{
	this->m_hWnd = nullptr;
	this->m_hDC = nullptr;
	this->m_hInstance = nullptr;
	ZeroMemory(&(this->m_WindowClass), sizeof(WNDCLASS));
	if (!this->_INIFileRead(strIniRoad))
	{
		this->_SetWindow(_SSTR("Default San Window Device"), _SSTR("DefaultSanWindowClass"), true, false, 800, 600, 1024, 768, 0, 0, NULL, NULL);
		this->m_bUseIniFile = false;
		this->m_strIniFilePath.clear();
	}
	this->m_bUseIniFile = true;
	this->m_strIniFilePath = strIniRoad;
}
cWindowCreater::cWindowCreater(const SString &strWindowTitle, const HWND &hWnd)//cSanWIndowObject WindowHandle
	:m_FPS(30.0),
	m_PerFrameWaitTime(1000.0 / 30.0)
{
	this->m_strWindowTitle = strWindowTitle;
	this->m_hWnd = m_hWnd;
	this->m_hDC = ::GetWindowDC(this->m_hWnd);
}
cWindowCreater::~cWindowCreater()
{
	this->_FreeImage();
}
bool cWindowCreater::_RegisterClass()
{
	ZeroMemory(&(this->m_WindowClass), sizeof(this->m_WindowClass));

	this->m_WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	this->m_WindowClass.lpfnWndProc = WindowProc;
	this->m_WindowClass.cbClsExtra = NULL;
	this->m_WindowClass.cbWndExtra = NULL;
	this->m_WindowClass.hInstance = this->m_hInstance;
	this->m_WindowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	this->m_WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	this->m_WindowClass.hbrBackground = NULL;
	this->m_WindowClass.lpszMenuName = NULL;

	this->m_WindowClass.lpszClassName = this->m_strClassName.c_str();
	if (!(::RegisterClass(&(this->m_WindowClass))))
	{
		return false;
	}

	return true;
}
bool cWindowCreater::_SetDisplayMode()
{
	DEVMODE WindowsDisplaySettings;

	gloMemSet(&WindowsDisplaySettings, 0, sizeof(DEVMODE));
	WindowsDisplaySettings.dmSize = sizeof(DEVMODE);
	WindowsDisplaySettings.dmPelsHeight = this->m_WindowSize.FullHeight;
	WindowsDisplaySettings.dmPelsWidth = this->m_WindowSize.FullWidth;
	WindowsDisplaySettings.dmBitsPerPel = 32;
	WindowsDisplaySettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	if (ChangeDisplaySettings(&WindowsDisplaySettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		MessageBox(NULL, _SSTR("error:set display mode filed"), _SSTR("error"), MB_OK);
		return false;
	}

	return true;
}
void cWindowCreater::_SwapBuffer()
{
	SwapBuffers(this->m_hDC);
}
bool cWindowCreater::_INIFileRead(const SString &strIniFile)
{
	schar* pstrWindowTitle = new schar[128];
	schar* pstrClassName = new schar[128];

	this->m_WindowSize.WindowHeight = GetPrivateProfileInt(_SSTR("WindowInfo"), _SSTR("WindowHeight"), 0, strIniFile.c_str());
	this->m_WindowSize.WindowWidth = GetPrivateProfileInt(_SSTR("WindowInfo"), _SSTR("WindowWidth"), 0, strIniFile.c_str());
	this->m_WindowSize.FullHeight = GetPrivateProfileInt(_SSTR("WindowInfo"), _SSTR("WindowFullHeight"), 0, strIniFile.c_str());
	this->m_WindowSize.FullWidth = GetPrivateProfileInt(_SSTR("WindowInfo"), _SSTR("WindowFullWidth"), 0, strIniFile.c_str());

	if (!(this->m_WindowSize.FullHeight&&this->m_WindowSize.FullWidth&&this->m_WindowSize.WindowHeight&&this->m_WindowSize.WindowWidth))
	{
		delete[] pstrWindowTitle;
		pstrWindowTitle = nullptr;
		delete[] pstrClassName;
		pstrClassName = nullptr;

		return false;
	}

	this->m_Paintx = GetPrivateProfileInt(_SSTR("WindowInfo"), _SSTR("WindowPositionX"), 0, strIniFile.c_str());
	this->m_Painty = GetPrivateProfileInt(_SSTR("WindowInfo"), _SSTR("WindowPositionY"), 0, strIniFile.c_str());
	this->m_bFullScreen = bool(GetPrivateProfileInt(_SSTR("WindowInfo"), _SSTR("WindowFullScreen"), 0, strIniFile.c_str()) % 2);
	this->m_bShowEdge = bool(GetPrivateProfileInt(_SSTR("WindowInfo"), _SSTR("WindowShowEdge"), 0, strIniFile.c_str()) % 2);
	GetPrivateProfileString(_SSTR("WindowInfo"), _SSTR("WindowTittleString"), _SSTR("BasicWindow"), pstrWindowTitle, 128, strIniFile.c_str());
	GetPrivateProfileString(_SSTR("WindowInfo"), _SSTR("ClassNameString"), _SSTR("DefaultSanWindowClass"), pstrWindowTitle, 128, strIniFile.c_str());
	this->m_strWindowTitle = pstrWindowTitle;
	this->m_strClassName = pstrClassName;
	this->m_dwExStyle = NULL;
	this->m_dwStyle = NULL;

	delete[] pstrWindowTitle;
	pstrWindowTitle = nullptr;
	delete[] pstrClassName;
	pstrClassName = nullptr;

	return true;
}
void cWindowCreater::_SetWindow(const SString &strWindowTitle, const SString &strClassName, const bool EdgeType, const bool ScreenType, const int32 Width, const int32 Height, const int32 FullWidth, const int32 FullHeight, const int32 Paintx, const int32 Painty, const DWORD ExStyle, const DWORD Style)
{
	this->m_hWnd = nullptr;
	this->m_hDC = nullptr;
	this->m_hInstance = nullptr;
	ZeroMemory(&(this->m_WindowClass), sizeof(WNDCLASS));
	this->m_bOnActive = false;
	this->m_bFullScreen = ScreenType;
	this->m_Paintx = Paintx;
	this->m_Painty = Painty;
	this->m_WindowSize.WindowHeight = Height;
	this->m_WindowSize.WindowWidth = Width;
	this->m_WindowSize.FullHeight = FullHeight;
	this->m_WindowSize.FullWidth = FullWidth;
	this->m_strWindowTitle = strWindowTitle;
	this->m_strClassName = strClassName;
	this->m_bShowEdge = EdgeType;
	this->m_dwExStyle = ExStyle;
	this->m_dwStyle = Style;
}
bool cWindowCreater::_OnInitEnvironments()
{
	return true;
}
bool cWindowCreater::_FreeImage()
{
	if (this->m_bFullScreen == true)
	{
		ChangeDisplaySettings(nullptr, 0);
		ShowCursor(true);
	}
	if (this->m_hDC != nullptr)
	{
		ReleaseDC(this->m_hWnd, this->m_hDC);
		this->m_hDC = nullptr;
	}
	if (this->m_hWnd != nullptr)
	{
		DestroyWindow(this->m_hWnd);
		this->m_hWnd = nullptr;
	}
	if (this->m_hInstance != nullptr)
	{
		UnregisterClass(this->m_strClassName.c_str(), this->m_hInstance);
	}
	return true;
}
bool cWindowCreater::iCreateWindows(LPARAM lParam)
{
	if (lParam == NULL)
	{
		lParam = reinterpret_cast<LPARAM>(this);
	}

	if (!this->_RegisterClass())
	{
		MessageBox(NULL, _SSTR("error: Windows class register filed"), _SSTR("error"), MB_OK);
		return false;
	}

	if (this->m_bFullScreen)
	{
		if (!this->_SetDisplayMode())
		{
			MessageBox(NULL, _SSTR("error: Set full screen type filed"), _SSTR("error"), MB_OK);
			this->m_bFullScreen = false;
		}
	}

	if (this->m_bFullScreen)
	{
		this->m_WindowRect.left = 0;
		this->m_WindowRect.top = 0;
		this->m_WindowRect.right = this->m_WindowSize.FullWidth;
		this->m_WindowRect.bottom = this->m_WindowSize.FullHeight;
		this->m_dwExStyle = WS_EX_APPWINDOW | WS_EX_TOPMOST;
		this->m_dwStyle = WS_POPUP;
		ShowCursor(false);
	}
	else
	{
		this->m_WindowRect.left = 0;
		this->m_WindowRect.top = 0;
		this->m_WindowRect.right = this->m_WindowSize.WindowWidth;
		this->m_WindowRect.bottom = this->m_WindowSize.WindowHeight;
		if (this->m_bShowEdge)
		{
			this->m_dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			this->m_dwStyle = WS_OVERLAPPEDWINDOW;
		}
		else
		{
			this->m_dwExStyle = WS_EX_APPWINDOW;
			this->m_dwStyle = WS_POPUP;
		}
	}

	AdjustWindowRectEx(&m_WindowRect, this->m_dwStyle, false, this->m_dwExStyle);
	if (!(this->m_hWnd = CreateWindowEx(this->m_dwExStyle, this->m_strClassName.c_str(), this->m_strWindowTitle.c_str(), this->m_dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, this->m_Paintx, this->m_Painty, this->m_WindowRect.right, this->m_WindowRect.bottom, NULL, NULL, this->m_hInstance, reinterpret_cast<LPVOID>(lParam))))
	{
		this->_FreeImage();
		MessageBox(NULL, _SSTR("error: Create window filed"), _SSTR("error"), MB_OK);
		return false;
	}

	if (!this->_OnInitEnvironments()){ return false; }

	return true;
}
void cWindowCreater::iSetFullSize(const int32 Width, const int32 Height)
{
	this->m_WindowSize.FullHeight = Height;
	this->m_WindowSize.FullWidth = Width;
}
void cWindowCreater::iSetFullScreen(const bool bFullScreen)
{
	this->m_bFullScreen = bFullScreen;
}
void cWindowCreater::iSetInstance(const HINSTANCE &hInstance)
{
	this->m_hInstance = hInstance;
}
void cWindowCreater::iSetFPS(const sfloat FPS)
{
	this->m_FPS = FPS >= 0.0 ? FPS : this->m_FPS;

	this->m_PerFrameWaitTime = this->m_FPS <= 0.0 ? 0 : 1000.0 / this->m_FPS;
}
int32 cWindowCreater::iGetWindowHeight() const
{
	return this->m_bFullScreen ? this->m_WindowSize.FullHeight : this->m_WindowSize.WindowHeight;
}
int32 cWindowCreater::iGetWindowWidth() const
{
	return this->m_bFullScreen ? this->m_WindowSize.FullWidth : this->m_WindowSize.WindowWidth;
}
HWND cWindowCreater::iGetWnd() const
{
	return this->m_hWnd;
}
HINSTANCE cWindowCreater::iGetInstance() const
{
	return this->m_hInstance;
}
bool cWindowCreater::iGetEdgeType() const
{
	if (this->m_bFullScreen || (!this->m_bShowEdge))
	{
		return false;
	}

	return true;
}
sfloat cWindowCreater::iGetFPS() const
{
	return this->m_FPS;
}
LRESULT CALLBACK WindowProc(HWND hWnd, uint32 uMsg, WPARAM wParam, LPARAM lParam)
{
	int32 UserData;
	UserData = GetWindowLong(hWnd, GWL_USERDATA);
	if (UserData == 0)
	{
		if (uMsg == WM_CREATE)
		{
			CREATESTRUCT *creation = reinterpret_cast<CREATESTRUCT*>(lParam);
			cWindowCreater *pWindow = reinterpret_cast<cWindowCreater*>(creation->lpCreateParams);
			SetWindowLong(hWnd, GWL_USERDATA, reinterpret_cast<int32>(pWindow));
		}
	}
	else
	{
		cWindowCreater *pWindow;
		pWindow = reinterpret_cast<cWindowCreater*>(UserData);
		return pWindow->iBasicProc(hWnd, uMsg, wParam, lParam);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
LRESULT cWindowCreater::iBasicProc(HWND hWnd, uint32 uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SYSCOMMAND:
		break;
	case WM_CLOSE:
		PostMessage(this->m_hWnd, WM_QUIT, 0, 0);
		return 0;
		break;
	case WM_PAINT:
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
int32 cWindowCreater::iBasicWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nCmdShow)
{
	this->iSetInstance(hInstance);
	if (this->iCreateWindows())
	{
		if (!this->iShowWindow())
		{
			return false;
		}
		else
		{
			MSG msg;
			bool MessageOnActive = true;
			while (MessageOnActive)
			{
				if (PeekMessage(&msg, this->m_hWnd, 0, 0, PM_REMOVE))
				{
					if (msg.message != WM_QUIT)
					{
						DispatchMessage(&msg);
					}
					else
					{
						MessageOnActive = false;
					}
				}
				else
				{
					this->iBasicDraw();
				}
			}
			return true;
		}
	}
	else
	{
		return false;
	}
}
bool cWindowCreater::iShowWindow()
{
	::ShowWindow(this->m_hWnd, SW_SHOW);
	SetForegroundWindow(this->m_hWnd);
	SetFocus(this->m_hWnd);
	this->iBasicDraw();
	return true;
}
void cWindowCreater::iBasicDraw()
{
	clock_t StartTime = ::clock();

	this->_SwapBuffer();

	int32 CurrentFrameWaitTime = this->m_PerFrameWaitTime == 0 ? 0 : this->m_PerFrameWaitTime - (::clock() - StartTime);

	if (CurrentFrameWaitTime > 0){ ::Sleep(CurrentFrameWaitTime); }
}