#include"cSanCameraDef.h"
using namespace std;
using namespace San;
using namespace San::Graphics;
stSANCAMERADESC::stSANCAMERADESC()
	:CameraCoord(0.0,0.0,0.0),
	CameraLookAtPoint(0.0,0.0,0.0),
	CameraOrien(0.0,0.0,1.0),
	ViewFOV(45.0f),
	ViewNear(1.0f),
	ViewFar(1000.0f),
	ViewWidth(1024),
	ViewHeight(768),
	bCameraMovable(true),
	bIsOrthoCamera(false)
{
};
stSANCAMERADESC::stSANCAMERADESC(SPOINT3 CameraCoord,SPOINT3 CameraLookAtPoint,SVECTOR3 CameraOrien,sfloat ViewFOV,sfloat ViewNear,sfloat ViewFar,uint32 ViewWidth,uint32 ViewHeight,bool bIsOrthoCamera)
	:CameraCoord(CameraCoord),
	CameraLookAtPoint(CameraLookAtPoint),
	CameraOrien(CameraOrien),
	ViewFOV(ViewFOV),
	ViewNear(ViewNear),
	ViewFar(ViewFar),
	ViewWidth(ViewWidth),
	ViewHeight(ViewHeight),
	bCameraMovable(true),
	bIsOrthoCamera(bIsOrthoCamera)
{
};