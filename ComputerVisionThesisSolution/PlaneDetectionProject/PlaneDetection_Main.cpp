#include"iostream"
#include"Core\SanTypes.h"
#include"Device\cSanTerminalDeviceWin.h"
#include"Device\cSanWindowObjectGL.h"
using namespace std;
using namespace San;
using namespace San::Device;
int main(int argc, char** argv)
{
	bool bUseTcpConnection = false; // -tcp [ip address] [port]
	bool bUseImageFile = false; // - img [file path]

	uint32 IPv4Address;
	uint32 PortNumber;

	SStringA strFilePath;

	sachar Buffer[1024];

	uint32 RenderBufferWidth = 640;
	uint32 RenderBufferHeight = 480;
	_sstream<sfloat> RenderBuffer(RenderBufferWidth * RenderBufferHeight * 4, 0.0);

	cSanTerminalDevice Terminal(_SSTR("San Plane Detection Terminal"));

	while(argc == 0)
	{
		Terminal.iOutputString(_SSTR("Please select data source mode [tcp/img]: "));
		::cin.getline(Buffer, 1024);

		if (SStringA(Buffer) == "tcp")
		{
			Terminal.iOutputString(_SSTR("Please enter server ip address: "));
			::cin.getline(Buffer, 1024);

			Terminal.iOutputString(_SSTR("Please enter port number: "));
			::cin.getline(Buffer, 1024);

			break;
		}
		if (SStringA(Buffer) == "img")
		{
			Terminal.iOutputString(_SSTR("Please enter image file path: "));
			::cin.getline(Buffer, 1024);

			strFilePath = Buffer;

			if (!strFilePath.empty())
			{
				//Load Image

				break;
			}
		}

		Terminal.iOutputString(_SSTR("Error: Invalid command\r\n"), STC_WHITE, STC_RED);
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

	cSanWindowObject RenderWindow(_SSTR("Plane Detection Render Window"), WinDesc, false);

	RenderWindow.iMountWindowRenderBuffer(RenderBuffer.pStream, RenderBufferWidth, RenderBufferHeight, 4, false);
	//RenderWindow.iRegisterMessageFunc(WindowMessageProcessFunc);
	RenderWindow.iCreateWindowObject(1);
	RenderWindow.iWinMain(::GetModuleHandle(nullptr), nullptr, nullptr, SW_SHOWNORMAL);

	return 0;
}