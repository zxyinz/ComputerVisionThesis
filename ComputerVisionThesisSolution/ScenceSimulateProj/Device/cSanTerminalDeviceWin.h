//Project: San Lab Game Engine
//Version: 2.1.1
//Debug State: Add functions and need test [buffer operation]
#include"..\Core\SanTypes.h"
#include"..\Core\SanMathematics.h"
//#include"..\Core\cSanResourceManagerDef.h"
using namespace std;
using namespace San::Mathematics;
#pragma once
namespace San
{
	namespace Device
	{
#ifdef _WINDOWS
#ifndef __CSANTERMINALDEVICEWIN_H__
#define __CSANTERMINALDEVICEWIN_H__
		enum eSANTERMINALCOLOR
		{
			STC_BLACK		= 0x0000,
			STC_BLUE		= 0x0001,
			STC_GREEN		= 0x0002,
			STC_CYAN		= 0x0003,
			STC_RED			= 0x0004,
			STC_MAGENTA		= 0x0005,
			STC_YELLOW		= 0x0006,
			STC_GREY		= 0x0007,
			STC_BLUE_HL		= 0x0009,
			STC_GREEN_HL	= 0x000A,
			STC_CYAN_HL		= 0x000B,
			STC_RED_HL		= 0x000C,
			STC_MAGENTA_HL	= 0x000D,
			STC_YELLOW_HL	= 0x000E,
			STC_WHITE		= 0x000F,
			STC_DEFAULT		= 0x00FF,
		};

		class cSanTerminalDeviceWin
		{
		private:
			SString m_strTerminalName;
			uint32 m_TerminalID;
			SHANDLE m_TerminalHandle;
			SString m_strTittle;
			SStringA m_strCodeLocate;
			eSANTERMINALCOLOR m_TextDefColor;
			eSANTERMINALCOLOR m_BackgroundDefColor;
			schar* m_pTerminalBuffer;//////////////
			uint32 m_BufferSize;///////////////////
			SString m_strBuffer;
			static bool m_bTerminalLock;
		protected:
			void _CreateConsoleDevice();
			void _OutputStringA(SStringA strString, eSANTERMINALCOLOR TextColor, eSANTERMINALCOLOR BackgroundColor);
			void _OutputStringW(SStringW strString, eSANTERMINALCOLOR TextColor, eSANTERMINALCOLOR BackgroundColor);
			void _LockTerminal();
			void _UnLockTerminal();
		public:
			cSanTerminalDeviceWin(SString strTerminalName=_SSTR("SanLabTerminal"),SString strTerminalTittle=_SSTR("San Lab Terminal"));
			~cSanTerminalDeviceWin();
			void iSetTerminalTittle(SString strTittle);
			void iSetTerminalCodeLocate(SStringA strLocate);
			void iSetDefaultTextColor(eSANTERMINALCOLOR Color);
			void iSetDefaultBackgroundColor(eSANTERMINALCOLOR Color);
			bool iSetTerminalBufferSize(uint32 BufferSize);
			SString iGetTerminalTittle();
			SStringA iGetTerminalCodeLocate();
			eSANTERMINALCOLOR iGetDefaultTextColor();
			eSANTERMINALCOLOR iGetDefaultBackgroundColor();
			uint32 iGetTerminalBufferSize();
			SPOINT3 iGetCurrentCursorPosition();
			SString iGetInputString(schar DelLim=_SSTR('\n'));
			void iOutputStringA(SStringA strString,eSANTERMINALCOLOR TextColor=STC_DEFAULT,eSANTERMINALCOLOR BackgroundColor=STC_DEFAULT);
			void iOutputStringW(SStringW strString,eSANTERMINALCOLOR TextColor=STC_DEFAULT,eSANTERMINALCOLOR BackgroundColor=STC_DEFAULT);
			void iOutputString(SString strString,eSANTERMINALCOLOR TextColor=STC_DEFAULT,eSANTERMINALCOLOR BackgroundColor=STC_DEFAULT);
			void iOutputStringLineA(SStringA strString, eSANTERMINALCOLOR TextColor = STC_DEFAULT, eSANTERMINALCOLOR BackgroundColor = STC_DEFAULT);
			void iOutputStringLineW(SStringW strString, eSANTERMINALCOLOR TextColor = STC_DEFAULT, eSANTERMINALCOLOR BackgroundColor = STC_DEFAULT);
			void iOutputStringLine(SString strString, eSANTERMINALCOLOR TextColor = STC_DEFAULT, eSANTERMINALCOLOR BackgroundColor = STC_DEFAULT);
			void iOutputStringA(SStringA strString, const SPOINT3 &Pos, eSANTERMINALCOLOR TextColor = STC_DEFAULT, eSANTERMINALCOLOR BackgroundColor = STC_DEFAULT);
			void iOutputStringW(SStringW strString, const SPOINT3 &Pos, eSANTERMINALCOLOR TextColor = STC_DEFAULT, eSANTERMINALCOLOR BackgroundColor = STC_DEFAULT);
			void iOutputString(SString strString, const SPOINT3 &Pos, eSANTERMINALCOLOR TextColor = STC_DEFAULT, eSANTERMINALCOLOR BackgroundColor = STC_DEFAULT);
			void iClearLine();/////////////////////
			void iClearScreen();
		};
		typedef cSanTerminalDeviceWin cSanTerminalDevice;
#endif
#endif
	}
}