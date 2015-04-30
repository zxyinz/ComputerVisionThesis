#include"../Core/cVectorManager.h"
#include"SanDevices.h"
#pragma once
using namespace std;
namespace San
{
	namespace Device
	{
#ifdef _DXDEVICE
#ifndef __CINPUTDEVICEDX_H__
#define __CINPUTDEVICEDX_H__

#include"dinput.h"
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

		class cInputDeviceDX
		{
		private:
			static LPDIRECTINPUT8 m_pInputDevice;
			static SString m_Name;
		protected:
			bool _CreateInputDevice(SString Name=_SSTR("Sys_InputDevice"));
			bool _CreateInputDevice(HINSTANCE hInstance,SString Name=_SSTR("Sys_InputDevice"));
			LPDIRECTINPUT8 _GetInputDevice();
		public:
			static bool m_bOnInit;
			cInputDeviceDX(SString Name=_SSTR("Sys_InputDevice"))
			{
				if(this->m_bOnInit)
				{
					this->m_pInputDevice=NULL;
					if(this->_CreateInputDevice(Name))
					{
						this->m_Name=Name;
					}

					else
					{
						SString strError=_SSTR("error: can not create direct input device");
						throw strError;
						this->m_pInputDevice=NULL;
					}
					this->m_bOnInit=false;
				}
			};
			cInputDeviceDX(HINSTANCE hInstance,SString Name=_SSTR("Sys_InputDevice"))
			{
				if(this->m_bOnInit)
				{
					this->m_pInputDevice=NULL;
					if(this->_CreateInputDevice(hInstance,Name))
					{
						this->m_Name=Name;
					}
					else
					{
						SString strError=_SSTR("error: can not create direct input device");
						throw strError;
						this->m_pInputDevice=NULL;
					}
					this->m_bOnInit=false;
				}
			};
			~cInputDeviceDX(){};
		};
#endif
#endif
	}
}