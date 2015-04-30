#include"../cSanCameraDef.h"
#pragma once
using namespace std;
using namespace San::Mathematics;
namespace San
{
	namespace Graphics
	{
#ifndef __CSANCAMERAGL_H__
#define __CSANCAMERAGL_H__
#ifdef _OPENGL
		class cSanCameraGL :public cSanObject
		{
		private:
			SString	m_strCameraName;
			SPOINT3 m_CameraLookAtOffset;
			lpSPOINT3 m_pBindCameraLookAtPoint;
			SANCAMERADESC	m_CameraDesc;
		public:
			cSanCameraGL(SString strCameraName)
				:cSanObject(SOT_CAMERA),
				m_strCameraName(strCameraName),
				m_CameraLookAtOffset(0.0,0.0,0.0)
			{
			};
			cSanCameraGL(SString strCameraName,SPOINT3 CameraCoord,SVECTOR3 CameraOrien,sfloat ViewNear=1.0,sfloat ViewFar=1000.0,uint32 ViewWidth=1024,uint32 ViewHeight=768,bool bIsOrthoCamera=false)
				:cSanObject(SOT_CAMERA,CameraCoord),
				m_strCameraName(strCameraName),
				m_CameraLookAtOffset(0.0,0.0,0.0),
				m_CameraDesc(CameraCoord,SPOINT3(0.0,0.0,0.0),CameraOrien,ViewNear,ViewFar,ViewWidth,ViewHeight,bIsOrthoCamera)
			{
			};
			~cSanCameraGL(){};
			bool iBindToNode(const lpSPOINT3 pNode, const lpSPOINT3 pRotation = nullptr);
			void iUnBindToNode();
			bool iBindLookAtPointToNode(const lpSPOINT3 pLookAtNode);
			void iUnBindLookAtPointToNode();
			void iSetCameraCoord(const SPOINT3 &CameraCoord);
			void iSetCameraCoord(const sfloat CoordX,const sfloat CoordY,const sfloat CoordZ);
			void iSetViewRange(const sfloat ViewNear,const sfloat ViewFar);
			void iSetViewSize(const uint32 ViewWidth,const uint32 ViewHeight);
			void iSetViewFOV(const sfloat FOV);
			void iSetCameraLookAtPoint(const SPOINT3 &Coord);
			void iSetCameraLookAtPoint(const sfloat CoordX,const sfloat CoordY,const sfloat CoordZ);
			void iSetCameraMovable(const bool bCameraMovable=true);
			void iSetCameraOrienVector(const SPOINT3 &Orien);
			void iSetCameraDesc(const SANCAMERADESC &Desc);
			void iCameraMove(const SPOINT3 &Coord);
			void iCameraMove(const sfloat CoordX=0.0,const sfloat CoordY=0.0,const sfloat CoordZ=0.0);
			/*void iCameraMoveByDir(const SVECTOR3 &Dir, const sfloat Pow);
			void iCameraMoveByDir(const sfloat CoordX, const sfloat CoordY, const sfloat CoordZ, const sfloat Pow);*/
			void iCameraMoveByLookAtDir(const sfloat Pow);
			void iCameraRotation(const SPOINT3 &RotateVector,const SPOINT3 &Axle);
			void iCameraRotation(const sfloat RotateX, const sfloat RotateY, const sfloat RotateZ, const sfloat AxleX, const sfloat AxleY, const sfloat AxleZ);
			void iCameraRotationByCoord(const SPOINT3 &RotateVector);
			void iCameraRotationByCoord(const sfloat RotateX, const sfloat RotateY, const sfloat RotateZ);
			void iCameraRotationByLookAtPoint(const SPOINT3 &RotateVector);
			void iCameraRotationByLookAtPoint(const sfloat RotateX, const sfloat RotateY, const sfloat RotateZ);
			bool iGetCameraMovableState() const;
			SIZE iGetCameraViewSize() const;
			sfloat iGetCameraViewFOV() const;
			SPOINT3 iGetCameraOrienVector() const;
			SString iGetCameraName() const;
			SANCAMERADESC iGetCameraDesc();
			void iUpdateCameraCoord();
			SRESULT iSendMessage(SVALUE wParam,SVALUE lParam);
			SRESULT iDoSomething(SVALUE wParam,SVALUE lParam);
		};
		typedef cSanCameraGL cSanCamera;
#endif
#endif
	}
}