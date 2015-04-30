#include"../RenderSystem.h"
#include"SanDevices.h"
#pragma once
using namespace std;
namespace San
{
	namespace Device
	{
#ifndef _DXDEVICE
#ifndef __CSANVISUALMOUSEDEVICE_H__
#define __CSANVISUALMOUSEDEVICE_H__

		enum eMouse_Device
		{
			DIMOFS_BUTTON0,
			DIMOFS_BUTTON1,
			DIMOFS_BUTTON2,
			DIMOFS_BUTTON3,
			DIMOFS_BUTTON4,
			DIMOFS_BUTTON5,
			DIMOFS_BUTTON6,
			DIMOFS_BUTTON7,
			DIMOFS_X,
			DIMOFS_Y,
			DIMOFS_Z
		};
		typedef eMouse_Device	MouseDevice;
		typedef eMouse_Device*	LPMouse_Device;

		struct stDIDEVICEOBJECTDATA
		{
			DWORD dwOfs;
			DWORD dwData;
			DWORD dwTimeStamp;
			DWORD dwSequence;
			UINT_PTR uAppData;
		};
		typedef stDIDEVICEOBJECTDATA	DIDEVICEOBJECTDATA;
		typedef stDIDEVICEOBJECTDATA*	LPDIDEVICEOBJECTDATA;

		class cSanVisualMouse
		{
		private:
			DIDEVICEOBJECTDATA m_MDBuffer[MD_SIZE];
			stSPOINT3EX<long> m_MousePos;
			bool m_bMouseMove;
			UINT m_SetCount;
		public:
			cSanVisualMouse()
				:m_bMouseMove(false),
				m_SetCount(0)
			{
				::memset(this->m_MDBuffer,0,sizeof(DWORD)*MD_SIZE);
				::memset(&(this->m_MousePos),0,sizeof(this->m_MousePos));
			};
			cSanVisualMouse(HINSTANCE hInstance)
				:m_bMouseMove(false),
				m_SetCount(0)
			{
				::memset(this->m_MDBuffer,0,sizeof(DWORD)*MD_SIZE);
				::memset(&(this->m_MousePos),0,sizeof(this->m_MousePos));
			};
			~cSanVisualMouse(){};
			bool iCreateDevice(UINT Type=NULL){ return true; };
			void iReleaseDevice(){};
			bool iUpdate();
			void iClear();
			void iSetMouseButtonState(UINT Button,bool bState);
			void iSetMousePosState(stSPOINT3EX<long> Pos);
			void iSetMousePos(stSPOINT3EX<long> Pos);
			void iSetMousePos(long x=0,long y=0,long z=0);
			stSPOINT3EX<long> iGetMousePos();
			DWORD iGetMouseState();
		};
		typedef cSanVisualMouse		cSanMouseDevice;
#endif
#endif
	}
}