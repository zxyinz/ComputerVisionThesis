#include"iostream"
#include"process.h"
#include"../RenderSystem.h"
#include"../Core/SanString.h"
#include"../Core/SanTypes.h"
#include"cSanRenderWindow.h"
#include"../Core/cSanObjectManager.h"
#include"../Graphics/GL/cSanObjRenderGL.h"
#include"../Graphics/DX/cSanObjRenderDX.h"
#include"../Graphics/GL/cSanCameraGL.h"
#include"../Graphics/DX/cSanCameraDX.h"
#include"../Graphics/cSanLightManager.h"
#include"../Graphics/GL/cSanGLBufferManager.h"
#include"../Graphics/GL/cSanShaderManagerGL.h"
using namespace std;
using namespace San;
using namespace San::Device;
using namespace San::Graphics;
unsigned int _stdcall RenderThread(void *pPtr);
unsigned int _stdcall TimeUpdateThread(void *pPtr);
unsigned int _stdcall MessageUpdateThread(void *pPtr);
bool bOnInit = true;
unsigned int CurrentTime;
POINT MouseHistory;
bool bMouseRButtonDown = false;
cSanObjRenderGL ObjRender;
cSanCamera Camera(_SSTR("Camera"));
cSanLight Light(_SSTR("Light"));
cSanGLBufferManager* pBufferManager;
cSanShaderManager* pShaderManager;
unsigned int FBOID;
int FPS = 0;
char strFPS[100];
UINT VertexShader;
UINT PixelShader;
UINT GLProgram;
SANRENDERDESC Desc;
UINT CharacterID;
UINT MeshID_A;
UINT MeshID_B;
UINT MeshID_L;
UINT MeshID_S;
UINT MeshID_T;
bool bShowArray = false;
eSANOBJRENDERSTYLE DisplayStyle = San::Graphics::SORS_TRANGLE;
unsigned char* pSubWinBuffer = nullptr;
SRESULT MoveCamera(SHANDLE wParam, SHANDLE lParam);
SRESULT ButtonMessage(SHANDLE wParam, SHANDLE lParam);
LRESULT cSanRenderWindow::iBasicProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//POINT Point;
	switch (uMsg)
	{
	case WM_SYSCOMMAND:
		break;
	case WM_CLOSE:
		PostMessage(this->iGetWnd(), WM_QUIT, 0, 0);
		return 0;
		break;
	case WM_PAINT:
		break;
	case WM_KEYDOWN:
		this->m_pMsgManager->iGetKeyBoardDevicePtr()->iSetKeyState(wParam, true);
		switch (wParam)
		{
		case VK_UP:
			Camera.iCameraMoveByLookAtDir(-10);
			//Camera.iCameraMove(0,0,-20);
			break;
		case VK_DOWN:
			Camera.iCameraMoveByLookAtDir(10);
			//Camera.iCameraMove(0,0,20);
			break;
		case VK_LEFT:
			Camera.iCameraMove(-20, 0, 0);
			break;
		case VK_RIGHT:
			Camera.iCameraMove(20, 0, 0);
			break;
		case 83:
			Camera.iCameraRotationByLookAtPoint(0.0, -0.01, 0.0);
			break;
		case 87:
			Camera.iCameraRotationByLookAtPoint(0.0, 0.01, 0.0);
			break;
		case 49:
			switch (DisplayStyle)
			{
			case San::Graphics::SORS_POINT:
				DisplayStyle = San::Graphics::SORS_LINE;
				break;
			case San::Graphics::SORS_LINE:
				DisplayStyle = San::Graphics::SORS_TRANGLE;
				break;
			case San::Graphics::SORS_TRANGLE:
				DisplayStyle = San::Graphics::SORS_POINT;
				break;
			default:
				break;
			}
			break;
		case 50:
			bShowArray = !bShowArray;
			break;
		default:
			break;
		}
		break;
	case WM_KEYUP:
		this->m_pMsgManager->iGetKeyBoardDevicePtr()->iSetKeyState(wParam, false);
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		this->m_pMsgManager->iGetMouseDevicePtr()->iSetMousePos((0xffff0000 & lParam) / 65536, (0x0000ffff)&lParam, 0);
		break;
	case WM_MOUSEWHEEL:
		break;
	case WM_ERASEBKGND:
		break;
	default:
		break;
	}
	return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}
void cSanRenderWindow::iOnInitSystem()
{
	this->m_pMsgManager->iCreateMessageManager();
	this->m_pResManager->iCreateResourceManager();
	::cout << "Window Starting..." << "\n";
	::gloRegisterVector(_SSTR("Sys_hWnd"), VT_SYS | VT_VAR, (SHANDLE) (this->iGetWnd()));
	::cout << "Varialbe Registe... Name: hWnd			Value: " << this->iGetWnd() << "\n";
	::gloRegisterVector(_SSTR("Sys_hInstance"), VT_SYS | VT_VAR, (SHANDLE) (this->iGetInstance()));
	::cout << "Varialbe Registe... Name: hInstance		Value: " << this->iGetInstance() << "\n";
	UINT *pTimeUpdateThread = new UINT(1);
	::_beginthreadex(nullptr, 0, TimeUpdateThread, this, 0, pTimeUpdateThread);
	::cout << "System time thread start" << "\n";
	UINT *pMessageUpdateThread = new UINT(2);
	::_beginthreadex(nullptr, 0, MessageUpdateThread, this, 0, pMessageUpdateThread);
	::cout << "Message Update thread start" << "\n";
	//------------------------------------------------------------//
	pBufferManager = new cSanGLBufferManager();
	pBufferManager->iCreateGLBufferManager();
	pShaderManager = new cSanShaderManager();
	pShaderManager->iCreateShaderManager();
	uint32 VID = pShaderManager->iCreateShaderObjByFile(L"Shader\\san_default_glsl.vert", San::Graphics::eSANSHADERTYPE::SST_VERTEXSHADER);
	uint32 FID = pShaderManager->iCreateShaderObjByFile(L"Shader\\san_default_glsl.frag", San::Graphics::eSANSHADERTYPE::SST_PIXELSHADER);
	lpSANSHADERVAR pVar01 = new SANSHADERVAR("color_array", SSDT_UNIFORM);
	pVar01->iAddSubVariable("color_array", SSDT_FLOAT, 3);
	float Array[3] = { 0.0, 0.0, 1.0 };
	pVar01->iSetSubVariable(0, Array, 3);
	pShaderManager->iAttachVariableToShader(FID, *pVar01);
	pShaderManager->iCreateProgram(L"BShader");
	pShaderManager->iAttachShaderToProgram(L"BShader", VID);
	pShaderManager->iAttachShaderToProgram(L"BShader", FID);
	pShaderManager->iEnableProgram(L"BShader");
	Desc.pDeviceResManager = pBufferManager;
	Desc.pShaderManager = pShaderManager;
	this->m_pResManager->iRegistGraphicsDeviceResourceManagerHandle(pBufferManager);
	MeshID_A = this->m_pResManager->iCreateMeshFromFile(L"E:\\CSE5999\\Scene\\SeaFoodTexture.obj");
	//------------------------------------------------------------//
	//-------------------Load Default File Path-------------------//
	LPWSTR ptr_path = new WCHAR[1024];
	GetPrivateProfileString(_SSTR("ResourceFilePathInfo"), _SSTR("ModelFilePath"), _SSTR(""), ptr_path, 1024, _SSTR(".\\config.ini"));
	this->m_pResManager->iRegistResourceDefaultPath(ptr_path);
	GetPrivateProfileString(_SSTR("ResourceFilePathInfo"), _SSTR("TextureFilePath"), _SSTR(""), ptr_path, 1024, _SSTR(".\\config.ini"));
	this->m_pResManager->iRegistResourceDefaultPath(ptr_path);
}
void cSanRenderWindow::iBasicDraw()
{
	if (bOnInit)
	{
		::glewInit();
		lpSPOINT3 pCameraPos = new SPOINT3(0.0, 0.0, 0.0);
		this->iOnInitSystem();
		bOnInit = false;
		SANCAMERADESC Desc;
		Desc.CameraCoord = SPOINT3(-331.042, -504.773, 43.734);
		Desc.CameraLookAtPoint = SPOINT3(-10.181, -56.6, -42.135);
		Desc.CameraOrien = SVECTOR3(0.0, 0.0, 1.0);
		Desc.ViewNear = 1.0;
		Desc.ViewFar = 10000.0;
		Desc.ViewWidth = this->m_WindowSize.WindowWidth;
		Desc.ViewHeight = this->m_WindowSize.WindowHeight;
		Desc.ViewFOV = 45.0;
		Desc.bIsOrthoCamera = false;
		Camera.iSetCameraDesc(Desc);
		Light.iSetLightColor(SANCOLOR(192, 192, 192, 255), SANCOLOR(192, 192, 192, 255), SANCOLOR(128, 128, 128, 255), SANCOLOR(128, 128, 128, 255));
		Light.iSetLightCoord(1000, 1000, 1000);
		Light.iSetLightColor(SANCOLOR(255, 255, 255, 255), SANCOLOR(192, 192, 192, 255), SANCOLOR(128, 128, 128, 255), SANCOLOR(128, 128, 128, 255));
		Light.iSetLightType(SLT_GLOBAL);
		Light.iSetLightType(SLT_DIRECTIONAL);
		Light.iSetLightLookAtPoint(0, 0, 0);
		Light.iSetLightEnable();
		::glEnable(GL_MULTISAMPLE);
	}
	::glEnable(GL_NORMALIZE);
	::glEnable(GL_LIGHTING);

	Desc.window_width = 800;
	Desc.window_hight = 600;

	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Camera.iDoSomething(NULL, NULL);
	Light.iDoSomething(NULL, NULL);

	cSanObjRender Render;
	lpSANRESPTRC pPtr = this->m_pResManager->iGetResourceConstPtr(MeshID_A);
	lpSANMESH pMesh = pPtr->iGetMeshPtr();
	pShaderManager->iUpdateProgram();
	Render.iDrawObject(pMesh, &SPOINT3(0.0f, 0.0f, 0.0f), &Desc);

	this->_SwapBuffer();
	FPS = FPS + 1;
}
unsigned int _stdcall TimeUpdateThread(void *pPtr)
{
	cSanObjectManager ObjManager;
	ObjManager.iSetObjectTime(0);
	CurrentTime = 0;
	UINT FPSCount = 0;
	while (true)
	{
		ObjManager.iObjectTimeAdd();
		::Sleep(100);
		if (FPSCount<10)
		{
			FPSCount = FPSCount + 1;
		}
		else
		{
			CurrentTime = CurrentTime + 1;
			::_itoa(FPS, strFPS, 10);
			FPS = 0;
			FPSCount = 0;
		}
	}
	return 0;
}
unsigned int _stdcall MessageUpdateThread(void* pPtr)
{
	cMessageManager *pMsgManager = (cMessageManager*) (::gloFoundVector(_SSTR("Sys_MessageManager")));
	if (pMsgManager == nullptr)
	{
		return 0;
	}
	pMsgManager->iSetUpdateTime(10);
	while (true)
	{
		pMsgManager->iUpdate();
	}
	return 0;
}
SRESULT MoveCamera(SHANDLE wParam, SHANDLE lParam)
{
	return 1;
}
SRESULT ButtonMessage(SHANDLE wParam, SHANDLE lParam)
{
	return 1;
}