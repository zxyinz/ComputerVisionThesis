#include"iostream"
#include"thread"
#include"Core\SanTypes.h"
#include"Device\cSanTerminalDeviceWin.h"
#include"Device\cSanWindowObjectGL.h"
#include"FileIO\cBMPReader.h"
#include"PlaneDetection.h"
using namespace std;
using namespace San;
using namespace San::Device;
using namespace San::FileIOStream;
uint32 WindowRenderFunc(uint8* pBuffer,const uint32 BufferWidth, const uint32 BufferHeight);
uint32 DebugWindowRenderFunc(uint8* pBuffer, const uint32 BufferWidth, const uint32 BufferHeight);

uint32 RenderBufferWidth = 640;
uint32 RenderBufferHeight = 480;

uint32 DebugBufferWidth = 640;
uint32 DebugBufferHeight = 480;

_sstream<uint8> RenderBuffer(RenderBufferWidth * RenderBufferHeight * 4, 0.0);
_sstream<uint8> DebugBuffer(DebugBufferWidth * DebugBufferHeight * 4, 0.0);

int main(int argc, char** argv)
{
	bool bUseTCPConnection = false; // -tcp [ip address] [port]
	bool bUseImageFile = false; // - img [file path]

	uint32 IPv4Address;
	uint32 PortNumber;

	SStringA strFilePath;

	sachar Buffer[1024];

	cSanTerminalDevice Terminal(_SSTR("San Plane Detection Terminal"));

#pragma region Command analysis
	if (argc >=  3)
	{
		if (SStringA(argv[1]) == "-tcp")
		{
			if (argc != 4)
			{
				Terminal.iOutputString(_SSTR("Error: Invalid command, -tcp should have 2 variables\r\n"), STC_WHITE, STC_RED);
				return 1;
			}

			bUseTCPConnection = true;
		}
		if (SStringA(argv[1]) == "-img")
		{
			if (argc != 3)
			{ 
				Terminal.iOutputString(_SSTR("Error: Invalid command, -img should have 1 variable\r\n"), STC_WHITE, STC_RED);
				return 1; 
			}

			strFilePath = argv[2];

			if (strFilePath.empty())
			{
				Terminal.iOutputString(_SSTR("Error: Invalid command, image file path is empty\r\n"), STC_WHITE, STC_RED);
				return 1;
			}

			bUseImageFile = true;
		}
	}

	while(argc == 1)
	{
		Terminal.iOutputString(_SSTR("Please select data source mode [tcp/img]: "));
		::cin.getline(Buffer, 1024);

		if (SStringA(Buffer) == "tcp")
		{
			Terminal.iOutputString(_SSTR("Please enter server ip address: "));
			::cin.getline(Buffer, 1024);

			Terminal.iOutputString(_SSTR("Please enter port number: "));
			::cin.getline(Buffer, 1024);

			bUseTCPConnection = true;
			break;
		}
		if (SStringA(Buffer) == "img")
		{
			Terminal.iOutputString(_SSTR("Please enter image file path: "));
			::cin.getline(Buffer, 1024);

			strFilePath = Buffer;

			if (!strFilePath.empty())
			{
				bUseImageFile = true;
				break;
			}
		}

		Terminal.iOutputString(_SSTR("Error: Invalid command\r\n"), STC_WHITE, STC_RED);
	}

	if (bUseTCPConnection == bUseImageFile)
	{
		Terminal.iOutputString(_SSTR("Error: Invalid command, -tcp and -img are conflict with each other\r\n"), STC_WHITE, STC_RED);
		return 1;
	}
#pragma endregion

	//Build TCP connection
	if (bUseTCPConnection)
	{

	}

	//Load Image
	if (bUseImageFile)
	{
		SStringW strImageFilePath = ::gloAToW(strFilePath);
		cBMPLoader Loader(strImageFilePath.c_str());

		if (Loader.iBMPLoad())
		{
			RenderBufferWidth = Loader.iBMPGetWeight();
			RenderBufferHeight = Loader.iBMPGetHigh();

			RenderBuffer.iReSizeStream(RenderBufferWidth * RenderBufferHeight * 4);

			uint8* pImg = Loader.iBMPGetImage();
			for (int32 seek = 0; seek < RenderBuffer.Size; seek = seek + 1)
			{
				RenderBuffer[seek] = pImg[seek];
			}

			delete[] pImg;
			pImg = nullptr;
		}
		else
		{
			Terminal.iOutputString(_SSTR("Error: Load image file failed\r\n"), STC_WHITE, STC_RED);
		}
	}

	//Resize debug buffer
	DebugBufferWidth = (RenderBufferWidth / 2 + (RenderBufferWidth % 2)) * 2;
	DebugBufferHeight = (RenderBufferHeight / 2 + (RenderBufferHeight % 2)) * 2;

	DebugBuffer.iReSizeStream(DebugBufferWidth*DebugBufferHeight * 4);
	DebugBuffer.iClear(0);

	//Create window
	::thread WindowRenderThread(WindowRenderFunc, RenderBuffer.pStream, RenderBufferWidth, RenderBufferHeight);

	::Sleep(100);

	::thread DebugWindowRenderThread(DebugWindowRenderFunc, DebugBuffer.pStream, DebugBufferWidth, DebugBufferHeight);

	//Plane detection
	PlaneDetectionFunc(RenderBuffer, RenderBufferWidth, RenderBufferHeight, DebugBuffer, DebugBufferWidth, DebugBufferHeight);

	::system("pause");

	return 0;
}
uint32 WindowRenderFunc(uint8* pBuffer,const uint32 BufferWidth, const uint32 BufferHeight)
{
	SANWINDEVDESC WinDesc;
	WinDesc.str_title = _SSTR("Plane Detection Render Window");
	WinDesc.win_width = BufferWidth;
	WinDesc.win_height = BufferHeight;
	WinDesc.win_fullwidth = BufferWidth;
	WinDesc.win_fullheight = BufferHeight;
	WinDesc.win_classname = _SSTR("PlaneDetectionRenderWindow");
	WinDesc.b_win_fullscreen = false;
	WinDesc.b_win_has_edge = true;
	WinDesc.b_use_inifile = false;
	WinDesc.render_fps = 30.0;

	cSanWindowObject RenderWindow(_SSTR("Plane Detection Render Window"), WinDesc, true);

	RenderWindow.iMountWindowRenderBuffer(pBuffer, BufferWidth, BufferHeight, 4, true);
	//RenderWindow.iRegisterMessageFunc(WindowMessageProcessFunc);
	RenderWindow.iCreateWindowObject(1);
	RenderWindow.iWinMain(::GetModuleHandle(nullptr), nullptr, nullptr, SW_SHOWNORMAL);

	return 0;
}
uint32 DebugWindowRenderFunc(uint8* pBuffer,const uint32 BufferWidth, const uint32 BufferHeight)
{
	SANWINDEVDESC WinDesc;
	WinDesc.str_title = _SSTR("Plane Detection Debug Window");
	WinDesc.win_width = BufferWidth;
	WinDesc.win_height = BufferHeight;
	WinDesc.win_fullwidth = BufferWidth;
	WinDesc.win_fullheight = BufferHeight;
	WinDesc.win_classname = _SSTR("PlaneDetectionDebugWindow");
	WinDesc.b_win_fullscreen = false;
	WinDesc.b_win_has_edge = true;
	WinDesc.b_use_inifile = false;
	WinDesc.render_fps = 30.0;

	cSanWindowObject RenderWindow(_SSTR("Plane Detection Debug Window"), WinDesc, true);

	RenderWindow.iMountWindowRenderBuffer(pBuffer, BufferWidth, BufferHeight, 4, false);
	//RenderWindow.iRegisterMessageFunc(WindowMessageProcessFunc);
	RenderWindow.iCreateWindowObject(2);
	RenderWindow.iWinMain(::GetModuleHandle(nullptr), nullptr, nullptr, SW_SHOWNORMAL);

	return 0;
}