#include"iostream"
#include"../Core/SanTypes.h"
#include"SanDevices.h"
#pragma once
using namespace std;
namespace San
{
	namespace Device
	{
#ifndef _DIRECTX
#ifndef __CSANVISUALKEYBOARD_H__
#define __CSANVISUALKEYBOARD_H__
		class cSanVisualKeyBoard
		{
		private:
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
			cSanVisualKeyBoard()
			{
				::memset((this->m_KBBuffer),0,sizeof(this->m_KBBuffer));
				::memset((this->m_KBHistory),0,sizeof(this->m_KBHistory));
				::memset((this->m_KBState),0,sizeof(this->m_KBState));
				this->m_strString.clear();
				this->m_TTL=30;
				this->m_bSetCapsLock=false;
				this->m_bShiftDown=false;
			};
			cSanVisualKeyBoard(HINSTANCE hInstance)
			{
				::memset((this->m_KBBuffer),0,sizeof(this->m_KBBuffer));
				::memset((this->m_KBState),0,sizeof(this->m_KBState));
				this->m_strString.clear();
				this->m_TTL=30;
				this->m_bSetCapsLock=false;
				this->m_bShiftDown=false;
			};
			~cSanVisualKeyBoard(){};
			bool iCreateDevice(UINT Type=NULL){ return true; };
			void iReleaseDevice() {};
			bool iUpdate();
			void iClear();
			void iClearString();
			void iSetKeyState(UINT Key,bool bState);
			void iSetTTL(UINT TTL);
			bool iCheck(UINT Key,UINT Type=KBT_BUTTONDOWN);
			SString iGetString();
		};
		typedef cSanVisualKeyBoard		cSanKeyBoardDevice;
#endif
#endif
	}
}