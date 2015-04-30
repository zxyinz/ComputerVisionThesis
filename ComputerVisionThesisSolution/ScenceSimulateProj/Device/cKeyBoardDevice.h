#include"iostream"
#include"bitset"
#include"../RenderSystem.h"
#include"SanDevices.h"
#include"cInputDeviceDX.h"
#pragma once
using namespace std;
namespace San
{
	namespace Device
	{
#ifdef _DXDEVICE
#ifndef __CKEYBOARDDEVICE_H__
#define __CKEYBOARDDEVICE_H__

		class cKeyBoardDevice :public cInputDeviceDX
		{
		private:
			LPDIRECTINPUTDEVICE8 m_pKeyBoardDevice;
			bool m_KBBuffer[KBD_SIZE];
			bool m_KBHistory[KBD_SIZE];
			UINT m_KBState[KBD_SIZE];
			string m_strString;
			UINT m_TTL;
			bool m_bSetCapsLock;
			bool m_bShiftDown;
		protected:
			void _AddToString();
		public:
			cKeyBoardDevice()
				:cInputDeviceDX(_SSTR("Sys_DirectInput"))
			{
				this->m_pKeyBoardDevice=NULL;
				::memset((this->m_KBBuffer),0,sizeof(this->m_KBBuffer));
				::memset((this->m_KBHistory),0,sizeof(this->m_KBHistory));
				::memset((this->m_KBState),0,sizeof(this->m_KBState));
				this->m_strString.clear();
				this->m_TTL=30;
				this->m_bSetCapsLock=false;
				this->m_bShiftDown=false;
			};
			cKeyBoardDevice(HINSTANCE hInstance)
				:cInputDeviceDX(hInstance,_SSTR("Sys_DirectInout"))
			{
				this->m_pKeyBoardDevice=NULL;
				::memset((this->m_KBBuffer),0,sizeof(this->m_KBBuffer));
				::memset((this->m_KBState),0,sizeof(this->m_KBState));
				this->m_strString.clear();
				this->m_TTL=30;
				this->m_bSetCapsLock=false;
				this->m_bShiftDown=false;
			};
			~cKeyBoardDevice(){};
			bool iCreateDevice(UINT Type=DISCL_FOREGROUND|DISCL_NONEXCLUSIVE);
			void iReleaseDevice();
			bool iUpdate();
			void iClear();
			void iClearString();
			void iSetTTL(UINT TTL);
			bool iCheck(UINT Key,UINT Type=KBT_BUTTONDOWN);
			SString iGetString();
		};
		typedef cKeyBoardDevice		cSanKeyBoardDevice;
#endif
#endif
	}
}