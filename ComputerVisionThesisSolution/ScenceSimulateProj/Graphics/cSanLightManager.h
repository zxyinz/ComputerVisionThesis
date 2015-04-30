#include"../Core/SanMathematics.h"
#include"SanGraphics.h"
#include"../Core/cVectorManager.h"
#include"GL/cSanLightGL.h"
#pragma once;
using namespace std;
using namespace San::Graphics;
namespace San
{
	namespace Graphics
	{
#ifndef __CSANLIGHTMANAGER_H__
#define __CSANLIGHTMANAGER_H__

		class cSanLightManager :protected cSanLight
		{
		private:
			list<cSanLight> m_GlobalLightList;
			list<cSanLight> m_PointLightList;
			list<cSanLight> m_DirectionalLightList;
			list<cSanLight> m_SpotLightList;
			list<cSanLight> m_UnknownLightList;
			SString m_strLightManagerName;
			uint32 m_LightManagerID;
			static bool m_bGlobalLightState;
			static uint32 m_MaxLightManagerID;
			static uint32 m_ActiveLightManagerID;
		protected:
			eSANLIGHTTYPE _GetLightPtr(SString strLightName,list<cSanLight>::iterator* pLightElement);
		public:
			cSanLightManager(SString strManagerName)
				:cSanLight(strManagerName),
				m_strLightManagerName(strManagerName)
			{
				this->m_LightManagerID=this->m_MaxLightManagerID;
				this->m_MaxLightManagerID=this->m_MaxLightManagerID+1;
			};
			~cSanLightManager(){};
			bool iCreateLightManager();
			void iReleaseLightManager();
			cSanLight* iRegisteLight(SString strLightName);
			void iUnRegisteLight(SString strLightName);
			cSanLight* iGetLightPtr(SString strLightName);
			void iEnableGraphicsLight();
			void iDisableGraphicsLight();
			void iEnableLightManager();////////////////////////PUSH POP?
			void iDisableLightManager();
			bool iGetGraphicsLightState();
			bool iGetLightManagerState();
			uint32 iGetCurrentActiveLightManagerID();
			void iUpdateLightManager();
			void iUpdateLight(SString strLightName);
			SRESULT iSendMessage(SVALUE wParam,SVALUE lParam);
			SRESULT iDoSomething(SVALUE wParam,SVALUE lParam);
		};
#endif
	}
}