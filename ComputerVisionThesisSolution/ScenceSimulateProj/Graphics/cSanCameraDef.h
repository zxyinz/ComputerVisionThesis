#include"../Core/cSanObject.h"
#include"SanGraphics.h"
using namespace std;
#pragma once
namespace San
{
	namespace Graphics
	{
#ifndef __CSANCAMERADEF_H__
#define __CSANCAMERADEF_H__
		struct stSANCAMERADESC
		{
		public:
			SPOINT3 CameraCoord;
			SPOINT3	CameraLookAtPoint;
			SVECTOR3 CameraOrien;
			sfloat	ViewFOV;
			sfloat	ViewNear;
			sfloat	ViewFar;
			uint32	ViewWidth;
			uint32	ViewHeight;
			bool	bCameraMovable;
			bool	bIsOrthoCamera;
		public:
			stSANCAMERADESC();
			stSANCAMERADESC(SPOINT3 CameraCoord,SPOINT3 CameraLookAtPoint,SVECTOR3 CameraOrien,sfloat ViewFOV,sfloat ViewNear=1.0,sfloat ViewFar=1000.0,uint32 ViewWidth=1024,uint32 ViewHeight=768,bool bIsOrthoCamera=false);
			~stSANCAMERADESC(){};
		};
		typedef stSANCAMERADESC	SANCAMERADESC;
		typedef stSANCAMERADESC* lpSANCAMERADESC;
#endif
	}
}