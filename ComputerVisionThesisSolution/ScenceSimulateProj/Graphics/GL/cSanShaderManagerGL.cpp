#include"..\..\FileIO\SanFileIO.h"
#include"cSanShaderManagerGL.h"
using namespace std;
using namespace San;
using namespace San::FileIOStream;
using namespace San::Graphics;
#ifdef _OPENGL
const SStringA strDefaultVertexShaderSM2Dot0 = "\
varying vec3 normal;\n\
varying vec3 direction;\n\
varying vec3 half_vec;\n\
varying vec4 ambient;\n\
varying vec4 diffuse;\n\
varying vec4 specular;\n\
varying vec4 glo_ambient;\n\
void main()\n\
{\n\
	normal = normalize(gl_NormalMatrix*gl_Normal);\n\
	direction = normalize(vec3(gl_LightSource[0].position));\n\
	diffuse = gl_FrontMaterial.diffuse*gl_LightSource[0].diffuse;\n\
	ambient = gl_FrontMaterial.ambient*gl_LightSource[0].ambient;\n\
	glo_ambient = gl_FrontMaterial.ambient*gl_LightModel.ambient;\n\
	gl_Position = ftransform();\n\
	gl_TexCoord[0] = gl_MultiTexCoord0;\n\
	gl_FrontColor = gl_Color;\n\
}";
const SStringA strDefaultPixelShaderSM2Dot0 = "\
uniform sampler2D texture0;\n\
uniform vec2 TexCoord;\n\
varying vec3 normal;\n\
varying vec3 direction;\n\
varying vec3 half_vec;\n\
varying vec4 ambient;\n\
varying vec4 diffuse;\n\
varying vec4 specular;\n\
varying vec4 glo_ambient;\n\
vec4 PhoneSepcular(vec3 normal, vec3 light_dir, vec3 half_vec, float spec_pow)\n\
{\n\
	vec4 Specular = vec4(0.0, 0.0, 0.0, 0.0);\n\
	half_vec = normalize(half_vec = vec3(0.0, 0.0, 1.0));\n\
	float dot_NormalLight = max(dot(normal, light_dir), 0.0);\n\
	float dot_NormalHalf = max(dot(normal, half_vec), 0.0);\n\
	if (dot_NormalLight>0.0)\n\
	{\n\
		Specular = vec4(pow(dot_NormalHalf, spec_pow));\n\
	}\n\
	return Specular;\n\
}\n\
void main()\n\
{\n\
	vec3 color = texture2D(texture0, gl_TexCoord[0].st).xyz;\n\
	float dot_NLight = max(dot(normal, direction), 0.0);\n\
	diffuse = dot_NLight*diffuse*vec4(color, 1.0);\n\
	ambient = ambient*vec4(color, 1.0);\n\
	glo_ambient = glo_ambient*vec4(color, 1.0);\n\
	vec4 Specular = PhoneSepcular(normal, direction, half_vec, 128.0);\n\
	gl_FragColor = (diffuse + ambient + glo_ambient + Specular);\n\
}";
bool cSanShaderManagerGL::iCreateShaderManager()
{
	if(::gloFoundVector(this->m_strManagerName))
	{
		return false;
	}
	this->iReleaseShaderManager();
	if (this->m_ManagerLogBufferSize != 0)
	{
		this->m_pManagerLogBuffer = new sachar[this->m_ManagerLogBufferSize];
	}
	SANSHADERPROGRAMDESC Desc;
	Desc.strProgramName = _SSTR("SanDefaultShaderSM2Dot0");
	Desc.bUsePath = false;
	Desc.ShaderStringList[SST_VERTEXSHADER] = strDefaultVertexShaderSM2Dot0;
	Desc.ShaderStringList[SST_PIXELSHADER] = strDefaultPixelShaderSM2Dot0;
	this->iCreateProgram(Desc.strProgramName, Desc);
	this->iCreateProgram(_SSTR("SanDynamicShader"));
	this->m_CurrentShaderMaxID = San::RT_BINARY_SHADER + 32;
	::gloRegisterVector(this->m_strManagerName,VT_SYS|VT_VAR,(SHANDLE)this);
	this->iEnableProgram();
	return true;
}
void cSanShaderManagerGL::iReleaseShaderManager()
{
	if (this->m_pManagerLogBuffer != nullptr)
	{
		if (this->m_ManagerLogBufferSize == 1)
		{
			delete this->m_pManagerLogBuffer;
		}
		else
		{
			delete[] this->m_pManagerLogBuffer;
		}
	}
	this->m_pManagerLogBuffer = nullptr;
	this->m_UBOStorageBuffer.iClear();
	SANSHADERPROGRAMLIST::iterator pProgram = this->m_ProgramArray.begin();
	while (pProgram!=this->m_ProgramArray.end())
	{
		this->_Release(*pProgram);
		delete (*pProgram);
	}
	this->m_ProgramArray.clear();
	SANSHADERLIST::iterator pShader = this->m_ShaderArray.begin();
	while (pShader != this->m_ShaderArray.end())
	{
		this->_Release(*pShader);
		delete (*pShader);
	}
	this->m_ShaderArray.clear();
	this->m_ErrorList.clear();
	::gloDestoryVector(this->m_strManagerName,VT_SYS|VT_VAR);
}
uint32 cSanShaderManagerGL::iCreateShaderObj(const SStringA &strCode, const eSANSHADERTYPE Type)
{
	if (this->m_CurrentShaderMaxID == San::RT_BINARY_SHADER_MAX)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: No enough available object ID to allocate")));
		return 0;
	}
	if (strCode.empty())
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Code file is empty")));
		return 0;
	}
	if (Type >= ::eSANSHADERTYPE::SST_SHADER_COUNT)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Wrong shader type")));
		return 0;
	}
	lpSANSHADEROBJ pObj = new SANSHADEROBJ();
	this->m_CurrentShaderMaxID = this->m_CurrentShaderMaxID + 1;
	pObj->ObjID = this->m_CurrentShaderMaxID;
	pObj->Desc.Type = Type;
	pObj->Desc.strSrcCode = strCode;
	pObj->Desc.strCodePath.clear();
	pObj->Desc.VariableList.clear();
	pObj->ShaderHandle = nullptr;
	pObj->bCompiled = false;
	pObj->RefList.clear();
	pObj->CompileLogList.clear();
	this->m_ShaderArray.push_back(pObj);
	return pObj->ObjID;
}
uint32 cSanShaderManagerGL::iCreateShaderObjByFile(const SString &strCodeFilePath, const eSANSHADERTYPE Type)
{
	SStringA strCode;
	if (!gloLoadFile(strCodeFilePath, strCode))
	{
		return 0;
	}
	uint32 ObjID = this->iCreateShaderObj(strCode, Type);
	if (ObjID != 0)
	{
		this->_Search(ObjID)->Desc.strCodePath = strCodeFilePath;
	}
	return ObjID;
}
uint32 cSanShaderManagerGL::iCopyShaderObj(const uint32 ShaderID)
{
	if (this->m_CurrentShaderMaxID == San::RT_BINARY_SHADER_MAX)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: No enough available object ID to allocate")));
		return 0;
	}
	lpSANSHADEROBJ pShader = this->_Search(ShaderID);
	if (pShader == nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Can not find the shader")));
		return 0;
	}
	lpSANSHADEROBJ pDestShader = new SANSHADEROBJ;
	if (pDestShader == nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Can not allocate the shader memory space")));
		return 0;
	}
	this->m_CurrentShaderMaxID = this->m_CurrentShaderMaxID + 1;
	pDestShader->ObjID = this->m_CurrentShaderMaxID;
	pDestShader->ShaderHandle = nullptr;
	pDestShader->Desc = pShader->Desc;
	pDestShader->bCompiled = false;
	pDestShader->RefList = pShader->RefList;
	pDestShader->CompileLogList.clear();
	this->m_ShaderArray.push_back(pDestShader);
	return pDestShader->ObjID;
}
bool cSanShaderManagerGL::_Compile(lpSANSHADEROBJ pShader)
{
	SString strLog = _SSTR("Compile Shader - ");
	if (pShader->Desc.Type >= SST_SHADER_COUNT)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Wrong Shader Type")));
		return false;
	}
	pShader->bCompiled = false;
	list<lpSANSHADERPROGRAM>::iterator pProgram = pShader->RefList.begin();
	while (pProgram != pShader->RefList.end())
	{
		(*pProgram)->bLinked = false;
		pProgram++;
	}
	if (pShader->ShaderHandle == nullptr)
	{
		uint32 type = 0;
		switch (pShader->Desc.Type)
		{
		case SST_VERTEXSHADER:
			type = GL_VERTEX_SHADER;
			strLog = strLog + _SSTR("GL_VERTEX_SHADER: \n");
			break;
		case SST_PIXELSHADER:
			type = GL_FRAGMENT_SHADER;
			strLog = strLog + _SSTR("GL_FRAGMENT_SHADER: \n");
			break;
		case SST_GL_TESSCONTROLSHADER:
			type = GL_TESS_CONTROL_SHADER;
			strLog = strLog + _SSTR("GL_TESS_CONTROL_SHADER: \n");
			break;
		case SST_GL_TESSEVALUATIONSHADER:
			type = GL_TESS_EVALUATION_SHADER;
			strLog = strLog + _SSTR("GL_TESS_EVALUATION_SHADER: \n");
			break;
		case SST_GL_GEOMETRYSHADER:
			type = GL_GEOMETRY_SHADER;
			strLog = strLog + _SSTR("GL_GEOMETRY_SHADER: \n");
			break;
		case SST_GL_COMPUTESHADER:
			type = GL_GEOMETRY_SHADER;
			strLog = strLog + _SSTR("SST_GL_COMPUTESHADER: \n");
			break;
		default:
			type = GL_VERTEX_SHADER;
			strLog = strLog + _SSTR("UNDNOWN SHADER: \n");
			break;
		}
		pShader->ShaderHandle = (SHANDLE)::glCreateShader(type);
		if (pShader->ShaderHandle == nullptr)
		{
			this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Can't allocate GL shader handle which id = 'ObjID'")));
			return false;
		}
	}
	const GLchar* pCode = pShader->Desc.strSrcCode.c_str();
	::glShaderSource((GLuint) (pShader->ShaderHandle), 1, &pCode, NULL);
	::glCompileShader((GLuint) (pShader->ShaderHandle));
	GLint status;
	::gloMemSet(this->m_pManagerLogBuffer, 0, sizeof(sachar) *this->m_ManagerLogBufferSize);
	::glGetShaderiv((GLuint) (pShader->ShaderHandle), GL_COMPILE_STATUS, &status);
	if (!status)
	{
		::glGetShaderInfoLog((GLuint) (pShader->ShaderHandle), this->m_ManagerLogBufferSize, nullptr, this->m_pManagerLogBuffer);
		if (SStringA(this->m_pManagerLogBuffer).length() == 0)
		{
			strLog = strLog + _SSTR("Compile successful\n");
			pShader->bCompiled = true;
			pShader->CompileLogList.insert(pShader->CompileLogList.end(), strLog);
			return true;
		}
	}
	strLog = strLog + ::gloAToT(this->m_pManagerLogBuffer);
	pShader->CompileLogList.insert(pShader->CompileLogList.end(), strLog);
	vector<SString> InfoList = ::gloGetStringItems(strLog, _SSTR("\n"));
	for (uint32 seek = 0; seek < InfoList.size(); seek = seek + 1)
	{
		if ((sint) (InfoList[seek].find(_SSTR(" error "))) >= 0)
		{
			return false;
		}
	}
	return true;
}
bool cSanShaderManagerGL::_Build(lpSANSHADERPROGRAM pProgram)
{
	SString strLog=_SSTR("Build Program - ")+pProgram->Desc.strProgramName+_SSTR(" :\n");
	pProgram->bLinked=false;
	if(pProgram->ProgramHandle==nullptr)
	{
		pProgram->ProgramHandle=(SHANDLE)::glCreateProgram();
		if(pProgram->ProgramHandle==nullptr)
		{
			this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Can't allocate GL program handle which program name = "))+pProgram->Desc.strProgramName);
			return false;
		}
	}
	bool bRes=true;
	lpSANSHADEROBJ pShader = nullptr;
	for (uint8 seek = 0; seek < ::eSANSHADERTYPE::SST_SHADER_COUNT; seek = seek + 1)
	{
		pShader = pProgram->ShaderPtrList[seek];
		if (pShader == nullptr)
		{
			continue;
		}
		if (!pShader->bCompiled)
		{
			bRes = this->_Compile(pShader);
			if (!pShader->CompileLogList.empty())
			{
				strLog = strLog + pShader->CompileLogList.back();
			}
			if (bRes == false)
			{
				strLog = strLog + _SSTR("Program build failed\n");
				pProgram->LinkLogList.insert(pProgram->LinkLogList.end(), strLog);
				return false;
			}
		}
		::glAttachShader((GLuint) (pProgram->ProgramHandle), (GLuint) (pShader->ShaderHandle));
	}
	strLog=strLog+_SSTR("Link program: \n");
	::glLinkProgram((GLuint)(pProgram->ProgramHandle));
	GLint status;
	::gloMemSet(this->m_pManagerLogBuffer, 0, sizeof(sachar) *this->m_ManagerLogBufferSize);
	::glGetProgramiv((GLuint)(pProgram->ProgramHandle),GL_LINK_STATUS,&status);
	if(!status)
	{
		::glGetProgramInfoLog((GLuint) (pProgram->ProgramHandle), this->m_ManagerLogBufferSize, nullptr, this->m_pManagerLogBuffer);
		if(SStringA(this->m_pManagerLogBuffer).length()!=0)
		{
			SString strBuildResult=_SSTR("Program build successful\n");
			bool bBuildResult=true;
			vector<SString> InfoList = ::gloGetStringItems(::gloAToT(this->m_pManagerLogBuffer), _SSTR("\n"));
			for (uint32 seek = 0; seek < InfoList.size(); seek = seek + 1)
			{
				if ((sint) (InfoList[seek].find(_SSTR(" error "))) >= 0)
				{
					strBuildResult = _SSTR("Program build failed\n");
					bBuildResult = false;
				}
			}
			strLog=strLog+strBuildResult;
			strLog=strLog+::gloAToT(this->m_pManagerLogBuffer);
			pProgram->LinkLogList.insert(pProgram->LinkLogList.end(), strLog);
			return bBuildResult;
		}
	}
	pProgram->bLinked=true;
	strLog=strLog+_SSTR("Program build successful\n");
	pProgram->LinkLogList.insert(pProgram->LinkLogList.end(), strLog);
	return true;
}
bool cSanShaderManagerGL::_Running(lpSANSHADEROBJ pShader)
{
	lpSANSHADERPROGRAM pProgram = this->_Search(_SSTR("SanDynamicShader"));
	if (pProgram == nullptr)
	{
		return false;
	}
	if (this->m_CurrentProgramPtr != nullptr)
	{
		pProgram->Desc.bUsePath = this->m_CurrentProgramPtr->Desc.bUsePath;
		for (uint8 seek = 0; seek < SST_SHADER_COUNT; seek = seek + 1)
		{
			pProgram->Desc.ShaderStringList[seek] = this->m_CurrentProgramPtr->Desc.ShaderStringList[seek];
			pProgram->Desc.ShaderIDList[seek] = this->m_CurrentProgramPtr->Desc.ShaderIDList[seek];
		}
	}
	pProgram->bLinked = false;
	pProgram->Desc.ShaderIDList[pShader->Desc.Type] = pShader->ObjID;
	if (this->_Build(pProgram))
	{
		::glUseProgram((GLuint) (pProgram->ProgramHandle));
		this->m_CurrentProgramPtr = pProgram;
		return true;
	}
	switch (this->m_ShaderModeVersion)
	{
	case 100:
		this->m_CurrentProgramPtr = nullptr;
		::glUseProgram(0);
		break;
	case 200:
		this->_Running(this->_Search(_SSTR("SanDefaultShaderSM2Dot0")));
		break;
	case 430:
		this->_Running(this->_Search(_SSTR("SanDefaultShaderSM4Dot3")));
		break;
	default:
		this->m_CurrentProgramPtr = nullptr;
		::glUseProgram(0);
		break;
	}
	return false;
}
bool cSanShaderManagerGL::_Running(lpSANSHADERPROGRAM pProgram)
{
	if (!pProgram->bLinked)
	{
		if (!this->_Build(pProgram))
		{
			::wcout<<pProgram->LinkLogList.back().c_str()<<"\n";
			return false;
		}
	}
	::glUseProgram((GLuint) (pProgram->ProgramHandle));
	this->m_CurrentProgramPtr = pProgram;
	::wcout<<pProgram->LinkLogList.back().c_str()<<"\n";
	return true;
}
lpSANSHADEROBJ cSanShaderManagerGL::_Search(const uint32 ShaderID) const
{
	SANSHADERLIST::const_iterator pShader = this->m_ShaderArray.begin();
	while (pShader != this->m_ShaderArray.end())
	{
		if ((*pShader)->ObjID == ShaderID)
		{
			return (*pShader);
		}
		pShader++;
	}
	return nullptr;
}
lpSANSHADERPROGRAM cSanShaderManagerGL::_Search(const SString &strProgramName) const
{
	SANSHADERPROGRAMLIST::const_iterator pProgram = this->m_ProgramArray.begin();
	while (pProgram != this->m_ProgramArray.end())
	{
		if ((*pProgram)->Desc.strProgramName == strProgramName)
		{
			return (*pProgram);
		}
		pProgram++;
	}
	return nullptr;
}
void cSanShaderManagerGL::_Release(lpSANSHADEROBJ pShader)
{
	if (pShader == nullptr)
	{
		return;
	}
	if (this->_IsSystemShader(pShader))
	{
		return;
	}
	list<lpSANSHADERPROGRAM>::iterator pItem = pShader->RefList.begin();
	lpSANSHADERPROGRAM pProgram;
	while (pItem != pShader->RefList.end())
	{
		pProgram = *pItem;
		if (pProgram != nullptr)
		{
			pProgram->ShaderPtrList[pShader->Desc.Type] = nullptr;
			pProgram->bLinked = false;
			pProgram->LinkLogList.insert(pProgram->LinkLogList.end(), _SSTR("Detach shader\n"));
		}
		pItem++;
	}
	pShader->RefList.clear();
	if (pShader->ShaderHandle != nullptr)
	{
		::glDeleteShader((GLuint) (pShader->ShaderHandle));
		pShader->ShaderHandle = nullptr;
	}
	pShader->bCompiled = false;
	pShader->Desc.strSrcCode.clear();
	pShader->Desc.strCodePath.clear();
	pShader->Desc.Type = SST_SHADER_COUNT;
	pShader->Desc.VariableList.clear();
	pShader->CompileLogList.clear();
	SANSHADERLIST::iterator pShaderObj = this->m_ShaderArray.begin();
	while (pShaderObj != this->m_ShaderArray.end())
	{
		if ((*pShaderObj)->ObjID == pShader->ObjID)
		{
			this->m_ShaderArray.erase(pShaderObj);
			break;
		}
		pShaderObj++;
	}
	pShader->ObjID = 0;
	delete pShader;
	pShader = nullptr;
}
void cSanShaderManagerGL::_Release(lpSANSHADERPROGRAM pProgram)
{
	if(pProgram==nullptr)
	{
		return;
	}
	if (this->_IsSystemProgram(pProgram))
	{
		return;
	}
	pProgram->bLinked=false;
	lpSANSHADEROBJ pShader=nullptr;
	list<lpSANSHADERPROGRAM>::iterator pItem;
	for (uint32 seek = 0; seek < SST_SHADER_COUNT; seek = seek + 1)
	{
		pShader = pProgram->ShaderPtrList[seek];
		if (pShader == nullptr)
		{
			continue;
		}
		pItem = pShader->RefList.begin();
		while (pItem != pShader->RefList.end())
		{
			if ((*pItem) == pProgram)
			{
				pItem = pShader->RefList.erase(pItem);
				break;
			}
			pItem++;
		}
	}
	SANSHADERPROGRAMLIST::iterator pProgramItem=this->m_ProgramArray.begin();
	while (pProgramItem != this->m_ProgramArray.end())
	{
		if ((*pProgramItem) == pProgram)
		{
			this->m_ProgramArray.erase(pProgramItem);
			break;
		}
		pProgramItem++;
	}
	if (pProgram->ProgramHandle != nullptr)
	{
		::glDeleteProgram((GLuint) pProgram->ProgramHandle);
		pProgram->ProgramHandle = nullptr;
	}
	for (uint8 seek = 0; seek < SST_SHADER_COUNT; seek = seek + 1)
	{
		pProgram->ShaderPtrList[seek]= nullptr;
		pProgram->Desc.ShaderIDList[seek] = 0;
		pProgram->Desc.ShaderStringList[seek].clear();
	}
	pProgram->Desc.strProgramName.clear();
	pProgram->LinkLogList.clear();
	delete pProgram;
	pProgram=nullptr;
}
bool cSanShaderManagerGL::_IsSystemShader(lpSANSHADEROBJ pShader)
{
	return (pShader->ObjID <= (RT_BINARY_SHADER + 32));
}
bool cSanShaderManagerGL::_IsSystemProgram(lpSANSHADERPROGRAM pProgram)
{
	if (pProgram->Desc.strProgramName == _SSTR("SanDefaultShaderSM2Dot0"))
	{
		return true;
	}
	if (pProgram->Desc.strProgramName == _SSTR("SanDefaultShaderSM4Dot3"))
	{
		return true;
	}
	if (pProgram->Desc.strProgramName == _SSTR("SanDynamicShader"))
	{
		return true;
	}
	return false;
}
void cSanShaderManagerGL::_UpdateAttributeVariable(lpSANSHADERPROGRAM pProgram, stSANUNKNOWNEX<SANSHADERVAR,uint32> &Var)
{
	lpSANSHADERVAR pVar = &(Var.description);
	GLint Location = ::glGetAttribLocation((GLuint) (pProgram->ProgramHandle), pVar->iGetVariableName().c_str());
	if (Location < 0)
	{
		return;
	}
	switch (pVar->iGetVariableType()&(~SSDT_TYPE_MASK))
	{
	case SSDT_INT:
		/*pItem->iGetInt((int32*)pBuffer,BufferSize/sizeof(uint32));
		::glVertexAttrib1i(Location,((GLint*)pBuffer)[0]);*/
		break;
	case SSDT_INT2:
		//pItem->iGetInt((int32*)pBuffer,BufferSize/sizeof(uint32));
		//::glVertexAttrib2i(Location,((GLint*)pBuffer)[0],((GLint*)pBuffer)[1]);
		break;
	case SSDT_INT3:
		//pItem->iGetInt((int32*)pBuffer,BufferSize/sizeof(uint32));
		//::glVertexAttrib3i(Location,((GLint*)pBuffer)[0],((GLint*)pBuffer)[1],((GLint*)pBuffer)[2]);
		break;
	case SSDT_INT4:
		//pItem->iGetInt((int32*)pBuffer,BufferSize/sizeof(uint32));
		//::glVertexAttrib4i(Location,((GLint*)pBuffer)[0],((GLint*)pBuffer)[1],((GLint*)pBuffer)[2],((GLint*)pBuffer)[3]);
		break;
	case SSDT_FLOAT:
		::glVertexAttrib1f(Location, (*pVar)[0].Data[0]);
		break;
	case SSDT_FLOAT2:
		::glVertexAttrib2f(Location, (*pVar)[0].Data[0], (*pVar)[0].Data[1]);
		break;
	case SSDT_FLOAT3:
		::glVertexAttrib3f(Location, (*pVar)[0].Data[0], (*pVar)[0].Data[1], (*pVar)[0].Data[2]);
		break;
	case SSDT_FLOAT4:
		::glVertexAttrib4f(Location, (*pVar)[0].Data[0], (*pVar)[0].Data[1], (*pVar)[0].Data[2], (*pVar)[0].Data[3]);
		break;
	case SSDT_BOOL:
		//pItem->iGetBool((bool*)pBuffer,BufferSize/sizeof(bool));
		//if(((bool*)pBuffer)[0])
		//{
		//::glVertexAttrib1i(Location,1);
		//}
		//else
		//{
		//::glVertexAttrib1i(Location,0);
		//}
		break;
	case SSDT_VECTOR3:
		::glVertexAttrib3f(Location, (*pVar)[0].Data[0], (*pVar)[0].Data[1], (*pVar)[0].Data[2]);
		break;
	case SSDT_VECTOR4:
		::glVertexAttrib4f(Location, (*pVar)[0].Data[0], (*pVar)[0].Data[1], (*pVar)[0].Data[2], (*pVar)[0].Data[3]);
		break;
	case SSDT_MAT3X3:
		//pItem->iGetMat3x3((sfloat*) pBuffer, BufferSize / sizeof(sfloat));
		//::glVertexAttribMatrix3fv(Location, 1, 1, (GLfloat*) pBuffer);
		break;
	case SSDT_MAT4X4:
		//pItem->iGetMat4x4((sfloat*)pBuffer,BufferSize/sizeof(sfloat));
		//::glVertexAttribMatrix4fv(Location,1,1,(GLfloat*)pBuffer);
		break;
	default:
		break;
	}
}
void cSanShaderManagerGL::_UpdateUniformVariable(lpSANSHADERPROGRAM pProgram, stSANUNKNOWNEX<SANSHADERVAR,uint32> &Var)
{
	lpSANSHADERVAR pVar = &(Var.description);
	if (pVar->iGetVariableType() == SSDT_UNIFORM)
	{
		GLint Location = ::glGetUniformLocation((GLuint) (pProgram->ProgramHandle), pVar->iGetVariableName().c_str());
		if (Location < 0)
		{
			return;
		}
		if (pVar->iGetSubVariableListSize() != 1)
		{
			return;
		}
		switch (pVar->iGetSubVariableType(0))
		{
		case SSDT_INT:
		case SSDT_BOOL:
			if ((*pVar)[0].Size == 1)
			{
				::glUniform1i(Location, (*pVar)[0].Data[0]);
			}
			else
			{
				stSANSTREAMEX<GLint> IntStream((*pVar)[0].Data.Size);
				::glUniform1iv(Location, (*pVar)[0].Size, IntStream.pStream);
			}
			break;
		case SSDT_INT2:
		case SSDT_BOOL2:
			if ((*pVar)[0].Size == 1)
			{
				::glUniform2i(Location, (*pVar)[0].Size, (*pVar)[0].Data[1]);
			}
			else
			{
				stSANSTREAMEX<GLint> IntStream((*pVar)[0].Data.Size);
				::glUniform2iv(Location, (*pVar)[0].Size, IntStream.pStream);
			}
			break;
		case SSDT_INT3:
		case SSDT_BOOL3:
			if ((*pVar)[0].Size == 1)
			{
				::glUniform3i(Location, (*pVar)[0].Data[0], (*pVar)[0].Data[1], (*pVar)[0].Data[2]);
			}
			else
			{
				stSANSTREAMEX<GLint> IntStream((*pVar)[0].Data.Size);
				::glUniform3iv(Location, (*pVar)[0].Size, IntStream.pStream);
			}
			break;
		case SSDT_INT4:
		case SSDT_BOOL4:
			if ((*pVar)[0].Size == 1)
			{
				::glUniform4i(Location, (*pVar)[0].Data[0], (*pVar)[0].Data[1], (*pVar)[0].Data[2], (*pVar)[0].Data[3]);
			}
			else
			{
				stSANSTREAMEX<GLint> IntStream((*pVar)[0].Data.Size);
				::glUniform4iv(Location, (*pVar)[0].Size, IntStream.pStream);
			}
			break;
		case SSDT_FLOAT:
			::glUniform1fv(Location, (*pVar)[0].Size, (*pVar)[0].Data.pStream);
			break;
		case SSDT_FLOAT2:
			::glUniform2fv(Location, (*pVar)[0].Size, (*pVar)[0].Data.pStream);
			break;
		case SSDT_FLOAT3:
			::glUniform3fv(Location, (*pVar)[0].Size, (*pVar)[0].Data.pStream);
			break;
		case SSDT_FLOAT4:
			::glUniform4fv(Location, (*pVar)[0].Size, (*pVar)[0].Data.pStream);
			break;
		/*case SSDT_BOOL:
			::glUniform1i(Location, !gloIsFloatEqual((*pVar)[0].Data[0], 0.0));
			break;
		case SSDT_BOOL2:
			::glUniform2i(Location, !gloIsFloatEqual((*pVar)[0].Data[0], 0.0), !gloIsFloatEqual((*pVar)[0].Data[0], 0.0));
			break;
		case SSDT_BOOL3:
			::glUniform3i(Location, !gloIsFloatEqual((*pVar)[0].Data[0], 0.0), !gloIsFloatEqual((*pVar)[0].Data[1], 0.0), !gloIsFloatEqual((*pVar)[0].Data[2], 0.0));
			break;
		case SSDT_BOOL4:
			::glUniform4i(Location, !gloIsFloatEqual((*pVar)[0].Data[0], 0.0), !gloIsFloatEqual((*pVar)[0].Data[1], 0.0), !gloIsFloatEqual((*pVar)[0].Data[2], 0.0), !gloIsFloatEqual((*pVar)[0].Data[3], 0.0));
			break;*/
		case SSDT_VECTOR3:
			::glUniform3fv(Location, (*pVar)[0].Size, (*pVar)[0].Data.pStream);
			break;
		case SSDT_VECTOR4:
			::glUniform4fv(Location, (*pVar)[0].Size, (*pVar)[0].Data.pStream);
			break;
		case SSDT_MAT3X3:
			::glUniformMatrix3fv(Location, (*pVar)[0].Size, 1, &(*pVar)[0].Data[0]);
			break;
		case SSDT_MAT4X4:
			::glUniformMatrix4fv(Location, (*pVar)[0].Size, 1, &(*pVar)[0].Data[0]);
			break;
		case SSDT_SAMPLER2D:
			break;
		default:
			break;
		}
		return;
	}
	if (pVar->iGetVariableType() == SSDT_UNIFORM_BLOCK)
	{
		GLuint Index = 0;
		if (pVar->iGetVariableState())
		{
			const GLchar* pName = nullptr;
			lpSANSHADERSUBVAR pSubVar = nullptr;
			this->m_UBOStorageBuffer.iClear();
			uint32 size = pVar->iGetSubVariableListSize();
			for (uint32 seek = 0; seek < size; seek = seek + 1)
			{
				pSubVar = &(*pVar)[seek];
				pName = pSubVar->strName.c_str();
				glGetUniformIndices((GLuint) (pProgram->ProgramHandle), 1, &pName, (GLuint*) &Index);
				GLint Offset = -1;
				if (pSubVar->Size != 1)
				{
					glGetActiveUniformsiv((GLuint) (pProgram->ProgramHandle), 1, (GLuint*) &(Index), GL_UNIFORM_ARRAY_STRIDE, &Offset);
				}
				else
				{
					switch (pSubVar->Type)
					{
					case SSDT_MAT3X3:
					case SSDT_MAT4X4:
						glGetActiveUniformsiv((GLuint) (pProgram->ProgramHandle), 1, (GLuint*) &(Index), GL_UNIFORM_MATRIX_STRIDE, &Offset);
						break;
					default:
						glGetActiveUniformsiv((GLuint) (pProgram->ProgramHandle), 1, (GLuint*) &(Index), GL_UNIFORM_OFFSET, &Offset);
						break;
					}
				}
				if (Offset == -1)
				{
					return;
				}
				this->m_UBOStorageBuffer.iSetStream(Offset, (uint8*) (pSubVar->Data.pStream), pSubVar->Data.iGetSizeInByte());
			}
			pVar->iSetVariableState(false);
			GLint BlockSize = -1;
			Index = ::glGetUniformBlockIndex((GLuint) (pProgram->ProgramHandle), (const GLchar*) pVar->iGetVariableName().c_str());
			::glGetActiveUniformBlockiv((GLuint) (pProgram->ProgramHandle), Index, GL_UNIFORM_BLOCK_DATA_SIZE, &BlockSize);
			if (BlockSize == -1)
			{
				return;
			}
			::glUniformBlockBinding((GLuint) (pProgram->ProgramHandle), Index, (GLuint) (pVar->iGetVariableInstance()));
			::glBindBuffer(GL_UNIFORM_BUFFER, (GLuint) (Var.value));
			::glBufferData(GL_UNIFORM_BUFFER, BlockSize, this->m_UBOStorageBuffer.pStream, GL_DYNAMIC_DRAW);
			::glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint) (pVar->iGetVariableInstance()), (GLuint) (Var.value));
			return;
		}
		else
		{
			Index = ::glGetUniformBlockIndex((GLuint) (pProgram->ProgramHandle), (const GLchar*) pVar->iGetVariableName().c_str());
			::glUniformBlockBinding((GLuint) (pProgram->ProgramHandle), Index, (GLuint) (pVar->iGetVariableInstance()));
			::glBindBuffer(GL_UNIFORM_BUFFER, (GLuint) (Var.value));
			::glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint) (pVar->iGetVariableInstance()), (GLuint) (Var.value));
			return;
		}
	}
}
void cSanShaderManagerGL::iReleaseShaderObjByID(const uint32 ShaderID, bool bForce)
{
	lpSANSHADEROBJ pShader=this->_Search(ShaderID);
	if (pShader == nullptr)
	{
		return;
	}
	if (this->_IsSystemShader(pShader))
	{
		return;
	}
	bool bRunning = false;
	if(this->m_CurrentProgramPtr!=nullptr)
	{
		if(this->m_CurrentProgramPtr->ShaderPtrList[pShader->Desc.Type] == pShader)
		{
			if (bForce)
			{
				this->iEnableProgram();
			}
			else
			{
				this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Shader still running on the GUP")));
				return;
			}
		}
	}
	this->_Release(pShader);
}
void cSanShaderManagerGL::iReleaseShaderObjByProgram(const SString &strProgramName, const bool bForce)
{
	if ((this->m_CurrentProgramPtr != nullptr) && (this->m_CurrentProgramPtr->Desc.strProgramName == strProgramName))
	{
		if (!bForce)
		{
			this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Program still running on the GUP.")));
			return;
		}
	}
	lpSANSHADERPROGRAM pProgram = this->_Search(strProgramName);
	if (pProgram == nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Can not find the program - ") + strProgramName));
		return;
	}
	for (uint8 seek = 0; seek < SST_SHADER_COUNT; seek = seek + 1)
	{
		if (pProgram->ShaderPtrList[seek] != nullptr)
		{
			this->iReleaseShaderObjByID(pProgram->ShaderPtrList[seek]->ObjID, bForce);
		}
	}
}
bool cSanShaderManagerGL::iCreateProgram(const SString &strProgramName)
{
	if (this->_Search(strProgramName) != nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Current program name already in use")));
		return false;
	}
	lpSANSHADERPROGRAM pProgram = new SANSHADERPROGRAM;
	pProgram->Desc.strProgramName = strProgramName;
	pProgram->Desc.bUsePath = true;
	for (uint8 seek = 0; seek < SST_SHADER_COUNT; seek = seek + 1)
	{
		pProgram->Desc.ShaderStringList[seek].clear();
		pProgram->Desc.ShaderIDList[seek] = 0;
	}
	pProgram->ProgramHandle = nullptr;
	pProgram->bLinked = false;
	this->m_ProgramArray.insert(this->m_ProgramArray.end(), pProgram);
	return true;
}
bool cSanShaderManagerGL::iCreateProgram(const SString strProgramName, const SANSHADERPROGRAMDESC Desc)
{
	if(!this->iCreateProgram(strProgramName))
	{
		return false;
	}
	lpSANSHADERPROGRAM pProgram = this->_Search(strProgramName);
	if (pProgram == nullptr)
	{
		return false;
	}
	uint32 ShaderID;
	if (Desc.bUsePath)
	{
		SStringA strCode;
		for (uint8 seek = 0; seek < SST_SHADER_COUNT; seek = seek + 1)
		{
			if (Desc.ShaderStringList[seek].empty())
			{
				this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Source code file path is empty")));
			}
			if (!gloLoadFile(::gloAToT(Desc.ShaderStringList[seek]), strCode))
			{
				this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Load source code file failed")));
			}
			ShaderID = this->iCreateShaderObj(strCode, eSANSHADERTYPE(seek));
			if (ShaderID == 0)
			{
				this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Load shader object failed")));
			}
			this->iAttachShaderToProgram(strProgramName, ShaderID);
			this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Attach shader object failed")));
		}
	}
	else
	{
		for (uint8 seek = 0; seek < SST_SHADER_COUNT; seek = seek + 1)
		{
			if (Desc.ShaderStringList[seek].empty())
			{
				this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Source code string is empty")));
			}
			ShaderID = this->iCreateShaderObj(Desc.ShaderStringList[seek], eSANSHADERTYPE(seek));
			if (ShaderID == 0)
			{
				this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Load shader object failed")));
			}
			this->iAttachShaderToProgram(strProgramName, ShaderID);
			this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Attach shader object failed")));
		}
	}
	return true;
}
bool cSanShaderManagerGL::iCopyProgram(const SString &strSrcProgramName, const SString &strDestProgramName)
{
	if ((strSrcProgramName.empty()) || (strDestProgramName.empty()))
	{
		return false;
	}
	if (this->_Search(strDestProgramName) != nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Current program name already in use")));
		return false;
	}
	lpSANSHADERPROGRAM pSrcProgram = this->_Search(strSrcProgramName);
	if (pSrcProgram == nullptr)
	{
		return false;
	}
	lpSANSHADERPROGRAM pDestProgram = new SANSHADERPROGRAM;
	lpSANSHADEROBJ pShader = nullptr;
	pDestProgram->Desc.strProgramName = strDestProgramName;
	pDestProgram->Desc.bUsePath = pSrcProgram->Desc.bUsePath;
	for (uint8 seek = 0; seek < SST_SHADER_COUNT; seek = seek + 1)
	{
		pShader = pSrcProgram->ShaderPtrList[seek];
		if (pShader != nullptr)
		{
			pShader->RefList.push_back(pDestProgram);
		}
		pDestProgram->ShaderPtrList[seek] = pShader;
		pDestProgram->Desc.ShaderStringList[seek] = pSrcProgram->Desc.ShaderStringList[seek];
		pDestProgram->Desc.ShaderIDList[seek] = pSrcProgram->Desc.ShaderIDList[seek];
	}
	pDestProgram->ProgramHandle = nullptr;
	pDestProgram->bLinked = false;
	this->m_ProgramArray.insert(this->m_ProgramArray.end(), pDestProgram);
	return true;
}
void cSanShaderManagerGL::iReleaseProgram(const SString &strProgramName, bool bForce)
{
	lpSANSHADERPROGRAM pProgram=nullptr;
	if(this->m_CurrentProgramPtr!=nullptr)
	{
		if(this->m_CurrentProgramPtr->Desc.strProgramName==strProgramName)
		{
			if(bForce)
			{
				pProgram=this->m_CurrentProgramPtr;
				this->iEnableProgram();
			}
			else
			{
				this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Program still running on the GUP.")));
				return;
			}
		}
	}
	if(pProgram==nullptr)
	{
		pProgram=this->_Search(strProgramName);
	}
	if(pProgram==nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Can not find the program - ")+strProgramName));
		return;
	}
	this->_Release(pProgram);
}
bool cSanShaderManagerGL::iAttachVariableToShader(const uint32 ShaderID, const SANSHADERVAR &Var)
{
	if (Var.iGetVariableType() == SSDT_NULL)
	{
		return false;
	}
	lpSANSHADEROBJ pShader = this->_Search(ShaderID);
	if (pShader == nullptr)
	{
		return false;
	}
	uint32 size = pShader->Desc.VariableList.size();
	for (uint32 seek = 0; seek < size; seek = seek + 1)
	{
		if (pShader->Desc.VariableList[seek].description.iGetVariableName() == Var.iGetVariableName())
		{
			return false;
		}
	}
	stSANUNKNOWNEX<SANSHADERVAR, uint32> Variable(Var, 0);
	::glGenBuffers(1, (GLuint*) &(Variable.value));
	pShader->Desc.VariableList.insert(pShader->Desc.VariableList.end(), Variable);
	return true;
}
void cSanShaderManagerGL::iDetachVariableFromShader(const uint32 ShaderID, const SString &VarName)
{
	lpSANSHADEROBJ pShader = this->_Search(ShaderID);
	if (pShader == nullptr)
	{
		return;
	}
	vector<stSANUNKNOWNEX<SANSHADERVAR,uint32>>::iterator pVar = pShader->Desc.VariableList.begin();
	while (pVar != pShader->Desc.VariableList.end())
	{
		if (pVar->description.iGetVariableName() == ::gloTToA(VarName))
		{
			::glDeleteBuffers(1, (GLuint*) &(pVar->value));
			pShader->Desc.VariableList.erase(pVar);
			return;
		}
		pVar++;
	}
}
bool cSanShaderManagerGL::iAttachShaderToProgram(const SString &strProgramName, const uint32 ShaderID)
{
	lpSANSHADERPROGRAM pProgram = this->_Search(strProgramName);
	lpSANSHADEROBJ pShader = this->_Search(ShaderID);
	if (pProgram == nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Can not find the program - ") + strProgramName));
		return false;
	}
	if (pShader == nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Can not find the shader")));
		return false;
	}
	if (pShader->Desc.Type >= SST_SHADER_COUNT)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Wrong shader type")));
		return false;
	}
	pProgram->bLinked = false;
	pProgram->ShaderPtrList[pShader->Desc.Type] = pShader;
	pShader->RefList.insert(pShader->RefList.end(), pProgram);
	this->iUpdateProgram();
	return true;
}
void cSanShaderManagerGL::iDetachShaderFromProgram(const SString &strProgramName, const eSANSHADERTYPE Type)
{
	lpSANSHADERPROGRAM pProgram = this->_Search(strProgramName);
	if (pProgram == nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Can not find the program - ") + strProgramName));
		return;
	}
	if (Type >= ::eSANSHADERTYPE::SST_SHADER_COUNT)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Wrong shader type")));
		return;
	}
	if (pProgram->ShaderPtrList[Type] == nullptr)
	{
		return;
	}
	list<::lpSANSHADERPROGRAM>::iterator pPro = pProgram->ShaderPtrList[Type]->RefList.begin();
	while (pPro != pProgram->ShaderPtrList[Type]->RefList.end())
	{
		if ((*pPro) == pProgram)
		{
			pProgram->ShaderPtrList[Type]->RefList.erase(pPro);
			break;
		}
		pPro++;
	}
	pProgram->bLinked = false;
	pProgram->ShaderPtrList[Type] = nullptr;
	this->iUpdateProgram();
	return;
}
bool cSanShaderManagerGL::iCompileShader(const uint32 ShaderID, SString* pstrCompileInfo)
{
	lpSANSHADEROBJ pShader=this->_Search(ShaderID);
	if(pShader==nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Can not find the shader")));
		return false;
	}
	bool bRes=this->_Compile(pShader);
	if ((pstrCompileInfo != nullptr) && (!pShader->CompileLogList.empty()))
	{
		*pstrCompileInfo = pShader->CompileLogList.back();
	}
	this->iUpdateProgram();
	return bRes;
}
bool cSanShaderManagerGL::iBuildProgram(const SString &strProgramName, SString* pstrBuildInfo)
{
	lpSANSHADERPROGRAM pProgram=this->_Search(strProgramName);
	if(pProgram==nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Can not find the program - ")+strProgramName));
		return false;
	}
	pProgram->bLinked = false;
	if (pProgram == this->m_CurrentProgramPtr)
	{
		this->iUpdateProgram();
	}
	else
	{
		this->_Build(pProgram);
	}
	if ((pstrBuildInfo != nullptr) && (!pProgram->LinkLogList.empty()))
	{
		*pstrBuildInfo = pProgram->LinkLogList.back();
	}
	return pProgram->bLinked;
}
bool cSanShaderManagerGL::iSetShaderCode(const uint32 ShaderID, const SStringA &strCodeString)
{
	if(strCodeString.empty())
	{
		return false;
	}
	lpSANSHADEROBJ pShader=this->_Search(ShaderID);
	if(pShader==nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Can not find the shader")));
		return false;
	}
	if (this->_IsSystemShader(pShader))
	{
		return false;
	}
	pShader->bCompiled=false;
	pShader->Desc.strSrcCode=strCodeString;
	pShader->Desc.strCodePath.clear();
	list<::lpSANSHADERPROGRAM>::iterator pPro=pShader->RefList.begin();
	while (pPro != pShader->RefList.end())
	{
		if ((*pPro) != nullptr)
		{
			(*pPro)->bLinked = false;
		}
		pPro++;
	}
	this->iUpdateProgram();
	return true;
}
bool cSanShaderManagerGL::iSetShaderCodeByFile(const uint32 ShaderID, const SString &strCodeFilePath)
{
	if (strCodeFilePath.empty())
	{
		return false;
	}
	lpSANSHADEROBJ pShader = this->_Search(ShaderID);
	if (pShader == nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Can not find the shader")));
		return false;
	}
	if (this->_IsSystemShader(pShader))
	{
		return false;
	}
	SStringA strSrcCode;
	if (::gloLoadFile(strCodeFilePath, strSrcCode))
	{
		return false;
	}
	pShader->bCompiled = false;
	pShader->Desc.strSrcCode = strSrcCode;
	pShader->Desc.strCodePath.clear();
	list<::lpSANSHADERPROGRAM>::iterator pPro = pShader->RefList.begin();
	while (pPro != pShader->RefList.end())
	{
		if ((*pPro) != nullptr)
		{
			(*pPro)->bLinked = false;
		}
		pPro++;
	}
	this->iUpdateProgram();
	return true;
}
bool cSanShaderManagerGL::iSetShaderType(const uint32 ShaderID, const eSANSHADERTYPE Type)
{
	if (Type == ::eSANSHADERTYPE::SST_SHADER_COUNT)
	{
		return false;
	}
	lpSANSHADEROBJ pShader = this->_Search(ShaderID);
	if (pShader == nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Can not find the shader")));
		return false;
	}
	if (this->_IsSystemShader(pShader))
	{
		return false;
	}
	pShader->bCompiled = false;
	list<lpSANSHADERPROGRAM>::iterator pPro = pShader->RefList.begin();
	while (pPro != pShader->RefList.end())
	{
		if ((*pPro) != nullptr)
		{
			(*pPro)->bLinked = false;
			this->iDetachShaderFromProgram((*pPro)->Desc.strProgramName, pShader->Desc.Type);
			if ((*pPro)->ShaderPtrList[Type] != nullptr)
			{
				this->iAttachShaderToProgram((*pPro)->Desc.strProgramName, pShader->ObjID);
			}
		}
		pPro++;
	}
	this->iUpdateProgram();
	pShader->Desc.Type = Type;
	return true;
}
bool cSanShaderManagerGL::iSetShaderDesc(const uint32 ShaderID, const SANSHADERDESC Desc)
{
	lpSANSHADEROBJ pShader = this->_Search(ShaderID);
	bool bUpdated = false;
	if ((pShader == nullptr) || (this->_IsSystemShader(pShader)))
	{
		return false;
	}
	if (!Desc.strCodePath.empty())
	{
		if (Desc.strCodePath.empty())
		{
			SStringA strCode;
			if (!::gloLoadFile(Desc.strCodePath, strCode))
			{
				return false;
			}
			pShader->bCompiled = false;
			pShader->Desc.strSrcCode = strCode;
			pShader->Desc.strCodePath.clear();
			bUpdated = true;
		}
	}
	if (!Desc.strSrcCode.empty())
	{
		pShader->bCompiled = false;
		pShader->Desc.strSrcCode = Desc.strSrcCode;
		pShader->Desc.strCodePath.clear();
		bUpdated = true;
	}
	list<::lpSANSHADERPROGRAM>::iterator pPro = pShader->RefList.begin();
	if (Desc.Type != pShader->Desc.Type)
	{
		pShader->bCompiled = false;
		if (bUpdated)
		{
			while (pPro != pShader->RefList.end())
			{
				if ((*pPro) != nullptr)
				{
					(*pPro)->bLinked = false;
					this->iDetachShaderFromProgram((*pPro)->Desc.strProgramName, pShader->Desc.Type);
					if ((*pPro)->ShaderPtrList[Desc.Type] != nullptr)
					{
						this->iAttachShaderToProgram((*pPro)->Desc.strProgramName, pShader->ObjID);
					}
				}
				pPro++;
			}
		}
	}
	else
	{
		if (bUpdated)
		{
			while (pPro != pShader->RefList.end())
			{
				if ((*pPro) != nullptr)
				{
					(*pPro)->bLinked = false;
				}
				pPro++;
			}
		}
	}
	this->iUpdateProgram();
	/*Update Variable List*/
}
bool cSanShaderManagerGL::iSetManagerLogBufferSize(const uint32 Size)
{
	if (Size < 32)
	{
		return false;
	}
	if (this->m_pManagerLogBuffer != nullptr)
	{
		delete [] this->m_pManagerLogBuffer;
		this->m_pManagerLogBuffer = nullptr;
	}
	this->m_pManagerLogBuffer = new sachar[Size];
	this->m_ManagerLogBufferSize = Size;
	return true;
}
bool cSanShaderManagerGL::iSetUBOStorageBufferSize(const uint32 Size)
{
	if (Size < 32)
	{
		return false;
	}
	this->m_UBOStorageBuffer.iReSizeStream(Size);
	return true;
}
bool cSanShaderManagerGL::iGetShaderVariable(const uint32 ShaderID, const SString &strVarName, SANSHADERVAR &Var)
{
	if (strVarName.empty())
	{
		return false;
	}
	SStringA strName = ::gloTToA(strVarName);
	lpSANSHADEROBJ pShader=this->_Search(ShaderID);
	if(pShader==nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Can not find the shader")));
		return false;
	}
	uint32 size = pShader->Desc.VariableList.size();
	for(uint32 seek = 0; seek < size; seek = seek + 1)
	{
		if (pShader->Desc.VariableList[seek].description.iGetVariableName() == strName)
		{
			Var = pShader->Desc.VariableList[seek].description;
			return true;
		}
	}
	return false;
}
vector<SANSHADERVAR> cSanShaderManagerGL::iGetShaderUniformVarList(const uint32 ShaderID)
{
	lpSANSHADEROBJ pShader=this->_Search(ShaderID);
	vector<SANSHADERVAR> List;
	if(pShader==nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Can not find the shader")));
		return List;
	}
	uint32 size = pShader->Desc.VariableList.size();
	for(uint32 seek = 0; seek < size; seek = seek + 1)
	{
		if ((pShader->Desc.VariableList[seek].description.iGetVariableType() == SSDT_UNIFORM) || (pShader->Desc.VariableList[seek].description.iGetVariableType() == SSDT_UNIFORM_BLOCK))
		{
			List.insert(List.end(), pShader->Desc.VariableList[seek].description);
		}
	}
	return List;
}
vector<SANSHADERVAR> cSanShaderManagerGL::iGetShaderAttributeVarList(const uint32 ShaderID)
{
	lpSANSHADEROBJ pShader = this->_Search(ShaderID);
	vector<SANSHADERVAR> List;
	if (pShader == nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Can not find the shader")));
		return List;
	}
	uint32 size = pShader->Desc.VariableList.size();
	for(uint32 seek = 0; seek < size; seek = seek + 1)
	{
		if (pShader->Desc.VariableList[seek].description.iGetVariableType() == SSDT_ATTRIBUTE)
		{
			List.insert(List.end(), pShader->Desc.VariableList[seek].description);
		}
	}
	return List;
}
vector<SANSHADERVAR> cSanShaderManagerGL::iGetShaderVariableList(const uint32 ShaderID)
{
	lpSANSHADEROBJ pShader=this->_Search(ShaderID);
	if(pShader==nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Can not find the shader")));
		return vector<SANSHADERVAR>();
	}
	vector<SANSHADERVAR> VarList;
	uint32 size = pShader->Desc.VariableList.size();
	for (uint32 seek = 0; seek < size; seek = seek + 1)
	{
		VarList.push_back(pShader->Desc.VariableList[seek].description);
	}
	return VarList;
}
SStringA cSanShaderManagerGL::iGetShaderCodeString(const uint32 ShaderID) const
{
	lpSANSHADEROBJ pShader=this->_Search(ShaderID);
	if(pShader==nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Can not find the shader")));
		return SStringA("");
	}
	return pShader->Desc.strSrcCode;
}
SString cSanShaderManagerGL::iGetShaderCodeFilePath(const uint32 ShaderID) const
{
	lpSANSHADEROBJ pShader=this->_Search(ShaderID);
	if(pShader==nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Can not find the shader")));
		return SString(_SSTR(""));
	}
	return pShader->Desc.strCodePath;
}
eSANSHADERTYPE cSanShaderManagerGL::iGetShaderType(const uint32 ShaderID) const
{
	lpSANSHADEROBJ pShader=this->_Search(ShaderID);
	if(pShader==nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Can not find the shader")));
		return SST_SHADER_COUNT;
	}
	return pShader->Desc.Type;
}
SANSHADERDESC cSanShaderManagerGL::iGetShaderDesc(const uint32 ShaderID) const
{
	SANSHADERDESC Desc;
	Desc.Type = SST_SHADER_COUNT;
	lpSANSHADEROBJ pShader = this->_Search(ShaderID);
	if (pShader == nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(), SString(_SSTR("Error: Can not find the shader")));
		return Desc;
	}
	return pShader->Desc;
}
uint32 cSanShaderManagerGL::iGetShaderIDByProgram(const SString &strProgramName, const eSANSHADERTYPE Type)
{
	if(Type==::eSANSHADERTYPE::SST_SHADER_COUNT)
	{
		return 0;
	}
	lpSANSHADERPROGRAM pProgram = pProgram = this->_Search(strProgramName);
	if(pProgram==nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Can not find the program - ")+strProgramName));
		return 0;
	}
	if(pProgram->ShaderPtrList[Type]!=nullptr)
	{
		return pProgram->ShaderPtrList[Type]->ObjID;
	}
	return 0;
}
SString cSanShaderManagerGL::iGetCurrentProgramName()
{
	if(this->m_CurrentProgramPtr!=nullptr)
	{
		return this->m_CurrentProgramPtr->Desc.strProgramName;
	}
	return SString(_SSTR(""));
}
list<SString> cSanShaderManagerGL::iGetShaderLogList(const uint32 ShaderID)
{
	lpSANSHADEROBJ pShader=this->_Search(ShaderID);
	if(pShader==nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Can not find the shader")));
		return list<SString>();
	}
	return pShader->CompileLogList;
}
list<SString> cSanShaderManagerGL::iGetProgramLogList(const SString &strProgramName)
{
	lpSANSHADERPROGRAM pProgram = this->_Search(strProgramName);
	if(pProgram==nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Can not find the program - ")+strProgramName));
		return list<SString>();
	}
	return pProgram->LinkLogList;
}
list<SString> cSanShaderManagerGL::iGetManagerLogList()
{
	return this->m_ErrorList;
}
uint32 cSanShaderManagerGL::iGetManagerLogBufferSize() const
{
	return this->m_ManagerLogBufferSize;
}
uint32 cSanShaderManagerGL::iGetUBOStorageBufferSize() const
{
	return this->m_UBOStorageBuffer.Size;
}
void cSanShaderManagerGL::iUpdateShader(const uint32 ShaderID)
{
	lpSANSHADEROBJ pShader=this->_Search(ShaderID);
	if(pShader==nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Can not find the shader")));
		return;
	}
	if(!pShader->bCompiled)
	{
		this->_Compile(pShader);
		list<lpSANSHADERPROGRAM>::iterator pPro=pShader->RefList.begin();
		while (pPro != pShader->RefList.end())
		{
			(*pPro)->bLinked = false;
			pPro++;
		}
		this->iUpdateProgram();
	}
}
void cSanShaderManagerGL::iUpdateProgram()
{
	if ((this->m_CurrentProgramPtr != nullptr) && (!this->m_CurrentProgramPtr->bLinked))
	{
		if (!this->iCopyProgram(this->m_CurrentProgramPtr->Desc.strProgramName, _SSTR("_SanDynamicShader")))
		{
			this->iEnableProgram();
			return;
		}
		lpSANSHADERPROGRAM pProgram = this->_Search(_SSTR("_SanDynamicShader"));
		if (pProgram == nullptr)
		{
			this->iEnableProgram();
			return;
		}
		this->_Build(pProgram);
		if (pProgram->bLinked)
		{
			::glUseProgram((GLuint) pProgram->ProgramHandle);
			::glDeleteProgram((GLuint)this->m_CurrentProgramPtr->ProgramHandle);
			this->m_CurrentProgramPtr->ProgramHandle = pProgram->ProgramHandle;
			pProgram->ProgramHandle = 0;
			this->m_CurrentProgramPtr->bLinked = true;
			if (!pProgram->LinkLogList.empty())
			{
				this->m_CurrentProgramPtr->LinkLogList.insert(this->m_CurrentProgramPtr->LinkLogList.end(), pProgram->LinkLogList.back());
			}
		}
		else
		{
			this->iEnableProgram();
		}
		this->iReleaseProgram(_SSTR("_SanDynamicShader"));
	}
	this->iUpdateAttributeVarList();
	this->iUpdateUniformVarList();
}
void cSanShaderManagerGL::iUpdateProgram(const SString &strProgramName)
{
	if ((this->m_CurrentProgramPtr != nullptr) && (this->m_CurrentProgramPtr->Desc.strProgramName == strProgramName))
	{
		this->iUpdateProgram();
		return;
	}
	lpSANSHADERPROGRAM pProgram = this->_Search(strProgramName);
	if(pProgram==nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Can not find the program - ")+strProgramName));
		return;
	}
	if(!pProgram->bLinked)
	{
		this->_Build(pProgram);
	}
}
void cSanShaderManagerGL::iUpdateUniformVar(const SString &strVarName)
{
	lpSANSHADERPROGRAM pProgram = this->m_CurrentProgramPtr;
	if (pProgram->ProgramHandle == nullptr)
	{
		return;
	}
	lpSANSHADEROBJ pShader = nullptr;
	for (uint32 seek = 0; seek < ::eSANSHADERTYPE::SST_SHADER_COUNT; seek = seek + 1)
	{
		pShader = pProgram->ShaderPtrList[seek];
		if (pShader != nullptr)
		{
			vector<stSANUNKNOWNEX<SANSHADERVAR, uint32>>::iterator pVar = pShader->Desc.VariableList.begin();
			while (pVar != pShader->Desc.VariableList.end())
			{
				if (((pVar->description.iGetVariableType() == SSDT_UNIFORM) || (pVar->description.iGetVariableType() == SSDT_UNIFORM_BLOCK)) && (pVar->description.iGetVariableName() == ::gloTToA(strVarName)))
				{
					this->_UpdateUniformVariable(pProgram, *pVar);
				}
				pVar++;
			}
		}
	}
}
void cSanShaderManagerGL::iUpdateUniformVarList()
{
	lpSANSHADERPROGRAM pProgram = this->m_CurrentProgramPtr;
	if (pProgram == nullptr)
	{
		return;
	}
	if (pProgram->ProgramHandle == nullptr)
	{
		return;
	}
	lpSANSHADEROBJ pShader = nullptr;
	for (uint32 seek = 0; seek < SST_SHADER_COUNT; seek = seek + 1)
	{
		pShader = pProgram->ShaderPtrList[seek];
		if (pShader != nullptr)
		{
			vector<stSANUNKNOWNEX<SANSHADERVAR, uint32>>::iterator pVar = pShader->Desc.VariableList.begin();
			while (pVar != pShader->Desc.VariableList.end())
			{
				if ((pVar->description.iGetVariableType() == SSDT_UNIFORM)||(pVar->description.iGetVariableType()==SSDT_UNIFORM_BLOCK))
				{
					this->_UpdateUniformVariable(pProgram, *pVar);
				}
				pVar++;
			}
		}
	}
}
void cSanShaderManagerGL::iUpdateAttributeVar(const SString &strVarName)
{
	lpSANSHADERPROGRAM pProgram = this->m_CurrentProgramPtr;
	if (pProgram == nullptr)
	{
		return;
	}
	if (pProgram->ProgramHandle == nullptr)
	{
		if (pProgram == this->m_CurrentProgramPtr)
		{
			this->iUpdateProgram();
			return;
		}
		if (!this->_Build(pProgram))
		{
			return;
		}
	}
	lpSANSHADEROBJ pShader = nullptr;
	for (uint32 seek = 0; seek < SST_SHADER_COUNT; seek = seek + 1)
	{
		pShader = pProgram->ShaderPtrList[seek];
		if (pShader != nullptr)
		{
			vector<stSANUNKNOWNEX<SANSHADERVAR, uint32>>::iterator pVar = pShader->Desc.VariableList.begin();
			while (pVar != pShader->Desc.VariableList.end())
			{
				if ((pVar->description.iGetVariableType() == SSDT_ATTRIBUTE) && (pVar->description.iGetVariableName() == ::gloTToA(strVarName)))//gloTToA
				{
					this->_UpdateAttributeVariable(pProgram, *pVar);
				}
				pVar++;
			}
		}
	}
}
void cSanShaderManagerGL::iUpdateAttributeVarList()
{
	lpSANSHADERPROGRAM pProgram = this->m_CurrentProgramPtr;
	if (pProgram == nullptr)
	{
		return;
	}
	if (pProgram->ProgramHandle == nullptr)
	{
		return;
	}
	lpSANSHADEROBJ pShader = nullptr;
	for (uint32 seek = 0; seek < ::eSANSHADERTYPE::SST_SHADER_COUNT; seek = seek + 1)
	{
		pShader = pProgram->ShaderPtrList[seek];
		if (pShader != nullptr)
		{
			SANSTREAM Str;
			vector<stSANUNKNOWNEX<SANSHADERVAR, uint32>>::iterator pVar = pShader->Desc.VariableList.begin();
			while (pVar != pShader->Desc.VariableList.end())
			{
				if (pVar->description.iGetVariableType() == SSDT_ATTRIBUTE)
				{
					this->_UpdateAttributeVariable(pProgram, *pVar);
				}
				pVar++;
			}
		}
	}
}
void cSanShaderManagerGL::iEnableShaderObj(uint32 ShaderID)
{
	lpSANSHADEROBJ pShader=this->_Search(ShaderID);
	if(pShader==nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Can not find the shader")));
		return;
	}
	this->_Running(pShader);
}
void cSanShaderManagerGL::iEnableProgram(const SString &strProgramName)
{
	if(strProgramName.empty())
	{
		return;
	}
	if(strProgramName==_SSTR("Default"))
	{
		switch (this->m_ShaderModeVersion)
		{
		case 100:
			this->m_CurrentProgramPtr = nullptr;
			::glUseProgram(0);
			break;
		case 200:
			this->_Running(this->_Search(_SSTR("SanDefaultShaderSM2Dot0")));
			break;
		case 430:
			this->_Running(this->_Search(_SSTR("SanDefaultShaderSM4Dot3")));
			break;
		default:
			this->m_CurrentProgramPtr = nullptr;
			::glUseProgram(0);
			break;
		}
		return;
	}
	if((this->m_CurrentProgramPtr!=nullptr)&&(this->m_CurrentProgramPtr->Desc.strProgramName==strProgramName))
	{
		if (this->m_CurrentProgramPtr->bLinked)
		{
			return;
		}
		else
		{
			this->iUpdateProgram();
			return;
		}
	}
	lpSANSHADERPROGRAM pProgram = this->_Search(strProgramName);
	if(pProgram==nullptr)
	{
		this->m_ErrorList.insert(this->m_ErrorList.end(),SString(_SSTR("Error: Can not find the program - ")+strProgramName));
		return;
	}
	if (!this->_Running(pProgram))
	{
		switch (this->m_ShaderModeVersion)
		{
		case 100:
			this->m_CurrentProgramPtr = nullptr;
			::glUseProgram(0);
			break;
		case 200:
			this->_Running(this->_Search(_SSTR("SanDefaultShaderSM2Dot0")));
			break;
		case 430:
			this->_Running(this->_Search(_SSTR("SanDefaultShaderSM4Dot3")));
			break;
		default:
			this->m_CurrentProgramPtr = nullptr;
			::glUseProgram(0);
			break;
		}
	}
}
#endif