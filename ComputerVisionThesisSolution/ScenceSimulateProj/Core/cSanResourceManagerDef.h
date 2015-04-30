#include"SanTypes.h"
#include"SanContainer.h"
#include"../Graphics/SanGraphics.h"
#pragma once
using namespace std;
using namespace San::Graphics;
namespace San
{
#ifndef __CSANRESOURCEMANAGERDEF_H__
#define __CSANRESOURCEMANAGERDEF_H__

	enum eSANRESOURCETYPE			// | 4Bit Type| 4Bit Sub Type | 24Bit Resource ID| SubType Orginized by SubResourceManager = DeviceManager/GLBufferManager
	{
		RT_UNKNOWN				= 0x00000000,
		RT_TEXTURE				= 0x10000000,
		RT_TEXTURE_2D			= 0x11000000,
		RT_TEXTURE_2D_MAX		= 0x11ffffff,
		RT_TEXTURE_3D			= 0x12000000,
		RT_TEXTURE_3D_MAX		= 0x12ffffff,
		RT_MESH					= 0x20000000,
		RT_MESH_STATIC			= 0x21000000,
		RT_MESH_STATIC_MAX		= 0x21ffffff,
		RT_MESH_DYNAMIC			= 0x22000000,
		RT_MESH_DYNAMIC_MAX		= 0x22ffffff,
		RT_SOUND				= 0x30000000,
		RT_BUFFER				= 0x40000000,
#ifdef _OPENGL
		RT_BUFFER_TBO			= 0x41000000,
		RT_BUFFER_TBO_MAX		= 0x41ffffff,
		RT_BUFFER_FBO			= 0x42000000,
		RT_BUFFER_FBO_MAX		= 0x42ffffff,
		RT_BUFFER_VBO			= 0x43000000,
		RT_BUFFER_VBO_MAX		= 0x43ffffff,
		RT_BUFFER_VAO			= 0x44000000,
		RT_BUFFER_VAO_MAX		= 0x44ffffff,
#endif
		RT_BINARY				= 0x50000000,
		RT_BINARY_EXEC			= 0x51000000,
		RT_BINARY_EXEC_MAX		= 0x51ffffff,
		RT_BINARY_DLL			= 0x52000000,
		RT_BINARU_DLL_MAX		= 0x52ffffff,
		RT_BINARY_SHADER		= 0x53000000,
		RT_BINARY_SHADER_MAX	= 0x53ffffff,
		RT_DEVICE				= 0x60000000,
		RT_DEVICE_WINDOW		= 0x61000000,
		RT_DEVICE_WINDOW_MAX	= 0x61ffffff,
		RT_TEXT					= 0x70000000,
		RT_TEXT_ASCII			= 0x71000000,
		RT_TEXT_ASCII_MAX		= 0x71ffffff,
		RT_TEXT_UNICODE			= 0x72000000,
		RT_TEXT_UNICODE_MAX		= 0x72ffffff,
		RT_TEXT_SRCCODE			= 0x73000000,
		RT_TEXT_SRCCODE_MAX		= 0x73ffffff,
		RT_TYPE_MASK			= 0xff000000,
	};

#ifndef _OPENGL
	static const uint32 RESOURCETYPECOUNT = 19U;
#else
	static const uint32 RESOURCETYPECOUNT = 23U;
#endif

	//static const uint32 RESOURCETYOECOUNT = 8U;
	static const uint32 MAXAVAILABLERESID = 0x00ffffff;

	typedef SRESULT(*SANRES_CREATEFUNC)(SString, SVALUE, SHANDLE*);//Path, Param, BufferHandle return Size
	typedef SRESULT(*SANRES_COPYFUNC)(SHANDLE, SHANDLE*);//SrcBufferHandle, DestBufferHandle return Size
	typedef SRESULT(*SANRES_RELEASEFUNC)(SHANDLE, SHANDLE, SVALUE);//BufferHandle, ResourceManagerPtr, Param return bool

	struct stSANFILEMATCHITEM
	{
		eSANRESOURCETYPE ResourceType;
		map<SString, list<SANRES_CREATEFUNC>> DefaultCreateFuncPtrMap;
		SANRES_COPYFUNC pDefaultCopyFunc;
		SANRES_RELEASEFUNC pDefaultReleaseFunc;
	};
	typedef stSANFILEMATCHITEM	SANFILEMATCHITEM;
	typedef stSANFILEMATCHITEM*	lpSANFILEMATCHITEM;

	typedef map<eSANRESOURCETYPE,SANFILEMATCHITEM>		SANFILEMATCHTABLE;

	class cSanResourceManager;

	struct stSANRESPTR
	{
	protected:
		SVALUE res_id;
		SHANDLE res_ptr;
		eSANRESOURCETYPE type;
		bool b_released;
		stSANRESPTR()
			: res_id(0),
			res_ptr(nullptr),
			b_released(false)
		{
		};
		virtual ~stSANRESPTR(){};
		stSANRESPTR(const stSANRESPTR&){};
		stSANRESPTR& operator=(const stSANRESPTR&){};
		friend class cSanResourceManager;
	};

	struct SANRESPTR : private stSANRESPTR
	{
	private:
		SANRESPTR()
			: stSANRESPTR()
		{};
		~SANRESPTR(){};
		SANRESPTR(const SANRESPTR&){};
		stSANRESPTR& operator=(const stSANRESPTR&) {};
		friend class cSanResourceManager;
	public:
		bool operator==(SHANDLE Ptr) const { return (res_ptr == Ptr); };
		bool operator!=(SHANDLE Ptr) const { return (res_ptr != Ptr); };
		inline lpSANTEXTURE iGetTexturePtr()
		{ 
			switch (this->type)
			{
			case RT_TEXTURE:/*case RT_TEXTURE_2D:case RT_TEXTURE_3D:*/return (lpSANTEXTURE)this->res_ptr; break; default:return nullptr; break;
			}
		};
		inline lpSANMESH iGetMeshPtr()
		{
			switch (this->type)
			{ 
			case RT_MESH:/*case RT_MESH_STATIC:case RT_MESH_DYNAMIC:*/return (lpSANMESH)this->res_ptr; break; default:return nullptr; break; 
			}
		};
		inline SHANDLE iGetResourcePtr(){ return this->res_ptr; };
		inline bool iIsReleased(){ return this->b_released; };
		inline void iRelease(){ this->b_released = true; };
	};
	typedef SANRESPTR*	lpSANRESPTR;

	struct SANRESPTRC :private stSANRESPTR
	{
	private:
		SANRESPTRC()
			: stSANRESPTR()
		{
		};
		~SANRESPTRC(){};
		SANRESPTRC(const SANRESPTRC&){};
		SANRESPTRC& operator=(const SANRESPTRC&) {};
		friend class cSanResourceManager;
	public:
		bool operator==(const SHANDLE Ptr) const { return (res_ptr == Ptr); };
		bool operator!=(const SHANDLE Ptr) const { return (res_ptr != Ptr); };
		inline const lpSANTEXTURE iGetTexturePtr()
		{
			switch (this->type)
			{
			case RT_TEXTURE:case RT_TEXTURE_2D:case RT_TEXTURE_3D:return (const lpSANTEXTURE) this->res_ptr; break; default: return nullptr;break;
			}
		};
		inline const lpSANMESH iGetMeshPtr()
		{
			switch (this->type)
			{ 
			case RT_MESH:case RT_MESH_STATIC:case RT_MESH_DYNAMIC:return (const lpSANMESH) this->res_ptr; break; default:return nullptr; break; 
			}
		};
		inline const SHANDLE iGetResourcePtr(){ return this->res_ptr; };
		inline bool iIsReleased(){ return this->b_released; };
		inline void iRelease(){ this->b_released = true; };
	};
	typedef SANRESPTRC*	lpSANRESPTRC;

	enum eSANRESOURCEIOACCESSFLAG			//32 bit   | 8 - manager flag | 8 - other user | 8 - group user | 8 - creater flag |
	{
		SRIO_CREATER_READ			= 0x00000001,
		SRIO_CREATER_WRITE			= 0x00000002,
		SRIO_CREATER_COPY			= 0x00000004,
		SRIO_GROUP_READ				= 0x00000100,
		SRIO_GROUP_WRITE			= 0x00000200,
		SRIO_GROUP_COPY				= 0x00000400,
		SRIO_GROUP_DELETE			= 0x00000800,
		SRIO_GLOBAL_READ			= 0x00010000,
		SRIO_GLOBAL_WRITE			= 0x00020000,
		SRIO_GLOBAL_COPY			= 0x00040000,
		SRIO_GLOBAL_DELETE			= 0x00080000,
		SRIO_ORIGIONAL_RESOURCE		= 0x01000000,
		SRIO_MANAGER_COPY_FLAG		= 0x02000000,
		SRIO_MANAGER_REFERENCE		= 0x04000000,
		SRIO_MANAGER_CONST			= 0x08000000,
		SRIO_MANAGER_AUTO_DELETE	= 0x10000000,
	};

	static const uint32 SRIO_DEFAULT_FLAG = SRIO_MANAGER_AUTO_DELETE | SRIO_GLOBAL_READ | SRIO_GLOBAL_COPY | SRIO_CREATER_READ | SRIO_GROUP_COPY | SRIO_CREATER_WRITE | SRIO_CREATER_COPY;

	struct stSANRESOURCEDESC
	{
		SVALUE ResID;
		uint32 Size;
		SVALUE CreaterID;
		SVALUE GroupID;
		uint32 AccessFlag;
		SANRES_COPYFUNC pCopyFunc;
		SANRES_RELEASEFUNC pReleaseFunc;
		stSANUNKNOWNEX<stSANRESOURCEDESC,SHANDLE>* OrigionResPtr;
		stSANUNKNOWNEX<stSANRESOURCEDESC,SHANDLE>* PreviousResPtr;
		list<lpSANRESPTR> AssignedPtr;
		list<lpSANRESPTRC> AssignedConstPtr;
		int32 ReferenceCount;
	};
	typedef stSANRESOURCEDESC	SANRESOURCEDESC;
	typedef stSANRESOURCEDESC*	lpSANRESOURCEDESC;

	//typedef list<stSANUNKNOWNEX<SANRESOURCEDESC,lpSANTEXTURE>> SANTEXTURERESLIST;
	//typedef list<stSANUNKNOWNEX<SANRESOURCEDESC,lpSANMESH>> SANMESHRESLIST;
	//typedef list<stSANUNKNOWNEX<SANRESOURCEDESC,SHANDLE>> SANSOUNDRESLIST;

	typedef stSANUNKNOWNEX<SANRESOURCEDESC,SHANDLE>		SANRESOURCE;
	typedef stSANUNKNOWNEX<SANRESOURCEDESC,SHANDLE>*	lpSANRESOURCE;

	typedef map<SVALUE,SANRESOURCE>	SANRESOURCETABLE;

	SVALUE gloRegisteResource(SHANDLE ResHandle, const uint32 ResSize, const eSANRESOURCETYPE ResType, SVALUE CreaterID, SVALUE GroupID = 0, uint32 Flag = SRIO_DEFAULT_FLAG, SANRES_COPYFUNC pCopyFunc = nullptr, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
	SRESULT gloReleaseResource(SVALUE ResID, const SVALUE ID, SANRES_RELEASEFUNC pReleaseFunc = nullptr);
	eSANRESOURCETYPE gloGetResourceType(const SVALUE ResID);
#endif
}