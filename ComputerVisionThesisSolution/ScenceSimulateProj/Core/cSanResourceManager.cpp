#include"cSanResourceManager.h"
using namespace std;
using namespace San;
using namespace San::FileIOStream;
bool cSanResourceManager::iCreateResourceManager()
{
	if(::gloFoundVector(this->m_strName))
	{
		return false;
	}
	this->m_bManagerLock=false;
	this->m_FileMatchTable.clear();
	//this->m_TextureList.clear();
	//this->m_MeshList.clear();
	//this->m_SoundList.clear();
	this->m_FilePathSearchTableForward.clear();
	this->m_FilepathSearchTableBackward.clear();
	this->m_pResourcesTableArray = new SANRESOURCETABLE[RESOURCETYPECOUNT];
	this->m_pAvailableIDListArray = new list<SVALUE>[RESOURCETYPECOUNT];
	this->m_pMaxAvailableIDArray = new SVALUE[RESOURCETYPECOUNT];
	for (uint32 seek = 0; seek < RESOURCETYPECOUNT; seek = seek + 1)
	{
		this->m_pResourcesTableArray[seek].clear();
		this->m_pMaxAvailableIDArray[seek] = (uint32) (this->_GetResourceTypeByArrayIndex(seek)) + 1;
		this->m_pAvailableIDListArray[seek].clear();
	}
	this->m_GraphicsDeviceResourceManagerHandle=nullptr;
	this->iRegistResourceCreaterFunc(_SSTR(".bmp"),this->_LoadBMPFile,RT_TEXTURE);
	this->iRegistResourceCreaterFunc(_SSTR(".bmp"), this->_LoadBMPFile, RT_TEXTURE_2D);
	this->iRegistResourceCreaterFunc(_SSTR(".BMP"),this->_LoadBMPFile,RT_TEXTURE);
	this->iRegistResourceCreaterFunc(_SSTR(".BMP"), this->_LoadBMPFile, RT_TEXTURE_2D);
	this->iRegistResourceCreaterFunc(_SSTR(".tga"),this->_LoadTGAFile,RT_TEXTURE);
	this->iRegistResourceCreaterFunc(_SSTR(".tga"), this->_LoadTGAFile, RT_TEXTURE_2D);
	this->iRegistResourceCreaterFunc(_SSTR(".TGA"),this->_LoadTGAFile,RT_TEXTURE);
	this->iRegistResourceCreaterFunc(_SSTR(".TGA"), this->_LoadTGAFile, RT_TEXTURE_2D);
	this->iRegistResourceCreaterFunc(_SSTR(".obj"),this->_LoadOBJFile,RT_MESH);
	this->iRegistResourceCreaterFunc(_SSTR(".obj"), this->_LoadOBJFile, RT_MESH_STATIC);
	this->iRegistResourceCreaterFunc(_SSTR(".OBJ"),this->_LoadOBJFile,RT_MESH);
	this->iRegistResourceCreaterFunc(_SSTR(".OBJ"), this->_LoadOBJFile, RT_MESH_STATIC);
	this->iRegistResourceCreaterFunc(_SSTR(".x"),this->_LoadXMDFile,RT_MESH);
	this->iRegistResourceCreaterFunc(_SSTR(".x"), this->_LoadXMDFile, RT_MESH_STATIC);
	this->iRegistResourceCreaterFunc(_SSTR(".X"),this->_LoadXMDFile,RT_MESH);
	this->iRegistResourceCreaterFunc(_SSTR(".X"), this->_LoadXMDFile, RT_MESH_STATIC);
	this->iRegistResourceCreaterFunc(_SSTR(".fbx"),this->_LoadFBXFile,RT_MESH);
	this->iRegistResourceCreaterFunc(_SSTR(".fbx"), this->_LoadFBXFile, RT_MESH_STATIC);
	this->iRegistResourceCreaterFunc(_SSTR(".fbx"), this->_LoadFBXFile, RT_MESH_DYNAMIC);
	this->iRegistResourceCreaterFunc(_SSTR(".FBX"),this->_LoadFBXFile,RT_MESH);
	this->iRegistResourceCreaterFunc(_SSTR(".FBX"), this->_LoadFBXFile, RT_MESH_STATIC);
	this->iRegistResourceCreaterFunc(_SSTR(".FBX"), this->_LoadFBXFile, RT_MESH_DYNAMIC);
	this->iRegistResourceCopyFunc(this->_CopyTexture,RT_TEXTURE);
	this->iRegistResourceCopyFunc(this->_CopyTexture, RT_TEXTURE_2D);
	this->iRegistResourceCopyFunc(this->_CopyTexture, RT_TEXTURE_3D);
	this->iRegistResourceCopyFunc(this->_CopyMesh, RT_MESH);
	this->iRegistResourceCopyFunc(this->_CopyMesh, RT_MESH_STATIC);
	this->iRegistResourceCopyFunc(this->_CopyMesh, RT_MESH_DYNAMIC);
	this->iRegistResourceCopyFunc(this->_CopyMesh, RT_SOUND);
	this->iRegistResourceReleaseFunc(this->_ReleaseTexture, RT_TEXTURE);
	this->iRegistResourceReleaseFunc(this->_ReleaseTexture, RT_TEXTURE_2D);
	this->iRegistResourceReleaseFunc(this->_ReleaseTexture, RT_TEXTURE_3D);
	this->iRegistResourceReleaseFunc(this->_ReleaseMesh, RT_MESH);
	this->iRegistResourceReleaseFunc(this->_ReleaseMesh, RT_MESH_STATIC);
	this->iRegistResourceReleaseFunc(this->_ReleaseMesh, RT_MESH_DYNAMIC);
	::gloRegisterVector(this->m_strName,VT_SYS|VT_VAR,(SHANDLE)this);
	return true;
}
void cSanResourceManager::iReleaseResourceManager()
{
	if(!this->m_FileMatchTable.empty())
	{
		this->m_FileMatchTable.clear();
	}
	for (uint32 seek = 0; seek < RESOURCETYPECOUNT; seek = seek + 1)
	{
		this->_ReleaseList(this->_GetResourceTypeByArrayIndex(seek));
		this->m_pMaxAvailableIDArray[seek] = (uint32) (this->_GetResourceTypeByArrayIndex(seek)) + 1;
		this->m_pAvailableIDListArray[seek].clear();
	}
	list<lpSANRESPTR>::iterator pRes = this->m_AssignedVoidPtrList.begin();
	while (pRes != this->m_AssignedVoidPtrList.end())
	{
		delete (*pRes);
		(*pRes) = nullptr;
		pRes++;
	}
	list<lpSANRESPTRC>::iterator pResC = this->m_AssignedVoidConstPtrList.begin();
	while (pResC != this->m_AssignedVoidConstPtrList.end())
	{
		delete (*pResC);
		(*pResC) = nullptr;
		pResC++;
	}
	delete this->m_pResourcesTableArray;
	this->m_pResourcesTableArray = nullptr;
	delete this->m_pAvailableIDListArray;
	this->m_pAvailableIDListArray = nullptr;
	delete this->m_pMaxAvailableIDArray;
	this->m_pMaxAvailableIDArray = nullptr;
	::gloDestoryVector(this->m_strName,VT_SYS|VT_VAR);
}
uint32 cSanResourceManager::iCreateResourceFromFile(const SString &strFilePath, const eSANRESOURCETYPE Type, const uint32 CreaterID, const uint32 GroupID, const uint32 Flag, SANRES_CREATEFUNC pCreateFunc, SANRES_COPYFUNC pCopyFunc, SANRES_RELEASEFUNC pReleaseFunc)
{
	if (strFilePath.empty())
	{
		return 0;
	}
	while (this->m_bManagerLock)
	{
	}
	this->m_bManagerLock = true;
	SVALUE ResID = this->iGetAvailableID(Type);
	if (ResID == 0)
	{
		this->m_bManagerLock = false;
		return 0;
	}
	if(this->m_FilePathSearchTableForward.find(strFilePath)!=this->m_FilePathSearchTableForward.end())
	{
		this->m_bManagerLock=false;
		return this->iGetResourceIDByPath(strFilePath);
	}
	if(pCreateFunc==nullptr)
	{
		pCreateFunc = this->iGetCreateFunc(strFilePath.substr(strFilePath.find(_SSTR(".")), strFilePath.length()), Type);
	}
	if(pCreateFunc==nullptr)
	{
		this->m_bManagerLock=false;
		return 0;
	}
	SANRESOURCE Res;
	Res.description.ResID=ResID;
	Res.description.CreaterID = CreaterID;
	Res.description.GroupID = GroupID;
	Res.description.AccessFlag = SRIO_ORIGIONAL_RESOURCE | Flag;
	Res.description.AccessFlag = (CreaterID == 0) ? (Res.description.AccessFlag | SRIO_MANAGER_AUTO_DELETE) : Res.description.AccessFlag;
	Res.description.pCopyFunc = pCopyFunc == nullptr ? this->iGetCopyFunc(Type) : pCopyFunc;
	Res.description.pReleaseFunc = pReleaseFunc == nullptr ? this->iGetReleaseFunc(Type) : pReleaseFunc;
	Res.description.OrigionResPtr = nullptr;
	Res.description.PreviousResPtr = nullptr;
	Res.description.AssignedPtr.clear();
	Res.description.AssignedConstPtr.clear();
	Res.description.ReferenceCount = -1;
	Res.value = nullptr;
	Res.description.Size = pCreateFunc(strFilePath, 0, &(Res.value));
	if ((Res.description.Size == 0) || (Res.value == nullptr))
	{
		this->m_bManagerLock = false;
		return 0;
	}
	this->_RegisteID(&Res, Type);
	this->m_FilePathSearchTableForward.insert(::make_pair(strFilePath, ResID));
	this->m_FilepathSearchTableBackward.insert(::make_pair(ResID, strFilePath));
	this->m_bManagerLock=false;
	return ResID;
}
SVALUE cSanResourceManager::iCreateTextureFromFile(const SString &strFilePath, const uint32 CreaterID, const uint32 GroupID, const uint32 Flag, SANRES_CREATEFUNC pCreateFunc, SANRES_COPYFUNC pCopyFunc, SANRES_RELEASEFUNC pReleaseFunc)
{
	SVALUE TexID = this->iCreateResourceFromFile(strFilePath, RT_TEXTURE, CreaterID, GroupID, Flag, pCreateFunc, pCopyFunc, pReleaseFunc);
	if (TexID == 0)
	{
		return TexID;
	}
	lpSANRESOURCE pTexRes = this->_GetResourceHandle(TexID, RT_TEXTURE);
	if (pTexRes == nullptr)
	{
		return 0;
	}
	lpSANTEXTURE pTex = (lpSANTEXTURE)pTexRes->value;
	pTex->CurrentParam = (SVALUE)this->iAddToDeviceTextureList(pTex);
	return TexID;
}
SVALUE cSanResourceManager::iCreateMeshFromFile(const SString &strFilePath, const uint32 CreaterID, const uint32 GroupID, const uint32 Flag, SANRES_CREATEFUNC pCreateFunc, SANRES_COPYFUNC pCopyFunc, SANRES_RELEASEFUNC pReleaseFunc)
{
	SVALUE MeshID = this->iCreateResourceFromFile(strFilePath, RT_MESH, CreaterID, GroupID, Flag, pCreateFunc, pCopyFunc, pReleaseFunc);
	if (MeshID == 0)
	{
		return MeshID;
	}
	lpSANRESOURCE pMeshRes = this->_GetResourceHandle(MeshID,RT_MESH);
	if (pMeshRes == nullptr)
	{
		return 0;
	}
	lpSANMESH pMesh = (lpSANMESH)pMeshRes->value;
	SANOBJECTLIST::iterator pObj = pMesh->value.begin();
	SVALUE TexID;
	while (pObj != pMesh->value.end())
	{
		pObj->ObjParam = this->iAddToDeviceMeshList(&(*pObj));
		list<stSANUNKNOWNEX<SANTEXRENDERDESC, SHANDLE>>::iterator pTex = pObj->Material.TexList.begin();
		while (pTex != pObj->Material.TexList.end())
		{
			TexID = this->iCreateTextureFromFile(pTex->description.strFilePath, CreaterID, GroupID, Flag, nullptr, nullptr, nullptr);
			lpSANRESPTRC pTextureRes = this->iGetTextureConstPtr(TexID, CreaterID, GroupID);
			if (pTextureRes->res_ptr != nullptr)
			{
				pTex->value = (SHANDLE) pTextureRes;
			}
			else
			{
				pTex->value = nullptr;
			}
			pTex++;
		}
		pObj++;
	}
	return MeshID;
}
SVALUE cSanResourceManager::iCreateResourceFromMemory(SHANDLE BufferPtr, SVALUE BufferSize, const eSANRESOURCETYPE Type, const uint32 CreaterID, const uint32 GroupID, const uint32 Flag, SANRES_COPYFUNC pCopyFunc, SANRES_RELEASEFUNC pReleaseFunc)
{
	if (BufferPtr == nullptr)
	{
		return 0;
	}
	while (this->m_bManagerLock)
	{
	}
	this->m_bManagerLock=true;
	SVALUE ResID = this->iGetAvailableID(Type);
	if (ResID == 0)
	{
		return 0;
	}
	SANRESOURCE Res;
	Res.description.ResID=ResID;
	Res.description.Size = BufferSize;
	Res.description.CreaterID = CreaterID;
	Res.description.GroupID = GroupID;
	Res.description.AccessFlag = SRIO_ORIGIONAL_RESOURCE | Flag;
	Res.description.pCopyFunc = pCopyFunc == nullptr ? this->iGetCopyFunc(Type) : pCopyFunc;
	Res.description.pReleaseFunc = pReleaseFunc == nullptr ? this->iGetReleaseFunc(Type) : pReleaseFunc;
	Res.description.OrigionResPtr = nullptr;
	Res.description.PreviousResPtr = nullptr;
	Res.description.AssignedPtr.clear();
	Res.description.AssignedConstPtr.clear();
	Res.description.ReferenceCount = -1;
	Res.value=BufferPtr;
	this->_RegisteID(&Res,Type);
	this->m_bManagerLock=false;
	return ResID;
}
SVALUE cSanResourceManager::iCreateTextureFromMemory(lpSANTEXTURE pTexture, const uint32 CreaterID, const uint32 GroupID, const uint32 Flag, SANRES_COPYFUNC pCopyFunc, SANRES_RELEASEFUNC pReleaseFunc)
{
	SVALUE TexID = this->iCreateResourceFromMemory(pTexture, sizeof(SANTEXTURE), RT_TEXTURE, CreaterID, GroupID, Flag, pCopyFunc, pReleaseFunc);
	if (TexID == 0)
	{
		return TexID;
	}
	lpSANRESOURCE pTexRes = this->_GetResourceHandle(TexID, RT_TEXTURE);
	if (pTexRes == nullptr)
	{
		return 0;
	}
	lpSANTEXTURE pTex = (lpSANTEXTURE) pTexRes->value;
	pTex->CurrentParam = (SVALUE)this->iAddToDeviceTextureList(pTex);
	return TexID;
}
SVALUE cSanResourceManager::iCreateMeshFromMemory(lpSANMESH pMesh, const uint32 CreaterID, const uint32 GroupID, const uint32 Flag, SANRES_COPYFUNC pCopyFunc, SANRES_RELEASEFUNC pReleaseFunc)
{
	SVALUE MeshID = this->iCreateResourceFromMemory(pMesh, sizeof(SANMESH), RT_MESH, CreaterID, GroupID, Flag, pCopyFunc, pReleaseFunc);
	if (MeshID == 0)
	{
		return MeshID;
	}
	lpSANRESOURCE pMeshRes = this->_GetResourceHandle(MeshID,RT_MESH);
	if (pMeshRes == nullptr)
	{
		return 0;
	}
	lpSANMESH MeshPtr = (lpSANMESH) pMeshRes->value;
	SANOBJECTLIST::iterator pObj = MeshPtr->value.begin();
	SVALUE TexID;
	while (pObj != MeshPtr->value.end())
	{
		pObj->ObjParam = this->iAddToDeviceMeshList(&(*pObj));
		list<stSANUNKNOWNEX<SANTEXRENDERDESC, SHANDLE>>::iterator pTex = pObj->Material.TexList.begin();
		while (pTex != pObj->Material.TexList.end())
		{
			TexID = this->iCreateTextureFromFile(pTex->description.strFilePath, CreaterID, GroupID, Flag, nullptr, nullptr, nullptr);
			lpSANRESPTRC pTextureRes = this->iGetTextureConstPtr(TexID, CreaterID, GroupID);
			if (pTextureRes->res_ptr != nullptr)
			{
				pTex->value = (SHANDLE) pTextureRes;
			}
			else
			{
				pTex->value = nullptr;
			}
			pTex++;
		}
		pObj++;
	}
	return MeshID;
}
SVALUE cSanResourceManager::iCopyResource(uint32 ResID, const uint32 RequireID, const uint32 CreaterID, const uint32 GroupID, const uint32 Flag, SANRES_COPYFUNC pCopyFunc, SANRES_RELEASEFUNC pReleaseFunc)
{
	while (this->m_bManagerLock)
	{
	}
	this->m_bManagerLock = true;
	eSANRESOURCETYPE Type = this->iGetResourceType(ResID);
	SVALUE DestResID = this->iGetAvailableID(Type);
	if (DestResID == 0)
	{
		this->m_bManagerLock = false;
		return 0;
	}
	lpSANRESOURCE pRes = this->_GetResourceHandle(ResID, Type);
	if (pRes == nullptr)
	{
		this->m_bManagerLock = false;
		return 0;
	}
	if ((pRes->description.AccessFlag&SRIO_MANAGER_REFERENCE) == SRIO_MANAGER_REFERENCE)
	{
		this->m_bManagerLock = false;
		return 0;
	}
	if (pRes->description.CreaterID == 0)
	{
		if ((pRes->description.AccessFlag&SRIO_GLOBAL_COPY) != SRIO_GLOBAL_COPY)
		{
			this->m_bManagerLock = false;
			return 0;
		}
	}
	else
	{
		if (pRes->description.CreaterID != RequireID)
		{
			if (pRes->description.GroupID == 0)
			{
				if ((pRes->description.AccessFlag&SRIO_GLOBAL_COPY) != SRIO_GLOBAL_COPY)
				{
					this->m_bManagerLock = false;
					return 0;
				}
			}
			else
			{
				if (pRes->description.GroupID != RequireID)
				{
					if ((pRes->description.AccessFlag&SRIO_GLOBAL_COPY) != SRIO_GLOBAL_COPY)
					{
						this->m_bManagerLock = false;
						return 0;
					}
				}
				else
				{
					if ((pRes->description.AccessFlag&SRIO_GROUP_COPY) != SRIO_GROUP_COPY)
					{
						this->m_bManagerLock = false;
						return 0;
					}
				}
			}
		}
		else
		{
			if ((pRes->description.AccessFlag&SRIO_CREATER_COPY) != SRIO_CREATER_COPY)
			{
				this->m_bManagerLock = false;
				return 0;
			}
		}
	}
	SANRESOURCE DestRes;
	DestRes.description.ResID = DestResID;
	DestRes.description.Size = pRes->description.Size;
	DestRes.description.CreaterID = CreaterID;
	DestRes.description.GroupID = GroupID;
	DestRes.description.AccessFlag = ((Flag&SRIO_MANAGER_COPY_FLAG) == SRIO_MANAGER_COPY_FLAG) ? pRes->description.AccessFlag : Flag;
	DestRes.description.AccessFlag = (CreaterID == 0) ? (DestRes.description.AccessFlag | SRIO_MANAGER_AUTO_DELETE) : DestRes.description.AccessFlag;
	DestRes.description.pCopyFunc = pCopyFunc == nullptr ? pRes->description.pCopyFunc : pCopyFunc;
	DestRes.description.pReleaseFunc = pReleaseFunc == nullptr ? pRes->description.pReleaseFunc : pReleaseFunc;
	DestRes.description.OrigionResPtr = ((pRes->description.AccessFlag&SRIO_ORIGIONAL_RESOURCE) == SRIO_ORIGIONAL_RESOURCE) ? pRes : pRes->description.OrigionResPtr;
	DestRes.description.PreviousResPtr = pRes;
	DestRes.description.AssignedPtr.clear();
	DestRes.description.AssignedConstPtr.clear();
	DestRes.description.ReferenceCount = -1;
	DestRes.description.Size = DestRes.description.pCopyFunc(pRes->value, &(DestRes.value));
	if (DestRes.value == 0)
	{
		this->m_bManagerLock = false;
		return 0;
	}
	this->_RegisteID(&DestRes, Type);
	this->m_bManagerLock = false;
	return ResID;
}
SVALUE cSanResourceManager::iCopyTexture(uint32 TexID, const uint32 RequireID, const uint32 CreaterID, const uint32 GroupID, const uint32 Flag, SANRES_COPYFUNC pCopyFunc, SANRES_RELEASEFUNC pReleaseFunc)
{
	SVALUE DestTexID = this->iCopyResource(TexID, RequireID, CreaterID, GroupID, Flag, pCopyFunc, pReleaseFunc);
	if (DestTexID == 0)
	{
		return DestTexID;
	}
	lpSANRESOURCE pTexRes = this->_GetResourceHandle(DestTexID, RT_TEXTURE);
	if (pTexRes == nullptr)
	{
		return 0;
	}
	lpSANTEXTURE pTex = (lpSANTEXTURE) pTexRes->value;
	pTex->CurrentParam = (SVALUE)this->iAddToDeviceTextureList(pTex);
	return DestTexID;
}
SVALUE cSanResourceManager::iCopyMesh(uint32 MeshID, const uint32 RequireID, const uint32 CreaterID, const uint32 GroupID, const uint32 Flag, SANRES_COPYFUNC pCopyFunc, SANRES_RELEASEFUNC pReleaseFunc)
{
	SVALUE DestMeshID = this->iCopyResource(MeshID, RequireID, CreaterID, GroupID, Flag, pCopyFunc, pReleaseFunc);
	if (DestMeshID == 0)
	{
		return DestMeshID;
	}
	lpSANRESOURCE pMeshRes = this->_GetResourceHandle(DestMeshID, RT_MESH);
	if (pMeshRes == nullptr)
	{
		return 0;
	}
	lpSANMESH pMesh = (lpSANMESH) pMeshRes->value;
	SANOBJECTLIST::iterator pObj = pMesh->value.begin();
	SVALUE TexID;
	while (pObj != pMesh->value.end())
	{
		pObj->ObjParam = this->iAddToDeviceMeshList(&(*pObj));
		list<stSANUNKNOWNEX<SANTEXRENDERDESC, SHANDLE>>::iterator pTex = pObj->Material.TexList.begin();
		while (pTex != pObj->Material.TexList.end())
		{
			TexID = this->iCreateTextureFromFile(pTex->description.strFilePath, CreaterID, GroupID, Flag, nullptr, nullptr, nullptr);
			lpSANRESPTRC pTextureRes = this->iGetTextureConstPtr(TexID, CreaterID, GroupID);
			if (pTextureRes->res_ptr != nullptr)
			{
				pTex->value = (SHANDLE) pTextureRes;
			}
			else
			{
				pTex->value = nullptr;
			}
			pTex++;
		}
		pObj++;
	}
	return DestMeshID;
}
bool cSanResourceManager::iReleaseResource(SVALUE ResID, const uint32 ReleaseID, SANRES_RELEASEFUNC pReleaseFunc)
{
	if(ResID==0)
	{
		return false;
	}
	eSANRESOURCETYPE ResType = this->iGetResourceType(ResID);
	while (this->m_bManagerLock)
	{
	}
	this->m_bManagerLock = true;
	if (this->_ReleaseID(ResID, ResType, ReleaseID, ReleaseID, pReleaseFunc))
	{
		map<SVALUE, SString>::iterator pItem = this->m_FilepathSearchTableBackward.find(ResID);
		if (pItem != this->m_FilepathSearchTableBackward.end())
		{
			this->m_FilepathSearchTableBackward.erase(ResID);
			this->m_FilePathSearchTableForward.erase(pItem->second);
		}
		this->m_bManagerLock = false;
		return true;
	}
	this->m_bManagerLock = false;
	return false;
}
bool cSanResourceManager::iReleaseTextureResource(SVALUE TexID, const uint32 ReleaseID, SANRES_RELEASEFUNC pReleaseFunc)
{
	return this->iReleaseResource(TexID, ReleaseID, pReleaseFunc);
}
bool cSanResourceManager::iReleaseMeshResource(SVALUE MeshID, const uint32 ReleaseID, SANRES_RELEASEFUNC pReleaseFunc)
{
	return this->iReleaseResource(MeshID, ReleaseID, pReleaseFunc);
}
bool cSanResourceManager::iUpdateResourceAccessFlag(uint32 ResID, const eSANRESOURCETYPE Type, const uint32 Flag, const uint32 RequireID)
{
	if (ResID == 0)
	{
		return false;
	}
	eSANRESOURCETYPE ResType = this->iGetResourceType(ResID);
	lpSANRESOURCE pRes = this->_GetResourceHandle(ResID, ResType);
	if (pRes == nullptr)
	{
		return false;
	}
	if (pRes->description.CreaterID != RequireID)
	{
		return false;
	}
	pRes->description.AccessFlag = Flag;
	this->m_bManagerLock = false;
	return false;
}
bool cSanResourceManager::iUpdateResourceCopyFunc(uint32 ResID, const eSANRESOURCETYPE Type, SANRES_COPYFUNC pCopyFunc, const uint32 RequireID)
{
	if (ResID == 0)
	{
		return false;
	}
	eSANRESOURCETYPE ResType = this->iGetResourceType(ResID);
	lpSANRESOURCE pRes = this->_GetResourceHandle(ResID, ResType);
	if (pRes == nullptr)
	{
		return false;
	}
	if (pRes->description.CreaterID != RequireID)
	{
		return false;
	}
	pRes->description.pCopyFunc = pCopyFunc;
	this->m_bManagerLock = false;
	return false;
}
bool cSanResourceManager::iUpdateResourceReleaseFunc(uint32 ResID, const eSANRESOURCETYPE Type, SANRES_RELEASEFUNC pReleaseFunc, const uint32 RequireID)
{
	if (ResID == 0)
	{
		return false;
	}
	eSANRESOURCETYPE ResType = this->iGetResourceType(ResID);
	lpSANRESOURCE pRes = this->_GetResourceHandle(ResID, ResType);
	if (pRes == nullptr)
	{
		return false;
	}
	if (pRes->description.CreaterID != RequireID)
	{
		return false;
	}
	pRes->description.pReleaseFunc = pReleaseFunc;
	this->m_bManagerLock = false;
	return false;
}
SVALUE cSanResourceManager::iRegisteResource(const SHANDLE ResHandle, const uint32 ResSize, const eSANRESOURCETYPE ResType, const uint32 CreaterID, const uint32 GroupID, const uint32 Flag, SANRES_COPYFUNC pCopyFunc, SANRES_RELEASEFUNC pReleaseFunc)
{
	while (this->m_bManagerLock)
	{
	}
	this->m_bManagerLock = true;
	SANRESOURCE Res;
	Res.description.ResID = 0;
	Res.description.Size = ResSize;
	Res.description.AccessFlag = SRIO_MANAGER_REFERENCE | Flag;
	Res.description.CreaterID = CreaterID;
	Res.description.GroupID = GroupID;
	Res.description.pReleaseFunc = pReleaseFunc;
	Res.description.OrigionResPtr = nullptr;
	Res.description.PreviousResPtr = nullptr;
	Res.description.AssignedPtr.clear();
	Res.description.AssignedConstPtr.clear();
	Res.description.ReferenceCount = -1;
	Res.value = ResHandle;
	this->m_bManagerLock = false;
	return this->_RegisteID(&Res, ResType);
}
SRESULT cSanResourceManager::_LoadBMPFile(SString strFilePath, SVALUE Param, SHANDLE* pHandle)
{
	if ((pHandle == nullptr) || ((*pHandle) != nullptr))
	{
		return 0;
	}
	SStringW strPath = ::gloTToW(strFilePath.c_str()).c_str();
	cBMPLoader *pBMPLoader=new cBMPLoader(strPath.c_str());
	if(!pBMPLoader->iBMPLoad())
	{
		delete pBMPLoader;
		pBMPLoader=nullptr;
		*pHandle = nullptr;
		return 0;
	}
	*pHandle = new SANTEXTURE;
	lpSANTEXTURE pTex = (lpSANTEXTURE) (*pHandle);
	::gloMemSet(pTex,0,sizeof(SANTEXTURE));
	pTex->Version=STV_STATIC;
	pTex->Type=STT_TEX2D;
	pTex->PixMapPtr = new SANPIXMAP2D;
	::gloMemSet((pTex->PixMapPtr), 0, sizeof(SANPIXMAP2D));
	lpSANPIXMAP2D(pTex->PixMapPtr)->width = pBMPLoader->iBMPGetWeight();
	lpSANPIXMAP2D(pTex->PixMapPtr)->height = pBMPLoader->iBMPGetHigh();
	unsigned int size = lpSANPIXMAP2D(pTex->PixMapPtr)->width*lpSANPIXMAP2D(pTex->PixMapPtr)->height;
	lpSANPIXMAP2D(pTex->PixMapPtr)->pPixMap = new SANPIX[size];
	::gloMemSet(lpSANPIXMAP2D(pTex->PixMapPtr)->pPixMap, 0, sizeof(SANPIX)*size);
	uint8 *pPtr=(uint8*)pBMPLoader->iBMPGetImage();
	if(pBMPLoader->iGetBitmapType()==BMP_RGB)
	{
		pTex->PixFormat=PT_RGB;
		for(int seek=0;seek<size;seek=seek+1)
		{
			unsigned int seekb=seek*3;
			lpSANPIXMAP2D(pTex->PixMapPtr)->pPixMap[seek].r = pPtr[seekb];
			lpSANPIXMAP2D(pTex->PixMapPtr)->pPixMap[seek].g = pPtr[seekb + 1];
			lpSANPIXMAP2D(pTex->PixMapPtr)->pPixMap[seek].b = pPtr[seekb + 2];
			lpSANPIXMAP2D(pTex->PixMapPtr)->pPixMap[seek].a = 0xff;
		}
	}
	else
	{
		pTex->PixFormat=PT_RGBA;
		::gloMemCopy(lpSANPIXMAP2D(pTex->PixMapPtr)->pPixMap, pPtr, size*pTex->PixFormat);
	}
	pTex->DataSize=pBMPLoader->iBMPGetSize();
	delete[] pPtr;
	pPtr=nullptr;
	delete pBMPLoader;
	pBMPLoader=nullptr;
	return pTex->DataSize;
}
SRESULT cSanResourceManager::_LoadTGAFile(SString strFilePath, SVALUE Param, SHANDLE* pHandle)
{
	if ((pHandle == nullptr) || ((*pHandle) != nullptr))
	{
		return 0;
	}
	SStringW strPath = ::gloTToW(strFilePath.c_str()).c_str();
	cTGALoader *pTGALoader=new cTGALoader(strPath.c_str());
	if(!pTGALoader->iTGALoad())
	{
		delete pTGALoader;
		pTGALoader=nullptr;
		*pHandle = nullptr;
		return 0;
	}
	*pHandle = new SANTEXTURE;
	lpSANTEXTURE pTex = (lpSANTEXTURE) (*pHandle);
	::gloMemSet(pTex, 0, sizeof(SANTEXTURE));
	pTex->Version=STV_STATIC;
	pTex->Type=STT_TEX2D;
	pTex->PixMapPtr=new SANPIXMAP2D;
	::gloMemSet((pTex->PixMapPtr), 0, sizeof(SANPIXMAP2D));
	lpSANPIXMAP2D(pTex->PixMapPtr)->width = pTGALoader->iTGAGetWeight();
	lpSANPIXMAP2D(pTex->PixMapPtr)->height = pTGALoader->iTGAGetHigh();
	unsigned int size = lpSANPIXMAP2D(pTex->PixMapPtr)->width*lpSANPIXMAP2D(pTex->PixMapPtr)->height;
	lpSANPIXMAP2D(pTex->PixMapPtr)->pPixMap = new SANPIX[size];
	::gloMemSet(lpSANPIXMAP2D(pTex->PixMapPtr)->pPixMap, 0, sizeof(SANPIX)*size);
	uint8 *pPtr=(uint8*)pTGALoader->iGetImage();
	if(pTGALoader->iGetTGAType()==BMP_RGB)
	{
		pTex->PixFormat=PT_RGB;
		for(unsigned int seek=0;seek<size;seek=seek+1)
		{
			unsigned int seekb=seek*3;
			lpSANPIXMAP2D(pTex->PixMapPtr)->pPixMap[seek].r = pPtr[seekb];
			lpSANPIXMAP2D(pTex->PixMapPtr)->pPixMap[seek].g = pPtr[seekb + 1];
			lpSANPIXMAP2D(pTex->PixMapPtr)->pPixMap[seek].b = pPtr[seekb + 2];
			lpSANPIXMAP2D(pTex->PixMapPtr)->pPixMap[seek].a = 0xff;
		}
	}
	else
	{
		pTex->PixFormat=PT_RGBA;
		::gloMemCopy(lpSANPIXMAP2D(pTex->PixMapPtr)->pPixMap, pPtr, size*pTex->PixFormat);
	}
	pTex->DataSize=pTGALoader->iTGAGetSize();
	delete[] pPtr;
	pPtr=nullptr;
	delete pTGALoader;
	pTGALoader=nullptr;
	return pTex->DataSize;
}
SRESULT cSanResourceManager::_LoadOBJFile(SString strFilePath, SVALUE Param, SHANDLE* pHandle)
{
	if ((pHandle == nullptr) || ((*pHandle) != nullptr))
	{
		return 0;
	}
	cOBJFileLoader *pOBJLoader=new cOBJFileLoader();
	if (!pOBJLoader->iLoadFile(::gloTToW(strFilePath)))
	{
		delete pOBJLoader;
		pOBJLoader=nullptr;
		*pHandle = nullptr;
		return 0;
	}
	*pHandle = new SANMESH;
	lpSANMESH pMesh = (lpSANMESH) (*pHandle);
	pMesh->description = strFilePath;
	pMesh->value=pOBJLoader->iGetObjectList();
	delete pOBJLoader;
	pOBJLoader=nullptr;
	return pMesh->value.size();
}
SRESULT cSanResourceManager::_LoadXMDFile(SString strFilePath, SVALUE Param, SHANDLE* pHandle)
{
#ifdef _DIRECTX
	cXFileLoader *pXFileLoader=new cXFileLoader();
	if(!pXFileLoader->iLoadXFile(strFile))
	{
		delete pXFileLoader;
		return nullptr;
	}
	SANFRAMEELEMENT* pFrameElement=new SANFRAMEELEMENT;
	pFrameElement->pFrame=pXFileLoader->iGetXFrame();
	pFrameElement->Time=0;
	pFrameElement->Speed=0.0;
	pFrameElement->strName=strFile;
	delete pXFileLoader;
	return (SHANDLE)pFrameElement;
#else
	return 0;
#endif
}
SRESULT cSanResourceManager::_LoadFBXFile(SString strFilePath, SVALUE Param, SHANDLE* pHandle)
{
	if ((pHandle == nullptr) || ((*pHandle) != nullptr))
	{
		return 0;
	}
	/*cFBXFileLoader *pFBXLoader=new cFBXFileLoader();
	if (!pFBXLoader->iLoadFile(strFilePath))
	{
		pFBXLoader->iRelease();
		delete pFBXLoader;
		*pHandle = nullptr;
		return 0;
	}
	*pHandle = new SANMESH;
	lpSANMESH pMesh = (lpSANMESH) (*pHandle);
	pMesh->description = strFilePath;
	pMesh->value=pFBXLoader->iGetMesh();
	delete pFBXLoader;
	pFBXLoader=nullptr;
	if(pMesh->value.empty())
	{
		pMesh->description.clear();
		delete pMesh;
		pMesh=nullptr;
	}
	return pMesh->value.size();*/
}
SRESULT cSanResourceManager::_CopyTexture(SHANDLE TextureHandle, SHANDLE* pDestHandle)
{
	if ((TextureHandle == nullptr) || (pDestHandle = nullptr))
	{
		return 0;
	}
	lpSANTEXTURE pTex = new SANTEXTURE;
	*pTex = *((lpSANTEXTURE) TextureHandle);
	pTex->PixMapPtr = nullptr;
	pTex->ParamList = nullptr;
	pTex->ParamList = new SVALUE[pTex->ParamListSize];
	for (uint32 seek = 0; seek < pTex->ParamListSize; seek = seek + 1)
	{
		pTex->ParamList[seek] = 0;
	}
	pTex->CurrentParam = 0;
	lpSANPIXMAP2D pMap2D = nullptr;
	lpSANPIXMAPF2D pMap2DF = nullptr;
	switch (pTex->PixFormat)
	{
	case eSANPIXTYPE::PT_RGB:
	case eSANPIXTYPE::PT_RGBA:
		pTex->PixMapPtr = new SANPIXMAP2D;
		pMap2D = (lpSANPIXMAP2D)pTex->PixMapPtr;
		*pMap2D = *((lpSANPIXMAP2D) ((lpSANTEXTURE) TextureHandle)->PixMapPtr);
		pMap2D->pPixMap = new SANPIX[pMap2D->width*pMap2D->height];
		pTex->DataSize = pMap2D->width*pMap2D->height;
		::gloMemCopy(pTex->PixMapPtr, ((lpSANTEXTURE) TextureHandle)->PixMapPtr, sizeof(SANPIX)*pTex->DataSize);
		break;
	case eSANPIXTYPE::PT_RGB_F:
	case eSANPIXTYPE::PT_RGBA_F:
		pTex->PixMapPtr = new SANPIXMAPF2D;
		pMap2DF = (lpSANPIXMAPF2D) pTex->PixMapPtr;
		*pMap2DF = *((lpSANPIXMAPF2D) ((lpSANTEXTURE) TextureHandle)->PixMapPtr);
		pMap2DF->pPixMap = new SANPIXF[pMap2DF->width*pMap2DF->height];
		pTex->DataSize = pMap2DF->width*pMap2DF->height;
		::gloMemCopy(pTex->PixMapPtr, ((lpSANTEXTURE) TextureHandle)->PixMapPtr, sizeof(SANPIXF)*pTex->DataSize);
		break;
	default:
		delete (pTex->ParamList);
		pTex->ParamList = nullptr;
		delete pTex;
		pTex = nullptr;
		return 0;
		break;
	}
	*pDestHandle = pTex;
	return pTex->DataSize;
}
SRESULT cSanResourceManager::_CopyMesh(SHANDLE MeshHandle, SHANDLE* pDestHandle)
{
	if ((MeshHandle == nullptr) || (pDestHandle = nullptr))
	{
		return 0;
	}
	lpSANMESH pMesh = new SANMESH;
	lpSANMESH pSrcMesh = (lpSANMESH) MeshHandle;
	pMesh->description = pSrcMesh->description;
	SANOBJECTLIST::iterator pObject = pSrcMesh->value.begin();
	SANOBJECT Obj;
	while (pObject != pSrcMesh->value.end())
	{
		Obj = *pObject;
		list<stSANUNKNOWNEX<SANTEXRENDERDESC, SHANDLE>>::iterator pTex = Obj.Material.TexList.begin();
		while (pTex != Obj.Material.TexList.end())
		{
			pTex->value = nullptr;
			pTex++;
		}
		Obj.ObjParam = 0;
		Obj.pVertexList = nullptr;
		Obj.pVertexList = new SANVERTEX[Obj.VertexListSize];
		for (uint32 seek = 0; seek < Obj.VertexListSize; seek = seek + 1)
		{
			Obj.pVertexList[seek] = pObject->pVertexList[seek];
		}
		pMesh->value.insert(pMesh->value.end(), Obj);
		pObject++;
	}
	*pDestHandle = pMesh;
	return pMesh->value.size();
}
SRESULT cSanResourceManager::_ReleaseTexture(SHANDLE TextureHandle, SHANDLE ResourceManagerPtr, SVALUE Param)
{
	if ((TextureHandle == nullptr) || (ResourceManagerPtr == nullptr))
	{
		return true;
	}
	cSanResourceManager* pManager = (cSanResourceManager*) ResourceManagerPtr;
	lpSANTEXTURE pTex=(lpSANTEXTURE)TextureHandle;
	if(pTex->Type==STT_TEX2D)
	{
		delete [](lpSANPIXMAP2D(pTex->PixMapPtr)->pPixMap);
		lpSANPIXMAP2D(pTex->PixMapPtr)->pPixMap = nullptr;
		delete (lpSANPIXMAP2D(pTex->PixMapPtr));
		pTex->PixMapPtr = nullptr;
	}
	else
	{
		delete [](lpSANPIXMAP3D(pTex->PixMapPtr)->pPixMap);
		lpSANPIXMAP3D(pTex->PixMapPtr)->pPixMap = nullptr;
		delete (lpSANPIXMAP3D(pTex->PixMapPtr));
		pTex->PixMapPtr = nullptr;
	}
	//pManager->iDeleteFormDeviceTextureList(pTex);
	pTex->DataSize=0;
	return true;
}
SRESULT cSanResourceManager::_ReleaseMesh(SHANDLE MeshHandle, SHANDLE ResourceManagerPtr, SVALUE Param)
{
	if ((MeshHandle == nullptr) || (ResourceManagerPtr == nullptr))
	{
		return true;
	}
	cSanResourceManager* pManager = (cSanResourceManager*) ResourceManagerPtr;
	lpSANMESH pMesh=(lpSANMESH)MeshHandle;
	SANOBJECTLIST::iterator pObject=pMesh->value.begin();
	while(pObject!=pMesh->value.end())
	{
		//if(pObject->ObjParam!=nullptr)
		//{
			//if (pManager->iDeleteFromDeviceMeshList(&(*pObject)))
			//{
				//return false;
			//}
		//}
		list<stSANUNKNOWNEX<SANTEXRENDERDESC, SHANDLE>>::iterator pTex = pObject->Material.TexList.begin();
		while (pTex != pObject->Material.TexList.end())
		{
			if (pTex->description.RenderType == STRT_NORMAL)
			{
#ifdef _DIRECTX//////////////////////////////////////////////////////////////////////////////////////////////////Create Member From Memory
				LPDIRECT3DTEXTURE9 pDXTex=(LPDIRECT3DTEXTURE9)(pTexElement->Param);
				if(pDXTex!=nullptr)
				{
					pDXTex->Release();
					delete pDXTex;
				}
#endif
			}
			if (((lpSANRESPTRC) (pTex->value))->iIsReleased())
			{
				delete ((lpSANRESPTRC) (pTex->value));
			}
			else
			{
				((lpSANRESPTRC) (pTex->value))->iRelease();
			}
			pTex++;
		}
		pObject->Material.TexList.clear();
		if(pObject->pVertexList!=nullptr)
		{
			delete[] (pObject->pVertexList);
			pObject->pVertexList=nullptr;
			pObject->VertexListSize=0;
		}
		pObject++;
	}
	pMesh->value.clear();
	return true;
}
bool cSanResourceManager::_ReleaseList(uint32 ListType)
{
	SANRESOURCETABLE::iterator pRes;
	eSANRESOURCETYPE ResType = this->_GetResourceTypeByArrayIndex(ListType);
	if (this->m_pResourcesTableArray[ListType].empty())
	{
		return true;
	}
	pRes = this->m_pResourcesTableArray[ListType].begin();
	lpSANMESH pMesh = nullptr;
	SANOBJECTLIST::iterator pObj;
	while (pRes != this->m_pResourcesTableArray[ListType].end())
	{
		this->m_FilePathSearchTableForward.erase(this->m_FilepathSearchTableBackward.find(pRes->first)->second);
		this->m_FilepathSearchTableBackward.erase(pRes->first);
		switch (ResType)
		{
		case RT_TEXTURE:
		case RT_TEXTURE_2D:
		case RT_TEXTURE_3D:
			this->iDeleteFormDeviceTextureList((lpSANTEXTURE) pRes->second.value);
			break;
		case RT_MESH:
		case RT_MESH_STATIC:
		case RT_MESH_DYNAMIC:
			pMesh = (lpSANMESH) pRes->second.value;
			pObj = pMesh->value.begin();
			while (pObj != pMesh->value.end())
			{
				this->iDeleteFromDeviceMeshList(&(*pObj));
			}
			break;
		case RT_SOUND:
			break;
		default:
			break;
		}
		if (!pRes->second.description.AssignedPtr.empty())
		{
			list<lpSANRESPTR>* pAsdList = &(pRes->second.description.AssignedPtr);
			list<lpSANRESPTR>::iterator pAsdPtr = pAsdList->begin();
			while (pAsdPtr != pAsdList->end())
			{
				(*pAsdPtr)->res_ptr = nullptr;
				if ((*pAsdPtr)->b_released)
				{
					delete (*pAsdPtr);
					(*pAsdPtr) = nullptr;
				}
				else
				{
					(*pAsdPtr)->b_released = true;
				}
				pAsdPtr++;
			}
			pRes->second.description.AssignedPtr.clear();
		}
		if (pRes->second.description.pReleaseFunc != nullptr)
		{
			pRes->second.description.pReleaseFunc(pRes->second.value, this, NULL);
		}
		pRes->second.description.pReleaseFunc = nullptr;
		pRes->second.description.Size = 0;
		pRes++;
	}
	this->m_pMaxAvailableIDArray[ListType] = this->_GetResourceTypeByArrayIndex(ListType) + 1;
	this->m_pAvailableIDListArray[ListType].clear();
	this->m_pResourcesTableArray[ListType].clear();
}
SVALUE cSanResourceManager::_RegisteID(lpSANRESOURCE pRes, const eSANRESOURCETYPE Type)
{
	uint32 Index = this->_GetResourceArrayIndexByType(Type);
	if (pRes->description.ResID == 0)
	{
		if (!this->m_pAvailableIDListArray[Index].empty())
		{
			pRes->description.ResID = *(this->m_pAvailableIDListArray[Index].begin());
			this->m_pAvailableIDListArray[Index].erase((this->m_pAvailableIDListArray[Index].begin()));
		}
		else
		{
			if (this->m_pMaxAvailableIDArray[Index] == 0)
			{
				return 0;
			}
			pRes->description.ResID = this->m_pMaxAvailableIDArray[Index];
			if (this->m_pMaxAvailableIDArray[Index] >= (Type + San::MAXAVAILABLERESID))
			{
				this->m_pMaxAvailableIDArray[Index] = 0;
			}
			else
			{
				this->m_pMaxAvailableIDArray[Index] = this->m_pMaxAvailableIDArray[Index] + 1;
			}
		}
	}
	this->m_pResourcesTableArray[Index].insert(::make_pair(pRes->description.ResID, *pRes));
	return pRes->description.ResID;
}
bool cSanResourceManager::_ReleaseID(SVALUE ResID, const eSANRESOURCETYPE Type, const uint32 CreaterID, const uint32 GroupID, SANRES_RELEASEFUNC pReleaseFunc)
{
	uint32 Index = this->_GetResourceArrayIndexByType(Type);
	SANRESOURCETABLE::iterator pRes = this->m_pResourcesTableArray[Index].find(ResID);
	if (pRes == this->m_pResourcesTableArray[Index].end())
	{
		return false;
	}
	if (pRes->second.description.CreaterID == 0)
	{
		if ((pRes->second.description.AccessFlag&SRIO_GLOBAL_DELETE) != SRIO_GLOBAL_DELETE)
		{
			return false;
		}
	}
	else
	{
		if (pRes->second.description.CreaterID != CreaterID)
		{
			if (pRes->second.description.GroupID == 0)
			{
				if ((pRes->second.description.AccessFlag&SRIO_GLOBAL_DELETE) != SRIO_GLOBAL_DELETE)
				{
					return false;
				}
			}
			else
			{
				if (pRes->second.description.GroupID != GroupID)
				{
					if ((pRes->second.description.AccessFlag&SRIO_GLOBAL_DELETE) != SRIO_GLOBAL_DELETE)
					{
						return false;
					}
				}
				else
				{
					if ((pRes->second.description.AccessFlag&SRIO_GROUP_DELETE) != SRIO_GROUP_DELETE)
					{
						return false;
					}
				}
			}
		}
	}
	map < uint32, stSANUNKNOWNEX<SANRESOURCEDESC, SHANDLE>>::iterator pSubRes = this->m_pResourcesTableArray[Index].begin();
	while (pSubRes != this->m_pResourcesTableArray[Index].end())
	{
		if (pSubRes->second.description.OrigionResPtr == (&(pRes->second)))
		{
			pSubRes->second.description.OrigionResPtr = nullptr;
		}
		if (pSubRes->second.description.PreviousResPtr == (&(pRes->second)))
		{
			pSubRes->second.description.PreviousResPtr = nullptr;
		}
		pSubRes++;
	}
	/*Count == 0*/
	if (!pRes->second.description.AssignedPtr.empty())
	{
		list<lpSANRESPTR>::iterator pPtr = pRes->second.description.AssignedPtr.begin();
		while (pPtr != pRes->second.description.AssignedPtr.end())
		{
			(*pPtr)->res_ptr = nullptr;
			if ((*pPtr)->b_released)
			{
				delete (*pPtr);
				(*pPtr) = nullptr;
			}
			else
			{
				(*pPtr)->res_id = 0;
				(*pPtr)->res_ptr = nullptr;
				this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), *pPtr);
			}
			pPtr++;
		}
		pRes->second.description.AssignedPtr.clear();
		pRes->second.description.AssignedConstPtr.clear();
	}
	if (!pRes->second.description.AssignedConstPtr.empty())
	{
		list<lpSANRESPTRC>::iterator pPtrC = pRes->second.description.AssignedConstPtr.begin();
		while (pPtrC != pRes->second.description.AssignedConstPtr.end())
		{
			(*pPtrC)->res_ptr = nullptr;
			if ((*pPtrC)->b_released)
			{
				delete (*pPtrC);
				(*pPtrC) = nullptr;
			}
			else
			{
				(*pPtrC)->res_id = 0;
				(*pPtrC)->res_ptr = nullptr;
				this->m_AssignedVoidConstPtrList.insert(this->m_AssignedVoidConstPtrList.end(), *pPtrC);
			}
			pPtrC++;
		}
	}
	lpSANMESH pMesh = nullptr;
	SANOBJECTLIST::iterator pObj;
	switch (Type)
	{
	case RT_TEXTURE:
	case RT_TEXTURE_2D:
	case RT_TEXTURE_3D:
		this->iDeleteFormDeviceTextureList((lpSANTEXTURE) pRes->second.value);
		break;
	case RT_MESH:
	case RT_MESH_STATIC:
	case RT_MESH_DYNAMIC:
		pMesh = (lpSANMESH) pRes->second.value;
		pObj = pMesh->value.begin();
		while (pObj != pMesh->value.end())
		{
			this->iDeleteFromDeviceMeshList(&(*pObj));
		}
		break;
	case RT_SOUND:
		break;
	default:
		break;
	}
	if (pReleaseFunc == nullptr)
	{
		pReleaseFunc = pRes->second.description.pReleaseFunc;
	}
	if (pReleaseFunc != nullptr)
	{
		pReleaseFunc(pRes->second.value, this, NULL);
	}
	this->m_pResourcesTableArray[Index].erase(pRes);
	this->m_pAvailableIDListArray[Index].insert(this->m_pAvailableIDListArray[Index].end(),ResID);
}
eSANRESOURCETYPE cSanResourceManager::_GetResourceTypeByArrayIndex(uint32 Index)
{
	switch (Index)
	{
	case 0:return RT_UNKNOWN; break;
	case 1:return RT_TEXTURE; break;
	case 2:return RT_TEXTURE_2D; break;
	case 3:return RT_TEXTURE_3D; break;
	case 4:return RT_MESH; break;
	case 5:return RT_MESH_STATIC; break;
	case 6:return RT_MESH_DYNAMIC; break;
	case 7:return RT_SOUND; break;
	case 8:return RT_BUFFER; break;
#ifndef _OPENGL
	case 9: return RT_BINARY; break;
	case 10:return RT_BINARY_EXEC; break;
	case 11:return RT_BINARY_DLL; break;
	case 12:return RT_BINARY_SHADER; break;
	case 13:return RT_DEVICE; break;
	case 14:return RT_DEVICE_WINDOW; break;
	case 15:return RT_TEXT; break;
	case 16:return RT_TEXT_ASCII; break;
	case 17:return RT_TEXT_UNICODE; break;
	case 18:return RT_TEXT_SRCCODE; break;
#else
	case 9:return RT_BUFFER_TBO; break;
	case 10:return RT_BUFFER_FBO; break;
	case 11:return RT_BUFFER_VBO; break;
	case 12:return RT_BUFFER_VAO; break;
	case 13: return RT_BINARY; break;
	case 14:return RT_BINARY_EXEC; break;
	case 15:return RT_BINARY_DLL; break;
	case 16:return RT_BINARY_SHADER; break;
	case 17:return RT_DEVICE; break;
	case 18:return RT_DEVICE_WINDOW; break;
	case 19:return RT_TEXT; break;
	case 20:return RT_TEXT_ASCII; break;
	case 21:return RT_TEXT_UNICODE; break;
	case 22:return RT_TEXT_SRCCODE; break;
#endif
	default:return RT_UNKNOWN; break;
	}
}
uint32 cSanResourceManager::_GetResourceArrayIndexByType(eSANRESOURCETYPE Type)
{
	switch (Type)
	{
	case RT_UNKNOWN: return 0; break;
	case RT_TEXTURE: return 1; break;
	case RT_TEXTURE_2D: return 2; break;
	case RT_TEXTURE_3D: return 3; break;
	case RT_MESH: return 4; break;
	case RT_MESH_STATIC: return 5; break;
	case RT_MESH_DYNAMIC: return 6; break;
	case RT_SOUND: return 7; break;
	case RT_BUFFER: return 8; break;
#ifndef _OPENGL
	case RT_BINARY: return 9; break;
	case RT_BINARY_EXEC: return 10; break;
	case RT_BINARY_DLL: return 11; break;
	case RT_BINARY_SHADER: return 12; break;
	case RT_DEVICE: return 13; break;
	case RT_DEVICE_WINDOW: return 14; break;
	case RT_TEXT: return 15; break;
	case RT_TEXT_ASCII: return 16; break;
	case RT_TEXT_UNICODE: return 17; break;
	case RT_TEXT_SRCCODE: return 18; break;
#else
	case RT_BUFFER_TBO: return 9; break;
	case RT_BUFFER_FBO: return 10; break;
	case RT_BUFFER_VBO: return 11; break;
	case RT_BUFFER_VAO: return 12; break;
	case RT_BINARY: return 13; break;
	case RT_BINARY_EXEC: return 14; break;
	case RT_BINARY_DLL: return 15; break;
	case RT_BINARY_SHADER: return 16; break;
	case RT_DEVICE: return 17; break;
	case RT_DEVICE_WINDOW: return 18; break;
	case RT_TEXT: return 19; break;
	case RT_TEXT_ASCII: return 20; break;
	case RT_TEXT_UNICODE: return 21; break;
	case RT_TEXT_SRCCODE: return 22; break;
#endif
	default:RT_UNKNOWN : return 0; break;
	}
}
lpSANRESOURCE cSanResourceManager::_GetResourceHandle(SVALUE ResID, const eSANRESOURCETYPE Type)
{
	if (ResID == 0)
	{
		return nullptr;
	}
	uint32 Index = this->_GetResourceArrayIndexByType(Type);
	SANRESOURCETABLE::iterator pRes = this->m_pResourcesTableArray[Index].find(ResID);
	if (pRes == this->m_pResourcesTableArray[Index].end())
	{
		return nullptr;
	}
	return &(pRes->second);
}
bool cSanResourceManager::iRegistResourceCreaterFunc(const SString &strFileName, SANRES_CREATEFUNC pCreateFunc, eSANRESOURCETYPE Type)
{
	SString strFileType = strFileName.substr(strFileName.find(_SSTR(".")), strFileName.length());
	if (strFileType.empty())
	{
		return false;
	}
	Type = static_cast<eSANRESOURCETYPE>(Type&RT_TYPE_MASK);
	SANFILEMATCHTABLE::iterator pItem=this->m_FileMatchTable.find(Type);
	if(pItem==this->m_FileMatchTable.end())
	{
		SANFILEMATCHITEM FileMatchItem;
		FileMatchItem.ResourceType = Type;
		FileMatchItem.DefaultCreateFuncPtrMap.clear();
		FileMatchItem.pDefaultCopyFunc = nullptr;
		FileMatchItem.pDefaultReleaseFunc = nullptr;
		this->m_FileMatchTable.insert(::make_pair(Type,FileMatchItem));
		pItem=this->m_FileMatchTable.find(Type);
	}
	map<SString, list<SANRES_CREATEFUNC>>::iterator pFunc = pItem->second.DefaultCreateFuncPtrMap.find(strFileType);
	if (pFunc == pItem->second.DefaultCreateFuncPtrMap.end())
	{
		list <SANRES_CREATEFUNC> FuncList;
		FuncList.insert(FuncList.end(), pCreateFunc);
		pItem->second.DefaultCreateFuncPtrMap.insert(make_pair(strFileType, FuncList));
		return true;
	}
	list<SANRES_CREATEFUNC>::iterator pFuncPtr = pFunc->second.begin();
	while (pFuncPtr != pFunc->second.end())
	{
		if ((*pFuncPtr) == pCreateFunc)
		{
			return false;
		}
		pFuncPtr++;
	}
	pFunc->second.insert(pFunc->second.begin(), pCreateFunc);
	return true;
}
bool cSanResourceManager::iReleaseResourceCreaterFunc(const SString &strFileName, SANRES_CREATEFUNC pCreateFunc, eSANRESOURCETYPE Type)
{
	SString strFileType = strFileName.substr(strFileName.find(_SSTR(".")), strFileName.length());
	if (strFileType.empty())
	{
		return false;
	}
	Type = static_cast<eSANRESOURCETYPE>(Type&RT_TYPE_MASK);
	SANFILEMATCHTABLE::iterator pItem = this->m_FileMatchTable.find(Type);
	if (pItem == this->m_FileMatchTable.end())
	{
		return false;
	}
	map<SString, list<SANRES_CREATEFUNC>>::iterator pFunc = pItem->second.DefaultCreateFuncPtrMap.find(strFileType);
	if (pFunc == pItem->second.DefaultCreateFuncPtrMap.end())
	{
		return false;
	}
	list<SANRES_CREATEFUNC>::iterator pFuncPtr = pFunc->second.begin();
	while (pFuncPtr != pFunc->second.end())
	{
		if ((*pFuncPtr) == pCreateFunc)
		{
			pFunc->second.erase(pFuncPtr);
			return true;
		}
		pFuncPtr++;
	}
	return false;
}
bool cSanResourceManager::iRegistResourceCopyFunc(SANRES_COPYFUNC pCopyFunc, eSANRESOURCETYPE Type)
{
	Type = static_cast<eSANRESOURCETYPE>(Type&RT_TYPE_MASK);
	SANFILEMATCHTABLE::iterator pItem = this->m_FileMatchTable.find(Type);
	if (pItem == this->m_FileMatchTable.end())
	{
		SANFILEMATCHITEM FileMatchItem;
		FileMatchItem.ResourceType = Type;
		FileMatchItem.DefaultCreateFuncPtrMap.clear();
		FileMatchItem.pDefaultCopyFunc = pCopyFunc;
		FileMatchItem.pDefaultReleaseFunc = nullptr;
		this->m_FileMatchTable.insert(::make_pair(Type, FileMatchItem));
		return true;
	}
	pItem->second.pDefaultCopyFunc = pCopyFunc;
	return true;
}
bool cSanResourceManager::iReleaseResourceCopyFunc(SANRES_COPYFUNC pCopyFunc, eSANRESOURCETYPE Type)
{
	Type = static_cast<eSANRESOURCETYPE>(Type&RT_TYPE_MASK);
	SANFILEMATCHTABLE::iterator pItem = this->m_FileMatchTable.find(Type);
	if (pItem == this->m_FileMatchTable.end())
	{
		return false;
	}
	pItem->second.pDefaultCopyFunc = nullptr;
}
bool cSanResourceManager::iRegistResourceReleaseFunc(SANRES_RELEASEFUNC pReleaseFunc, eSANRESOURCETYPE Type)
{
	Type = static_cast<eSANRESOURCETYPE>(Type&RT_TYPE_MASK);
	SANFILEMATCHTABLE::iterator pItem = this->m_FileMatchTable.find(Type);
	if (pItem == this->m_FileMatchTable.end())
	{
		SANFILEMATCHITEM FileMatchItem;
		FileMatchItem.ResourceType = Type;
		FileMatchItem.DefaultCreateFuncPtrMap.clear();
		FileMatchItem.pDefaultCopyFunc = nullptr;
		FileMatchItem.pDefaultReleaseFunc = pReleaseFunc;
		this->m_FileMatchTable.insert(::make_pair(Type, FileMatchItem));
		return true;
	}
	pItem->second.pDefaultReleaseFunc = pReleaseFunc;
	return true;
}
bool cSanResourceManager::iReleaseResourceReleaseFunc(SANRES_RELEASEFUNC pReleaseFunc, eSANRESOURCETYPE Type)
{
	Type = static_cast<eSANRESOURCETYPE>(Type&RT_TYPE_MASK);
	SANFILEMATCHTABLE::iterator pItem = this->m_FileMatchTable.find(Type);
	if (pItem == this->m_FileMatchTable.end())
	{
		return false;
	}
	pItem->second.pDefaultReleaseFunc = nullptr;
}
bool cSanResourceManager::iRegistResourceDefaultPath(const SString &strDefaultPath)
{
	if(strDefaultPath.empty())
	{
		return false;
	}
	list<SString>::iterator pSeek=this->m_DefaultPathList.begin();
	while(pSeek!=this->m_DefaultPathList.end())
	{
		if((*pSeek)==strDefaultPath)
		{
			return false;
		}
		pSeek++;
	}
	this->m_DefaultPathList.insert(this->m_DefaultPathList.end(),strDefaultPath);
	return true;
}
void cSanResourceManager::iReleaseResourceDefaultPath(const SString &strDefaultPath)
{
	if(strDefaultPath.empty())
	{
		return;
	}
	list<SString>::iterator pSeek=this->m_DefaultPathList.begin();
	while(pSeek!=this->m_DefaultPathList.end())
	{
		if((*pSeek)==strDefaultPath)
		{
			this->m_DefaultPathList.erase(pSeek);
			return;
		}
		pSeek++;
	}
}
bool cSanResourceManager::iIsIDInUse(const SVALUE ResID)
{
	uint32 Index = this->_GetResourceArrayIndexByType(this->iGetResourceType(ResID));
	if(this->m_pMaxAvailableIDArray[Index]!=0)
	{
		if (ResID >= this->m_pMaxAvailableIDArray[Index])
		{
			return false;
		}
	}
	else
	{
		if(this->m_pAvailableIDListArray[Index].empty())
		{
			return true;
		}
	}
	list<uint32>::iterator pID=this->m_pAvailableIDListArray[Index].begin();
	while(pID!=this->m_pAvailableIDListArray[Index].end())
	{
		if ((*pID) == ResID)
		{
			return true;
		}
		pID++;
	}
	return false;
}
SVALUE cSanResourceManager::iGetAvailableID(eSANRESOURCETYPE ResType)
{
	SVALUE ID = 0;
	uint32 Index = this->_GetResourceArrayIndexByType(ResType);
	if(this->m_pAvailableIDListArray[Index].empty())
	{
		ID = this->m_pMaxAvailableIDArray[Index];
		if (this->m_pMaxAvailableIDArray[Index] == 0)
		{
			return 0;
		}
		this->m_pMaxAvailableIDArray[Index] = this->m_pMaxAvailableIDArray[Index] + 1;
		return ID;
	}
	ID=*(this->m_pAvailableIDListArray[Index].begin());
	this->m_pAvailableIDListArray[Index].erase(this->m_pAvailableIDListArray[Index].begin());
	return ID;
}
SANRES_CREATEFUNC cSanResourceManager::iGetCreateFunc(const SString &strFileName, eSANRESOURCETYPE Type)
{
	SString strFileType = strFileName.substr(strFileName.find(_SSTR(".")), strFileType.length());
	if (strFileType.empty())
	{
		return false;
	}
	Type = static_cast<eSANRESOURCETYPE>(Type&RT_TYPE_MASK);
	SANFILEMATCHTABLE::iterator pItem = this->m_FileMatchTable.find(Type);
	if (pItem == this->m_FileMatchTable.end())
	{
		return nullptr;
	}
	map<SString, list<SANRES_CREATEFUNC>>::iterator pFunc = pItem->second.DefaultCreateFuncPtrMap.find(strFileType);
	if (pFunc == pItem->second.DefaultCreateFuncPtrMap.end())
	{
		return nullptr;
	}
	return *pFunc->second.begin();
}
SANRES_COPYFUNC cSanResourceManager::iGetCopyFunc(eSANRESOURCETYPE Type)
{
	SANFILEMATCHTABLE::iterator pItem = this->m_FileMatchTable.find(static_cast<eSANRESOURCETYPE>(Type&RT_TYPE_MASK));
	if (pItem == this->m_FileMatchTable.end())
	{
		return false;
	}
	return pItem->second.pDefaultCopyFunc;
}
SANRES_RELEASEFUNC cSanResourceManager::iGetReleaseFunc(eSANRESOURCETYPE Type)
{
	SANFILEMATCHTABLE::iterator pItem = this->m_FileMatchTable.find(static_cast<eSANRESOURCETYPE>(Type&RT_TYPE_MASK));
	if (pItem == this->m_FileMatchTable.end())
	{
		return false;
	}
	return pItem->second.pDefaultReleaseFunc;
}
uint32 cSanResourceManager::iGetResourceIDByPath(const SString &strFilePath)
{
	map<SString,uint32>::iterator pItem=this->m_FilePathSearchTableForward.find(strFilePath);
	if(pItem==this->m_FilePathSearchTableForward.end())
	{
		return 0;
	}
	return pItem->second;
}
SString cSanResourceManager::iGetResourcePathByID(const SVALUE ResID)
{
	map<uint32, SString>::iterator pItem = this->m_FilepathSearchTableBackward.find(ResID);
	if(pItem==this->m_FilepathSearchTableBackward.end())
	{
		return 0;
	}
	return pItem->second;
}
eSANRESOURCETYPE cSanResourceManager::iGetResourceType(const SVALUE ResID)
{
	if (ResID == 0)
	{
		return ::RT_UNKNOWN;
	}
	return (eSANRESOURCETYPE) (ResID&::RT_TYPE_MASK);
}
bool cSanResourceManager::iFindResource(const SVALUE ResID)
{
	if(ResID==0)
	{
		return false;
	}
	uint32 Index = this->_GetResourceArrayIndexByType(this->iGetResourceType(ResID));
	SANRESOURCETABLE::iterator pRes=this->m_pResourcesTableArray[Index].find(ResID);
	if(pRes!=this->m_pResourcesTableArray[Index].end())
	{
		return true;
	}
	return false;
}
bool cSanResourceManager::iFindTexture(const SVALUE TexID)
{
	return this->iFindResource(TexID);
}
bool cSanResourceManager::iFindMesh(const SVALUE MeshID)
{
	return this->iFindResource(MeshID);
}
lpSANRESPTRC cSanResourceManager::iGetResourceConstPtr(const SVALUE ResID, const uint32 RequireID, const uint32 RequireGroupID)
{
	eSANRESOURCETYPE ResType = this->iGetResourceType(ResID);
	lpSANRESPTRC pPtr = new SANRESPTRC;
	if (ResID == 0)
	{
		this->m_AssignedVoidConstPtrList.insert(this->m_AssignedVoidConstPtrList.end(), pPtr);
		return pPtr;
	}
	uint32 Index = this->_GetResourceArrayIndexByType(ResType);
	SANRESOURCETABLE::iterator pRes = this->m_pResourcesTableArray[Index].find(ResID);
	if (pRes != this->m_pResourcesTableArray[Index].end())
	{
		if ((pRes->second.description.AccessFlag&SRIO_MANAGER_REFERENCE) == SRIO_MANAGER_REFERENCE)
		{
			this->m_AssignedVoidConstPtrList.insert(this->m_AssignedVoidConstPtrList.end(), pPtr);
			return pPtr;
		}
		if (pRes->second.description.CreaterID == 0)
		{
			if ((pRes->second.description.AccessFlag&SRIO_GLOBAL_READ) != SRIO_GLOBAL_READ)
			{
				this->m_AssignedVoidConstPtrList.insert(this->m_AssignedVoidConstPtrList.end(), pPtr);
				return pPtr;
			}
		}
		else
		{
			if (pRes->second.description.CreaterID != RequireID)
			{
				if (pRes->second.description.GroupID == 0)
				{
					if ((pRes->second.description.AccessFlag&SRIO_GLOBAL_READ) != SRIO_GLOBAL_READ)
					{
						this->m_AssignedVoidConstPtrList.insert(this->m_AssignedVoidConstPtrList.end(), pPtr);
						return pPtr;
					}
				}
				else
				{
					if (pRes->second.description.GroupID != RequireID)
					{
						if ((pRes->second.description.AccessFlag&SRIO_GLOBAL_READ) != SRIO_GLOBAL_READ)
						{
							this->m_AssignedVoidConstPtrList.insert(this->m_AssignedVoidConstPtrList.end(), pPtr);
							return pPtr;
						}
					}
					else
					{
						if ((pRes->second.description.AccessFlag&SRIO_GROUP_READ) != SRIO_GROUP_READ)
						{
							this->m_AssignedVoidConstPtrList.insert(this->m_AssignedVoidConstPtrList.end(), pPtr);
							return pPtr;
						}
					}
				}
			}
			else
			{
				if ((pRes->second.description.AccessFlag&SRIO_CREATER_READ) != SRIO_CREATER_READ)
				{
					this->m_AssignedVoidConstPtrList.insert(this->m_AssignedVoidConstPtrList.end(), pPtr);
					return pPtr;
				}
			}
		}
		pPtr->res_id = ResID;
		pPtr->type = ResType;
		pPtr->res_ptr = pRes->second.value;
		pRes->second.description.AssignedConstPtr.insert(pRes->second.description.AssignedConstPtr.end(), pPtr);
		return pPtr;
	}
	this->m_AssignedVoidConstPtrList.insert(this->m_AssignedVoidConstPtrList.end(), pPtr);
	return pPtr;
}
lpSANRESPTRC cSanResourceManager::iGetTextureConstPtr(const SVALUE TexID, const uint32 RequireID, const uint32 RequireGroupID)
{
	return this->iGetResourceConstPtr(TexID, RequireID, RequireGroupID);
}
lpSANRESPTRC cSanResourceManager::iGetMeshConstPtr(const SVALUE MeshID, const uint32 RequireID, const uint32 RequireGroupID)
{
	return this->iGetResourceConstPtr(MeshID, RequireID, RequireGroupID);
}
lpSANRESPTRC cSanResourceManager::iCopyResourceConstPtr(const lpSANRESPTRC Ptr, const uint32 RequireID, const uint32 RequireGroupID)
{
	eSANRESOURCETYPE ResType = this->iGetResourceType(Ptr->res_id);
	lpSANRESPTRC pDestPtr = new SANRESPTRC;
	if (Ptr == nullptr)
	{
		this->m_AssignedVoidConstPtrList.insert(this->m_AssignedVoidConstPtrList.end(), pDestPtr);
		return pDestPtr;
	}
	if (Ptr->b_released)
	{
		this->m_AssignedVoidConstPtrList.insert(this->m_AssignedVoidConstPtrList.end(), pDestPtr);
		return pDestPtr;
	}
	uint32 Index = this->_GetResourceArrayIndexByType(ResType);
	SANRESOURCETABLE::iterator pRes = this->m_pResourcesTableArray[Index].find(Ptr->res_id);
	if (pRes != this->m_pResourcesTableArray[Index].end())
	{
		if (pRes->second.description.CreaterID == 0)
		{
			if ((pRes->second.description.AccessFlag&SRIO_GLOBAL_COPY) != SRIO_GLOBAL_COPY)
			{
				this->m_AssignedVoidConstPtrList.insert(this->m_AssignedVoidConstPtrList.end(), pDestPtr);
				return pDestPtr;
			}
		}
		else
		{
			if (pRes->second.description.CreaterID != RequireID)
			{
				if (pRes->second.description.GroupID == 0)
				{
					if ((pRes->second.description.AccessFlag&SRIO_GLOBAL_COPY) != SRIO_GLOBAL_COPY)
					{
						this->m_AssignedVoidConstPtrList.insert(this->m_AssignedVoidConstPtrList.end(), pDestPtr);
						return pDestPtr;
					}
				}
				else
				{
					if (pRes->second.description.GroupID != RequireID)
					{
						if ((pRes->second.description.AccessFlag&SRIO_GLOBAL_COPY) != SRIO_GLOBAL_COPY)
						{
							this->m_AssignedVoidConstPtrList.insert(this->m_AssignedVoidConstPtrList.end(), pDestPtr);
							return pDestPtr;
						}
					}
					else
					{
						if ((pRes->second.description.AccessFlag&SRIO_GROUP_COPY) != SRIO_GROUP_COPY)
						{
							this->m_AssignedVoidConstPtrList.insert(this->m_AssignedVoidConstPtrList.end(), pDestPtr);
							return pDestPtr;
						}
					}
				}
			}
			else
			{
				if ((pRes->second.description.AccessFlag&SRIO_CREATER_COPY) != SRIO_CREATER_COPY)
				{
					this->m_AssignedVoidConstPtrList.insert(this->m_AssignedVoidConstPtrList.end(), pDestPtr);
					return pDestPtr;
				}
			}
		}
		pDestPtr->res_id = Ptr->res_id;
		pDestPtr->type = Ptr->type;
		pDestPtr->res_ptr = pRes->second.value;
		pRes->second.description.ReferenceCount = pRes->second.description.ReferenceCount < 0 ? 1 : (pRes->second.description.ReferenceCount + 1);
		pRes->second.description.AssignedConstPtr.insert(pRes->second.description.AssignedConstPtr.end(), pDestPtr);
		return pDestPtr;
	}
	this->m_AssignedVoidConstPtrList.insert(this->m_AssignedVoidConstPtrList.end(), pDestPtr);
	return pDestPtr;
}
lpSANRESPTRC cSanResourceManager::iCopyTextureConstPtr(const lpSANRESPTRC Ptr, const uint32 RequireID, const uint32 RequireGroupID)
{
	return this->iCopyResourceConstPtr(Ptr, RequireID, RequireGroupID);
}
lpSANRESPTRC cSanResourceManager::iCopyMeshConstPtr(const lpSANRESPTRC Ptr, const uint32 RequireID, const uint32 RequireGroupID)
{
	return this->iCopyResourceConstPtr(Ptr, RequireID, RequireGroupID);
}
lpSANRESPTR cSanResourceManager::iGetResourcePtr(const uint32 ResID, const uint32 RequireID, const uint32 RequireGroupID)
{
	eSANRESOURCETYPE ResType = this->iGetResourceType(ResID);
	lpSANRESPTR pPtr = new SANRESPTR;
	if (ResID == 0)
	{
		this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), pPtr);
		return pPtr;
	}
	uint32 Index = this->_GetResourceArrayIndexByType(ResType);
	SANRESOURCETABLE::iterator pRes=this->m_pResourcesTableArray[Index].find(ResID);
	if(pRes!=this->m_pResourcesTableArray[Index].end())
	{
		if ((pRes->second.description.AccessFlag&SRIO_MANAGER_REFERENCE) == SRIO_MANAGER_REFERENCE)
		{
			this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), pPtr);
			return pPtr;
		}
		if ((pRes->second.description.AccessFlag&SRIO_MANAGER_CONST) == SRIO_MANAGER_CONST)
		{
			this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), pPtr);
			return pPtr;
		}
		if (pRes->second.description.CreaterID == 0)
		{
			if ((pRes->second.description.AccessFlag&SRIO_GLOBAL_READ) != SRIO_GLOBAL_READ)
			{
				this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), pPtr);
				return pPtr;
			}
		}
		else
		{
			if (pRes->second.description.CreaterID != RequireID)
			{
				if (pRes->second.description.GroupID == 0)
				{
					if ((pRes->second.description.AccessFlag&SRIO_GLOBAL_READ) != SRIO_GLOBAL_READ)
					{
						this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), pPtr);
						return pPtr;
					}
				}
				else
				{
					if (pRes->second.description.GroupID != RequireID)
					{
						if ((pRes->second.description.AccessFlag&SRIO_GLOBAL_READ) != SRIO_GLOBAL_READ)
						{
							this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), pPtr);
							return pPtr;
						}
					}
					else
					{
						if ((pRes->second.description.AccessFlag&SRIO_GROUP_READ) != SRIO_GROUP_READ)
						{
							this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), pPtr);
							return pPtr;
						}
					}
				}
			}
			else
			{
				if ((pRes->second.description.AccessFlag&SRIO_CREATER_READ) != SRIO_CREATER_READ)
				{
					this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), pPtr);
					return pPtr;
				}
			}
		}
		pPtr->res_id = ResID;
		pPtr->type = ResType;
		pPtr->res_ptr = pRes->second.value;
		pRes->second.description.ReferenceCount = pRes->second.description.ReferenceCount < 0 ? 1 : (pRes->second.description.ReferenceCount + 1);
		pRes->second.description.AssignedPtr.insert(pRes->second.description.AssignedPtr.end(), pPtr);
		return pPtr;
	}
	this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), pPtr);
	return pPtr;
}
lpSANRESPTR cSanResourceManager::iGetTexturePtr(const uint32 TexID, const uint32 RequireID, const uint32 RequireGroupID)
{
	return this->iGetResourcePtr(TexID, RequireID, RequireGroupID);
}
lpSANRESPTR cSanResourceManager::iGetMeshPtr(const uint32 MeshID, const uint32 RequireID, const uint32 RequireGroupID)
{
	return this->iGetResourcePtr(MeshID, RequireID, RequireGroupID);
}
lpSANRESPTR cSanResourceManager::iCopyResourcePtr(const lpSANRESPTR Ptr, const uint32 RequireID, const uint32 RequireGroupID)
{
	eSANRESOURCETYPE ResType = this->iGetResourceType(Ptr->res_id);
	lpSANRESPTR pDestPtr = new SANRESPTR;
	if (Ptr == nullptr)
	{
		this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), pDestPtr);
		return pDestPtr;
	}
	if (Ptr->b_released)
	{
		this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), pDestPtr);
		return pDestPtr;
	}
	uint32 Index = this->_GetResourceArrayIndexByType(ResType);
	SANRESOURCETABLE::iterator pRes = this->m_pResourcesTableArray[Index].find(Ptr->res_id);
	if (pRes != this->m_pResourcesTableArray[Index].end())
	{
		if ((pRes->second.description.AccessFlag&SRIO_MANAGER_CONST) == SRIO_MANAGER_CONST)
		{
			this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), pDestPtr);
			return pDestPtr;
		}
		if (pRes->second.description.CreaterID == 0)
		{
			if ((pRes->second.description.AccessFlag&SRIO_GLOBAL_COPY) != SRIO_GLOBAL_COPY)
			{
				this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), pDestPtr);
				return pDestPtr;
			}
		}
		else
		{
			if (pRes->second.description.CreaterID != RequireID)
			{
				if (pRes->second.description.GroupID == 0)
				{
					if ((pRes->second.description.AccessFlag&SRIO_GLOBAL_COPY) != SRIO_GLOBAL_COPY)
					{
						this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), pDestPtr);
						return pDestPtr;
					}
				}
				else
				{
					if (pRes->second.description.GroupID != RequireID)
					{
						if ((pRes->second.description.AccessFlag&SRIO_GLOBAL_COPY) != SRIO_GLOBAL_COPY)
						{
							this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), pDestPtr);
							return pDestPtr;
						}
					}
					else
					{
						if ((pRes->second.description.AccessFlag&SRIO_GROUP_COPY) != SRIO_GROUP_COPY)
						{
							this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), pDestPtr);
							return pDestPtr;
						}
					}
				}
			}
			else
			{
				if ((pRes->second.description.AccessFlag&SRIO_CREATER_COPY) != SRIO_CREATER_COPY)
				{
					this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), pDestPtr);
					return pDestPtr;
				}
			}
		}
		pDestPtr->res_id = Ptr->res_id;
		pDestPtr->type = Ptr->type;
		pDestPtr->res_ptr = pRes->second.value;
		pRes->second.description.ReferenceCount = pRes->second.description.ReferenceCount < 0 ? 1 : (pRes->second.description.ReferenceCount + 1);
		pRes->second.description.AssignedPtr.insert(pRes->second.description.AssignedPtr.end(), pDestPtr);
		return pDestPtr;
	}
	this->m_AssignedVoidPtrList.insert(this->m_AssignedVoidPtrList.end(), pDestPtr);
	return pDestPtr;
}
lpSANRESPTR cSanResourceManager::iCopyTexturePtr(const lpSANRESPTR Ptr, const uint32 RequireID, const uint32 RequireGroupID)
{
	return this->iCopyResourcePtr(Ptr, RequireID, RequireGroupID);
}
lpSANRESPTR cSanResourceManager::iCopyMeshPtr(const lpSANRESPTR Ptr, const uint32 RequireID, const uint32 RequireGroupID)
{
	return this->iCopyResourcePtr(Ptr, RequireID, RequireGroupID);
}
bool cSanResourceManager::iRegistGraphicsDeviceResourceManagerHandle(SHANDLE Handle)
{
	this->m_GraphicsDeviceResourceManagerHandle=Handle;
	return true;
}
SHANDLE cSanResourceManager::iGetDeviceResourceManagerHandle()
{
	return this->m_GraphicsDeviceResourceManagerHandle;
}
SHANDLE cSanResourceManager::iAddToDeviceTextureList(lpSANTEXTURE pTexture)
{
#ifdef _DIRECTX
	cVectorManager *pManager=new cVectorManager();
	LPDIRECT3DDEVICE9 pDXDevice=(LPDIRECT3DDEVICE9)(pManager->iFound(_SSTR("Sys_DXDevice")));
	if(pDXDevice==nullptr)
	{
		delete pManager;
		return  nullptr;
	}
	delete pManager;
	LPDIRECT3DTEXTURE9 pDXTexture;
	D3DFORMAT TextureFormat;
	if(pTexture->PixFormat==PT_RGB)
	{
		TextureFormat=D3DFMT_R8G8B8;
	}
	else
	{
		TextureFormat=D3DFMT_A8R8G8B8;
	}
	if(::D3DXCreateTextureFromFileW(pDXDevice,pTexture->strFile.c_str(),&pDXTexture)==D3D_OK)
	{
		if(pDXTexture!=nullptr)
		{
			return (uint32)pDXTexture;
		}
	}
	return nullptr;
#endif
#ifdef _OPENGL
	cSanGLBufferManager* pBufferManager=(cSanGLBufferManager*)this->m_GraphicsDeviceResourceManagerHandle;
	if(pBufferManager==nullptr)
	{
		return nullptr;
	}
	SANTBODESC TboDesc;
	if(pTexture->ParamList!=nullptr)
	{
		if(pTexture->DataSize==1)
		{
			pBufferManager->iReleaseTBOObject(pTexture->ParamList[0]);
			delete pTexture->ParamList;
		}
		else
		{
			for(uint32 seek=0;seek<pTexture->ParamListSize;seek=seek+1)
			{
				pBufferManager->iReleaseTBOObject(pTexture->ParamList[seek]);
			}
			delete[] pTexture->ParamList;
		}
		pTexture->CurrentParam=0;
		pTexture->ParamList=nullptr;
		pTexture->ParamListSize=0;
	}
	uint32 FrameSize=0;
	switch(pTexture->Type)
	{
	case STT_TEX1D:
		break;
	case STT_TEX2D:
		pTexture->ParamList=new SVALUE;
		pTexture->ParamListSize=1;
		switch(pTexture->PixFormat)
		{
		case PT_RGB:
			TboDesc.Type=SGTT_RGBA_8;//////////////////////////////////////
			break;
		case PT_RGBA:
			TboDesc.Type=SGTT_RGBA_8;
			break;
		case PT_RGB_F:
			TboDesc.Type=SGTT_RGB_F;
			break;
		case PT_RGBA_F:
			TboDesc.Type=SGTT_RGBA_F;
			break;
		default:
			TboDesc.Type=SGTT_RGB_8;
			break;
		}
		TboDesc.Width=lpSANPIXMAP2D(pTexture->PixMapPtr)->width;
		TboDesc.Height = lpSANPIXMAP2D(pTexture->PixMapPtr)->height;
		TboDesc.BufferSize = pTexture->DataSize;
		TboDesc.pBuffer = (SHANDLE) lpSANPIXMAP2D(pTexture->PixMapPtr)->pPixMap;
		pTexture->ParamList[0] = pBufferManager->iCreateTBOObject(TboDesc);
		if(pTexture->ParamList[0]!=0)
		{
			pTexture->CurrentParam = pTexture->ParamList[0];
			return (SHANDLE) (pTexture->ParamList[0]);
		}
		return false;
		break;
	case STT_TEX3D:
		pTexture->ParamListSize = lpSANPIXMAP3D(pTexture->PixMapPtr)->depth;
		pTexture->ParamList = new SVALUE[pTexture->DataSize];
		FrameSize = pTexture->DataSize / pTexture->ParamListSize;
		switch(pTexture->PixFormat)
		{
		case PT_RGB:
			TboDesc.Type=SGTT_RGBA_8;//////////////////////////////////////
			break;
		case PT_RGBA:
			TboDesc.Type=SGTT_RGBA_8;
			break;
		case PT_RGB_F:
			TboDesc.Type=SGTT_RGB_F;
			break;
		case PT_RGBA_F:
			TboDesc.Type=SGTT_RGBA_F;
			break;
		default:
			TboDesc.Type=SGTT_RGB_8;
			break;
		}
		TboDesc.Width = lpSANPIXMAP2D(pTexture->PixMapPtr)->width;
		TboDesc.Height = lpSANPIXMAP2D(pTexture->PixMapPtr)->height;
		TboDesc.BufferSize = pTexture->DataSize;
		for(uint32 seek=0;seek<pTexture->DataSize;seek=seek+1)
		{
			TboDesc.pBuffer = (SHANDLE) ((uint32) lpSANPIXMAP2D(pTexture->PixMapPtr)->pPixMap + FrameSize*seek);
			pTexture->ParamList[seek] = pBufferManager->iCreateTBOObject(&(*pTexture));
			if(pTexture->ParamList[seek]==0)
			{
				return nullptr;
			}
		}
		pTexture->CurrentParam=pTexture->ParamList[0];
		return (SHANDLE)(pTexture->ParamList[0]);
		break;
	default:
		return nullptr;
		break;
	}
	return nullptr;
#endif
}
SHANDLE cSanResourceManager::iAddToDeviceMeshList(lpSANOBJECT pObject)
{
#ifdef _DIRECTX
	cVectorManager *pManager=new cVectorManager();
	LPDIRECT3DDEVICE9 pDXDevice=(LPDIRECT3DDEVICE9)(pManager->iFound(_SSTR("Sys_DXDevice")));
	if(pDXDevice==nullptr)
	{
		delete pManager;
		return 0;
	}
	delete pManager;
	//uint32 VertexListSize=Object.VertexList.size()*8;
	//sfloat *pVertexList=new sfloat[VertexListSize];
	uint32 ElementSize=6;
	uint32 TextureType=D3DFVF_TEX0;
	TextureType=TextureType+Object.Material.TextureList.size()*D3DFVF_TEX1;
	ElementSize=ElementSize+Object.Material.TextureList.size()*2;
	uint32 VertexListSize=Object.VertexListSize*ElementSize;
	sfloat *pVertexList=new sfloat[VertexListSize];
	::gloMemSet(pVertexList,0,sizeof(pVertexList));
	uint32 Seek=0;
	//stVERTEXLIST::iterator pVertex=Object.VertexList.begin();
	stVERTEXELEMENT *pVertex=Object.pVertexList;
	while(Seek<VertexListSize/*pVertex!=Object.VertexList.end()*/)
	{
		pVertexList[Seek]=pVertex->v.x;
		pVertexList[Seek+1]=pVertex->v.y;
		pVertexList[Seek+2]=pVertex->v.z;
		Seek=Seek+3;
		pVertexList[Seek]=-pVertex->vn.x;
		pVertexList[Seek+1]=pVertex->vn.y;
		pVertexList[Seek+2]=-pVertex->vn.z;
		Seek=Seek+3;
		for(int seek=0;seek<Object.Material.TextureList.size();seek=seek+1)
		{
			pVertexList[Seek]=pVertex->vt.x;
			pVertexList[Seek+1]=-pVertex->vt.y;
			Seek=Seek+2;
		}
		pVertex++;
	}
	LPDIRECT3DVERTEXBUFFER9 pDXVertexList;
	if(pDXDevice->CreateVertexBuffer(sizeof(sfloat)*VertexListSize,0,D3DFVF_XYZ|D3DFVF_NORMAL|TextureType/*D3DFVF_TEX1*/,D3DPOOL_DEFAULT,&pDXVertexList,nullptr)==D3D_OK)
	{
		if(pDXVertexList!=nullptr)
		{
			unsigned char *pDXVertexPtr;
			if(FAILED(pDXVertexList->Lock(0,sizeof(sfloat)*VertexListSize,(void**)(&pDXVertexPtr),0)))
			{
				delete[] pVertexList;
				pVertexList=nullptr;
				pDXVertexList->Release();
				return 0;
			}
			::gloMemCopy(pDXVertexPtr,pVertexList,sizeof(sfloat)*VertexListSize);
			pDXVertexList->Unlock();
			delete[] pVertexList;
			pVertexList=nullptr;
			return (uint32)pDXVertexList;
		}
		else
		{
			pDXVertexList->Release();
			delete[] pVertexList;
			pVertexList=nullptr;
			return 0;
		}
	}
	delete[] pVertexList;
	return 0;
#else
	cSanGLBufferManager* pBufferManager=(cSanGLBufferManager*)this->m_GraphicsDeviceResourceManagerHandle;
	if(pBufferManager==nullptr)
	{
		return 0;
	}
	switch(pObject->ObjStructVersion)
	{
	case ::SOSV_TRADITIONAL:
		return (SHANDLE)pBufferManager->iCreateVAOObject(pObject,SVBS_STATIC);
		break;
	case ::SOSV_INDEXVERTEX:
		return nullptr;
		break;
	case ::SOSV_FRAMEANIMATION:
		return (SHANDLE)pBufferManager->iCreateVAOObject(pObject,SVBS_STATIC);
		break;
	case ::SOSV_SKELETONANIMATION:
		return (SHANDLE)pBufferManager->iCreateVAOObject(pObject,SVBS_DYNAMIC);
		return nullptr;
		break;
	default:
		return nullptr;
		break;
	}
#endif
}
bool cSanResourceManager::iDeleteFormDeviceTextureList(lpSANTEXTURE pTexture)
{
#ifdef _DIRECTX
	if(Param==0)
	{
		return false;
	}
	LPDIRECT3DTEXTURE9 pDXTexture=(LPDIRECT3DTEXTURE9)Param;
	pDXTexture->Release();
	pDXTexture=nullptr;
	return true;
#endif
#ifdef _OPENGL
	cSanGLBufferManager* pBufferManager=(cSanGLBufferManager*)this->m_GraphicsDeviceResourceManagerHandle;
	if(pBufferManager==nullptr)
	{
		return false;
	}
	if(pTexture->ParamList!=nullptr)
	{
		if(pTexture->DataSize==1)
		{
			pBufferManager->iReleaseTBOObject(pTexture->ParamList[0]);
			delete pTexture->ParamList;
		}
		else
		{
			for(uint32 seek=0;seek<pTexture->ParamListSize;seek=seek+1)
			{
				pBufferManager->iReleaseTBOObject(pTexture->ParamList[seek]);
			}
			delete[] pTexture->ParamList;
		}
		pTexture->CurrentParam=0;
		pTexture->ParamList=nullptr;
		pTexture->ParamListSize=0;
	}
	return true;
#endif
}
bool cSanResourceManager::iDeleteFromDeviceMeshList(lpSANOBJECT pObject)
{
#ifdef _DIRECTX
	if(Param==0)
	{
		return false;
	}
	LPDIRECT3DVERTEXBUFFER9 pDXVertexList=(LPDIRECT3DVERTEXBUFFER9)Param;
	pDXVertexList->Release();
	pDXVertexList=nullptr;
	return true;
#else
	if(pObject->ObjParam!=nullptr)
	{
		cSanGLBufferManager* pBufferManager=(cSanGLBufferManager*)this->m_GraphicsDeviceResourceManagerHandle;
		if(pBufferManager==nullptr)
		{
			return false;
		}
		pBufferManager->iReleaseVAOObject((uint32)(pObject->ObjParam));
		pObject->ObjParam=nullptr;
	}
	return true;
#endif
}

SVALUE San::gloRegisteResource(SHANDLE ResHandle, const uint32 ResSize, const eSANRESOURCETYPE ResType, SVALUE CreaterID, SVALUE GroupID, uint32 Flag, SANRES_COPYFUNC pCopyFunc, SANRES_RELEASEFUNC pReleaseFunc)
{
	cSanResourceManager *pResManager = (cSanResourceManager*) San::gloFoundVector(_SSTR("Sys_SanResourceManager"));
	if (pResManager == nullptr)
	{
		return 0;
	}
	pResManager->iRegisteResource(ResHandle, ResSize, ResType, CreaterID, GroupID, Flag, pCopyFunc, pReleaseFunc);
}

SRESULT San::gloReleaseResource(SVALUE ResID, const SVALUE ID, SANRES_RELEASEFUNC pReleaseFunc)
{
	cSanResourceManager *pResManager = (cSanResourceManager*) San::gloFoundVector(_SSTR("Sys_SanResourceManager"));
	if (pResManager == nullptr)
	{
		return 0;
	}
	pResManager->iReleaseResource(ResID, ID, pReleaseFunc);
}

eSANRESOURCETYPE San::gloGetResourceType(const SVALUE ResID)
{
	cSanResourceManager *pResManager = (cSanResourceManager*) San::gloFoundVector(_SSTR("Sys_SanResourceManager"));
	if (pResManager == nullptr)
	{
		return RT_UNKNOWN;
	}
	return pResManager->iGetResourceType(ResID);
}