#include"../../Core/cVectorManager.h"
#include"../../Core/cSanResourceManagerDef.h"
#include"../SanGraphics.h"
#include"../cSanShaderManagerDef.h"
#pragma once
using namespace std;
namespace San
{
	namespace Graphics
	{
#ifndef __CSANGLBUFFERMANAGER_H__
#define __CSANGLBUFFERMANAGER_H__
#ifdef _OPENGL
		static const uint32 MAX_ENGINE_SUPPORTED_GL_TEXTURE_NUM = 9U; //minimum value is 1U
		static const uint32 MAX_ENGINE_SUPPORTED_COLOR_BUFFER_NUM = 3U; //minimum value is 2U

		enum eSANGLTEXTURETYPE
		{
			SGTT_RGB_8		= 0,
			SGTT_RGBA_8		= 1,
			SGTT_RGB_F		= 2,
			SGTT_RGBA_F		= 3,
			SGTT_DEPTH_24	= 4,
			SGTT_DEPTH_32	= 5,
			SGTT_STENCIL	= 6,
		};

		enum eSANVBOBUFFERSTATE
		{
			SVBS_STATIC		= 0,
			SVBS_STREAM		= 1,
			SVBS_DYNAMIC	= 2,
		};

		enum eSANVBOTYPE
		{
			SVBOT_VERTEX		= 0,
			SVBOT_NORMAL		= 1,
			SVBOT_COLOR			= 2,
			SVBOT_TEXTURE_BEGIN = 3,
			SVBOT_TEXTURE_END	= SVBOT_TEXTURE_BEGIN + ::MAX_ENGINE_SUPPORTED_GL_TEXTURE_NUM,
			SVBOT_COUNT,
		};

		enum eSANFBOBUFFERTYPE
		{
			SFBT_FBO			= 0,
			SFBT_COLOR_BEGIN	= 1,
			SFBT_COLOR_END		= ::MAX_ENGINE_SUPPORTED_COLOR_BUFFER_NUM,
			SFBT_DEPTH,			//= ::MAX_ENGINE_SUPPORTED_COLOR_BUFFER_NUM,
			SFBT_STENCIL,		//= ::MAX_ENGINE_SUPPORTED_COLOR_BUFFER_NUM+1,
			SFBT_BUFFER_COUNT,	//= ::MAX_ENGINE_SUPPORTED_COLOR_BUFFER_NUM+2,
			SFBT_ATTACH_NULL,
			SFBT_ATTACH_TEXTURE,
			SFBT_ATTACH_RBO,
		};

		enum eSANVAOLOCATION
		{
			SVAOL_POSITION		= 0,
			SVAOL_NORMAL		= 1,
			SVAOL_COLOR			= 2,
			SVAOL_COORD_BEGIN	= 3,
			SVAOL_COORD_END		= SVAOL_COORD_BEGIN + ::MAX_ENGINE_SUPPORTED_GL_TEXTURE_NUM,
			SVAOL_DEF_COUNT,
		};

		struct stSANGLTEXDESC
		{
			uint32 Width;
			uint32 Height;
			SHANDLE pBuffer;
			uint32 BufferSize;
			eSANGLTEXTURETYPE Type;
			uint32 MipmapLevel;
		public:
			stSANGLTEXDESC()
				:Width(0),
				Height(0),
				pBuffer(0),
				BufferSize(0),
				Type(SGTT_RGBA_F),
				MipmapLevel(1)
			{};
			~stSANGLTEXDESC()
			{};
		};
		typedef stSANGLTEXDESC	SANTBODESC;
		typedef stSANGLTEXDESC*	lpSANTBODESC;

		struct stSANGLTEXOBJ
		{
			uint32 ID;
			SHANDLE Handle;
			SANTBODESC Desc;
		};
		typedef stSANGLTEXOBJ	SANTBO;
		typedef stSANGLTEXOBJ*	lPSANTBO;

		struct stSANVBO
		{
			uint32 ID;
			SHANDLE Handle;
			uint32 DataSize;
			eSANVBOBUFFERSTATE State;
		};
		typedef stSANVBO	SANVBO;
		typedef stSANVBO*	lpSANVBO;

		struct stSANVAO
		{
			uint32	ID;
			SHANDLE	VAOHandle;
			uint32	VBOIDArray[SVBOT_COUNT];
			SHANDLE	VBOHandleArray[SVBOT_COUNT];
			uint32	TexListSize;
		};
		typedef stSANVAO	SANVAO;
		typedef stSANVAO*	lpSANVAO;

		struct stSANFBODESC
		{
		public:
			uint32	width;
			uint32	height;
			eSANGLTEXTURETYPE	pix_type;
			SHANDLE	attach_handle_array[SFBT_BUFFER_COUNT];// Texture Set Texture Handle, RBO Set Local Memory Buffer Ptr
			eSANFBOBUFFERTYPE	attach_flag_array[SFBT_BUFFER_COUNT];
		public:
			stSANFBODESC()
				:width(0),
				height(0),
				pix_type(eSANGLTEXTURETYPE::SGTT_RGB_8)
			{
				for(uint32 seek=0;seek<SFBT_BUFFER_COUNT;seek=seek+1)
				{
					this->attach_handle_array[seek]=nullptr;
					this->attach_flag_array[seek]=SFBT_ATTACH_NULL;
				}
			};
			~stSANFBODESC(){};
		};
		typedef stSANFBODESC	SANFBODESC;
		typedef stSANFBODESC*	lpSANFBODESC;

		struct stSANFBO
		{
			uint32 ID;
			SHANDLE obj_array[SFBT_BUFFER_COUNT];
			//SHANDLE tex_array[::eSANFBOBUFFERTYPE::SFBT_BUFFER_COUNT];
			stSANFBODESC desc;
		};
		typedef stSANFBO	SANFBO;
		typedef stSANFBO*	lpSANFBO;

		class cSanGLBufferManager
		{
		private:
			SString m_strManagerName;
			list<SANVBO>	m_VBOList;
			list<SANVAO>	m_VAOList;
			list<SANFBO>	m_FBOList;
			list<SANTBO>	m_TBOList;
			uint32			m_MaxDeviceSupportedTextureSize;
		protected:
			uint32 _CreateVBO(const sfloat* pBuffer,const uint32 BufferSize,const eSANVBOBUFFERSTATE State=SVBS_STATIC,SHANDLE* pVBOHandle=nullptr);
			uint32 _UpdateVBO(const uint32 VBOID,const sfloat* pBuffer,const uint32 BufferSize,const eSANVBOBUFFERSTATE State=SVBS_STATIC,SHANDLE* pVBOHandle=nullptr);
			lpSANVBO _FindVBO(uint32 VBOID);
			bool _CreateVAO(SANVAO &VAO,const lpSANOBJECT pObject,const eSANVBOBUFFERSTATE State);
			void _DeleteVAO(SANVAO &VAO);
			bool _CreateFBO(SANFBO &FBO);
			void _UpdateFBO(SANFBO &FBO,SANFBODESC Desc);
			void _DeleteFBO(SANFBO &FBO);
			void _UpdateTBO(const SHANDLE TBOHandle,const SANTBODESC &Desc);
			SHANDLE	_FindTBOHandle(const uint32 TBOID);
			bool _CheckFBODesc(const SANFBODESC &Desc);
			void _Release();
		public:
			cSanGLBufferManager(SString strManagerName=_SSTR("Sys_GLBufferManager"))
				:m_strManagerName(strManagerName)
			{
				GLint Size = 0;
				::glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &Size);
				this->m_MaxDeviceSupportedTextureSize = Size <= 0 ? 0 : Size;
			};
			~cSanGLBufferManager(){ this->iReleaseGLBufferManager(); };
			bool	iCreateGLBufferManager();
			void	iReleaseGLBufferManager();
			uint32	iCreateVBOObject(const sfloat* pBuffer,const uint32 BufferSize,const eSANVBOBUFFERSTATE State=SVBS_STATIC);
			uint32	iUpdateVBOObject(const uint32 VBOID,const sfloat* pBuffer,const uint32 BufferSize,const eSANVBOBUFFERSTATE State=SVBS_STATIC);
			void	iReleaseVBOObject(const uint32 VBOID);
			uint32	iCreateVAOObject(const lpSANOBJECT pObj,const eSANVBOBUFFERSTATE State=SVBS_STATIC);
			uint32	iUpdateVAOObject(const uint32 VAOID,const lpSANOBJECT pObj,eSANVBOBUFFERSTATE State=SVBS_STATIC);
			void	iReleaseVAOObject(const uint32 VAOID);
			uint32	iCreateFBOObject(const SANFBODESC Desc);
			void	iReleaseFBOObject(const uint32 FBOID);
			bool	iUpdateFBOObject(const uint32 FBOID,const SANFBODESC Desc);
			uint32	iCreateTBOObject(const SANTBODESC &Desc);
			uint32	iCreateTBOObject(lpSANTEXTURE pTexture);
			uint32	iUpdateTBOObject(const uint32 TBOID,const SANTBODESC &Desc);
			uint32	iUpdateTBOObject(const uint32 TBOID,lpSANTEXTURE pTexture);
			void	iReleaseTBOObject(const uint32 TBOID);
			SANFBODESC iGetFBODesc(const uint32 FBOID);
			uint32	iGetFBOBuffer(const uint32 FBOID,SHANDLE pBuffer,const uint32 Size,const eSANFBOBUFFERTYPE Type);
			//SHANDLE	iGetFBOBufferHandle(const uint32 FBOID,const eSANFBOBUFFERTYPE Type);
			SANTBODESC iGetTBODesc(const uint32 TBOID);
			void	iEnableVBOObject(const uint32 VBOID = 0, const eSANVBOTYPE Type = SVBOT_VERTEX, const uint32 ShaderVersion = 430);
			void	iEnableVAOObject(const uint32 VAOID = 0, const uint32 ShaderVersion = 430);
			void	iEnableFBOObject(const uint32 FBOID = 0);
			void	iEnableTBOObject(const uint32 TBOID = 0, const uint32 Position = 0, const SANRENDERDESC &FilterDesc = SANRENDERDESC());
		};
#endif
#endif
	}
}