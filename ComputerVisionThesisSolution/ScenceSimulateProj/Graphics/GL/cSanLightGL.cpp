#include"cSanLightGL.h"
using namespace std;
using namespace San;
using namespace San::Mathematics;
using namespace San::Graphics;
#ifdef _OPENGL
list<cSanLight*> cSanLight::m_GlobalLightList;
void cSanLightGL::iSetLightCoord(SPOINT3 LightCoord)
{
	if(this->m_LightDesc.b_light_movable)
	{
		this->iSetCoord(LightCoord);
	}
}
void cSanLightGL::iSetLightCoord(sfloat CoordX,sfloat CoordY,sfloat CoordZ)
{
	this->iSetLightCoord(SPOINT3(CoordX,CoordY,CoordZ));
}
void cSanLightGL::iSetLightLookAtPoint(SPOINT3 Coord)
{
	this->iUpdateLightCoord();
	if (this->m_pBindLightLookAtPoint == nullptr)
	{
		this->m_LightLookAtOffset = Coord - this->m_Coord;
	}
}
void cSanLightGL::iSetLightLookAtPoint(sfloat CoordX,sfloat CoordY,sfloat CoordZ)
{
	this->iSetLightLookAtPoint(SPOINT3(CoordX,CoordY,CoordZ));
}
uint32 cSanLightGL::_AddToLightList(cSanLightGL* pLight)
{
	uint32 LightID=::SLT_MAXSOFTWARELIGHT;
	if(pLight==nullptr)
	{
		return LightID;
	}
	if(pLight->m_LightID<::SLT_MAXSOFTWARELIGHT)
	{
		return pLight->m_LightID;
	}
	LightID=cSanLightGL::m_GlobalLightList.size();
	cSanLightGL::m_GlobalLightList.insert(cSanLightGL::m_GlobalLightList.end(),pLight);
	return LightID;
}
bool cSanLightGL::_InsertToLightList(cSanLightGL* pLight,uint32 DestLightID)
{
	if(DestLightID>cSanLightGL::m_GlobalLightList.size())
	{
		return false;
	}
	if(DestLightID==cSanLightGL::m_GlobalLightList.size())
	{
		cSanLightGL::_AddToLightList(pLight);
		return true;
	}
	bool bForwardSearch=true;
	if(pLight->m_LightID>DestLightID)
	{
		bForwardSearch=false;
	}
	list<cSanLight*>::iterator pLightElement;
	if(bForwardSearch)
	{
		pLightElement=cSanLightGL::m_GlobalLightList.begin();
	}
	else
	{
		pLightElement=cSanLightGL::m_GlobalLightList.end();
		pLightElement--;
	}
	if(pLight->m_LightID<cSanLightGL::m_GlobalLightList.size())
	{
		while((*pLightElement)!=pLight)
		{
			if(bForwardSearch)
			{
				pLightElement++;
			}
			else
			{
				pLightElement--;
			}
		}
		cSanLightGL::m_GlobalLightList.erase(pLightElement);
		if(pLightElement==cSanLightGL::m_GlobalLightList.end())
		{
			pLightElement--;
		}
	}
	while((*pLightElement)->m_LightID!=DestLightID)
	{
		if(bForwardSearch)
		{
			(*pLightElement)->m_LightID=(*pLightElement)->m_LightID-1;
			pLightElement++;
		}
		else
		{
			(*pLightElement)->m_LightID=(*pLightElement)->m_LightID+1;
			pLightElement--;
		}
	}
	if(bForwardSearch)
	{
		(*pLightElement)->m_LightID=(*pLightElement)->m_LightID-1;
		pLightElement++;
	}
	else
	{
		(*pLightElement)->m_LightID=(*pLightElement)->m_LightID+1;
	}
	cSanLightGL::m_GlobalLightList.insert(pLightElement,pLight);
	return true;
}
void cSanLightGL::_DeleteFromLightList(uint32 LightID)
{
	if(LightID==::SLT_MAXSOFTWARELIGHT)
	{
		return;
	}
	if(LightID>=cSanLightGL::m_GlobalLightList.size())
	{
		return;
	}
	list<cSanLight*>::iterator pLight=cSanLightGL::m_GlobalLightList.begin();
	while(pLight!=cSanLightGL::m_GlobalLightList.end())
	{
		if((*pLight)->m_LightID==LightID)
		{
			cSanLightGL::m_GlobalLightList.erase(pLight);
			if(cSanLightGL::m_GlobalLightList.empty())
			{
				break;
			}
			continue;
		}
		if((*pLight)->m_LightID>LightID)
		{
			(*pLight)->m_LightID=(*pLight)->m_LightID-1;
		}
		pLight++;
	}
}
void cSanLightGL::iSetLightMovable(bool bCameraMovable)
{
	this->m_LightDesc.b_light_movable=bCameraMovable;
}
void cSanLightGL::iSetLightType(eSANLIGHTTYPE Type)
{
	if(Type<San::Graphics::SLT_MAXHARDWAERLIGHT)
	{
		this->m_LightDesc.type=Type;
		this->m_bLightChange=true;
	}
}
void cSanLightGL::iSetLightColor(SANCOLORF Diffuse,SANCOLORF Specular,SANCOLORF Ambient,SANCOLORF Emission)
{
	this->m_LightDesc.diffuse=Diffuse;
	this->m_LightDesc.specular=Specular;
	this->m_LightDesc.ambient=Ambient;
	this->m_LightDesc.emission=Emission;
	this->m_bLightChange=true;
}
void cSanLightGL::iSetLightRange(sfloat Range)
{
	this->m_LightDesc.range=Range;
	this->m_bLightChange=true;
}
void cSanLightGL::iSetLightFallOff(sfloat FallOff)
{
	this->m_LightDesc.falloff=FallOff;
	this->m_bLightChange=true;
}
void cSanLightGL::iSetLightRadius(sfloat Inner,sfloat Outer)
{
	this->m_LightDesc.radius_inside=Inner;
	this->m_LightDesc.radius_outside=Outer;
	this->m_bLightChange=true;
}
void cSanLightGL::iSetLightAngle(sfloat Theta,sfloat Phi)
{
	this->m_LightDesc.angle_theta=Theta;
	this->m_LightDesc.angle_phi=Phi;
	this->m_bLightChange=true;
}
void cSanLightGL::iSetLightAttenuation(sfloat Constant,sfloat Linear,sfloat Quadratic)
{
	this->m_LightDesc.attenuation=Constant;
	this->m_LightDesc.attenuation_linear=Linear;
	this->m_LightDesc.attenuation_quadratic=Quadratic;
	this->m_bLightChange=true;
}
void cSanLightGL::iSetLightDescription(SANLIGHTDESC Desc)
{
	this->m_LightDesc=Desc;
	this->m_bLightChange=true;
}
void cSanLightGL::iSetLightEnable()
{
	this->m_LightID=cSanLightGL::_AddToLightList(this);
	this->m_bLightEnable=true;
}
void cSanLightGL::iSetLightDisable()
{
	if(this->m_LightID==::SLT_MAXSOFTWARELIGHT)
	{
		return;
	}
	cSanLightGL::_DeleteFromLightList(this->m_LightID);
	this->m_LightID=::SLT_MAXSOFTWARELIGHT;
	this->m_bLightEnable=false;
}
void cSanLightGL::iLightMove(SPOINT3 Coord)
{
	this->iUpdateLightCoord();
	SPOINT3 CameraCoord;
	if (this->m_LightDesc.b_light_movable)
	{
		this->iSetLightCoord(this->m_Coord + Coord);
	}
}
void cSanLightGL::iLightMove(sfloat CoordX,sfloat CoordY,sfloat CoordZ)
{
	this->iLightMove(SPOINT3(CoordX,CoordY,CoordZ));
}
void cSanLightGL::iLightRotation(SPOINT3 RotateVector, SVECTOR3 Axle)
{
	this->iUpdateLightCoord();
	if (Axle == this->m_Coord)
	{
		if (this->m_pBindLightLookAtPoint != nullptr)
		{
			return;
		}
	}
	if (Axle == (this->m_Coord + this->m_LightLookAtOffset))
	{
		if (this->m_pBindNode != nullptr)
		{
			return;
		}
	}
	sfloat LengthAxleCoord = ::gloLength(Axle - this->m_Coord);
	sfloat LengthAxleLookAt = ::gloLength(Axle - this->m_Coord + this->m_LightLookAtOffset);
	SVECTOR3 Dir = ::gloNormalize(this->m_LightLookAtOffset);
	if (RotateVector.x != 0)
	{
		sfloat ArcTanYZ = ::atan(sfloat((Dir.y) / (Dir.z)));
		if (Dir.z < 0.0f)
		{
			ArcTanYZ = ArcTanYZ + S_PI;
		}
		ArcTanYZ = ArcTanYZ + RotateVector.x;
		sfloat radius = ::sqrt(Dir.y*Dir.y + Dir.z*Dir.z);
		Dir.y = radius*::sin(ArcTanYZ);
		Dir.z = radius*::cos(ArcTanYZ);
	}
	if (RotateVector.y != 0)
	{
		sfloat ArcTanXZ = ::atan(sfloat(Dir.x) / (Dir.z));
		if (Dir.z < 0)
		{
			ArcTanXZ = ArcTanXZ + S_PI;
		}
		::cout << "Radius" << int((sfloat) (ArcTanXZ / S_PI) * 180) << "\n";
		ArcTanXZ = ArcTanXZ + RotateVector.y;
		sfloat radius = ::sqrt(Dir.x*Dir.x + Dir.z*Dir.z);
		Dir.x = radius*::sin(ArcTanXZ);
		Dir.z = radius*::cos(ArcTanXZ);
	}
	if (RotateVector.z != 0)
	{
		sfloat ArcTanXY = ::atan(sfloat(Dir.x) / (Dir.y));
		if (Dir.y < 0)
		{
			ArcTanXY = ArcTanXY + S_PI;
		}
		ArcTanXY = ArcTanXY + RotateVector.z;
		sfloat radius = ::sqrt(Dir.x*Dir.x + Dir.y*Dir.y);
		Dir.x = radius*::sin(ArcTanXY);
		Dir.y = radius*::cos(ArcTanXY);
	}
	if (this->m_pBindNode == nullptr)
	{
		/*signal*/
		this->m_Coord = Axle + Dir*LengthAxleCoord;
	}
	if (this->m_pBindLightLookAtPoint == nullptr)
	{
		/*signal*/
		this->m_LightLookAtOffset = Axle + Dir*LengthAxleLookAt;
	}
}
void cSanLightGL::iLightRotation(sfloat RotateX, sfloat RotateY, sfloat RotateZ, sfloat AxleX, sfloat AxleY, sfloat AxleZ)
{
	this->iLightRotation(SPOINT3(RotateX, RotateY, RotateZ), SVECTOR3(AxleX, AxleY, AxleZ));
}
void cSanLightGL::iLightRotationByCoord(SPOINT3 RotateVector)
{
	if (this->m_pBindLightLookAtPoint != nullptr)
	{
		return;
	}
	this->iUpdateLightCoord();
	this->iLightRotation(RotateVector, this->m_Coord);
}
void cSanLightGL::iLightRotationByCoord(sfloat RotateX, sfloat RotateY, sfloat RotateZ)
{
	if (this->m_pBindLightLookAtPoint != nullptr)
	{
		return;
	}
	this->iUpdateLightCoord();
	this->iLightRotation(SPOINT3(RotateX, RotateY, RotateZ), this->m_Coord);
}
void cSanLightGL::iLightRotationByLookAtPoint(SPOINT3 RotateVector)
{
	if (this->m_pBindNode != nullptr)
	{
		return;
	}
	this->iUpdateLightCoord();
	this->iLightRotation(RotateVector, this->m_Coord + this->m_LightLookAtOffset);
}
void cSanLightGL::iLightRotationByLookAtPoint(sfloat RotateX, sfloat RotateY, sfloat RotateZ)
{
	if (this->m_pBindNode != nullptr)
	{
		return;
	}
	this->iUpdateLightCoord();
	this->iLightRotation(SPOINT3(RotateX, RotateY, RotateZ), this->m_Coord + this->m_LightLookAtOffset);
}
bool cSanLightGL::iGetLightMovableState()
{
	return this->m_LightDesc.b_light_movable;
}
SString cSanLightGL::iGetLightName()
{
	return this->m_strLightName;
}
uint32 cSanLightGL::iGetLightID()
{
	return this->m_LightID;
}
SANLIGHTDESC cSanLightGL::iGetLightDescription()
{
	this->iUpdateLightCoord();
	this->m_LightDesc.coord = this->m_Coord;
	this->m_LightDesc.look_at = this->m_Coord + this->m_LightLookAtOffset;
	this->m_LightDesc.direction = ::gloNormalize(this->m_LightLookAtOffset);
	return this->m_LightDesc;
}
bool cSanLightGL::iGetLightState()
{
	return this->m_bLightEnable;
}
bool cSanLightGL::iBindLookAtPointToNode(lpSPOINT3 pNode)
{
	if (pNode == nullptr)
	{
		return false;
	}
	if (this->m_pBindLightLookAtPoint != nullptr)
	{
		this->iUnBindLookAtPointToNode();
	}
	this->m_pBindLightLookAtPoint = pNode;
	this->iUpdateLightCoord();
	return true;
}
void cSanLightGL::iUnBindLookAtPointToNode()
{
	if (this->m_pBindLightLookAtPoint == nullptr)
	{
		return;
	}
	this->iUpdateLightCoord();
	this->m_pBindLightLookAtPoint = nullptr;
}
void cSanLightGL::iUpdateLightCoord()
{
	this->iUpdateCoord();
	if (this->m_pBindLightLookAtPoint != nullptr)
	{
		this->m_LightLookAtOffset = this->m_Coord - (*this->m_pBindLightLookAtPoint);
		this->m_bLightChange = true;
	}
	if (this->m_pBindNode != nullptr)
	{
		this->m_LightDesc.b_light_movable = true;
		this->m_bLightChange = true;
	}
}
uint32 cSanLightGL::iDoSomething(uint32 wParam,uint32 lParam)
{
	if(this->m_LightID>=::SLT_MAXHARDWAERLIGHT)
	{
		return 0;
	}
	if(!this->m_bLightEnable)
	{
		::glDisable(GL_LIGHT0+this->m_LightID);
		return 0;
	}
	this->iUpdateLightCoord();
	if(!this->m_bLightChange)
	{
		uint32 light_id=GL_LIGHT0+this->m_LightID;
		sfloat light_coord[4]={0.0,0.0,0.0,1.0};
		sfloat *plight_direction=(this->m_Coord+this->m_CoordOffset).p;
		switch(this->m_LightDesc.type)
		{
		case SLT_GLOBAL:
			break;
		case SLT_POINT:
			::glLightfv(light_id,GL_POSITION,light_coord);
			break;
		case SLT_DIRECTIONAL:
			light_coord[3]=0.0;
			::glLightfv(light_id,GL_POSITION,light_coord);
			::glLightfv(light_id,GL_SPOT_DIRECTION,plight_direction);
			::glEnable(light_id);
			break;
		case SLT_SPOT:
			::glLightfv(light_id,GL_POSITION,light_coord);
			::glLightfv(light_id,GL_SPOT_DIRECTION,plight_direction);
			::glEnable(light_id);
			break;
		default:
			break;
		}
	}
	uint32 LightID=GL_LIGHT0+this->m_LightID;
	sfloat LightCoord[4]={0.0,0.0,0.0,1.0};
	sfloat* pLightDirection=this->m_Coord.p;
	LightCoord[0]=this->m_Coord.x;
	LightCoord[1]=this->m_Coord.y;
	LightCoord[2]=this->m_Coord.z;
	LightCoord[3]=1.0;
	pLightDirection=(this->m_Coord+this->m_CoordOffset).p;
	switch(this->m_LightDesc.type)
	{
	case SLT_GLOBAL:
		::glLightModelfv(GL_LIGHT_MODEL_AMBIENT,this->m_LightDesc.ambient.color_a);/////////////////////////global light redef
		break;
	case SLT_POINT:
		::glLightfv(LightID,GL_AMBIENT,this->m_LightDesc.ambient.color_a);
		::glLightfv(LightID,GL_DIFFUSE,this->m_LightDesc.diffuse.color_a);
		::glLightfv(LightID,GL_SPECULAR,this->m_LightDesc.specular.color_a);
		::glLightfv(LightID,GL_POSITION,LightCoord);
		::glLightf(LightID,GL_CONSTANT_ATTENUATION,this->m_LightDesc.attenuation);////////////////////////////range
		::glLightf(LightID,GL_LINEAR_ATTENUATION,this->m_LightDesc.attenuation_linear);
		::glLightf(LightID,GL_QUADRATIC_ATTENUATION,this->m_LightDesc.attenuation_quadratic);
		::glEnable(LightID);
		break;
	case SLT_DIRECTIONAL:
		LightCoord[3]=0.0;
		::glLightfv(LightID,GL_AMBIENT,this->m_LightDesc.ambient.color_a);
		::glLightfv(LightID,GL_DIFFUSE,this->m_LightDesc.diffuse.color_a);
		::glLightfv(LightID,GL_SPECULAR,this->m_LightDesc.specular.color_a);
		::glLightfv(LightID,GL_POSITION,LightCoord);
		::glLightfv(LightID,GL_SPOT_DIRECTION,pLightDirection);
		::glLightf(LightID,GL_CONSTANT_ATTENUATION,this->m_LightDesc.attenuation);////////////////////////////range
		::glLightf(LightID,GL_LINEAR_ATTENUATION,this->m_LightDesc.attenuation_linear);
		::glLightf(LightID,GL_QUADRATIC_ATTENUATION,this->m_LightDesc.attenuation_quadratic);
		::glEnable(LightID);
		break;
	case SLT_SPOT:
		::glLightfv(LightID,GL_AMBIENT,this->m_LightDesc.ambient.color_a);
		::glLightfv(LightID,GL_DIFFUSE,this->m_LightDesc.diffuse.color_a);
		::glLightfv(LightID,GL_SPECULAR,this->m_LightDesc.specular.color_a);
		::glLightfv(LightID,GL_POSITION,LightCoord);
		::glLightf(LightID,GL_SPOT_CUTOFF,this->m_LightDesc.angle_theta/2);
		::glLightfv(LightID,GL_SPOT_DIRECTION,pLightDirection);////////////////////////////range
		::glLightf(LightID,GL_CONSTANT_ATTENUATION,this->m_LightDesc.attenuation);
		::glLightf(LightID,GL_LINEAR_ATTENUATION,this->m_LightDesc.attenuation_linear);
		::glLightf(LightID,GL_QUADRATIC_ATTENUATION,this->m_LightDesc.attenuation_quadratic);
		::glEnable(LightID);
		break;
	default:
		break;
	}
	this->m_bLightChange=false;
	return 0;
}
#endif