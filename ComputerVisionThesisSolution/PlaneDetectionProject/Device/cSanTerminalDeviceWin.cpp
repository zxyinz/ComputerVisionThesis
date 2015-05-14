#include"cSanTerminalDeviceWin.h"
using namespace std;
using namespace San;
using namespace San::Device;
#ifdef _WINDOWS
bool cSanTerminalDeviceWin::m_bTerminalLock = false;
cSanTerminalDeviceWin::cSanTerminalDeviceWin(SString strTerminalName, SString strTerminalTittle)
	:m_strTerminalName(strTerminalName),
	m_TerminalID(0),
	m_TerminalHandle(nullptr),
	m_strTittle(strTerminalTittle),
	m_strCodeLocate(""),
	m_TextDefColor(STC_WHITE),
	m_BackgroundDefColor(STC_BLACK),
	m_pTerminalBuffer(nullptr),
	m_BufferSize(1024),
	m_strBuffer(_SSTR(" "))
{
	this->m_TerminalHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if (this->m_TerminalHandle == nullptr)
	{
		this->_CreateConsoleDevice();
	}
	this->m_TerminalHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	::SetConsoleTitle(this->m_strTittle.c_str());
	this->m_strCodeLocate = ::setlocale(LC_ALL, this->m_strCodeLocate.c_str());
	this->m_pTerminalBuffer = new schar[this->m_BufferSize];
	::memset(this->m_pTerminalBuffer, 0, sizeof(schar) *this->m_BufferSize);
	::SetConsoleTextAttribute(this->m_TerminalHandle, (this->m_TextDefColor) | (this->m_BackgroundDefColor << 4));
};
cSanTerminalDeviceWin::~cSanTerminalDeviceWin()
{
	delete[] this->m_pTerminalBuffer;
	this->m_pTerminalBuffer = nullptr;
};
void cSanTerminalDeviceWin::iSetTerminalTittle(SString strTittle)
{
	if(!strTittle.empty())
	{
		this->m_strTittle=strTittle;
		::SetConsoleTitle(this->m_strTittle.c_str());
	}
}
void cSanTerminalDeviceWin::iSetTerminalCodeLocate(SStringA strLocate)
{
	this->m_strCodeLocate=::setlocale(LC_ALL,strLocate.c_str());
}
void cSanTerminalDeviceWin::iSetDefaultTextColor(eSANTERMINALCOLOR Color)
{
	if(Color!=STC_DEFAULT)
	{
		this->m_TextDefColor=Color;
		::SetConsoleTextAttribute(this->m_TerminalHandle,this->m_TextDefColor|(this->m_BackgroundDefColor<<4));
	}
}
void cSanTerminalDeviceWin::iSetDefaultBackgroundColor(eSANTERMINALCOLOR Color)
{
	if(Color!=STC_DEFAULT)
	{
		this->m_BackgroundDefColor=Color;
		::SetConsoleTextAttribute(this->m_TerminalHandle,this->m_TextDefColor|(this->m_BackgroundDefColor<<4));
	}
}
void cSanTerminalDeviceWin::_CreateConsoleDevice()
{
	::AllocConsole();
	::AttachConsole(ATTACH_PARENT_PROCESS);
	::freopen("CONIN$","r+t",stdin);
	::freopen("CONOUT$","w+t",stdout);
}
void cSanTerminalDeviceWin::_OutputStringA(SStringA strString, eSANTERMINALCOLOR TextColor, eSANTERMINALCOLOR BackgroundColor)
{
	if ((TextColor == STC_DEFAULT) && (BackgroundColor == STC_DEFAULT))
	{
		//::printf("%s",strString.c_str());
		::cout << strString.c_str();
	}
	else
	{
		if (TextColor == STC_DEFAULT)
		{
			TextColor = this->m_TextDefColor;
		}
		if (BackgroundColor == STC_DEFAULT)
		{
			BackgroundColor = this->m_BackgroundDefColor;
		}
		::SetConsoleTextAttribute(this->m_TerminalHandle, TextColor | (BackgroundColor << 4));
		//::printf("%s",strString.c_str());
		::cout << strString.c_str();
		::SetConsoleTextAttribute(this->m_TerminalHandle, this->m_TextDefColor | (this->m_BackgroundDefColor << 4));
	}
}
void cSanTerminalDeviceWin::_OutputStringW(SStringW strString, eSANTERMINALCOLOR TextColor, eSANTERMINALCOLOR BackgroundColor)
{
	if ((TextColor == STC_DEFAULT) && (BackgroundColor == STC_DEFAULT))
	{
		//::wprintf(L"%s",strString.c_str());
		::wcout << strString.c_str();
	}
	else
	{
		if (TextColor == STC_DEFAULT)
		{
			TextColor = this->m_TextDefColor;
		}
		if (BackgroundColor == STC_DEFAULT)
		{
			BackgroundColor = this->m_BackgroundDefColor;
		}
		::SetConsoleTextAttribute(this->m_TerminalHandle, TextColor | (BackgroundColor << 4));
		//::wprintf(L"%s",strString.c_str());
		::wcout << strString.c_str();
		::SetConsoleTextAttribute(this->m_TerminalHandle, this->m_TextDefColor | (this->m_BackgroundDefColor << 4));
	}
}
void cSanTerminalDeviceWin::_LockTerminal()
{
	while (this->m_bTerminalLock)
	{
	}
	this->m_bTerminalLock = true;
}
void cSanTerminalDeviceWin::_UnLockTerminal()
{
	this->m_bTerminalLock = false;
}
bool cSanTerminalDeviceWin::iSetTerminalBufferSize(uint32 BufferSize)
{
	if(BufferSize==0)
	{
		return false;
	}
	delete[] this->m_pTerminalBuffer;
	this->m_pTerminalBuffer=nullptr;
	this->m_pTerminalBuffer=new schar[BufferSize];
	::memset(this->m_pTerminalBuffer,0,sizeof(schar)*BufferSize);
	this->m_BufferSize=BufferSize;
	return true;
}
SString cSanTerminalDeviceWin::iGetTerminalTittle()
{
	return this->m_strTittle;
}
SStringA cSanTerminalDeviceWin::iGetTerminalCodeLocate()
{
	return this->m_strCodeLocate;
}
eSANTERMINALCOLOR cSanTerminalDeviceWin::iGetDefaultTextColor()
{
	return this->m_TextDefColor;
}
eSANTERMINALCOLOR cSanTerminalDeviceWin::iGetDefaultBackgroundColor()
{
	return this->m_BackgroundDefColor;
}
uint32 cSanTerminalDeviceWin::iGetTerminalBufferSize()
{
	return this->m_BufferSize;
}
SPOINT3 cSanTerminalDeviceWin::iGetCurrentCursorPosition()
{
	CONSOLE_SCREEN_BUFFER_INFO BufferInfo;
	::GetConsoleScreenBufferInfo(this->m_TerminalHandle, &BufferInfo);
	return SPOINT3(BufferInfo.dwCursorPosition.X, BufferInfo.dwCursorPosition.Y, 0);
}
SString cSanTerminalDeviceWin::iGetInputString(schar DelLim)
{
#ifndef _UNICODE
	::getline(cin,this->m_strBuffer,DelLim);
#else
	::getline(wcin,this->m_strBuffer,DelLim);
#endif
	return this->m_strBuffer;
}
void cSanTerminalDeviceWin::iOutputStringA(SStringA strString,eSANTERMINALCOLOR TextColor,eSANTERMINALCOLOR BackgroundColor)
{
	if(strString.empty())
	{
		return;
	}
	this->_LockTerminal();
	this->_OutputStringA(strString, TextColor, BackgroundColor);
	this->_UnLockTerminal();
}
void cSanTerminalDeviceWin::iOutputStringW(SStringW strString,eSANTERMINALCOLOR TextColor,eSANTERMINALCOLOR BackgroundColor)
{
	if(strString.empty())
	{
		return;
	}
	this->_LockTerminal();
	this->_OutputStringW(strString, TextColor, BackgroundColor);
	this->_UnLockTerminal();
}
void cSanTerminalDeviceWin::iOutputString(SString strString,eSANTERMINALCOLOR TextColor,eSANTERMINALCOLOR BackgroundColor)
{
#ifndef _UNICODE
	this->iOutputStringA(strString,TextColor,BackgroundColor);
#else
	this->iOutputStringW(strString,TextColor,BackgroundColor);
#endif
}
void cSanTerminalDeviceWin::iOutputStringLineA(SStringA strString, eSANTERMINALCOLOR TextColor, eSANTERMINALCOLOR BackgroundColor)
{
	return this->iOutputStringA(strString + "\r\n", TextColor, BackgroundColor);
}
void cSanTerminalDeviceWin::iOutputStringLineW(SStringW strString, eSANTERMINALCOLOR TextColor, eSANTERMINALCOLOR BackgroundColor)
{
	return this->iOutputStringW(strString + L"\r\n", TextColor, BackgroundColor);
}
void cSanTerminalDeviceWin::iOutputStringLine(SString strString, eSANTERMINALCOLOR TextColor, eSANTERMINALCOLOR BackgroundColor)
{
#ifndef _UNICODE
	return this->iOutputStringA(strString + "\r\n", TextColor, BackgroundColor);
#else
	return this->iOutputStringW(strString + L"\r\n", TextColor, BackgroundColor);
#endif
}
void cSanTerminalDeviceWin::iOutputStringA(SStringA strString, const SPOINT3 &Pos, eSANTERMINALCOLOR TextColor, eSANTERMINALCOLOR BackgroundColor)
{
	this->_LockTerminal();
	CONSOLE_SCREEN_BUFFER_INFO BufferInfo;
	::GetConsoleScreenBufferInfo(this->m_TerminalHandle, &BufferInfo);

	COORD Coord;
	Coord.X = Pos.x;
	Coord.Y = Pos.y;

	::SetConsoleCursorPosition(this->m_TerminalHandle, Coord);
	this->_OutputStringA(strString, TextColor, BackgroundColor);

	::SetConsoleCursorPosition(this->m_TerminalHandle, BufferInfo.dwCursorPosition);
	this->_UnLockTerminal();
}
void cSanTerminalDeviceWin::iOutputStringW(SStringW strString, const SPOINT3 &Pos, eSANTERMINALCOLOR TextColor, eSANTERMINALCOLOR BackgroundColor)
{
	this->_LockTerminal();
	CONSOLE_SCREEN_BUFFER_INFO BufferInfo;
	::GetConsoleScreenBufferInfo(this->m_TerminalHandle, &BufferInfo);

	COORD Coord;
	Coord.X = Pos.x;
	Coord.Y = Pos.y;

	::SetConsoleCursorPosition(this->m_TerminalHandle, Coord);
	this->_OutputStringW(strString, TextColor, BackgroundColor);

	::SetConsoleCursorPosition(this->m_TerminalHandle, BufferInfo.dwCursorPosition);
	this->_UnLockTerminal();
}
void cSanTerminalDeviceWin::iOutputString(SString strString, const SPOINT3 &Pos, eSANTERMINALCOLOR TextColor, eSANTERMINALCOLOR BackgroundColor)
{
#ifndef _UNICODE
	this->iOutputStringA(strString, Pos, TextColor, BackgroundColor);
#else
	this->iOutputStringW(strString, Pos, TextColor, BackgroundColor);
#endif
}
//void cSanTerminalDeviceWin::iClearLine()
//{
void cSanTerminalDeviceWin::iClearScreen()
{
	this->_LockTerminal();
	::system("cls");
	this->_UnLockTerminal();
}
#endif