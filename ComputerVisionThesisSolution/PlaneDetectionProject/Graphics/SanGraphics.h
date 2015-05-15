#include"../RenderSystem.h"
#include"../Core/SanTypes.h"
#include"../Core/SanContainer.h"
#include"../Core/SanMathematics.h"
#pragma once
using namespace std;
using namespace San::Mathematics;
namespace San
{
	namespace Graphics
	{
#ifndef __STDSANGRAPHICS_H__
#define __STDSANGRAPHICS_H__

		enum eSANPIXTYPE
		{
			PT_RGB		= 3,
			PT_RGBA		= 4,
			PT_RGB_F	= 3*sizeof(sfloat),
			PT_RGBA_F	= 4*sizeof(sfloat),
		};

		struct stSANCOLORF;

		struct stSANCOLOR
		{
		public:
			union
			{
				struct
				{
					uint8	r;
					uint8	g;
					uint8	b;
					uint8	a;
				};
				uint8	color_a[4];
				uint32	c_32;
			};
		public:
			stSANCOLOR(){};
			stSANCOLOR(const uint8 red,const uint8 green,const uint8 blue,const uint8 alpha)
				:r(red),g(green),b(blue),a(alpha)
			{};
			stSANCOLOR(const uint32 color32)
				:c_32(color32)
			{};
			~stSANCOLOR(){};
			stSANCOLOR& operator+(const stSANCOLOR& Color);
			stSANCOLOR& operator-(const stSANCOLOR& Color);
			stSANCOLOR& operator=(const stSANCOLORF& Color);
		};
		typedef stSANCOLOR	SANCOLOR;
		typedef stSANCOLOR*	lpSANCOLOR;

		struct stSANCOLORF
		{
		public:
			union
			{
				struct
				{
					sfloat r;
					sfloat g;
					sfloat b;
					sfloat a;
				};
				sfloat color_a[4];
			};
		public:
			stSANCOLORF(){};
			stSANCOLORF(const sfloat red,const sfloat green,const sfloat blue,const sfloat alpha)
				:r(red),g(green),b(blue),a(alpha)
			{};
			stSANCOLORF(const SANCOLOR &color)
				:r(color.r/255.0f),g(color.g/255.0f),b(color.b/255.0f),a(color.a/255.0f)
			{};
			~stSANCOLORF(){};
			stSANCOLORF& operator+(const stSANCOLORF& Color);
			stSANCOLORF& operator-(const stSANCOLORF& Color);
			stSANCOLORF& operator=(const stSANCOLOR& Color);
		};
		typedef stSANCOLORF	SANCOLORF;
		typedef SANCOLORF*	lpSANCOLORF;

		typedef SANCOLOR	SANPIX;
		typedef SANPIX*		lpSANPIX;
		typedef SANCOLORF	SANPIXF;
		typedef SANPIXF*	lpSANPIXF;

		enum eSANTEXTYPE
		{
			STT_TEX1D		= 0x0100,//Not Support Yet
			STT_TEX2D		= 0x0200,
			STT_TEX3D		= 0x0300,
		};

		enum eSANTEXVERSION
		{
			STV_STATIC		= 0x0000,
			STV_DYNAMIC		= 0x0001,
			STV_VIDEO		= 0x0002,
		};

		struct stSANPIXMAP2D
		{
			lpSANPIX pPixMap;
			uint32 width;
			uint32 height;
		};
		typedef stSANPIXMAP2D	SANPIXMAP2D;
		typedef stSANPIXMAP2D*	lpSANPIXMAP2D;
		
		struct stSANPIXMAP3D
		{
			lpSANPIX pPixMap;
			uint32 width;
			uint32 height;
			uint32 depth;
		};
		typedef stSANPIXMAP3D	SANPIXMAP3D;
		typedef stSANPIXMAP3D*	lpSANPIXMAP3D;

		struct stSANPIXMAPF2D
		{
			lpSANPIXF pPixMap;
			uint32 width;
			uint32 height;
		};
		typedef stSANPIXMAPF2D	SANPIXMAPF2D;
		typedef stSANPIXMAPF2D*	lpSANPIXMAPF2D;
		
		struct stSANPIXMAPF3D
		{
			lpSANPIXF pPixMap;
			uint32 width;
			uint32 height;
			uint32 depth;
		};
		typedef stSANPIXMAPF3D	SANPIXMAPF3D;
		typedef stSANPIXMAPF3D*	lpSANPIXMAPF3D;

		struct stSANTEXTURE
		{
		public:
			eSANTEXVERSION Version;
			eSANTEXTYPE	Type;
			eSANPIXTYPE	PixFormat;
			SHANDLE DataHandle;
			uint32	DataSize;
			SVALUE	CurrentParam;
			SVALUE* ParamList;
			uint32	ParamListSize;
		public:
			/*stSANTEXTURE();
			stSANTEXTURE(eSANTEXTYPE Type,eSANPIXTYPE PixFormat,SHANDLE pBuffer,uint32 BufferSize,uint32 Width,uint32 Height,uint32 Depth);
			stSANTEXTURE(const stSANTEXTURE& Texture);
			~stSANTEXTURE();
			virtual void iSetTexType(eSANTEXTYPE Type);
			virtual void iSetPixFormat(eSANPIXTYPE PixFormat);
			virtual void iSetBufferData(SHANDLE pBuffer,uint32 BufferSize);
			virtual void iSetTextureSize(SPOINT3 TexSize);
			virtual void iUpdateTexture(eSANTEXTYPE Type,eSANPIXTYPE PixFormat,SHANDLE pBuffer,uint32 BufferSize,uint32 Width,uint32 Height,uint32 Depth);
			virtual eSANTEXTYPE iGetTexType() const;
			virtual eSANPIXTYPE iGetPixFormat() const;
			virtual uint32 iGetBufferData(SHANDLE* pBuffer,uint32 BeginIndex=0,uint32 EndIndex=0) const;
			virtual SPOINT3 iGetTextureSize() const;*/
			virtual void iUpdate(){};
			/*stSANTEXTURE& operator=(const stSANTEXTURE& Texture);*/
		};
		typedef stSANTEXTURE	SANTEXTURE;
		typedef stSANTEXTURE*	lpSANTEXTURE;
		typedef list<SANTEXTURE>	SANTEXTURELIST;
		typedef list<SANTEXTURE>*	lpSANTEXTURELIST;

		struct stSANACTION
		{
			uint32 Identity;
			SString strName;
			uint32 Begin;
			uint32 End;
			SVECTOR3 Translation;
			SVECTOR3 Rotation;
			SANMATRIX4X4 Matrix;
			list<SHANDLE> ParamList;
		};
		
		struct stSANGRAPHICSPOINT
		{
		public:
			SVECTOR3 v;
			SVECTOR3 vn;
			lpSVECTOR3	p_vt;
			uint8 vt_size;
			SANCOLOR vc;
		public:
			stSANGRAPHICSPOINT();
			stSANGRAPHICSPOINT(const stSANGRAPHICSPOINT& P);
			~stSANGRAPHICSPOINT();
			stSANGRAPHICSPOINT& operator=(const stSANGRAPHICSPOINT& P);
		};
		typedef stSANGRAPHICSPOINT	SANGRAPHICSPOINT;
		typedef stSANGRAPHICSPOINT*	lpSANGRAPHICSPOINT;
		typedef SANGRAPHICSPOINT	SANVERTEX;
		typedef SANGRAPHICSPOINT*	lpSANVERTEX;

		struct stSANSKELETONPOINTDESC
		{
		public:
			uint32*	skeleton_list_ptr;
			sfloat*	weight_list_ptr;
			uint32	list_size;
		public:
			stSANSKELETONPOINTDESC();
			stSANSKELETONPOINTDESC(const stSANSKELETONPOINTDESC& P);
			~stSANSKELETONPOINTDESC();
			stSANSKELETONPOINTDESC& operator=(const stSANSKELETONPOINTDESC& P);
		};
		typedef stSANSKELETONPOINTDESC	SANSKELETONPOINTDESC;
		typedef stSANSKELETONPOINTDESC*	lpSANSKELETONPOINTDESC;

		enum eSANOBJRENDERSTYLE
		{
			SORS_POINT		= 1,
			SORS_LINE		= 2,
			SORS_TRANGLE	= 3,
		};
		
		enum eSANTEXRENDERTYPE
		{
			STRT_DIFFUSE	= 0,
			STRT_EMISSIVE	= 1,
			STRT_NORMAL		= 2,
			STRT_AMBIENT	= 3,
			STRT_SPECULAR	= 4,
			STRT_USERDEF	= 5,
			/*STRT_DIFFUSE	= 1,
			STRT_HIGHLIGHT	= 2,
			STRT_NORMAL		= 3,
			STRT_ALPHA		= 4,
			STRT_TARGET		= 5,
			STRT_UNKNOWN	= 6,*/
		};

		enum eSANTEXTUREFILTER
		{
			STEXF_NONE			= 0,
			STEXF_NEAREST		= 1,
			STEXF_LINEAR		= 2,
			STEXF_ANISOTROPIC	= 3,
			STEXF_BILINEAR		= 4,
			STEXF_TRILINEAR		= 5,
		};

		struct stSANRENDERDESC
		{
		public:
			/*Default Information*/
			uint32 window_width;
			uint32 window_hight;
			/*3D Point Render Information*/
			sfloat point_size;
			/*3D Line Render Information*/
			sfloat line_size;
			/*3D Object Render Information*/
			eSANOBJRENDERSTYLE ObjRenderStyle;
			/*Texture Render Information*/
			bool bUseTexture;
			uint8 tex_mipLevel;
			eSANTEXTUREFILTER tex_filter_min;
			eSANTEXTUREFILTER tex_filter_mag;
			eSANTEXTUREFILTER tex_filter_mip_min;
			eSANTEXTUREFILTER tex_filter_mip_mag;
			bool b_tex_wrap_s_repeat;
			bool b_tex_wrap_t_repeat;
			/*Color Render Information*/
			bool bUseObjColor;
			bool bUseMaterialColor;//bUseTexture=false
			SANCOLORF obj_color;
			/*Transform Information*/
			SVECTOR3 translation;
			SVECTOR3 rotation;
			SVECTOR3 scale;
			bool bUseMatrix;
			SANMATRIX4X4 matrix;
			/*Render Information*/
			bool bShowBackFaces;
			bool bEnableDepthTest;
			/*Shader Information*/
			bool bUseShader;//////////////////////////////////////////////////////Enable shader in scene level
			bool bUseShaderProgram;
			SString strShaderProgramName;
			/*Manager Information*/
			SHANDLE pDeviceResManager;
			SHANDLE pShaderManager;
		public:
			stSANRENDERDESC()
				:window_width(1024),
				window_hight(768),
				point_size(1),
				line_size(1),
				ObjRenderStyle(SORS_TRANGLE),
				bUseTexture(true),
				tex_mipLevel(0),
				tex_filter_min(STEXF_NONE),
				tex_filter_mag(STEXF_NONE),
				tex_filter_mip_min(STEXF_NONE),
				tex_filter_mip_mag(STEXF_NONE),
				b_tex_wrap_s_repeat(true),
				b_tex_wrap_t_repeat(true),
				bUseObjColor(false),
				bUseMaterialColor(false),
				obj_color(0.5,0.5,0.5,1.0),
				translation(0,0,0),
				rotation(0,0,0),
				scale(1,1,1),
				bUseMatrix(false),
				matrix(),
				bShowBackFaces(false),
				bEnableDepthTest(true),
				bUseShader(false),
				bUseShaderProgram(false),
				pDeviceResManager(nullptr),
				pShaderManager(nullptr)
			{
				this->strShaderProgramName=_SSTR("Default");
			};
			~stSANRENDERDESC(){};
		};
		typedef stSANRENDERDESC		SANRENDERDESC;
		typedef stSANRENDERDESC*	lpSANRENDERDESC;

		struct stSANTEXRENDERDESC
		{
			SString strFilePath;
			eSANTEXRENDERTYPE RenderType;
			uint32 UserDefOffsetID;
		};
		typedef stSANTEXRENDERDESC	SANTEXRENDERDESC;
		typedef stSANTEXRENDERDESC*	lpSANTEXRENDERDESC;

		struct stSANMATERIAL
		{
			SANCOLORF Ambient;
			SANCOLORF Diffuse;
			SANCOLORF Specular;
			SANCOLORF Emissive;
			sfloat Shininess;
			list<stSANRESPTR*> TexturePtrList;
			list<SANTEXRENDERDESC> TexRenderDescList;
			SHANDLE EffectHandle;
			SANRENDERDESC ObjectRenderDesc;
		};
		typedef stSANMATERIAL	SANMATERIAL;
		typedef stSANMATERIAL*	lpSANMATERIAL;

		enum eSANOBJECTSTRUCTVERSION
		{
			SOSV_TRADITIONAL		= 0x01,
			SOSV_INDEXVERTEX		= 0x02,
			SOSV_FRAMEANIMATION		= 0x03,
			SOSV_SKELETONANIMATION	= 0x04,
		};

		//struct San::Physics::stSANCOLLISIONELEMENT;

		struct stSANOBJECT
		{
		public:
			lpSANGRAPHICSPOINT pVertexList;
			uint32 VertexListSize;
			SANMATERIAL Material;
			SString strObjName;
			SHANDLE ObjParam;
			//SANCOLLISIONOBJ CollisionObj;
			eSANOBJECTSTRUCTVERSION ObjStructVersion;
		public:
			stSANOBJECT()
				:ObjStructVersion(San::Graphics::SOSV_TRADITIONAL)
			{};
			~stSANOBJECT(){};
		};
		typedef stSANOBJECT		SANOBJECT;
		typedef stSANOBJECT*	lpSANOBJECT;

		struct stSANSKELETONOBJECT :public stSANOBJECT
		{
		public:
			lpSANSKELETONPOINTDESC pDescList;
		public:
			stSANSKELETONOBJECT()
				:stSANOBJECT()
			{ this->ObjStructVersion=San::Graphics::SOSV_SKELETONANIMATION;};
			~stSANSKELETONOBJECT(){};
		};
		typedef stSANSKELETONOBJECT		SANSKELETONOBJECT;
		typedef stSANSKELETONOBJECT*	lpSANSKELETONOBJECT;

		typedef list<SANOBJECT>		SANOBJECTLIST;
		typedef list<SANOBJECT>*	lpSANOBJECTLIST;
		//typedef list<lpSANOBJECT>	SANOBJECTLIST;
		//typedef list<lpSANOBJECT>*	lpSANOBJECTLIST;

		typedef _spair<SString,SANOBJECTLIST>	SANMESH;
		typedef _spair<SString,SANOBJECTLIST>*	lpSANMESH;
		typedef list<SANMESH>	SANMESHLIST;
		typedef list<SANMESH>*	lpSANMESHLIST;
#endif
		}
}