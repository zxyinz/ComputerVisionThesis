#include"../../RenderSystem.h"
#include"../../Core/SanString.h"
#include"../SanGraphics.h"
using namespace std;
namespace San
{
	namespace Graphics
	{
#ifndef __CSANFONTGL_H__
#define __CSANFONTGL_H__
#ifdef _OPENGL 
		class cSanFontGL
		{
		private:
			SHANDLE m_pFont;
			RECT m_TextRect;
		public:
			cSanFontGL(SHANDLE pFont=NULL)
			{
			};
			~cSanFontGL(){};
			bool iCreateFont(int Height,int Weight,int Width,SString strFontName){ return false; };
			bool iCreateFont(SHANDLE pFont){ return false; };
			void iReleaseFont() {};
			void iSetDrawRect(RECT DrawRect) {};
			uint32 iGetFont() { return 0; };
			RECT iGetDrawRect() {return this->m_TextRect; };
			void iDrawString(SString strString,uint32 Format,uint32 FontColor,RECT *pRect=NULL,SHANDLE pFont=NULL) {};
		};
		typedef cSanFontGL cSanFont;
#endif
#endif
	}
}