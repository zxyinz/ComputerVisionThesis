#include"iostream"
#include"Core\SanTypes.h"
#include"Device\cSanTerminalDeviceWin.h"
#include"Device\cSanWindowObjectGL.h"
#include"FileIO\cBMPReader.h"
using namespace std;
using namespace San;
using namespace San::Device;
using namespace San::FileIOStream;
int main(int argc, char** argv)
{
	bool bUseTCPConnection = false; // -tcp [ip address] [port]
	bool bUseImageFile = false; // - img [file path]

	uint32 IPv4Address;
	uint32 PortNumber;

	SStringA strFilePath;

	sachar Buffer[1024];

	uint32 RenderBufferWidth = 640;
	uint32 RenderBufferHeight = 480;
	_sstream<uint8> RenderBuffer(RenderBufferWidth * RenderBufferHeight * 4, 0.0);

	cSanTerminalDevice Terminal(_SSTR("San Plane Detection Terminal"));

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

	SANWINDEVDESC WinDesc;
	WinDesc.str_title = _SSTR("Plane Detection Render Window");
	WinDesc.win_width = RenderBufferWidth;
	WinDesc.win_height = RenderBufferHeight;
	WinDesc.win_fullwidth = RenderBufferWidth;
	WinDesc.win_fullheight = RenderBufferHeight;
	WinDesc.win_classname = _SSTR("PlaneDetectionRenderWIndow");
	WinDesc.b_win_fullscreen = false;
	WinDesc.b_win_has_edge = true;
	WinDesc.b_use_inifile = false;
	WinDesc.render_fps = 30.0;

	cSanWindowObject RenderWindow(_SSTR("Plane Detection Render Window"), WinDesc, true);

	RenderWindow.iMountWindowRenderBuffer(RenderBuffer.pStream, RenderBufferWidth, RenderBufferHeight, 4, true);
	//RenderWindow.iRegisterMessageFunc(WindowMessageProcessFunc);
	RenderWindow.iCreateWindowObject(1);
	RenderWindow.iWinMain(::GetModuleHandle(nullptr), nullptr, nullptr, SW_SHOWNORMAL);

	return 0;
}