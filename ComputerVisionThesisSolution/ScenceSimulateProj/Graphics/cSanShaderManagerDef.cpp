#include"../Core/SanMathematics.h"
#include"cSanShaderManagerDef.h"
using namespace San;
using namespace San::Mathematics;
using namespace San::Graphics;
San::Graphics::stSANSVARIABLE::stSANSVARIABLE(const SStringA &strVarName,const uint32 Type)
	:strName(strVarName),
	Type(eSANSHADERDATATYPE(Type&SSDT_TYPE_MASK)),
	Instance(0),
	bChanged(true)
{
	SANSHADERSUBVAR Var;
	switch (this->Type)
	{
	case SSDT_UNIFORM:
	case SSDT_ATTRIBUTE:
		Var.strName = this->strName + "." + strVarName;
		Var.Type = (eSANSHADERDATATYPE) (Type&(~SSDT_TYPE_MASK));
		Var.Size = 1;
		switch (Var.Type)
		{
		case SSDT_INT:
		case SSDT_FLOAT:
		case SSDT_BOOL:
			Var.Data.iReSizeStream(1 * Var.Size);
			break;
		case SSDT_INT2:
		case SSDT_FLOAT2:
		case SSDT_BOOL2:
			Var.Data.iReSizeStream(2 * Var.Size);
			break;
		case SSDT_INT3:
		case SSDT_FLOAT3:
		case SSDT_BOOL3:
		case SSDT_VECTOR3:
			Var.Data.iReSizeStream(3 * Var.Size);
			break;
		case SSDT_INT4:
		case SSDT_FLOAT4:
		case SSDT_BOOL4:
		case SSDT_VECTOR4:
			Var.Data.iReSizeStream(4 * Var.Size);
		case SSDT_MAT3X3:
			Var.Data.iReSizeStream(9 * Var.Size);
			break;
		case SSDT_MAT4X4:
			Var.Data.iReSizeStream(16 * Var.Size);
			break;
		default:
			break;
		}
		this->SubVarList.push_back(Var);
		break;
	case SSDT_UNIFORM_BLOCK:
		break;
	default:
		break;
	}
}
San::Graphics::stSANSVARIABLE::stSANSVARIABLE(const San::Graphics::stSANSVARIABLE &Var)
{
	this->_Release();
	this->strName=Var.strName;
	this->Type=Var.Type;
	this->SubVarList = Var.SubVarList;
	this->Instance = Var.Instance;
	this->bChanged = true;
}
San::Graphics::stSANSVARIABLE::~stSANSVARIABLE()
{
	this->_Release();
}
void San::Graphics::stSANSVARIABLE::_Release()
{
	uint32 size = this->SubVarList.size();
	for (uint32 seek = 0; seek < size; seek = seek + 1)
	{
		this->SubVarList[seek].strName.clear();
		this->SubVarList[seek].Type = SSDT_NULL;
		this->SubVarList[seek].Data.iReSizeStream(0);
		this->SubVarList[seek].Size = 0;
	}
	this->SubVarList.clear();
	this->Type = SSDT_NULL;
	this->strName.clear();
}
San::Graphics::stSANSVARIABLE& San::Graphics::stSANSVARIABLE::operator=(const San::Graphics::stSANSVARIABLE& Var)
{
	this->strName=Var.strName;
	this->Type=Var.Type;
	this->SubVarList = Var.SubVarList;
	this->Instance = Var.Instance;
	this->bChanged = true;
	return *this;
}
const San::Graphics::SANSHADERSUBVAR& San::Graphics::stSANSVARIABLE::operator[](const size_t Position) const
{
	return this->SubVarList[Position];
}
San::Graphics::SANSHADERSUBVAR& San::Graphics::stSANSVARIABLE::operator[](const size_t Position)
{
	return this->SubVarList[Position];
}
bool San::Graphics::stSANSVARIABLE::iAddSubVariable(const SStringA &strSubVarName, const eSANSHADERDATATYPE Type,const uint32 Size)
{
	if (this->Type != SSDT_UNIFORM_BLOCK)
	{
		return false;
	}
	if (strSubVarName.empty())
	{
		return false;
	}
	if (this->iGetSubVariableIndex(strSubVarName) != -1)
	{
		return false;
	}
	SANSHADERSUBVAR Var;
	Var.strName = this->strName + "." + strSubVarName;
	Var.Type = Type;
	Var.Size = Size;
	switch (Var.Type)
	{
	case SSDT_INT:
	case SSDT_FLOAT:
	case SSDT_BOOL:
		Var.Data.iReSizeStream(1 * Size);
		break;
	case SSDT_INT2:
	case SSDT_FLOAT2:
	case SSDT_BOOL2:
		Var.Data.iReSizeStream(2 * Size);
		break;
	case SSDT_INT3:
	case SSDT_FLOAT3:
	case SSDT_BOOL3:
	case SSDT_VECTOR3:
		Var.Data.iReSizeStream(3 * Size);
		break;
	case SSDT_INT4:
	case SSDT_FLOAT4:
	case SSDT_BOOL4:
	case SSDT_VECTOR4:
		Var.Data.iReSizeStream(4 * Size);
	case SSDT_MAT3X3:
		Var.Data.iReSizeStream(9 * Size);
		break;
	case SSDT_MAT4X4:
		Var.Data.iReSizeStream(16 * Size);
		break;
	default:
		break;
	}
	this->SubVarList.push_back(Var);
	return true;
}
bool San::Graphics::stSANSVARIABLE::iDeleteSubVariable(const SStringA &strSubVarName)
{
	if (this->Type != SSDT_UNIFORM_BLOCK)
	{
		return false;
	}
	if (strSubVarName.empty())
	{
		return false;
	}
	SStringA strVName = this->strName + "." + strSubVarName;
	vector<SANSHADERSUBVAR>::iterator pSubVar = this->SubVarList.begin();
	while (pSubVar!=this->SubVarList.end())
	{
		if (pSubVar->strName == strVName)
		{
			break;
		}
		pSubVar++;
	}
	if (pSubVar == this->SubVarList.end())
	{
		return false;
	}
	return true;
}
void San::Graphics::stSANSVARIABLE::iSetVariableInstance(const uint32 Instance)
{
	this->Instance = Instance;
}
void San::Graphics::stSANSVARIABLE::iSetVariableState(const bool bState)
{
	this->bChanged = bState;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const int32 Data)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, Data);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const int32 Data)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_INT)
	{
		return;
	}
	this->SubVarList[Index].Data[0] = Data;
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const int32 Data1, const int32 Data2)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, Data1,Data2);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const int32 Data1, const int32 Data2)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_INT2)
	{
		return;
	}
	this->SubVarList[Index].Data[0] = Data1;
	this->SubVarList[Index].Data[1] = Data2;
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const int32 Data1, const int32 Data2, const int32 Data3)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, Data1, Data2,Data3);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const int32 Data1, const int32 Data2, const int32 Data3)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_INT3)
	{
		return;
	}
	this->SubVarList[Index].Data[0] = Data1;
	this->SubVarList[Index].Data[1] = Data2;
	this->SubVarList[Index].Data[2] = Data3;
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const int32 Data1, const int32 Data2, const int32 Data3, const int32 Data4)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, Data1, Data2, Data3,Data4);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const int32 Data1, const int32 Data2, const int32 Data3, const int32 Data4)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_INT4)
	{
		return;
	}
	this->SubVarList[Index].Data[0] = Data1;
	this->SubVarList[Index].Data[1] = Data2;
	this->SubVarList[Index].Data[2] = Data3;
	this->SubVarList[Index].Data[3] = Data4;
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const int32* pData, uint32 Size)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, pData, Size);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const int32* pData, uint32 Size)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	uint32 UnitSize = 1;
	switch (this->SubVarList[Index].Type)
	{
	case SSDT_INT:
		UnitSize = 1;
	case SSDT_INT2:
		UnitSize = 2;
	case SSDT_INT3:
		UnitSize = 3;
	case SSDT_INT4:
		UnitSize = 4;
	default:
		return;
		break;
	}
	UnitSize = UnitSize*this->SubVarList[Index].Size;
	Size = Size < UnitSize ? Size : UnitSize;
	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		this->SubVarList[Index].Data[seek] = pData[seek];
	}
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const sfloat Data)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, Data);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const sfloat Data)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_FLOAT)
	{
		return;
	}
	this->SubVarList[Index].Data[0] = Data;
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const sfloat Data1, const sfloat Data2)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, Data1, Data2);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const sfloat Data1, const sfloat Data2)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_FLOAT2)
	{
		return;
	}
	this->SubVarList[Index].Data[0] = Data1;
	this->SubVarList[Index].Data[1] = Data2;
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const sfloat Data1, const sfloat Data2, const sfloat Data3)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, Data1, Data2, Data3);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const sfloat Data1, const sfloat Data2, const sfloat Data3)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_FLOAT3)
	{
		return;
	}
	this->SubVarList[Index].Data[0] = Data1;
	this->SubVarList[Index].Data[1] = Data2;
	this->SubVarList[Index].Data[2] = Data3;
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const sfloat Data1, const sfloat Data2, const sfloat Data3, const sfloat Data4)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, Data1, Data2, Data3, Data4);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const sfloat Data1, const sfloat Data2, const sfloat Data3, const sfloat Data4)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_FLOAT4)
	{
		return;
	}
	this->SubVarList[Index].Data[0] = Data1;
	this->SubVarList[Index].Data[1] = Data2;
	this->SubVarList[Index].Data[2] = Data3;
	this->SubVarList[Index].Data[3] = Data4;
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const sfloat* pData, uint32 Size)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, pData, Size);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const sfloat* pData, uint32 Size)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	uint32 UnitSize = 1;
	switch (this->SubVarList[Index].Type)
	{
	case SSDT_FLOAT:
		UnitSize = 1;
	case SSDT_FLOAT2:
		UnitSize = 2;
	case SSDT_FLOAT3:
		UnitSize = 3;
	case SSDT_FLOAT4:
		UnitSize = 4;
	default:
		return;
		break;
	}
	UnitSize = UnitSize*this->SubVarList[Index].Size;
	Size = Size < UnitSize ? Size : UnitSize;
	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		this->SubVarList[Index].Data[seek] = pData[seek];
	}
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const bool Data)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, Data);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const bool Data)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_BOOL)
	{
		return;
	}
	this->SubVarList[Index].Data[0] = Data;
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const bool Data1, const bool Data2)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, Data1, Data2);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const bool Data1, const bool Data2)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_BOOL2)
	{
		return;
	}
	this->SubVarList[Index].Data[0] = Data1;
	this->SubVarList[Index].Data[1] = Data2;
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const bool Data1, const bool Data2, const bool Data3)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, Data1, Data2, Data3);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const bool Data1, const bool Data2, const bool Data3)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_BOOL3)
	{
		return;
	}
	this->SubVarList[Index].Data[0] = Data1;
	this->SubVarList[Index].Data[1] = Data2;
	this->SubVarList[Index].Data[2] = Data3;
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const bool Data1, const bool Data2, const bool Data3, const bool Data4)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, Data1, Data2, Data3, Data4);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const bool Data1, const bool Data2, const bool Data3, const bool Data4)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_BOOL4)
	{
		return;
	}
	this->SubVarList[Index].Data[0] = Data1;
	this->SubVarList[Index].Data[1] = Data2;
	this->SubVarList[Index].Data[2] = Data3;
	this->SubVarList[Index].Data[3] = Data4;
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const bool* pData, uint32 Size)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, pData, Size);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const bool* pData, uint32 Size)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	uint32 UnitSize = 1;
	switch (this->SubVarList[Index].Type)
	{
	case SSDT_BOOL:
		UnitSize = 1;
	case SSDT_BOOL2:
		UnitSize = 2;
	case SSDT_BOOL3:
		UnitSize = 3;
	case SSDT_BOOL4:
		UnitSize = 4;
	default:
		return;
		break;
	}
	UnitSize = UnitSize*this->SubVarList[Index].Size;
	Size = Size < UnitSize ? Size : UnitSize;
	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		this->SubVarList[Index].Data[seek] = pData[seek];
	}
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const SVECTOR3 &Data)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, Data);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const SVECTOR3 &Data)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_VECTOR3)
	{
		return;
	}
	this->SubVarList[Index].Data[0] = Data.x;
	this->SubVarList[Index].Data[1] = Data.y;
	this->SubVarList[Index].Data[2] = Data.z;
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const lpSVECTOR3 pData, uint32 Size)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, pData, Size);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const lpSVECTOR3 pData, uint32 Size)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_VECTOR3)
	{
		return;
	}
	Size = Size < this->SubVarList[Index].Size ? Size : this->SubVarList[Index].Size;
	uint32 BufferIndex = 0;
	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		this->SubVarList[Index].Data[BufferIndex] = pData[seek].x;
		this->SubVarList[Index].Data[BufferIndex + 1] = pData[seek].y;
		this->SubVarList[Index].Data[BufferIndex + 2] = pData[seek].z;
		BufferIndex = BufferIndex + 3;
	}
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const SVECTOR4 &Data)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, Data);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const SVECTOR4 &Data)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_VECTOR4)
	{
		return;
	}
	this->SubVarList[Index].Data[0] = Data.x;
	this->SubVarList[Index].Data[1] = Data.y;
	this->SubVarList[Index].Data[2] = Data.z;
	this->SubVarList[Index].Data[3] = Data.w;
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const lpSVECTOR4 pData, uint32 Size)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, pData, Size);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const lpSVECTOR4 pData, uint32 Size)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_VECTOR4)
	{
		return;
	}
	Size = Size < this->SubVarList[Index].Size ? Size : this->SubVarList[Index].Size;
	uint32 BufferIndex = 0;
	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		this->SubVarList[Index].Data[BufferIndex] = pData[seek].x;
		this->SubVarList[Index].Data[BufferIndex + 1] = pData[seek].y;
		this->SubVarList[Index].Data[BufferIndex + 2] = pData[seek].z;
		this->SubVarList[Index].Data[BufferIndex + 3] = pData[seek].w;
		BufferIndex = BufferIndex + 4;
	}
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const SANMATRIX3X3 &Data)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, Data);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const SANMATRIX3X3 &Data)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_MAT3X3)
	{
		return;
	}
	for (uint8 seek = 0; seek < 9; seek = seek + 1)
	{
		this->SubVarList[Index].Data[seek] = Data.ma[seek];
	}
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const lpSANMATRIX3X3 pData, uint32 Size)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, pData, Size);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const lpSANMATRIX3X3 pData, uint32 Size)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_MAT3X3)
	{
		return;
	}
	Size = Size < this->SubVarList[Index].Size ? Size : this->SubVarList[Index].Size;
	uint32 BufferIndex = 0;
	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		for (uint8 seek_e = 0; seek_e < 9; seek_e = seek_e + 1)
		{
			this->SubVarList[Index].Data[BufferIndex + seek_e] = pData[seek].ma[seek_e];
		}
		BufferIndex = BufferIndex + 9;
	}
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const SANMATRIX4X4 &Data)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, Data);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const SANMATRIX4X4 &Data)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_MAT4X4)
	{
		return;
	}
	for (uint8 seek = 0; seek < 16; seek = seek + 1)
	{
		this->SubVarList[Index].Data[seek] = Data.ma[seek];
	}
	this->bChanged = true;
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const SStringA &strSubVarName, const lpSANMATRIX4X4 pData, uint32 Size)
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		this->iSetSubVariable(Index, pData, Size);
	}
}
void San::Graphics::stSANSVARIABLE::iSetSubVariable(const uint32 Index, const lpSANMATRIX4X4 pData, uint32 Size)
{
	if (Index >= this->SubVarList.size())
	{
		return;
	}
	if (this->SubVarList[Index].Type != SSDT_MAT4X4)
	{
		return;
	}
	Size = Size < this->SubVarList[Index].Size ? Size : this->SubVarList[Index].Size;
	uint32 BufferIndex = 0;
	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		for (uint8 seek_e = 0; seek_e < 16; seek_e = seek_e + 1)
		{
			this->SubVarList[Index].Data[BufferIndex + seek_e] = pData[seek].ma[seek_e];
		}
		BufferIndex = BufferIndex + 16;
	}
	this->bChanged = true;
}
SStringA San::Graphics::stSANSVARIABLE::iGetVariableName() const
{
	return this->strName;
}
SStringA San::Graphics::stSANSVARIABLE::iGetSubVariableName(const uint32 Index) const
{
	if (Index >= this->SubVarList.size())
	{
		return SStringA();
	}
	return ::gloGetStringItemsA(this->SubVarList[Index].strName, ".")[1];
}
int32 San::Graphics::stSANSVARIABLE::iGetVariableInstance() const
{
	return this->Instance;
}
bool San::Graphics::stSANSVARIABLE::iGetVariableState() const
{
	return this->bChanged;
}
eSANSHADERDATATYPE San::Graphics::stSANSVARIABLE::iGetVariableType() const
{
	return this->Type;
}
eSANSHADERDATATYPE San::Graphics::stSANSVARIABLE::iGetSubVariableType(const SStringA &strSubVarName) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	return this->iGetSubVariableType(Index);
}
eSANSHADERDATATYPE San::Graphics::stSANSVARIABLE::iGetSubVariableType(const uint32 Index) const
{
	if ((Index < 0) || (Index >= this->SubVarList.size()))
	{
		return SSDT_NULL;
	}
	return this->SubVarList[Index].Type;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariableSize(const SStringA &strSubVarName) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	return this->iGetSubVariableSize(Index);
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariableSize(const uint32 Index) const
{
	if ((Index < 0) || (Index >= this->SubVarList.size()))
	{
		return SSDT_NULL;
	}
	return this->SubVarList[Index].Size;
}
int32 San::Graphics::stSANSVARIABLE::iGetSubVariableIndex(const SStringA &strSubVarName) const
{
	uint32 size = this->SubVarList.size();
	SStringA strVName = this->strName + "." + strSubVarName;
	for (uint32 seek = 0; seek < size; seek = seek + 1)
	{
		if (this->SubVarList[seek].strName == strVName)
		{
			return seek;
		}
	}
	return -1;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariableListSize() const
{
	return this->SubVarList.size();
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, int32 &Data) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, Data);
	}
	return 0;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, int32 &Data) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_INT)
	{
		return 0;
	}
	Data = this->SubVarList[Index].Data[0];
	return 1;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, int32 &Data1, int32 &Data2) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, Data1, Data2);
	}
	return 0;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, int32 &Data1, int32 &Data2) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_INT2)
	{
		return 0;
	}
	Data1 = this->SubVarList[Index].Data[0];
	Data2 = this->SubVarList[Index].Data[1];
	return 2;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, int32 &Data1, int32 &Data2, int32 &Data3) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, Data1, Data2, Data3);
	}
	return 0;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, int32 &Data1, int32 &Data2, int32 &Data3) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_INT3)
	{
		return 0;
	}
	Data1 = this->SubVarList[Index].Data[0];
	Data2 = this->SubVarList[Index].Data[1];
	Data3 = this->SubVarList[Index].Data[2];
	return 3;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, int32 &Data1, int32 &Data2, int32 &Data3, int32 &Data4) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, Data1, Data2, Data3, Data4);
	}
	return 0;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, int32 &Data1, int32 &Data2, int32 &Data3, int32 &Data4) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_INT4)
	{
		return 0;
	}
	Data1 = this->SubVarList[Index].Data[0];
	Data2 = this->SubVarList[Index].Data[1];
	Data3 = this->SubVarList[Index].Data[2];
	Data4 = this->SubVarList[Index].Data[3];
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, int32* pBuffer, uint32 Size) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, pBuffer, Size);
	}
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, int32* pBuffer, uint32 Size) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	uint32 UnitSize = 1;
	switch (this->SubVarList[Index].Type)
	{
	case SSDT_INT:
		UnitSize = 1;
	case SSDT_INT2:
		UnitSize = 2;
	case SSDT_INT3:
		UnitSize = 3;
	case SSDT_INT4:
		UnitSize = 4;
	default:
		return 0;
		break;
	}
	UnitSize = UnitSize*this->SubVarList[Index].Size;
	Size = Size < UnitSize ? Size : UnitSize;
	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		pBuffer[seek] = this->SubVarList[Index].Data[seek];
	}
	return Size;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, sfloat &Data) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, Data);
	}
	return 0;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, sfloat &Data) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_FLOAT)
	{
		return 0;
	}
	Data = this->SubVarList[Index].Data[0];
	return 1;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, sfloat &Data1, sfloat &Data2) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, Data1, Data2);
	}
	return 0;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, sfloat &Data1, sfloat &Data2) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_FLOAT2)
	{
		return 0;
	}
	Data1 = this->SubVarList[Index].Data[0];
	Data2 = this->SubVarList[Index].Data[1];
	return 2;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, sfloat &Data1, sfloat &Data2, sfloat &Data3) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, Data1, Data2, Data3);
	}
	return 0;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, sfloat &Data1, sfloat &Data2, sfloat &Data3) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_FLOAT3)
	{
		return 0;
	}
	Data1 = this->SubVarList[Index].Data[0];
	Data2 = this->SubVarList[Index].Data[1];
	Data3 = this->SubVarList[Index].Data[2];
	return 3;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, sfloat &Data1, sfloat &Data2, sfloat &Data3, sfloat &Data4) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, Data1, Data2, Data3, Data4);
	}
	return 0;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, sfloat &Data1, sfloat &Data2, sfloat &Data3, sfloat &Data4) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_FLOAT4)
	{
		return 0;
	}
	Data1 = this->SubVarList[Index].Data[0];
	Data2 = this->SubVarList[Index].Data[1];
	Data3 = this->SubVarList[Index].Data[2];
	Data4 = this->SubVarList[Index].Data[3];
	return 4;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, sfloat* pBuffer, uint32 Size) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, pBuffer, Size);
	}
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, sfloat* pBuffer, uint32 Size) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	uint32 UnitSize = 1;
	switch (this->SubVarList[Index].Type)
	{
	case SSDT_FLOAT:
		UnitSize = 1;
	case SSDT_FLOAT2:
		UnitSize = 2;
	case SSDT_FLOAT3:
		UnitSize = 3;
	case SSDT_FLOAT4:
		UnitSize = 4;
	default:
		return 0;
		break;
	}
	UnitSize = UnitSize*this->SubVarList[Index].Size;
	Size = Size < UnitSize ? Size : UnitSize;
	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		pBuffer[seek] = this->SubVarList[Index].Data[seek];
	}
	return Size;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, bool &Data) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, Data);
	}
	return 0;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, bool &Data) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_BOOL)
	{
		return 0;
	}
	Data = (!gloIsFloatEqual(this->SubVarList[Index].Data[0], 0.0));
	return 1;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, bool &Data1, bool &Data2) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, Data1, Data2);
	}
	return 0;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, bool &Data1, bool &Data2) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_BOOL2)
	{
		return 0;
	}
	Data1 = (!gloIsFloatEqual(this->SubVarList[Index].Data[0], 0.0));
	Data2 = (!gloIsFloatEqual(this->SubVarList[Index].Data[1], 0.0));
	return 2;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, bool &Data1, bool &Data2, bool &Data3) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, Data1, Data2, Data3);
	}
	return 0;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, bool &Data1, bool &Data2, bool &Data3) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_BOOL3)
	{
		return 0;
	}
	Data1 = (!gloIsFloatEqual(this->SubVarList[Index].Data[0], 0.0));
	Data2 = (!gloIsFloatEqual(this->SubVarList[Index].Data[1], 0.0));
	Data3 = (!gloIsFloatEqual(this->SubVarList[Index].Data[2], 0.0));
	return 3;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, bool &Data1, bool &Data2, bool &Data3, bool &Data4) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, Data1, Data2, Data3, Data4);
	}
	return 0;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, bool &Data1, bool &Data2, bool &Data3, bool &Data4) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_BOOL4)
	{
		return 0;
	}
	Data1 = (!gloIsFloatEqual(this->SubVarList[Index].Data[0], 0.0));
	Data2 = (!gloIsFloatEqual(this->SubVarList[Index].Data[1], 0.0));
	Data3 = (!gloIsFloatEqual(this->SubVarList[Index].Data[2], 0.0));
	Data4 = (!gloIsFloatEqual(this->SubVarList[Index].Data[3], 0.0));
	return 4;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, bool* pBuffer, uint32 Size) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, pBuffer, Size);
	}
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, bool* pBuffer, uint32 Size) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	uint32 UnitSize = 1;
	switch (this->SubVarList[Index].Type)
	{
	case SSDT_BOOL:
		UnitSize = 1;
	case SSDT_BOOL2:
		UnitSize = 2;
	case SSDT_BOOL3:
		UnitSize = 3;
	case SSDT_BOOL4:
		UnitSize = 4;
	default:
		return 0;
		break;
	}
	UnitSize = UnitSize*this->SubVarList[Index].Size;
	Size = Size < UnitSize ? Size : UnitSize;
	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		pBuffer[seek] = ::gloIsFloatEqual(this->SubVarList[Index].Data[seek], 0.0);
	}
	return Size;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, SVECTOR3 &Data) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, Data);
	}
	return 0;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, SVECTOR3 &Data) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_VECTOR3)
	{
		return 0;
	}
	Data.x = this->SubVarList[Index].Data[0];
	Data.y = this->SubVarList[Index].Data[1];
	Data.z = this->SubVarList[Index].Data[2];
	return 1;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, lpSVECTOR3 pBuffer, uint32 Size) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, pBuffer, Size);
	}
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, lpSVECTOR3 pBuffer, uint32 Size) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_VECTOR3)
	{
		return 0;
	}
	Size = Size < this->SubVarList[Index].Size ? Size : this->SubVarList[Index].Size;
	uint32 BufferIndex = 0;
	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		pBuffer[seek].x = this->SubVarList[Index].Data[BufferIndex];
		pBuffer[seek].y = this->SubVarList[Index].Data[BufferIndex + 1];
		pBuffer[seek].z = this->SubVarList[Index].Data[BufferIndex + 2];
		BufferIndex = BufferIndex + 3;
	}
	return Size;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, SVECTOR4 &Data) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, Data);
	}
	return 0;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, SVECTOR4 &Data) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_VECTOR4)
	{
		return 0;
	}
	Data.x = this->SubVarList[Index].Data[0];
	Data.y = this->SubVarList[Index].Data[1];
	Data.z = this->SubVarList[Index].Data[2];
	Data.w = this->SubVarList[Index].Data[3];
	return 1;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, lpSVECTOR4 pBuffer, uint32 Size) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, pBuffer, Size);
	}
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, lpSVECTOR4 pBuffer, uint32 Size) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_VECTOR4)
	{
		return 0;
	}
	Size = Size < this->SubVarList[Index].Size ? Size : this->SubVarList[Index].Size;
	uint32 BufferIndex = 0;
	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		pBuffer[seek].x = this->SubVarList[Index].Data[BufferIndex];
		pBuffer[seek].y = this->SubVarList[Index].Data[BufferIndex + 1];
		pBuffer[seek].z = this->SubVarList[Index].Data[BufferIndex + 2];
		pBuffer[seek].w = this->SubVarList[Index].Data[BufferIndex + 3];
		BufferIndex = BufferIndex + 4;
	}
	return Size;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, SANMATRIX3X3 &Data) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, Data);
	}
	return 0;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, SANMATRIX3X3 &Data) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_MAT3X3)
	{
		return 0;
	}
	for (uint8 seek = 0; seek < 9; seek = seek + 1)
	{
		Data.ma[seek] = this->SubVarList[Index].Data[seek];
	}
	return 1;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, lpSANMATRIX3X3 pBuffer, uint32 Size) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, pBuffer, Size);
	}
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, lpSANMATRIX3X3 pBuffer, uint32 Size) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_MAT3X3)
	{
		return 0;
	}
	Size = Size < this->SubVarList[Index].Size ? Size : this->SubVarList[Index].Size;
	uint32 BufferIndex = 0;
	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		for (uint8 seek_e = 0; seek_e < 9; seek_e = seek_e + 1)
		{
			pBuffer[seek].ma[seek_e] = this->SubVarList[Index].Data[BufferIndex + seek_e];
		}
		BufferIndex = BufferIndex + 9;
	}
	return Size;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, SANMATRIX4X4 &Data) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, Data);
	}
	return 1;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, SANMATRIX4X4 &Data) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_MAT4X4)
	{
		return 0;
	}
	for (uint8 seek = 0; seek < 16; seek = seek + 1)
	{
		Data.ma[seek] = this->SubVarList[Index].Data[seek];
	}
	return 1;
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const SStringA &strSubVarName, lpSANMATRIX4X4 pBuffer, uint32 Size) const
{
	int32 Index = this->iGetSubVariableIndex(strSubVarName);
	if (Index >= 0)
	{
		return this->iGetSubVariable(Index, pBuffer, Size);
	}
}
uint32 San::Graphics::stSANSVARIABLE::iGetSubVariable(const uint32 Index, lpSANMATRIX4X4 pBuffer, uint32 Size) const
{
	if (Index >= this->SubVarList.size())
	{
		return 0;
	}
	if (this->SubVarList[Index].Type != SSDT_MAT4X4)
	{
		return 0;
	}
	Size = Size < this->SubVarList[Index].Size ? Size : this->SubVarList[Index].Size;
	uint32 BufferIndex = 0;
	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		for (uint8 seek_e = 0; seek_e < 16; seek_e = seek_e + 1)
		{
			pBuffer[seek].ma[seek_e] = this->SubVarList[Index].Data[BufferIndex + seek_e];
		}
		BufferIndex = BufferIndex + 16;
	}
	return Size;
}
San::Graphics::stSANSHADERPROGRAM::stSANSHADERPROGRAM(SString strProgramName)
	:ProgramHandle(nullptr),
	bLinked(false)
{
	Desc.strProgramName = strProgramName;
	for (uint32 seek = 0; seek < SST_SHADER_COUNT; seek = seek + 1)
	{
		this->ShaderPtrList[seek] = nullptr;
	}
	this->LinkLogList.clear();
}
San::Graphics::stSANSHADERPROGRAM::~stSANSHADERPROGRAM()
{
}