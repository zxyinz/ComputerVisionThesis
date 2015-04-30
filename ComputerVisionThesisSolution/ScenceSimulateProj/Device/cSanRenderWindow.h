#include"../Core/cVectorManager.h"
#include"cWindowBasicGL.h"
#include"cWindowBasicDX.h"
#include"cSanWindowObjectGL.h"
#include"../Core/cMessageManager.h"
#include"../Core/cSanResourceManager.h"
using namespace std;
namespace San
{
	namespace Device
	{
#ifndef __CSANRENDERWINDOW_H__
#define __CSANRENDERWINDOW_H__
		class cSanRenderWindow :public cSanWindowObject//cWindowCreaterGL
		{
		private:
			cVectorManager *m_pManager;
			cMessageManager *m_pMsgManager;
			cSanResourceManager *m_pResManager;
		public:
			cSanRenderWindow(const SString &strWindowName, const SANWINDEVDESC &Desc = SANWINDEVDESC(), const bool bSubWindow = false)
				:cSanWindowObject(strWindowName, Desc, bSubWindow)
			{
				this->m_pManager = new cVectorManager();
				this->m_pMsgManager = new cMessageManager();
				this->m_pResManager = new cSanResourceManager();
			};
			cSanRenderWindow(SString strINIFile)
				:cSanWindowObject(strINIFile.c_str())
			{
				this->m_pManager = new cVectorManager();
				this->m_pMsgManager = new cMessageManager();
				this->m_pResManager = new cSanResourceManager();
			};
			~cSanRenderWindow(){};
			LRESULT iBasicProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			void iOnInitSystem();
			void iBasicDraw();
		};
#endif
	}
}