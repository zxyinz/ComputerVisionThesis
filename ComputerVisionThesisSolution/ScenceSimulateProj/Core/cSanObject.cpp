#include"cSanObject.h"
using namespace std;
using namespace San;
uint32 cSanObject::m_Time;
bool cSanObject::iBindToNode(lpSPOINT3 pNode, lpSPOINT3 pRotation)
{
	if (pNode == nullptr)
	{
		return false;
	}
	if (this->m_pBindNode != nullptr)
	{
		this->iUnBindToNode();
	}
	this->m_pBindNode = pNode;
	this->m_pBindRotation = pRotation;
	this->m_CoordOffset = this->m_Coord - *(this->m_pBindNode);
	if (this->m_pBindRotation != nullptr)
	{
		if (this->m_CoordOffset != SPOINT3(0.0, 0.0, 0.0))
		{
			sfloat Length = ::gloLength(this->m_CoordOffset);
			this->m_ReleativeRotation = ::gloACos(this->m_CoordOffset, Length) - *(this->m_pBindRotation);
		}
	}
	return true;
}
void cSanObject::iUnBindToNode()
{
	this->iUpdateCoord();
	if (this->m_pBindNode != nullptr)
	{
		this->m_Coord = this->m_CoordOffset + *(this->m_pBindNode);
		this->m_CoordOffset = SPOINT3(0.0f, 0.0f, 0.0f);
		this->m_ReleativeRotation = SPOINT3(0.0f, 0.0f, 0.0f);
		this->m_pBindNode = nullptr;
		this->m_pBindRotation = nullptr;
	}
}
bool cSanObject::iSetCoord(SPOINT3 Coord)
{
	if (this->m_pBindNode != nullptr)
	{
		return false;
	}
	this->m_Coord = Coord;
	return true;
}
bool cSanObject::iSetRotation(SPOINT3 Rotation)
{
	this->m_Rotation = Rotation;
	return true;
}
bool cSanObject::iSetCoordOffset(SPOINT3 CoordOffset)
{
	if (this->m_pBindNode == nullptr)
	{
		return false;
	}
	this->m_CoordOffset = CoordOffset;
}
SPOINT3 cSanObject::iGetCoord()
{
	this->iUpdateCoord();
	return this->m_Coord;
}
lpSPOINT3 cSanObject::iGetCoordPtr()
{
	this->iUpdateCoord();
	return &(this->m_Coord);
}
const lpSPOINT3 cSanObject::iGetCoordPtrConst()
{
	this->iUpdateCoord();
	return &(this->m_Coord);
}
SPOINT3 cSanObject::iGetRotation()
{
	return this->m_Rotation;
}
lpSPOINT3 cSanObject::iGetRotationPtr()
{
	return &(this->m_Rotation);
}
const lpSPOINT3 cSanObject::iGetRotationPtrConst()
{
	return &(this->m_Rotation);
}
SPOINT3 cSanObject::iGetCoordOffset()
{
	//this->iUpdateCoord();
	if (this->m_pBindRotation != nullptr)
	{
		return this->m_CoordOffset*::gloCos(this->m_ReleativeRotation + *(this->m_pBindNode));
	}
	return this->m_CoordOffset;
}
lpSPOINT3 cSanObject::iGetCoordOffsetPtr()
{
	this->iUpdateCoord();
	return &(this->m_CoordOffset);
}
const lpSPOINT3 cSanObject::iGetCoordOffsetPtrConst()
{
	this->iUpdateCoord();
	return &(this->m_CoordOffset);
}
lpSPOINT3 cSanObject::iGetBindNodePtr() const
{
	return this->m_pBindNode;
}
const lpSPOINT3 cSanObject::iGetBindNodePtrConst() const
{
	return this->m_pBindNode;
}
lpSPOINT3 cSanObject::iGetBindRotationPtr() const
{
	return this->m_pBindRotation;
}
const lpSPOINT3 cSanObject::iGetBindRotationPtrConst() const
{
	return this->m_pBindRotation;
}
bool cSanObject::iGetBindState() const
{
	if(this->m_pBindNode!=nullptr)
	{
		return true;
	}
	return false;
}
eSANOBJECTTYPE cSanObject::iGetType() const
{
	return this->m_Type;
}
void cSanObject::iUpdateCoord()
{
	if (this->m_pBindNode != nullptr)
	{
		this->m_Coord = this->m_CoordOffset;
		if (this->m_pBindRotation != nullptr)
		{
			if (this->m_CoordOffset != SPOINT3(0.0, 0.0, 0.0))
			{
				/*SVECTOR3 theta = this->m_ReleativeRotation + *(this->m_pBindRotation);
				sfloat length = ::gloLength(this->m_CoordOffset);
				this->m_Coord = SPOINT3(length, length, length);
				if (!::gloIsFloatEqual(theta.x, 0.0f))
				{
					this->m_Coord.x = length*cos(theta.x);
				}
				if (!::gloIsFloatEqual(theta.y, 0.0f))
				{
					this->m_Coord.y = length*cos(theta.y);
				}
				if (!::gloIsFloatEqual(theta.z, 0.0f))//axle z
				{
					this->m_Coord.z = length*cos(theta.z);
				}//*/
				this->m_Coord = ::gloNormalize(::gloCos(this->m_ReleativeRotation + *(this->m_pBindRotation)))*::gloLength(this->m_CoordOffset);
			}
		}
		this->m_Coord = *(this->m_pBindNode) + this->m_Coord;
	}
	else
	{
		this->m_CoordOffset = SPOINT3(0.0, 0.0, 0.0);
		this->m_ReleativeRotation = SPOINT3(0.0, 0.0, 0.0);
	}
}