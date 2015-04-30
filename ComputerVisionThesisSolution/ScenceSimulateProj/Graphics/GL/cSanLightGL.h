#include"../../Core/cSanObject.h"
#include"../cSanLightDef.h"
#pragma once
using namespace std;
using namespace San::Mathematics;
namespace San
{
	namespace Graphics
	{
#ifndef __CSANLIGHTGL_H__
#define __CSANLIGHTGL_H__
#ifdef _OPENGL

		class cSanLightGL :public cSanObject
		{
		private:
			uint32 m_LightID;
			lpSPOINT3 m_pBindLightLookAtPoint;
			SPOINT3 m_LightLookAtOffset;
			SANLIGHTDESC m_LightDesc;
			SString m_strLightName;
			bool m_bLightChange;
			bool m_bLightEnable;
		protected:
			static list<cSanLightGL*> m_GlobalLightList;
			static uint32 _AddToLightList(cSanLightGL* pLight);
			static bool _InsertToLightList(cSanLightGL* pLight,uint32 DestLightID);
			static void _DeleteFromLightList(uint32 LightID);
		public:
			cSanLightGL(SString strLightName)
				:cSanObject(SOT_LIGHT),
				m_LightID(San::Graphics::SLT_MAXSOFTWARELIGHT),
				m_pBindLightLookAtPoint(nullptr),
				m_LightLookAtOffset(0.0,0.0,1.0),
				m_strLightName(strLightName),
				m_bLightChange(false),
				m_bLightEnable(false)
			{
			};
			cSanLightGL(SString strLightName,const SPOINT3 &LightCoord,const SPOINT3 &LightLookAtPoint,const SANLIGHTDESC &Desc)
				:cSanObject(SOT_LIGHT,LightCoord),
				m_LightID(San::Graphics::SLT_MAXSOFTWARELIGHT),
				m_pBindLightLookAtPoint(nullptr),
				m_LightLookAtOffset(0.0, 0.0, 1.0),
				m_LightDesc(Desc),
				m_strLightName(strLightName),
				m_bLightChange(false),
				m_bLightEnable(false)
			{
			};
			~cSanLightGL()
			{
				this->_DeleteFromLightList(this->m_LightID);
			};
			void iSetLightCoord(SPOINT3 LightCoord);
			void iSetLightCoord(sfloat CoordX,sfloat CoordY,sfloat CoordZ);
			void iSetLightLookAtPoint(SPOINT3 Coord);
			void iSetLightLookAtPoint(sfloat CoordX,sfloat CoordY,sfloat CoordZ);
			void iSetLightMovable(bool bLightMovable=true);
			void iSetLightType(eSANLIGHTTYPE Type);
			void iSetLightColor(SANCOLORF Diffuse,SANCOLORF Specualr,SANCOLORF Ambient,SANCOLORF Emission);
			void iSetLightRange(sfloat Range);
			void iSetLightFallOff(sfloat FallOff);
			void iSetLightRadius(sfloat Inner,sfloat Outer);
			void iSetLightAngle(sfloat Theta,sfloat Phi);
			void iSetLightAttenuation(sfloat Constant,sfloat Linear=0.0,sfloat Quadratic=0.0);
			void iSetLightDescription(SANLIGHTDESC Desc);
			void iSetLightEnable();
			void iSetLightDisable();
			void iLightMove(SPOINT3 Coord);
			void iLightMove(sfloat CoordX=0.0,sfloat CoordY=0.0,sfloat CoordZ=0.0);
			void iLightMoveByDir(SPOINT3 Coord);
			void iLightMoveByDir(sfloat CoordX = 0.0, sfloat CoordY = 0.0, sfloat CoordZ = 0.0);
			void iLightRotation(SPOINT3 RotatVector,SPOINT3 Axle);
			void iLightRotation(sfloat RotatX,sfloat RotatY,sfloat RotatZ,sfloat AxleX,sfloat AxleY,sfloat AxleZ);
			void iLightRotationByCoord(SPOINT3 RotatVector);
			void iLightRotationByCoord(sfloat RotatX, sfloat RotatY, sfloat RotatZ);
			void iLightRotationByLookAtPoint(SPOINT3 RotatVector);
			void iLightRotationByLookAtPoint(sfloat RotatX, sfloat RotatY, sfloat RotatZ);
			bool iGetLightMovableState();
			SString iGetLightName();
			uint32 iGetLightID();
			SANLIGHTDESC iGetLightDescription();
			bool iGetLightState();
			bool iBindLookAtPointToNode(lpSPOINT3 pNode);
			void iUnBindLookAtPointToNode();
			void iUpdateLightCoord();
			SRESULT iSendMessage(SVALUE wParam,SVALUE lParam);
			SRESULT iDoSomething(SVALUE wParam,SVALUE lParam);
		};
		typedef cSanLightGL	cSanLight;
#endif
#endif
	}
}