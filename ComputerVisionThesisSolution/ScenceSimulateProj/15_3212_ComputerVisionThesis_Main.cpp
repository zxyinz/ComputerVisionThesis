#include"iostream"
#include"thread"
#include"opencv2\core\core.hpp"
#include"opencv2\highgui\highgui.hpp"
#include"opencv2\nonfree\features2d.hpp"
#include"Device\cSanTerminalDeviceWin.h"
#include"Device\cSanWindowObjectGL.h"
#include"Device\cSanRenderWindow.h"
//#pragma comment(lib,"opencv_core246.lib")
//#pragma comment(lib,"opencv_imgproc246.lib")
//#pragma comment(lib,"opencv_highgui246.lib")
//#pragma comment(lib,"opencv_features2d246.lib")
//#pragma comment(lib,"opencv_nonfree246.lib")
using namespace std;
using namespace cv;
using namespace San;
using namespace San::Device;
void TerminalMessageProcessFunc(cSanTerminalDevice* pTerminal);

/*Render Window Definition*/
const uint32 WindowWidth = 900;
const uint32 WindowHeight = 600;
const sfloat FPS = 30.0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	cSanTerminalDevice Terminal(_SSTR("San Computer Vision Terminal"));

	//::thread TermianlThread(TerminalMessageProcessFunc, &Terminal);

	//::IplImage* pImage = ::cvLoadImage("E:\\Depth.bmp",0);

	//cv::Mat input(pImage);// = cv::imread("E:\\Depth.bmp", 0); //Load as grayscale

	//cv::SiftFeatureDetector detector;
	//std::vector<cv::KeyPoint> keypoints;
	//detector.detect(input, keypoints);

	//// Add results to image and save.
	//cv::Mat output;
	//cv::drawKeypoints(input, keypoints, output);
	//cv::imwrite("E:\\DepthC.bmp", output);

	SANWINDEVDESC WinDesc;
	WinDesc.str_title = _SSTR("San Computer Vision Render Screen");
	WinDesc.win_width = WindowWidth;
	WinDesc.win_height = WindowHeight;
	WinDesc.win_fullwidth = WindowWidth;
	WinDesc.win_fullheight = WindowHeight;
	WinDesc.win_classname = _SSTR("SanComputerVisionRenderWindow");
	WinDesc.b_win_fullscreen = false;
	WinDesc.b_win_has_edge = true;
	WinDesc.b_use_inifile = false;
	WinDesc.render_fps = 30.0;
	//WinDesc.str_inifile_path = _SSTR(".\\config.ini");

	cSanRenderWindow MainWindow(_SSTR("San Computer Vision"), WinDesc, false);
	MainWindow.iWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	/*cSanWindowObject RenderWindow(_SSTR("San Computer Vision"), WinDesc, true);
	RenderWindow.iCreateWindowObject(1);
	RenderWindow.iWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);*/

	return true;
}
void TerminalMessageProcessFunc(cSanTerminalDevice* pTerminal)
{
	if (pTerminal == nullptr)
	{
		return;
	}

	char Buffer[1024];

	while (true)
	{
		pTerminal->iOutputString(_SSTR("Commander>"), STC_GREY);

		::cin.getline(Buffer, 1024);

		vector<SString> strCommandList = ::gloGetStringItems(::gloAToT(Buffer));

		if (strCommandList.empty()){ continue; }

		if (strCommandList[0] == _SSTR("Exit")){ break; }

		pTerminal->iOutputString(_SSTR("Error: Invalid Command\r\n"), STC_WHITE, STC_RED);
	}

}