#include"iostream"
#include"Core\SanTypes.h"
#include"Device\cSanTerminalDeviceWin.h"
#include"Device\cSanWindowObjectGL.h"
using namespace std;
using namespace San;
using namespace San::Device;
int main(int argc, char** argv)
{
	cSanTerminalDevice Terminal(_SSTR("San Plane Detection Terminal"));

	uint32 RenderBufferWidth = 640;
	uint32 RenderBufferHeight = 480;

	_sstream<sfloat> RenderBuffer(RenderBufferWidth * RenderBufferHeight * 4, 0.0);

	cSanWindowObject RenderWindow;

	return 0;
}