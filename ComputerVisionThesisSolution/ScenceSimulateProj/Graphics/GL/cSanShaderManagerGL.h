#include"../../Core/cSanResourceManager.h"
#include"../cSanShaderManagerDef.h"
#pragma once
using namespace std;
namespace San
{
	namespace Graphics
	{
#ifndef __CSANSHADERMANAGERGL_H__
#define __CSANSHADERMANAGERGL_H__
#ifdef _OPENGL
		class cSanShaderManagerGL
		{
		private:
			SString					m_strManagerName;
			SANSHADERLIST			m_ShaderArray;
			SANSHADERPROGRAMLIST	m_ProgramArray;
			lpSANSHADERPROGRAM		m_CurrentProgramPtr;
			uint32					m_CurrentShaderMaxID;
			uint32					m_ShaderModeVersion;
			sachar*					m_pManagerLogBuffer;
			uint32					m_ManagerLogBufferSize;
			SANSTREAM				m_UBOStorageBuffer;
			mutable list<SString>	m_ErrorList;
		protected:
			bool _Compile(lpSANSHADEROBJ pShader);
			bool _Build(lpSANSHADERPROGRAM pProgram);
			bool _Running(lpSANSHADEROBJ pShader);
			bool _Running(lpSANSHADERPROGRAM pProgram);
			lpSANSHADEROBJ _Search(const uint32 ShaderID) const;
			lpSANSHADERPROGRAM _Search(const SString &strProgramName) const;
			void _Release(lpSANSHADEROBJ pShader);
			void _Release(lpSANSHADERPROGRAM pProgram);
			bool _IsSystemShader(lpSANSHADEROBJ pShader);
			bool _IsSystemProgram(lpSANSHADERPROGRAM pProgram);
			void _UpdateAttributeVariable(lpSANSHADERPROGRAM pProgram, stSANUNKNOWNEX<SANSHADERVAR, uint32> &Var);
			void _UpdateUniformVariable(lpSANSHADERPROGRAM pProgram, stSANUNKNOWNEX<SANSHADERVAR,uint32> &Var);
		public:
			cSanShaderManagerGL(SString strManagerName=_SSTR("Sys_SanShaderManager"))
				:m_strManagerName(strManagerName),
				m_CurrentProgramPtr(nullptr),
				m_CurrentShaderMaxID(San::RT_BINARY_SHADER),
				m_ShaderModeVersion(200),
				m_pManagerLogBuffer(nullptr),
				m_ManagerLogBufferSize(2048),
				m_UBOStorageBuffer(1024)
			{
				this->m_pManagerLogBuffer = new sachar[this->m_ManagerLogBufferSize];
				gloMemSet(this->m_pManagerLogBuffer, 0, sizeof(sachar) *this->m_ManagerLogBufferSize);
			};
			~cSanShaderManagerGL()
			{
				this->iReleaseShaderManager();
			};
			bool iCreateShaderManager();
			void iReleaseShaderManager();
			uint32 iCreateShaderObj(const SStringA &strCode, const eSANSHADERTYPE ShaderType);
			uint32 iCreateShaderObjByFile(const SString &strCodeFilePath, const eSANSHADERTYPE ShaderType);
			uint32 iCopyShaderObj(const uint32 ShaderID);
			void iReleaseShaderObjByID(const uint32 ShaderID, const bool bForce = false);
			void iReleaseShaderObjByProgram(const SString &strProgramName, const bool bForce = false);
			bool iCreateProgram(const SString &strProgramName);
			bool iCreateProgram(const SString strProgramName, const SANSHADERPROGRAMDESC Desc);
			bool iCopyProgram(const SString &strSrcProgramName, const SString &strDestProgramName);
			void iReleaseProgram(const SString &strProgramName, const bool bForce = false);
			bool iAttachVariableToShader(const uint32 Shader, const SANSHADERVAR &Var);
			void iDetachVariableFromShader(const uint32 Shader, const SString &strVarName);
			bool iAttachShaderToProgram(const SString &strProgramName, const uint32 ShaderID);
			void iDetachShaderFromProgram(const SString &strProgramName, const eSANSHADERTYPE ShaderType);
			bool iCompileShader(const uint32 ShaderID, SString* pstrComplineInfo = nullptr);
			bool iBuildProgram(const SString &strProgramName, SString* pstrBuildInfo = nullptr);
			bool iSetShaderCode(const uint32 ShaderID, const SStringA &strCodeString);
			bool iSetShaderCodeByFile(const uint32 ShaderID,const SString &strCodeFilePath);
			bool iSetShaderType(const uint32 ShaderID,const eSANSHADERTYPE Type);
			bool iSetShaderDesc(const uint32 ShaderID, const SANSHADERDESC Desc);
			bool iSetManagerLogBufferSize(const uint32 Size);
			bool iSetUBOStorageBufferSize(const uint32 Size);
			SStringA iGetShaderCodeString(const uint32 ShaderID) const;
			SString iGetShaderCodeFilePath(const uint32 ShaderID) const;
			eSANSHADERTYPE iGetShaderType(const uint32 ShaderID) const;
			SANSHADERDESC iGetShaderDesc(const uint32 ShaderID) const;
			uint32 iGetShaderIDByProgram(const SString &strProgramName, const eSANSHADERTYPE ShaderType);
			bool iGetShaderVariable(const uint32 ShaderID,const SString &strVarName,SANSHADERVAR &Var);
			vector<SANSHADERVAR> iGetShaderUniformVarList(const uint32 ShaderID);
			vector<SANSHADERVAR> iGetShaderAttributeVarList(const uint32 ShaderID);
			vector<SANSHADERVAR> iGetShaderVariableList(const uint32 ShaderID);
			SString iGetCurrentProgramName();
			list<SString> iGetShaderLogList(const uint32 ShaderID);
			list<SString> iGetProgramLogList(const SString &strProgramName);
			list<SString> iGetManagerLogList();
			uint32 iGetManagerLogBufferSize() const;
			uint32 iGetUBOStorageBufferSize() const;
			void iUpdateShader(const uint32 ShaderID);
			void iUpdateProgram();
			void iUpdateProgram(const SString &strProgramName);
			void iUpdateUniformVar(const SString &strVarName);
			void iUpdateUniformVarList();
			void iUpdateAttributeVar(const SString &strVarName);
			void iUpdateAttributeVarList();
			void iEnableShaderObj(const uint32 ShaderID);
			void iEnableProgram(const SString &strProgramName=_SSTR("Default"));
		};
		typedef cSanShaderManagerGL cSanShaderManager;
#endif
#endif
	}
}