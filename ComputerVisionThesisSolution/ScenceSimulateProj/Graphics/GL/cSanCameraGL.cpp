#include"cSanCameraGL.h"
using namespace std;
using namespace San;
using namespace San::Graphics;
#ifdef _OPENGL
bool cSanCameraGL::iBindToNode(const lpSPOINT3 pNode, const lpSPOINT3 pRotation)
{
	if (!this->m_CameraDesc.bCameraMovable)
	{
		return false;
	}
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
void cSanCameraGL::iUnBindToNode()
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
bool cSanCameraGL::iBindLookAtPointToNode(const lpSPOINT3 pLookAtNode)
{
	if (pLookAtNode == nullptr)
	{
		return false;
	}
	if (this->m_pBindCameraLookAtPoint != nullptr)
	{
		this->iUnBindLookAtPointToNode();
	}
	this->m_pBindCameraLookAtPoint = pLookAtNode;
	return true;
}
void cSanCameraGL::iUnBindLookAtPointToNode()
{
	if (this->m_pBindCameraLookAtPoint == nullptr)
	{
		return;
	}
	this->iUpdateCoord();
	this->m_CameraLookAtOffset = *this->m_pBindCameraLookAtPoint - this->m_Coord;
	this->m_pBindCameraLookAtPoint = nullptr;
}
void cSanCameraGL::iSetCameraCoord(const SPOINT3 &CameraCoord)
{
	if (this->m_CameraDesc.bCameraMovable)
	{
		this->iUpdateCoord();
		SPOINT3 LookAtPoint = this->m_Coord + this->m_CameraLookAtOffset;
		this->iSetCoord(CameraCoord);
		this->m_CameraLookAtOffset = LookAtPoint - this->m_Coord;
	}
}
void cSanCameraGL::iSetCameraCoord(const sfloat CoordX, const sfloat CoordY, const sfloat CoordZ)
{
	this->iSetCameraCoord(SPOINT3(CoordX, CoordY, CoordZ));
}
void cSanCameraGL::iSetViewRange(const sfloat ViewNear, const sfloat ViewFar)
{
	this->m_CameraDesc.ViewNear = ViewNear;
	this->m_CameraDesc.ViewFar = ViewFar;
}
void cSanCameraGL::iSetViewSize(const uint32 ViewWidth, const uint32 ViewHeight)
{
	this->m_CameraDesc.ViewWidth = (ViewWidth != 0) ? ViewWidth : 1;
	this->m_CameraDesc.ViewHeight = (ViewHeight != 0) ? ViewHeight : 1;
}
void cSanCameraGL::iSetViewFOV(const sfloat FOV)
{
	this->m_CameraDesc.ViewFOV=FOV;
}
void cSanCameraGL::iSetCameraLookAtPoint(const SPOINT3 &Coord)
{
	this->iUpdateCameraCoord();
	if (this->m_pBindCameraLookAtPoint == nullptr)
	{
		this->m_CameraLookAtOffset = Coord - this->m_Coord;
	}
}
void cSanCameraGL::iSetCameraLookAtPoint(const sfloat CoordX, const sfloat CoordY, const sfloat CoordZ)
{
	this->iSetCameraLookAtPoint(SPOINT3(CoordX, CoordY, CoordZ));
}
void cSanCameraGL::iSetCameraMovable(const bool bCameraMovable)
{
	this->m_CameraDesc.bCameraMovable = bCameraMovable;
	this->iUpdateCameraCoord();
}
void cSanCameraGL::iSetCameraOrienVector(const SPOINT3 &Orien)
{
	this->m_CameraDesc.CameraOrien = Orien;
}
void cSanCameraGL::iSetCameraDesc(const SANCAMERADESC &Desc)
{
	this->m_CameraDesc = Desc;
	this->iSetCameraCoord(Desc.CameraCoord);
	this->iSetCameraLookAtPoint(Desc.CameraLookAtPoint);
	if (gloIsFloatEqual(this->m_CameraDesc.ViewWidth, 0.0))
	{
		this->m_CameraDesc.ViewWidth = 1;
	}
	if (gloIsFloatEqual(this->m_CameraDesc.ViewHeight, 0.0))
	{
		this->m_CameraDesc.ViewHeight = 1;
	}
	this->iUpdateCameraCoord();
}
void cSanCameraGL::iCameraMove(const SPOINT3 &Coord)
{
	if (this->m_CameraDesc.bCameraMovable)
	{
		this->iUpdateCameraCoord();
		SPOINT3 CameraCoord;
		this->iSetCameraCoord(this->m_Coord + Coord);
	}
}
void cSanCameraGL::iCameraMove(const sfloat CoordX, const sfloat CoordY, const sfloat CoordZ)
{
	this->iCameraMove(SPOINT3(CoordX, CoordY, CoordZ));
}
//void cSanCameraGL::iCameraMoveByDir(const SVECTOR3 &Dir, const sfloat Pow)
//{
//	if (this->m_CameraDesc.bCameraMovable)
//	{
//		this->iSetCameraCoord(this->m_Coord + Dir*Pow);
//	}
//}
//void cSanCameraGL::iCameraMoveByDir(const sfloat CoordX, const sfloat CoordY, const sfloat CoordZ, const sfloat Pow)
//{
//	this->iCameraMoveByDir(SVECTOR3(CoordX, CoordY, CoordZ), Pow);
//}
void cSanCameraGL::iCameraMoveByLookAtDir(const sfloat Pow)
{
	if (this->m_CameraDesc.bCameraMovable)
	{
		this->iUpdateCameraCoord();
		SVECTOR3 Dir = this->m_CameraDesc.CameraLookAtPoint - this->m_CameraDesc.CameraCoord;
		Dir = ::gloNormalize(Dir);
		this->iSetCameraCoord(this->m_Coord + Dir*Pow);
	}
}
void cSanCameraGL::iCameraRotation(const SPOINT3 &RotateVector, const SVECTOR3 &Axle)
{
	this->iUpdateCameraCoord();
	if ((Axle != this->m_CameraDesc.CameraCoord) && (Axle != this->m_CameraDesc.CameraLookAtPoint))
	{
		if ((!this->m_CameraDesc.bCameraMovable) || (this->m_pBindNode != nullptr) || (this->m_pBindCameraLookAtPoint != nullptr))
		{
			return;
		}
	}
	else
	{
		if (Axle == this->m_CameraDesc.CameraCoord)
		{
			if (this->m_pBindCameraLookAtPoint != nullptr)
			{
				return;
			}
		}
		else
		{
			if (!this->m_CameraDesc.bCameraMovable)
			{
				return;
			}
			if (this->m_pBindNode != nullptr)
			{
				return;
			}
		}
	}
	sfloat LengthAxleCoord = ::gloLength(Axle - this->m_CameraDesc.CameraCoord);
	sfloat LengthAxleLookAt = ::gloLength(Axle - this->m_CameraDesc.CameraLookAtPoint);
	SVECTOR3 Dir = ::gloNormalize(this->m_CameraLookAtOffset);
	/*SVECTOR3 Theta = ::gloACos(Dir, 1.0f);
	Theta = Theta + RotateVector;
	Dir = ::gloCos(Theta);*/
	if (RotateVector.x != 0.0f)
	{
		sfloat ArcTanYZ = (Dir.z == 0.0f) ? (S_PI / 2) : (::atan(Dir.y / Dir.z));
		ArcTanYZ = ArcTanYZ < 0.0f ? ArcTanYZ + S_PI : ArcTanYZ;
		ArcTanYZ = ArcTanYZ + RotateVector.x;
		sfloat radius = ::sqrt(this->m_CameraLookAtOffset.y*this->m_CameraLookAtOffset.y + this->m_CameraLookAtOffset.z*this->m_CameraLookAtOffset.z);
		Dir.y = radius*::sin(ArcTanYZ);
		Dir.z = radius*::cos(ArcTanYZ);
		Dir.x=this->m_CameraLookAtOffset.x;
	}
	if (RotateVector.y != 0.0f)
	{
		sfloat ArcTanXZ = (Dir.z == 0.0f) ? (S_PI / 2) : (::atan(Dir.x / Dir.z));
		ArcTanXZ = ArcTanXZ < 0.0f ? ArcTanXZ + S_PI : ArcTanXZ;
		::cout << "Radius" << int((sfloat) (ArcTanXZ / S_PI) * 180) << "\n";
		ArcTanXZ = ArcTanXZ + RotateVector.y;
		sfloat radius = ::sqrt(this->m_CameraLookAtOffset.x*this->m_CameraLookAtOffset.x + this->m_CameraLookAtOffset.z*this->m_CameraLookAtOffset.z);
		Dir.x = radius*::sin(ArcTanXZ);
		Dir.z = radius*::cos(ArcTanXZ);
		Dir.y=this->m_CameraLookAtOffset.y;
	}
	if (RotateVector.z != 0.0f)
	{
		sfloat ArcTanXY = (Dir.y == 0.0f) ? (S_PI / 2) : (::atan(Dir.x / Dir.y));
		ArcTanXY = ArcTanXY < 0.0f ? ArcTanXY + S_PI : ArcTanXY;
		ArcTanXY = ArcTanXY + RotateVector.z;
		sfloat radius = ::sqrt(this->m_CameraLookAtOffset.x*this->m_CameraLookAtOffset.x + this->m_CameraLookAtOffset.y*this->m_CameraLookAtOffset.y);
		Dir.x = radius*::sin(ArcTanXY);
		Dir.y = radius*::cos(ArcTanXY);
		Dir.z=this->m_CameraLookAtOffset.z;
	}
	Dir = ::gloNormalize(Dir);
	if (this->m_pBindNode == nullptr)
	{
		/*signal*/
		this->iSetCoord(Axle - Dir*LengthAxleCoord);
	}
	if (this->m_pBindCameraLookAtPoint == nullptr)
	{
		/*signal*/
		this->m_CameraLookAtOffset = Axle - Dir*LengthAxleLookAt - this->m_Coord;
	}
}
void cSanCameraGL::iCameraRotation(const sfloat RotateX, const sfloat RotateY, const sfloat RotateZ, const sfloat AxleX, const sfloat AxleY, const sfloat AxleZ)
{
	this->iCameraRotation(SPOINT3(RotateX, RotateY, RotateZ), SVECTOR3(AxleX, AxleY, AxleZ));
}
void cSanCameraGL::iCameraRotationByCoord(const SPOINT3 &RotateVector)
{
	if (this->m_pBindCameraLookAtPoint != nullptr)
	{
		return;
	}
	this->iUpdateCameraCoord();
	this->iCameraRotation(RotateVector, this->m_Coord);
}
void cSanCameraGL::iCameraRotationByCoord(const sfloat RotateX, const sfloat RotateY, const sfloat RotateZ)
{
	if (this->m_pBindCameraLookAtPoint != nullptr)
	{
		return;
	}
	this->iUpdateCameraCoord();
	this->iCameraRotation(SPOINT3(RotateX, RotateY, RotateZ), this->m_Coord);
}
void cSanCameraGL::iCameraRotationByLookAtPoint(const SPOINT3 &RotateVector)
{
	if (!this->m_CameraDesc.bCameraMovable)
	{
		return;
	}
	if (this->m_pBindNode != nullptr)
	{
		return;
	}
	this->iUpdateCameraCoord();
	this->iCameraRotation(RotateVector, this->m_Coord + this->m_CameraLookAtOffset);
}
void cSanCameraGL::iCameraRotationByLookAtPoint(const sfloat RotateX, const sfloat RotateY, const sfloat RotateZ)
{
	if (!this->m_CameraDesc.bCameraMovable)
	{
		return;
	}
	if (this->m_pBindNode != nullptr)
	{
		return;
	}
	this->iUpdateCameraCoord();
	this->iCameraRotation(SPOINT3(RotateX, RotateY, RotateZ), this->m_Coord + this->m_CameraLookAtOffset);
}
bool cSanCameraGL::iGetCameraMovableState() const
{
	return this->m_CameraDesc.bCameraMovable;
}
SIZE cSanCameraGL::iGetCameraViewSize() const
{
	SIZE ViewSize;
	ViewSize.cx=this->m_CameraDesc.ViewWidth;
	ViewSize.cy=this->m_CameraDesc.ViewHeight;
	return ViewSize;
}
sfloat cSanCameraGL::iGetCameraViewFOV() const
{
	return this->m_CameraDesc.ViewFOV;
}
SPOINT3 cSanCameraGL::iGetCameraOrienVector() const
{
	return this->m_CameraDesc.CameraOrien;
}
SString cSanCameraGL::iGetCameraName() const
{
	return this->m_strCameraName;
}
SANCAMERADESC cSanCameraGL::iGetCameraDesc()
{
	this->iUpdateCameraCoord();
	return this->m_CameraDesc;
}
void cSanCameraGL::iUpdateCameraCoord()
{
	if (this->m_CameraDesc.bCameraMovable)
	{
		this->iUpdateCoord();
	}
	if (this->m_pBindCameraLookAtPoint != nullptr)
	{
		this->m_CameraLookAtOffset = (*this->m_pBindCameraLookAtPoint) - this->m_Coord;
	}
	this->m_CameraDesc.CameraCoord = this->m_Coord;
	this->m_CameraDesc.CameraLookAtPoint = this->m_Coord + this->m_CameraLookAtOffset;
}
uint32 cSanCameraGL::iDoSomething(uint32 wParam,uint32 lParam)
{
	this->iUpdateCameraCoord();
	SPOINT3 LookAtPoint = this->m_CameraDesc.CameraLookAtPoint;
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();
	::gluPerspective(this->m_CameraDesc.ViewFOV, (sfloat)this->m_CameraDesc.ViewWidth / (sfloat)this->m_CameraDesc.ViewHeight, this->m_CameraDesc.ViewNear, this->m_CameraDesc.ViewFar);
	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
	if (!this->m_CameraDesc.bIsOrthoCamera)
	{
		::gluLookAt(this->m_Coord.x, this->m_Coord.y, this->m_Coord.z, LookAtPoint.x, LookAtPoint.y, LookAtPoint.z, this->m_CameraDesc.CameraOrien.x, this->m_CameraDesc.CameraOrien.y, this->m_CameraDesc.CameraOrien.z);
	}
	else
	{
		sfloat HalfWidth = this->m_CameraDesc.ViewWidth / 2.0;
		sfloat HalfHeight = this->m_CameraDesc.ViewHeight / 2.0;
		::gluOrtho2D(-HalfWidth, HalfWidth, -HalfHeight, HalfHeight);// coord: 0.0 0.0 0.0
	}
	return 0;
}
#endif