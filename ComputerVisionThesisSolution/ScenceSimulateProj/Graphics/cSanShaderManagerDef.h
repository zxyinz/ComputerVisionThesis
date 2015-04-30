#include"../Core/SanTypes.h"
#include"../Core/SanMathematics.h"
#pragma once
using namespace std;
using namespace San::Mathematics;
namespace San
{
	namespace Graphics
	{
#ifndef __CSANSHADERMANAGERDEF_H__
#define __CSANSHADERMANAGERDEF_H__

		enum eSANSHADERTYPE
		{
			SST_VERTEXSHADER = 0,
			SST_PIXELSHADER,
#ifdef _OPENGL
			//SST_GL_TESSELLATIONSHADER,
			SST_GL_TESSCONTROLSHADER,
			SST_GL_TESSEVALUATIONSHADER,
			SST_GL_GEOMETRYSHADER,
			SST_GL_COMPUTESHADER,
#endif
			SST_SHADER_COUNT,
		};

		enum eSANSHADERDATATYPE
		{
			SSDT_NULL			= 0x00000000,
			SSDT_INT			= 0x00000001,
			SSDT_INT2			= 0x00000002,
			SSDT_INT3			= 0x00000003,
			SSDT_INT4			= 0x00000004,
			SSDT_FLOAT			= 0x00000005,
			SSDT_FLOAT2			= 0x00000006,
			SSDT_FLOAT3			= 0x00000007,
			SSDT_FLOAT4			= 0x00000008,
			SSDT_BOOL			= 0x00000009,
			SSDT_BOOL2			= 0x0000000A,
			SSDT_BOOL3			= 0x0000000B,
			SSDT_BOOL4			= 0x0000000C,
			SSDT_VECTOR3		= 0x0000000D,
			SSDT_VECTOR4		= 0x0000000E,
			SSDT_MAT3X3			= 0x0000000F,
			SSDT_MAT4X4			= 0x00000010,
			SSDT_SAMPLER2D		= 0x00000020,
			//SSDT_COUNT,
			//SSDT_MASK		= 0x00ffffff,
#ifdef _OPENGL
			SSDT_UNIFORM		= 0x01000000,
			SSDT_UNIFORM_BLOCK	= 0x02000000,
			SSDT_ATTRIBUTE		= 0x03000000,
#endif
			SSDT_TYPE_MASK		= 0xff000000,
		};

		struct stSANSSUBVAR
		{
			SStringA				strName;
			eSANSHADERDATATYPE		Type;
			stSANSTREAMEX<sfloat>	Data;
			uint32					Size;
		};
		typedef stSANSSUBVAR	SANSHADERSUBVAR;
		typedef stSANSSUBVAR*	lpSANSHADERSUBVAR;

		struct stSANSVARIABLE
		{
		protected:
			SStringA					strName;
			eSANSHADERDATATYPE			Type;
			uint32						Instance;
			vector<SANSHADERSUBVAR>		SubVarList;
			bool						bChanged;
		private:
			void _Release();
		public:
			stSANSVARIABLE(const SStringA &strVarName, const uint32 Type);
			stSANSVARIABLE(const stSANSVARIABLE &Var);
			~stSANSVARIABLE();
			stSANSVARIABLE& operator=(const stSANSVARIABLE&);
			const SANSHADERSUBVAR& operator[](const size_t Position) const;
			SANSHADERSUBVAR & operator[](const size_t Position);
			bool iAddSubVariable(const SStringA &strSubVarName, const eSANSHADERDATATYPE Type, const uint32 Size = 1);
			bool iDeleteSubVariable(const SStringA &strSubVarName);
			void iSetVariableInstance(const uint32 Instance);
			void iSetVariableState(const bool bState = false);
			void iSetSubVariable(const SStringA &strSubVarName, const int32 Data);
			void iSetSubVariable(const uint32 Index, const int32 Data);
			void iSetSubVariable(const SStringA &strSubVarName, const int32 Data1, const int32 Data2);
			void iSetSubVariable(const uint32 Index, const int32 Data1, const int32 Data2);
			void iSetSubVariable(const SStringA &strSubVarName, const int32 Data1, const int32 Data2, const int32 Data3);
			void iSetSubVariable(const uint32 Index, const int32 Data1, const int32 Data2, const int32 Data3);
			void iSetSubVariable(const SStringA &strSubVarName, const int32 Data1, const int32 Data2, const int32 Data3, const int32 Data4);
			void iSetSubVariable(const uint32 Index, const int32 Data1, const int32 Data2, const int32 Data3, const int32 Data4);
			void iSetSubVariable(const SStringA &strSubVarName, const int32* pData, uint32 Size);
			void iSetSubVariable(const uint32 Index, const int32* pData, uint32 Size);
			void iSetSubVariable(const SStringA &strSubVarName, const sfloat Data);
			void iSetSubVariable(const uint32 Index, const sfloat Data);
			void iSetSubVariable(const SStringA &strSubVarName, const sfloat Data1, const sfloat Data2);
			void iSetSubVariable(const uint32 Index, const sfloat Data1, const sfloat Data2);
			void iSetSubVariable(const SStringA &strSubVarName, const sfloat Data1, const sfloat Data2, const sfloat Data3);
			void iSetSubVariable(const uint32 Index, const sfloat Data1, const sfloat Data2, const sfloat Data3);
			void iSetSubVariable(const SStringA &strSubVarName, const sfloat Data1, const sfloat Data2, const sfloat Data3, const sfloat Data4);
			void iSetSubVariable(const uint32 Index, const sfloat Data1, const sfloat Data2, const sfloat Data3, const sfloat Data4);
			void iSetSubVariable(const SStringA &strSubVarName, const sfloat* pData, uint32 Size);
			void iSetSubVariable(const uint32 Index, const sfloat* pData, uint32 Size);
			void iSetSubVariable(const SStringA &strSubVarName, const bool Data);
			void iSetSubVariable(const uint32 Index, const bool Data);
			void iSetSubVariable(const SStringA &strSubVarName, const bool Data1, const bool Data2);
			void iSetSubVariable(const uint32 Index, const bool Data1, const bool Data2);
			void iSetSubVariable(const SStringA &strSubVarName, const bool Data1, const bool Data2, const bool Data3);
			void iSetSubVariable(const uint32 Index, const bool Data1, const bool Data2, const bool Data3);
			void iSetSubVariable(const SStringA &strSubVarName, const bool Data1, const bool Data2, const bool Data3, const bool Data4);
			void iSetSubVariable(const uint32 Index, const bool Data1, const bool Data2, const bool Data3, const bool Data4);
			void iSetSubVariable(const SStringA &strSubVarName, const bool* pData, uint32 Size);
			void iSetSubVariable(const uint32 Index, const bool* pData, uint32 Size);
			void iSetSubVariable(const SStringA &strSubVarName, const SVECTOR3 &Data);
			void iSetSubVariable(const uint32 Index, const SVECTOR3 &Data);
			void iSetSubVariable(const SStringA &strSubVarName, const lpSVECTOR3 pData, uint32 Size);
			void iSetSubVariable(const uint32 Index, const lpSVECTOR3 pData, uint32 Size);
			void iSetSubVariable(const SStringA &strSubVarName, const SVECTOR4 &Data);
			void iSetSubVariable(const uint32 Index, const SVECTOR4 &Data);
			void iSetSubVariable(const SStringA &strSubVarName, const lpSVECTOR4 pData, uint32 Size);
			void iSetSubVariable(const uint32 Index, const lpSVECTOR4 pData, uint32 Size);
			void iSetSubVariable(const SStringA &strSubVarName, const SANMATRIX3X3 &Data);
			void iSetSubVariable(const uint32 Index, const SANMATRIX3X3 &Data);
			void iSetSubVariable(const SStringA &strSubVarName, const lpSANMATRIX3X3 pData, uint32 Size);
			void iSetSubVariable(const uint32 Index, const lpSANMATRIX3X3 pData, uint32 Size);
			void iSetSubVariable(const SStringA &strSubVarName, const SANMATRIX4X4 &Data);
			void iSetSubVariable(const uint32 Index, const SANMATRIX4X4 &Data);
			void iSetSubVariable(const SStringA &strSubVarName, const lpSANMATRIX4X4 pData, uint32 Size);
			void iSetSubVariable(const uint32 Index, const lpSANMATRIX4X4 pData, uint32 Size);
			SStringA iGetVariableName() const;
			SStringA iGetSubVariableName(const uint32 Index) const;
			int32 iGetVariableInstance() const;
			bool iGetVariableState() const;
			eSANSHADERDATATYPE iGetVariableType() const;
			eSANSHADERDATATYPE iGetSubVariableType(const SStringA &strSubVarName) const;
			eSANSHADERDATATYPE iGetSubVariableType(const uint32 Index) const;
			uint32 iGetSubVariableSize(const SStringA &strSubVarName) const;
			uint32 iGetSubVariableSize(const uint32 Index) const;
			int32 iGetSubVariableIndex(const SStringA &strSubVarName) const;
			uint32 iGetSubVariableListSize() const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, int32 &Data) const;
			uint32 iGetSubVariable(const uint32 Index, int32 &Data) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, int32 &Data1, int32 &Data2) const;
			uint32 iGetSubVariable(const uint32 Index, int32 &Data1, int32 &Data2) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, int32 &Data1, int32 &Data2, int32 &Data3) const;
			uint32 iGetSubVariable(const uint32 Index, int32 &Data1, int32 &Data2, int32 &Data3) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, int32 &Data1, int32 &Data2, int32 &Data3, int32 &Data4) const;
			uint32 iGetSubVariable(const uint32 Index, int32 &Data1, int32 &Data2, int32 &Data3, int32 &Data4) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, int32* pBuffer, uint32 Size) const;
			uint32 iGetSubVariable(const uint32 Index, int32* pBuffer, uint32 Size) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, sfloat &Data) const;
			uint32 iGetSubVariable(const uint32 Index, sfloat &Data) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, sfloat &Data1, sfloat &Data2) const;
			uint32 iGetSubVariable(const uint32 Index, sfloat &Data1, sfloat &Data2) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, sfloat &Data1, sfloat &Data2, sfloat &Data3) const;
			uint32 iGetSubVariable(const uint32 Index, sfloat &Data1, sfloat &Data2, sfloat &Data3) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, sfloat &Data1, sfloat &Data2, sfloat &Data3, sfloat &Data4) const;
			uint32 iGetSubVariable(const uint32 Index, sfloat &Data1, sfloat &Data2, sfloat &Data3, sfloat &Data4) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, sfloat* pBuffer, uint32 Size) const;
			uint32 iGetSubVariable(const uint32 Index, sfloat* pBuffer, uint32 Size) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, bool &Data) const;
			uint32 iGetSubVariable(const uint32 Index, bool &Data) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, bool &Data1, bool &Data2) const;
			uint32 iGetSubVariable(const uint32 Index, bool &Data1, bool &Data2) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, bool &Data1, bool &Data2, bool &Data3) const;
			uint32 iGetSubVariable(const uint32 Index, bool &Data1, bool &Data2, bool &Data3) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, bool &Data1, bool &Data2, bool &Data3, bool &Data4) const;
			uint32 iGetSubVariable(const uint32 Index, bool &Data1, bool &Data2, bool &Data3, bool &Data4) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, bool* pBuffer, uint32 Size) const;
			uint32 iGetSubVariable(const uint32 Index, bool* pBuffer, uint32 Size) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, SVECTOR3 &Data) const;
			uint32 iGetSubVariable(const uint32 Index, SVECTOR3 &Data) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, lpSVECTOR3 pBuffer, uint32 Size) const;
			uint32 iGetSubVariable(const uint32 Index, lpSVECTOR3 pBuffer, uint32 Size) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, SVECTOR4 &Data) const;
			uint32 iGetSubVariable(const uint32 Index, SVECTOR4 &Data) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, lpSVECTOR4 pBuffer, uint32 Size) const;
			uint32 iGetSubVariable(const uint32 Index, lpSVECTOR4 pBuffer, uint32 Size) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, SANMATRIX3X3 &Data) const;
			uint32 iGetSubVariable(const uint32 Index, SANMATRIX3X3 &Data) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, lpSANMATRIX3X3 pBuffer, uint32 Size) const;
			uint32 iGetSubVariable(const uint32 Index, lpSANMATRIX3X3 pBuffer, uint32 Size) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, SANMATRIX4X4 &Data) const;
			uint32 iGetSubVariable(const uint32 Index, SANMATRIX4X4 &Data) const;
			uint32 iGetSubVariable(const SStringA &strSubVarName, lpSANMATRIX4X4 pBuffer, uint32 Size) const;
			uint32 iGetSubVariable(const uint32 Index, lpSANMATRIX4X4 pBuffer, uint32 Size) const;
		};
		typedef stSANSVARIABLE	SANSHADERVAR;
		typedef stSANSVARIABLE*	lpSANSHADERVAR;

		struct stSANSHADERPROGRAM;

		struct stSANSHADERDESC
		{
			eSANSHADERTYPE			Type;
			SStringA				strSrcCode;
			SString					strCodePath;
			vector<stSANUNKNOWNEX<SANSHADERVAR,uint32>>	VariableList;
		};
		typedef stSANSHADERDESC		SANSHADERDESC;
		typedef stSANSHADERDESC*	lpSANSHADERDESC;

		struct stSANSHADEROBJ
		{
			uint32			ObjID;
			SANSHADERDESC	Desc;
			SHANDLE			ShaderHandle;
			list<SString>	CompileLogList;
			bool			bCompiled;
			list<stSANSHADERPROGRAM*> RefList;
		};
		typedef stSANSHADEROBJ	SANSHADEROBJ;
		typedef stSANSHADEROBJ*	lpSANSHADEROBJ;

		typedef list<lpSANSHADEROBJ> SANSHADERLIST;

		struct stSANSHADERPROGRAMDESC
		{
			SString		strProgramName;
			uint32		ShaderIDList[SST_SHADER_COUNT];
			SStringA	ShaderStringList[SST_SHADER_COUNT];
			bool		bUsePath;
		};
		typedef stSANSHADERPROGRAMDESC	SANSHADERPROGRAMDESC;
		typedef stSANSHADERPROGRAMDESC*	lpSANSHADERPROGRAMDESC;

		struct stSANSHADERPROGRAM
		{
		public:
			SHANDLE					ProgramHandle;
			SANSHADERPROGRAMDESC	Desc;
			lpSANSHADEROBJ			ShaderPtrList[SST_SHADER_COUNT];
			bool					bLinked;
			list<SString>			LinkLogList;
		public:
			stSANSHADERPROGRAM(SString strProgramName=_SSTR("ShaderProgram"));
			~stSANSHADERPROGRAM();
		};
		typedef stSANSHADERPROGRAM	SANSHADERPROGRAM;
		typedef stSANSHADERPROGRAM*	lpSANSHADERPROGRAM;

		typedef list<lpSANSHADERPROGRAM> SANSHADERPROGRAMLIST;

#ifdef _DIRECTX
		typedef IDirect3DVertexShader	SANVERTEXSHADER;
		typedef IDirect3DVertexShader*	LPSANVERTEXSHADER;
		typedef IDirect3DPixelShader	SANPIXELSHADER;
		typedef IDirect3DPixelShader*	LPSANPIXELSHADER;

		typedef ID3DXConstantTable		SANSHADERCONSTANTTABLE;
		typedef ID3DXConstantTable*		LPSANSHADERCONSTANTTABLE;

		typedef ID3DXBuffer				SANSHADERBUFFER;
		typedef ID3DXBuffer*			LPSANSHADERBUFFER;
//#endif

		typedef stSANUNKNOWNEX<SString,SANVERTEXSHADER>		VERTEXSHADERELEMENT;
		typedef stSANUNKNOWNEX<SString,SANVERTEXSHADER>*	LPVERTEXSHADERELEMENT;
		typedef list<VERTEXSHADERELEMENT>					VERTEXSHADERLIST;
		typedef list<VERTEXSHADERELEMENT>*					LPVERTEXSHADERLIST;

		typedef stSANUNKNOWNEX<SString,SANPIXELSHADER>		PIXELSHADERELEMENT;
		typedef stSANUNKNOWNEX<SString,SANPIXELSHADER>*		LPPIXELSHADERELEMENT;
		typedef list<PIXELSHADERELEMENT>					PIXELSHADERLIST;
		typedef list<PIXELSHADERELEMENT>*					LPPIXELSHADERLIST;
#endif

#endif
	}
}