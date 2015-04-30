#include"cSanResourceManagerDef.h"
#include"../FileIO/cBMPReader.h"
#include"../FileIO/cTGAReader.h"
#include"../FileIO/cOBJFileLoader.h"
//#include"../FileIO/cFBXFileLoader.h"
#ifdef _DIRECTX
#include"../FileIO/cXFileLoader.h"
#endif
#ifdef _OPENGL
#include"../Graphics/GL/cSanGLBufferManager.h"
#endif
#pragma once
using namespace std;
namespace San
{
#ifndef __CSANRESOURCEMANAGER_H__
#define __CSANRESOURCEMANAGER_H__

	class cSanResourceManager
	{
	private:
		SANFILEMATCHTABLE	m_FileMatchTable;
		//SANTEXTURERESLIST	m_TextureList;
		//SANMESHRESLIST		m_MeshList;
		//SANSOUNDRESLIST		m_SoundList;
		SANRESOURCETABLE*	m_pResourcesTableArray;
		list<SVALUE>*		m_pAvailableIDListArray;
		SVALUE*				m_pMaxAvailableIDArray;
		map<SString,SVALUE>	m_FilePathSearchTableForward;
		map<SVALUE,SString>	m_FilepathSearchTableBackward;
		bool				m_bManagerLock; // Be Careful
		list<SString>		m_DefaultPathList;///////////
		list<lpSANRESPTR>	m_AssignedVoidPtrList;
		list<lpSANRESPTRC>	m_AssignedVoidConstPtrList;
		SHANDLE				m_GraphicsDeviceResourceManagerHandle;
		SString				m_strName;
	protected:
		static SRESULT _LoadBMPFile(SString strFilePath, SVALUE Param, SHANDLE* pHandle);
		static SRESULT _LoadTGAFile(SString strFilePath, SVALUE Param, SHANDLE* pHandle);
		static SRESULT _LoadOBJFile(SString strFilePath, SVALUE Param, SHANDLE* pHandle);
		static SRESULT _LoadXMDFile(SString strFilePath, SVALUE Param, SHANDLE* pHandle);
		static SRESULT _LoadFBXFile(SString strFilePath, SVALUE Param, SHANDLE* pHandle);
		static SRESULT _CopyTexture(SHANDLE TextureHandle, SHANDLE* pDestHandle);
		static SRESULT _CopyMesh(SHANDLE MeshHandle, SHANDLE* pDestHandle);
		static SRESULT _CopySound(SHANDLE SoundHandle, SHANDLE* pDestHandle);
		static SRESULT _ReleaseTexture(SHANDLE TextureHandle, SHANDLE ResourceManagerPtr, SVALUE Param);
		static SRESULT _ReleaseMesh(SHANDLE MeshHandle, SHANDLE ResourceManagerPtr, SVALUE Param);
		static SRESULT _ReleaseSound(SHANDLE SoundHandle, SHANDLE ResourceManagerPtr, SVALUE Param);
		bool _ReleaseList(uint32 ListType);
		SVALUE _RegisteID(lpSANRESOURCE pRes, const eSANRESOURCETYPE Type);
		bool _ReleaseID(SVALUE ResID, const eSANRESOURCETYPE Type, const uint32 CreaterID, const uint32 GroupID, SANRES_RELEASEFUNC pReleaseFunc);
		eSANRESOURCETYPE _GetResourceTypeByArrayIndex(uint32 Index);
		uint32 _GetResourceArrayIndexByType(eSANRESOURCETYPE Type);
		lpSANRESOURCE _GetResourceHandle(SVALUE ResID, const eSANRESOURCETYPE Type);
	public:
		cSanResourceManager()
			:m_strName(_SSTR("Sys_SanResourceManager"))
		{
		};
		~cSanResourceManager()
		{
			this->iReleaseResourceManager();
		};
		bool iCreateResourceManager();
		void iReleaseResourceManager();
		SVALUE iCreateResourceFromFile(const SString &strFilePath, const eSANRESOURCETYPE Type, const uint32 CreaterID = 0, const uint32 GroupID = 0, const uint32 Flag = SRIO_DEFAULT_FLAG, SANRES_CREATEFUNC pCreateFunc = nullptr, SANRES_COPYFUNC pCopyFunc = nullptr, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
		SVALUE iCreateTextureFromFile(const SString &strFilePath, const uint32 CreaterID = 0, const uint32 GroupID = 0, const uint32 Flag = SRIO_DEFAULT_FLAG, SANRES_CREATEFUNC pCreateFunc = nullptr, SANRES_COPYFUNC pCopyFunc = nullptr, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
		SVALUE iCreateMeshFromFile(const SString &strFilePath, const uint32 CreaterID = 0, const uint32 GroupID = 0, const uint32 Flag = SRIO_DEFAULT_FLAG, SANRES_CREATEFUNC pCreateFunc = nullptr, SANRES_COPYFUNC pCopyFunc = nullptr, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
		SVALUE iCreateSoundFromFile(const SString &strFilePath, const uint32 CreaterID = 0, const uint32 GroupID = 0, const uint32 Flag = SRIO_DEFAULT_FLAG, SANRES_CREATEFUNC pCreateFunc = nullptr, SANRES_COPYFUNC pCopyFunc = nullptr, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
		SVALUE iCreateResourceFromMemory(SHANDLE BufferPtr, SVALUE BufferSize, const eSANRESOURCETYPE Type, const uint32 CreaterID = 0, const uint32 GroupID = 0, const uint32 Flag = SRIO_DEFAULT_FLAG, SANRES_COPYFUNC pCopyFunc = nullptr, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
		SVALUE iCreateTextureFromMemory(lpSANTEXTURE pTexture, const uint32 CreaterID = 0, const uint32 GroupID = 0, const uint32 Flag = SRIO_DEFAULT_FLAG, SANRES_COPYFUNC pCopyFunc = nullptr, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
		SVALUE iCreateMeshFromMemory(lpSANMESH pMesh, const uint32 CreaterID = 0, const uint32 GroupID = 0, const uint32 Flag = SRIO_DEFAULT_FLAG, SANRES_COPYFUNC pCopyFunc = nullptr, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
		SVALUE iCreateSoundFromMemory(SHANDLE pSound, const uint32 CreaterID = 0, const uint32 GroupID = 0, const uint32 Flag = SRIO_DEFAULT_FLAG, SANRES_COPYFUNC pCopyFunc = nullptr, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
		SVALUE iCopyResource(uint32 ResID, const uint32 CreaterID = 0, const uint32 RequireID = 0, const uint32 GroupID = 0, const uint32 Flag = SRIO_DEFAULT_FLAG, SANRES_COPYFUNC pCopyFunc = nullptr, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
		SVALUE iCopyTexture(uint32 TexID, const uint32 CreaterID = 0, const uint32 RequireID = 0, const uint32 GroupID = 0, const uint32 Flag = SRIO_DEFAULT_FLAG, SANRES_COPYFUNC pCopyFunc = nullptr, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
		SVALUE iCopyMesh(uint32 MeshID, const uint32 CreaterID = 0, const uint32 RequireID = 0, const uint32 GroupID = 0, const uint32 Flag = SRIO_DEFAULT_FLAG, SANRES_COPYFUNC pCopyFunc = nullptr, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
		SVALUE iCopySound(uint32 SoundID, const uint32 CreaterID = 0, const uint32 RequireID = 0, const uint32 GroupID = 0, const uint32 Flag = SRIO_DEFAULT_FLAG, SANRES_COPYFUNC pCopyFunc = nullptr, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
		bool iReleaseResource(SVALUE ResID, const uint32 ReleaseID = 0, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
		bool iReleaseTextureResource(SVALUE TexID, const uint32 ReleaseID = 0, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
		bool iReleaseMeshResource(SVALUE MeshID, const uint32 ReleaseID = 0, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
		bool iReleaseSoundResource(SVALUE SoundID, const uint32 ReleaseID = 0, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
		bool iUpdateResourceAccessFlag(uint32 ResID, const eSANRESOURCETYPE Type, const uint32 Flag, const uint32 RequireID);
		bool iUpdateResourceCopyFunc(uint32 ResID, const eSANRESOURCETYPE Type, SANRES_COPYFUNC pCopyFunc, const uint32 RequireID);
		bool iUpdateResourceReleaseFunc(uint32 ResID, const eSANRESOURCETYPE Type, SANRES_RELEASEFUNC pReleaseFunc, const uint32 RequireID);
		SVALUE iRegisteResource(const SHANDLE ResHandle, const uint32 ResSize, const eSANRESOURCETYPE ResType, const uint32 CreaterID, const uint32 GroupID = 0, const uint32 Flag = SRIO_DEFAULT_FLAG, SANRES_COPYFUNC pCopyFunc = nullptr, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
		bool iRegistResourceCreaterFunc(const SString &strFileName, SANRES_CREATEFUNC pCreateFunc, eSANRESOURCETYPE Type);
		bool iReleaseResourceCreaterFunc(const SString &strFileName, SANRES_CREATEFUNC pCreateFunc, eSANRESOURCETYPE Type);
		bool iRegistResourceCopyFunc(SANRES_COPYFUNC pCopyFunc, eSANRESOURCETYPE Type);
		bool iReleaseResourceCopyFunc(SANRES_COPYFUNC pCopyFunc, eSANRESOURCETYPE Type);
		bool iRegistResourceReleaseFunc(SANRES_RELEASEFUNC pReleaseFunc, eSANRESOURCETYPE Type);
		bool iReleaseResourceReleaseFunc(SANRES_RELEASEFUNC pReleaseFunc, eSANRESOURCETYPE Type);
		bool iRegistResourceDefaultPath(const SString &strDefaultPath);
		void iReleaseResourceDefaultPath(const SString &strDefaultPath);
		SVALUE iGetAvailableID(eSANRESOURCETYPE ResType);
		SANRES_CREATEFUNC iGetCreateFunc(const SString &strFileName,eSANRESOURCETYPE Type);
		SANRES_COPYFUNC iGetCopyFunc(eSANRESOURCETYPE Type);
		SANRES_RELEASEFUNC iGetReleaseFunc(eSANRESOURCETYPE Type);
		SVALUE iGetResourceIDByPath(const SString &strFilePath);
		SString iGetResourcePathByID(const SVALUE ResID);
		eSANRESOURCETYPE iGetResourceType(const SVALUE ResID);
		bool iIsIDInUse(const SVALUE ResID);
		bool iFindResource(const SVALUE ResID);
		bool iFindTexture(const SVALUE TexID);
		bool iFindMesh(const SVALUE MeshID);
		bool iFindSound(const SVALUE SoundID);
		lpSANRESPTRC iGetResourceConstPtr(const SVALUE ResID, const uint32 RequireID = 0, const uint32 RequireGroupID = 0);
		lpSANRESPTRC iGetTextureConstPtr(const SVALUE TexID, const uint32 RequireID = 0, const uint32 RequireGroupID = 0);
		lpSANRESPTRC iGetMeshConstPtr(const SVALUE MeshID, const uint32 RequireID = 0, const uint32 RequireGroupID = 0);
		lpSANRESPTRC iGetSoundConstPtr(const SVALUE SoundID, const uint32 RequireID = 0, const uint32 RequireGroupID = 0);
		lpSANRESPTRC iCopyResourceConstPtr(const lpSANRESPTRC Ptr, const uint32 RequireID = 0, const uint32 RequireGroupID = 0);
		lpSANRESPTRC iCopyTextureConstPtr(const lpSANRESPTRC Ptr, const uint32 RequireID = 0, const uint32 RequireGroupID = 0);
		lpSANRESPTRC iCopyMeshConstPtr(const lpSANRESPTRC Ptr, const uint32 RequireID = 0, const uint32 RequireGroupID = 0);
		lpSANRESPTRC iCopySoundConstPtr(const lpSANRESPTRC Ptr, const uint32 RequireID = 0, const uint32 RequireGroupID = 0);
		void iReleaseResourceConstPtr(const lpSANRESPTRC Ptr);
		void iReleaseTextureConstPtr(const lpSANRESPTRC Ptr);
		void iReleaseMeshConstPtr(const lpSANRESPTRC Ptr);
		void iReleaseSoundConstPtr(const lpSANRESPTRC Ptr);
		lpSANRESPTR iGetResourcePtr(const uint32 ResID, const uint32 RequireID = 0, const uint32 RequireGroupID = 0);
		lpSANRESPTR iGetTexturePtr(const uint32 TexID, const uint32 RequireID = 0, const uint32 RequireGroupID = 0);
		lpSANRESPTR iGetMeshPtr(const uint32 MeshID, const uint32 RequireID = 0, const uint32 RequireGroupID = 0);
		lpSANRESPTR iGetSoundPtr(const uint32 SoundID, const uint32 RequireID = 0, const uint32 RequireGroupID = 0);
		lpSANRESPTR iCopyResourcePtr(const lpSANRESPTR Ptr, const uint32 RequireID = 0, const uint32 RequireGroupID = 0);
		lpSANRESPTR iCopyTexturePtr(const lpSANRESPTR Ptr, const uint32 RequireID = 0, const uint32 RequireGroupID = 0);
		lpSANRESPTR iCopyMeshPtr(const lpSANRESPTR Ptr, const uint32 RequireID = 0, const uint32 RequireGroupID = 0);
		lpSANRESPTR iCopySoundPtr(const lpSANRESPTR Ptr, const uint32 RequireID = 0, const uint32 RequireGroupID = 0);
		void iReleaseResourcePtr(const lpSANRESPTR Ptr);
		void iReleaseTexturePtr(const lpSANRESPTR Ptr);
		void iReleaseMeshPtr(const lpSANRESPTR Ptr);
		void iReleaseSoundPtr(const lpSANRESPTR Ptr);
		void iUpdateResourceManager();
		virtual bool iRegistGraphicsDeviceResourceManagerHandle(const SHANDLE pManagerHandle);
		virtual SHANDLE iGetDeviceResourceManagerHandle();
		virtual SHANDLE iAddToDeviceTextureList(const lpSANTEXTURE Texture);
		virtual SHANDLE iAddToDeviceMeshList(const lpSANOBJECT Object);
		virtual bool iDeleteFormDeviceTextureList(const lpSANTEXTURE Texture);
		virtual bool iDeleteFromDeviceMeshList(const lpSANOBJECT Object);
	};

#endif
}