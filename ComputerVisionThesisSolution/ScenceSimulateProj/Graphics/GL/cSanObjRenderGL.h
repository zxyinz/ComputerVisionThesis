#include"../../Core/SanString.h"
#include"../../Physics/SanPhysics.h"
#include"../SanGraphics.h"
#include"cSanGLBufferManager.h"
#include"cSanShaderManagerGL.h"
#pragma once
using namespace std;
using namespace San::Mathematics;
namespace San
{
	namespace Graphics
	{
#ifndef __CSANOBJRENDERGL_H__
#define __CSANOBJRENDERGL_H__
#ifdef _OPENGL
		class cSanObjRenderGL
		{
		protected:
		public:
			cSanObjRenderGL(){};
			~cSanObjRenderGL(){};
			static void iDrawPix(const SPOINT3 &PixPos,SANCOLOR Color,const lpSANRENDERDESC pRenderDesc=nullptr);
			static void iDrawPoint(const SPOINT3 Pos,const SANCOLORF Color,const lpSANRENDERDESC pRenderDesc=nullptr);
			static void iDrawLine(const SPOINT3 SrcPos,const SPOINT3 DescPos,const SANCOLORF Color,const lpSANRENDERDESC pRenderDesc=nullptr);
			static void iDrawShape(const SANSHAPE Shape,const lpSANRENDERDESC pRenderDesc=nullptr);
			static void iDrawObject(const SPOINT3 Pos,const lpSANPIXMAP2D pPixMap2D,const eSANPIXTYPE Format=PT_RGBA,const LPRECT pCutRect=nullptr,const lpSANRENDERDESC pRenderDesc=nullptr);///////////////////////////////////////////////////////////
			static void iDrawObject(const SPOINT3 Pos,const lpSANPIXMAP3D pPixMap3D,const int32 Identity=-1,const eSANPIXTYPE Format=PT_RGBA,const LPRECT pCutRect=nullptr,const lpSANRENDERDESC pRenderDesc=nullptr);///////////////////////////////////////////
			static void iDrawObject(const lpSANMESH pMesh,const lpSPOINT3 pCoord,const lpSANRENDERDESC pRenderDesc=nullptr);
			static void iDrawObject(const lpSANOBJECT pObject,const lpSPOINT3 pCoord,const lpSANRENDERDESC pRenderDesc=nullptr);
			//static void iDeawObject(lpSANOBJECT pObject,lpSPOINT3 pCoord,cSanShaderManager* pShaderManager,lpSANRENDERDESC pRenderDesc=nullptr);
			static void iDrawObject(const lpSANVERTEX pVertexArray,const uint32 VertexListSize,const SANMATERIAL Material,const lpSPOINT3 pCoord=nullptr,const lpSANRENDERDESC pRenderDesc=nullptr);////////////////////////////////////////////////////
			static void iDrawObject(const uint32 VertexParam,const uint32 VertexListSize,const SANMATERIAL Material,const lpSPOINT3 pCoord=nullptr,const lpSANRENDERDESC pRenderDesc=nullptr);////////////////////////////////////////////////////////////
		};
		typedef cSanObjRenderGL cSanObjRender;
#endif
#endif
	}
}