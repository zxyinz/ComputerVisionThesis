#include"iostream"
#include"TCHAR.h"
#include"cWindowBasicGL.h"
using namespace std;
using namespace San;
#ifdef _OPENGL
LRESULT CALLBACK WindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
bool cWindowCreaterGL::_OnInitEnvironments()
{
	int PixelFormat;
	PIXELFORMATDESCRIPTOR pfd={
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0,0,0,0,0,0,
		0,
		0,
		0,
		0,0,0,0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};
	if(!(this->m_hDC=GetDC(this->m_hWnd)))
	{
		this->_FreeImage();
		MessageBox(NULL,_SSTR("error: Create DC filed"),_SSTR("error"),MB_OK);
		return false;
	}
	if(!(PixelFormat=ChoosePixelFormat(this->m_hDC,&pfd)))
	{
		this->_FreeImage();
		MessageBox(NULL,_SSTR("error: Find pixel format filed"),_SSTR("error"),MB_OK);
		return false;
	}
	if(!SetPixelFormat(this->m_hDC,PixelFormat,&pfd))
	{
		this->_FreeImage();
		MessageBox(NULL,_SSTR("error: Set pixel format filed"),_SSTR("error"),MB_OK);
		return false;
	}
	if(!(this->m_hRC=wglCreateContext(this->m_hDC)))
	{
		this->_FreeImage();
		MessageBox(NULL,_SSTR("error: Set openGL RC filed"),_SSTR("error"),MB_OK);
		return false;
	}
	if(!wglMakeCurrent(this->m_hDC,this->m_hRC))
	{
		this->_FreeImage();
		MessageBox(NULL,_SSTR("error: Current openGL environment filed"),_SSTR("error"),MB_OK);
		return false;
	}
	return true;
}
bool cWindowCreaterGL::_OnInitGlScene()
{
	::glShadeModel(GL_SMOOTH);
	::glClearColor(0,0,0,0);
	::glClearDepth(1);
	::glEnable(GL_DEPTH_TEST);
	::glDepthFunc(GL_LEQUAL);
	::glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	return true;
}
void cWindowCreaterGL::_SetGlWindow(int Weight,int High)
{
	if(High==0)
	{
		High=1;
	}
	::glViewport(0,0,Weight,High);
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();
	::gluPerspective(0,(sfloat)Weight/(sfloat)High,0,100);
	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
}
bool cWindowCreaterGL::iShowWindow()
{
	::ShowWindow(this->m_hWnd,SW_SHOW);
	SetForegroundWindow(this->m_hWnd);
	SetFocus(this->m_hWnd);
	if(this->m_bFullScreen)
	{
		this->_SetGlWindow(this->m_WindowSize.FullWidth ,this->m_WindowSize.FullHeight);
	}
	else
	{
		this->_SetGlWindow(this->m_WindowSize.WindowWidth,this->m_WindowSize.WindowHeight);
	}
	if(!this->_OnInitGlScene())
	{
		MessageBox(NULL,_SSTR("error: On Init openGl environments filed"),_SSTR("error"),MB_OK);
		return false;
	}
	this->iBasicDraw();
	return true;
}
bool cWindowCreaterGL::_FreeImage()
{
	if(this->m_hRC!=NULL)
	{
		::wglMakeCurrent(NULL,NULL);
		::wglDeleteContext(this->m_hRC);
		this->m_hRC=NULL;
	}
	return cWindowCreater::_FreeImage();
}
void cWindowCreaterGL::iBasicDraw()
{
	::glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	TextOut(this->m_hDC,0,0,_SSTR("WindowBasic"),::strnlen("WindowBasic",100));
	::glColor3f((0.92549),(0.91372),(0.84705));
	::glBegin(GL_POLYGON);
	::glVertex2f(-1,-1);
	::glVertex2f(-1,1);
	::glVertex2f(1,1);
	::glVertex2f(1,-1);
	::glEnd();
	::glFlush();
	this->_SwapBuffer();
}
#endif