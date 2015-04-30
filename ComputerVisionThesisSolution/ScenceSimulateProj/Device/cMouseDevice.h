#include"SanDevices.h"
#include"cInputDeviceDX.h"
#pragma once
using namespace std;
namespace San
{
	namespace Device
	{
#ifdef _DXDEVICE
#ifndef __CMOUSEDEVICE_H__
#define __CMOUSEDEVICE_H__

		class cMouseDevice :public cInputDeviceDX
		{
		private:
			LPDIRECTINPUTDEVICE8 m_pMouseDevice;
			DIDEVICEOBJECTDATA m_MDBuffer[MD_SIZE];
			stSPOINTEX<long> m_MousePos;
			bool m_bMouseMove;
		public:
			cMouseDevice()
				:cInputDeviceDX(_SSTR("Sys_DirectInput"))
			{
				this->m_pMouseDevice=NULL;
				::memset((this->m_MDBuffer),0,sizeof(this->m_MDBuffer));
				::memset(&(this->m_MousePos),0,sizeof(this->m_MousePos));
				this->m_bMouseMove=false;
			};
			cMouseDevice(HINSTANCE hInstance)
				:cInputDeviceDX(hInstance,_SSTR("Sys_DirectInput"))
			{
				this->m_pMouseDevice=NULL;
				::memset((this->m_MDBuffer),0,sizeof(this->m_MDBuffer));
				::memset(&(this->m_MousePos),0,sizeof(this->m_MousePos));
				this->m_bMouseMove=false;
			};
			~cMouseDevice(){};
			bool iCreateDevice(UINT Type=DISCL_BACKGROUND|DISCL_NONEXCLUSIVE);
			void iReleaseDevice();
			bool iUpdate();
			void iClear();
			void iSetMousePos(stSPOINTEX<long> Pos);
			void iSetMousePos(long x=0,long y=0,long z=0);
			stSPOINTEX<long> iGetMousePos();
			DWORD iGetMouseState();
		};
		typedef cMouseDevice	cSanMouseDevice;
#endif
#endif
	}
}