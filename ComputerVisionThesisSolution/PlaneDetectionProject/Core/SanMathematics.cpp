#include"SanMathematics.h"
using namespace std;
using namespace San;
using namespace San::Mathematics;
San::Mathematics::stSPOINT3::stSPOINT3()
	:x(0.0), y(0.0), z(0.0)
{
}
San::Mathematics::stSPOINT3::stSPOINT3(const stSPOINT3& Point3)
	:x(Point3.x), y(Point3.y), z(Point3.z)
{
}
San::Mathematics::stSPOINT3::stSPOINT3(const SPOINT4 &Point4)
	:x(Point4.x), y(Point4.y), z(Point4.z)
{
	if (!::gloIsFloatEqual(Point4.w, 1.0))
	{
		if (!gloIsFloatEqual(Point4.w, 0.0))
		{
			this->x = this->x / Point4.w;
			this->y = this->y / Point4.w;
			this->z = this->z / Point4.w;
		}
	}
}
San::Mathematics::stSPOINT3::stSPOINT3(const SVECTOR4 &Vec4)
	:x(Vec4.x), y(Vec4.y), z(Vec4.z)
{
}
San::Mathematics::stSPOINT3::stSPOINT3(const sfloat x, const sfloat y, const sfloat z)
	:x(x), y(y), z(z)
{
}
San::Mathematics::stSPOINT3::stSPOINT3(const sfloat* pArray)
	:x(pArray[0]), y(pArray[1]), z(pArray[2])
{
}
San::Mathematics::stSPOINT3& San::Mathematics::stSPOINT3::operator=(const POINT &Point2)
{
	this->x = Point2.x;
	this->y = Point2.y;
	this->z = 0.0;
	return *this;
}
San::Mathematics::stSPOINT3& San::Mathematics::stSPOINT3::operator=(const stSPOINT3 &Point3)
{
	this->x=Point3.x;
	this->y=Point3.y;
	this->z=Point3.z;
	return *this;
}
San::Mathematics::stSPOINT3& San::Mathematics::stSPOINT3::operator = (const SPOINT4 &Point4)
{
	this->x = Point4.x;
	this->y = Point4.y;
	this->z = Point4.z;
	if (!::gloIsFloatEqual(Point4.w, 1.0))
	{
		if (!gloIsFloatEqual(Point4.w, 0.0))
		{
			this->x = this->x / Point4.w;
			this->y = this->y / Point4.w;
			this->z = this->z / Point4.w;
		}
	}
	return *this;
}
San::Mathematics::stSPOINT3& San::Mathematics::stSPOINT3::operator = (const SVECTOR4 &Vec4)
{
	this->x = Vec4.x;
	this->y = Vec4.y;
	this->z = Vec4.z;
	return *this;
}
San::Mathematics::stSPOINT3 San::Mathematics::stSPOINT3::operator*(const stSPOINT3 &Point3) const
{
	return stSPOINT3(this->x*Point3.x, this->y*Point3.y, this->z*Point3.z);
}
San::Mathematics::stSPOINT3 San::Mathematics::stSPOINT3::operator+(const stSPOINT3 &Point3) const
{
	return stSPOINT3(this->x + Point3.x, this->y + Point3.y, this->z + Point3.z);
}
San::Mathematics::stSPOINT3 San::Mathematics::stSPOINT3::operator-(const stSPOINT3 &Point3) const
{
	return stSPOINT3(this->x - Point3.x, this->y - Point3.y, this->z - Point3.z);
}
San::Mathematics::stSPOINT3 San::Mathematics::stSPOINT3::operator*(const sfloat Value) const
{
	return stSPOINT3(this->x*Value, this->y*Value, this->z*Value);
}
San::Mathematics::stSPOINT3 San::Mathematics::stSPOINT3::operator+(const sfloat Value) const
{
	return stSPOINT3(this->x + Value, this->y + Value, this->z + Value);
}
San::Mathematics::stSPOINT3 San::Mathematics::stSPOINT3::operator-(const sfloat Value) const
{
	return stSPOINT3(this->x - Value, this->y - Value, this->z - Value);
}
bool San::Mathematics::stSPOINT3::operator==(const stSPOINT3 &Point3) const
{
	if (!::gloIsFloatEqual(this->x, Point3.x))
	{
		return false;
	}
	if (!::gloIsFloatEqual(this->y, Point3.y))
	{
		return false;
	}
	if (!::gloIsFloatEqual(this->y, Point3.y))
	{
		return false;
	}
	return true;
}
bool San::Mathematics::stSPOINT3::operator!=(const stSPOINT3 &Point3) const
{
	bool bEqual = true;
	if (!::gloIsFloatEqual(this->x, Point3.x))
	{
		bEqual = false;
	}
	if (!::gloIsFloatEqual(this->y, Point3.y))
	{
		bEqual = false;
	}
	if (!::gloIsFloatEqual(this->y, Point3.y))
	{
		bEqual = false;
	}
	return !bEqual;
}
San::Mathematics::SPOINT4::SPOINT4()
	:stSPOINT4(0.0, 0.0, 0.0, 1.0)
{
}
San::Mathematics::SPOINT4::SPOINT4(const SPOINT3 &Point3)
	:stSPOINT4(Point3.x, Point3.y, Point3.z, 1.0)
{
}
San::Mathematics::SPOINT4::SPOINT4(const SPOINT4 &Point4)
	:stSPOINT4(Point4.x, Point4.y, Point4.z, 1.0)
{
	if (!::gloIsFloatEqual(Point4.w, 1.0))
	{
		if (!gloIsFloatEqual(Point4.w, 0.0))
		{
			this->x = this->x / Point4.w;
			this->y = this->y / Point4.w;
			this->z = this->z / Point4.w;
		}
	}
}
San::Mathematics::SPOINT4::SPOINT4(const SVECTOR4 &Vec4)
	:stSPOINT4(Vec4.x, Vec4.y, Vec4.z, 1.0)
{
}
San::Mathematics::SPOINT4::SPOINT4(const sfloat x, const sfloat y, const sfloat z, const sfloat w)
	:stSPOINT4(x, y, z, 1.0)
{
	if (!::gloIsFloatEqual(w, 1.0))
	{
		if (!gloIsFloatEqual(w, 0.0))
		{
			this->x = this->x / w;
			this->y = this->y / w;
			this->z = this->z / w;
		}
	}
}
San::Mathematics::SPOINT4::SPOINT4(const sfloat* pArray)
	:stSPOINT4(pArray[0], pArray[1], pArray[2], 1.0)
{
	if (!::gloIsFloatEqual(pArray[3], 1.0))
	{
		if (!gloIsFloatEqual(pArray[3], 0.0))
		{
			this->x = this->x / pArray[3];
			this->y = this->y / pArray[3];
			this->z = this->z / pArray[3];
		}
	}
}
San::Mathematics::SPOINT4& San::Mathematics::SPOINT4::operator=(const SPOINT3 &Point3)
{
	this->x = Point3.x;
	this->y = Point3.y;
	this->z = Point3.z;
	this->w = 1.0;
	return *this;
}
San::Mathematics::SPOINT4& San::Mathematics::SPOINT4::operator=(const SPOINT4 &Point4)
{
	this->x = Point4.x;
	this->y = Point4.y;
	this->z = Point4.z;
	this->w = 1.0;
	if (!::gloIsFloatEqual(Point4.w, 1.0))
	{
		if (!gloIsFloatEqual(Point4.w, 0.0))
		{
			this->x = this->x / Point4.w;
			this->y = this->y / Point4.w;
			this->z = this->z / Point4.w;
		}
	}
	return *this;
}
San::Mathematics::SPOINT4& San::Mathematics::SPOINT4::operator=(const SVECTOR4 &Vec4)
{
	this->x = Vec4.x;
	this->y = Vec4.y;
	this->z = Vec4.z;
	this->w = 1.0;
	return *this;
}
San::Mathematics::SPOINT4 San::Mathematics::SPOINT4::operator+(const SPOINT4 &Point4) const
{
	SPOINT4 HomoPointSrc(*this);
	SPOINT4 HomoPointDesc(Point4);
	return SPOINT4(HomoPointSrc.x + HomoPointDesc.x, HomoPointSrc.y + HomoPointDesc.y, HomoPointSrc.z + HomoPointDesc.z, 1.0);
}
San::Mathematics::SPOINT4 San::Mathematics::SPOINT4::operator-(const SPOINT4 &Point4) const
{
	SPOINT4 HomoPointSrc(*this);
	SPOINT4 HomoPointDesc(Point4);
	return SPOINT4(HomoPointSrc.x - HomoPointDesc.x, HomoPointSrc.y - HomoPointDesc.y, HomoPointSrc.z - HomoPointDesc.z, 1.0);
}
San::Mathematics::SPOINT4 San::Mathematics::SPOINT4::operator*(const SPOINT4 &Point4) const
{
	SPOINT4 HomoPointSrc(*this);
	SPOINT4 HomoPointDesc(Point4);
	return SPOINT4(HomoPointSrc.x*HomoPointDesc.x, HomoPointSrc.y*HomoPointDesc.y, HomoPointSrc.z*HomoPointDesc.z, 1.0);
}
San::Mathematics::SPOINT4 San::Mathematics::SPOINT4::operator+(const sfloat Val) const
{
	return SPOINT4(this->x + Val, this->y + Val, this->z + Val, 1.0);
}
San::Mathematics::SPOINT4 San::Mathematics::SPOINT4::operator-(const sfloat Val) const
{
	return SPOINT4(this->x - Val, this->y - Val, this->z - Val, 1.0);
}
San::Mathematics::SPOINT4 San::Mathematics::SPOINT4::operator*(const sfloat Val) const
{
	return SPOINT4(this->x*Val, this->y*Val, this->z*Val, 1.0);
}
bool San::Mathematics::SPOINT4::operator==(const SPOINT4 &Vec4) const
{
	if (!::gloIsFloatEqual(this->x, Vec4.x))
	{
		return false;
	}
	if (!::gloIsFloatEqual(this->y, Vec4.y))
	{
		return false;
	}
	if (!::gloIsFloatEqual(this->z, Vec4.z))
	{
		return false;
	}
	if (!::gloIsFloatEqual(this->w, Vec4.w))
	{
		return false;
	}
	return true;
}
bool San::Mathematics::SPOINT4::operator!=(const SPOINT4 &Vec4) const
{
	bool bEqual = true;
	if (!::gloIsFloatEqual(this->x, Vec4.x))
	{
		bEqual = false;
	}
	if (!::gloIsFloatEqual(this->y, Vec4.y))
	{
		bEqual = false;
	}
	if (!::gloIsFloatEqual(this->z, Vec4.z))
	{
		bEqual = false;
	}
	if (!::gloIsFloatEqual(this->w, Vec4.w))
	{
		bEqual = false;
	}
	return !bEqual;
}
void San::Mathematics::SPOINT4::iHomogenzation()
{
	if (!gloIsFloatEqual(this->w, 1.0))
	{
		if (!gloIsFloatEqual(this->w, 0.0))
		{
			this->x = this->x / this->w;
			this->y = this->y / this->w;
			this->z = this->z / this->w;
		}
		this->w = 1.0;
	}
}
San::Mathematics::SVECTOR4::SVECTOR4()
	:stSPOINT4(0.0,0.0,0.0,0.0)
{
}
San::Mathematics::SVECTOR4::SVECTOR4(const SPOINT3 &Point3)
	:stSPOINT4(Point3.x, Point3.y, Point3.z, 0.0)
{
}
San::Mathematics::SVECTOR4::SVECTOR4(const SPOINT4 &Point4)
	:stSPOINT4(Point4.x,Point4.y,Point4.z,0.0)
{
	if (!::gloIsFloatEqual(Point4.w, 1.0))
	{
		if (!gloIsFloatEqual(Point4.w, 0.0))
		{
			this->x = this->x / Point4.w;
			this->y = this->y / Point4.w;
			this->z = this->z / Point4.w;
		}
	}
}
San::Mathematics::SVECTOR4::SVECTOR4(const SVECTOR4 &Vec4)
	:stSPOINT4(Vec4.x, Vec4.y, Vec4.z, 0.0)
{
}
San::Mathematics::SVECTOR4::SVECTOR4(const sfloat x,const sfloat y,const sfloat z,const sfloat w)
	:stSPOINT4(x,y,z, 0.0)
{
	if (!::gloIsFloatEqual(w, 1.0))
	{
		if (!gloIsFloatEqual(w, 0.0))
		{
			this->x = this->x / w;
			this->y = this->y / w;
			this->z = this->z / w;
		}
	}
}
San::Mathematics::SVECTOR4::SVECTOR4(const sfloat* pArray)
	:stSPOINT4(pArray[0], pArray[1], pArray[2], 0.0)
{
	if (!::gloIsFloatEqual(pArray[3], 1.0))
	{
		if (!gloIsFloatEqual(pArray[3], 0.0))
		{
			this->x = this->x / pArray[3];
			this->y = this->y / pArray[3];
			this->z = this->z / pArray[3];
		}
	}
}
San::Mathematics::SVECTOR4& San::Mathematics::SVECTOR4::operator=(const SPOINT3 &Point3)
{
	this->x = Point3.x;
	this->y = Point3.y;
	this->z = Point3.z;
	this->w = 0.0;
	return *this;
}
San::Mathematics::SVECTOR4& San::Mathematics::SVECTOR4::operator=(const SPOINT4 &Point4)
{
	this->x = Point4.x;
	this->y = Point4.y;
	this->z = Point4.z;
	if (!::gloIsFloatEqual(Point4.w, 1.0))
	{
		if (!gloIsFloatEqual(Point4.w, 0.0))
		{
			this->x = this->x / Point4.w;
			this->y = this->y / Point4.w;
			this->z = this->z / Point4.w;
		}
	}
	return *this;
}
San::Mathematics::SVECTOR4& San::Mathematics::SVECTOR4::operator=(const SVECTOR4 &Vec4)
{
	this->x = Vec4.x;
	this->y = Vec4.y;
	this->z = Vec4.z;
	this->w = 0.0;
	return *this;
}
San::Mathematics::SVECTOR4 San::Mathematics::SVECTOR4::operator+(const SVECTOR4 &Vec4) const
{
	return SVECTOR4(this->x+Vec4.x,this->y+Vec4.y,this->z+Vec4.z,0.0);
}
San::Mathematics::SVECTOR4 San::Mathematics::SVECTOR4::operator-(const SVECTOR4 &Vec4) const
{
	return SVECTOR4(this->x-Vec4.x,this->y-Vec4.y,this->z-Vec4.z,0.0);
}
San::Mathematics::SVECTOR4 San::Mathematics::SVECTOR4::operator*(const SVECTOR4 &Vec4) const
{
	return SVECTOR4(this->x*Vec4.x,this->y*Vec4.y,this->z*Vec4.z,0.0);
}
San::Mathematics::SVECTOR4 San::Mathematics::SVECTOR4::operator+(const sfloat Val) const
{
	return SVECTOR4(this->x+Val,this->y+Val,this->z+Val,0.0);
}
San::Mathematics::SVECTOR4 San::Mathematics::SVECTOR4::operator-(const sfloat Val) const
{
	return SVECTOR4(this->x-Val,this->y-Val,this->z-Val,0.0);
}
San::Mathematics::SVECTOR4 San::Mathematics::SVECTOR4::operator*(const sfloat Val) const
{
	return SVECTOR4(this->x*Val,this->y*Val,this->z*Val,0.0);
}
bool San::Mathematics::SVECTOR4::operator==(const SVECTOR4 &Vec4) const
{
	if (!::gloIsFloatEqual(this->x, Vec4.x))
	{
		return false;
	}
	if (!::gloIsFloatEqual(this->y, Vec4.y))
	{
		return false;
	}
	if (!::gloIsFloatEqual(this->z, Vec4.z))
	{
		return false;
	}
	if (!::gloIsFloatEqual(this->w, Vec4.w))
	{
		return false;
	}
	return true;
}
bool San::Mathematics::SVECTOR4::operator!=(const SVECTOR4 &Vec4) const
{
	bool bEqual = true;
	if (!::gloIsFloatEqual(this->x, Vec4.x))
	{
		bEqual = false;
	}
	if (!::gloIsFloatEqual(this->y, Vec4.y))
	{
		bEqual = false;
	}
	if (!::gloIsFloatEqual(this->z, Vec4.z))
	{
		bEqual = false;
	}
	if (!::gloIsFloatEqual(this->w, Vec4.w))
	{
		bEqual = false;
	}
	return !bEqual;
}
void San::Mathematics::SVECTOR4::iHomogenzation()
{
	this->w = 0.0f;
}
San::Mathematics::SANMATRIX3X3::SANMATRIX3X3(const SANMATRIX3X3 &Mat3x3)
{
	this->e11 = Mat3x3.e11; this->e12 = Mat3x3.e12; this->e13 = Mat3x3.e13;
	this->e21 = Mat3x3.e21; this->e22 = Mat3x3.e22; this->e23 = Mat3x3.e23;
	this->e31 = Mat3x3.e31; this->e32 = Mat3x3.e32; this->e33 = Mat3x3.e33;
}
San::Mathematics::SANMATRIX3X3::SANMATRIX3X3(const SVECTOR3 &Vec3_1, const SVECTOR3 &Vec3_2, const SVECTOR3 &Vec3_3)
{
	this->e11 = Vec3_1.x; this->e12 = Vec3_1.y; this->e13 = Vec3_1.z;
	this->e21 = Vec3_2.x; this->e22 = Vec3_2.y; this->e23 = Vec3_2.z;
	this->e31 = Vec3_3.x; this->e32 = Vec3_3.y; this->e33 = Vec3_3.z;
}
San::Mathematics::SANMATRIX3X3::SANMATRIX3X3(const sfloat e11, const sfloat e12, const sfloat e13,
	const sfloat e21, const sfloat e22, const sfloat e23,
	const sfloat e31, const sfloat e32, const sfloat e33)
{
	this->e11 = e11; this->e12 = e12; this->e13 = e13;
	this->e21 = e21; this->e22 = e22; this->e23 = e23;
	this->e31 = e31; this->e32 = e32; this->e33 = e33;
}
San::Mathematics::SANMATRIX3X3::SANMATRIX3X3(const sfloat* pArray)
{
	::gloMemCopy((void*) this->ma, (const void*) pArray, sizeof(sfloat)*9);
}
San::Mathematics::SANMATRIX3X3& San::Mathematics::SANMATRIX3X3::operator=(const SANMATRIX3X3 &Mat3x3)
{
	this->e11 = Mat3x3.e11; this->e12 = Mat3x3.e12; this->e13 = Mat3x3.e13;
	this->e21 = Mat3x3.e21; this->e22 = Mat3x3.e22; this->e23 = Mat3x3.e23;
	this->e31 = Mat3x3.e31; this->e32 = Mat3x3.e32; this->e33 = Mat3x3.e33;
	return *this;
}
San::Mathematics::SANMATRIX3X3& San::Mathematics::SANMATRIX3X3::operator=(const sfloat* pArray)
{
	::gloMemCopy((void*) this->ma, (const void*) pArray, sizeof(sfloat)*9);
	return *this;
}
San::Mathematics::SANMATRIX3X3 San::Mathematics::SANMATRIX3X3::operator+(const SANMATRIX3X3 &Mat3x3) const
{
	return SANMATRIX3X3(this->e11 + Mat3x3.e11, this->e12 + Mat3x3.e12, this->e13 + Mat3x3.e13,
		this->e21 + Mat3x3.e21, this->e22 + Mat3x3.e22, this->e23 + Mat3x3.e23,
		this->e31 + Mat3x3.e31, this->e32 + Mat3x3.e32, this->e33 + Mat3x3.e33);
}
San::Mathematics::SANMATRIX3X3 San::Mathematics::SANMATRIX3X3::operator-(const SANMATRIX3X3 &Mat3x3) const
{
	return SANMATRIX3X3(this->e11 - Mat3x3.e11, this->e12 - Mat3x3.e12, this->e13 - Mat3x3.e13,
		this->e21 - Mat3x3.e21, this->e22 - Mat3x3.e22, this->e23 - Mat3x3.e23,
		this->e31 - Mat3x3.e31, this->e32 - Mat3x3.e32, this->e33 - Mat3x3.e33);
}
San::Mathematics::SANMATRIX3X3 San::Mathematics::SANMATRIX3X3::operator*(const SANMATRIX3X3 &Mat3x3) const
{
	SANMATRIX3X3 Mat;
	SVECTOR3 Vec;
	for (uint8 s_row = 0; s_row < 3; s_row = s_row + 1)
	{
		for (uint8 s_col = 0; s_col < 3; s_col = s_col + 1)
		{
			Vec.x = this->m[s_row][0] * Mat3x3.m[0][s_col];
			Vec.y = this->m[s_row][1] * Mat3x3.m[1][s_col];
			Vec.z = this->m[s_row][2] * Mat3x3.m[2][s_col];
			Mat.m[s_row][s_col] = Vec.x + Vec.y + Vec.z;
		}
	}
	return Mat;
}
San::Mathematics::SANMATRIX3X3 San::Mathematics::SANMATRIX3X3::operator*(const sfloat Value) const
{
	return SANMATRIX3X3(this->e11*Value, this->e12*Value, this->e13*Value,
		this->e21*Value, this->e22*Value, this->e23*Value,
		this->e31*Value, this->e32*Value, this->e33*Value);
}
bool San::Mathematics::SANMATRIX3X3::operator==(const SANMATRIX3X3 &Mat3x3) const
{
	for (uint8 seek_row = 0; seek_row < 3; seek_row = seek_row + 1)
	{
		for (uint8 seek_col = 0; seek_col < 3; seek_col = seek_col + 1)
		{
			if (!::gloIsFloatEqual(this->m[seek_row][seek_col], Mat3x3.m[seek_row][seek_col]))
			{
				return false;
			}
		}
	}
	return true;
}
bool San::Mathematics::SANMATRIX3X3::operator!=(const SANMATRIX3X3 &Mat3x3) const
{
	bool bEqual = true;
	for (uint8 seek_row = 0; seek_row < 3; seek_row = seek_row + 1)
	{
		for (uint8 seek_col = 0; seek_col < 3; seek_col = seek_col + 1)
		{
			if (!::gloIsFloatEqual(this->m[seek_row][seek_col], Mat3x3.m[seek_row][seek_col]))
			{
				bEqual = false;
			}
		}
	}
	return !bEqual;
}
void San::Mathematics::SANMATRIX3X3::iIdentity()
{
	this->e11 = 1.0f; this->e12 = 0.0; this->e13 = 0.0;
	this->e21 = 0.0; this->e22 = 1.0f; this->e23 = 0.0;
	this->e31 = 0.0; this->e32 = 0.0; this->e33 = 1.0f;
}
void San::Mathematics::SANMATRIX3X3::iAdd(const SVECTOR3 &Vec3, const uint8 Index, bool bIsRow)
{
	if (Index > 2)
	{
		return;
	}
	if (bIsRow)
	{
		this->m[Index][0] = this->m[Index][0] + Vec3.x;
		this->m[Index][1] = this->m[Index][1] + Vec3.y;
		this->m[Index][2] = this->m[Index][2] + Vec3.z;
	}
	else
	{
		this->m[0][Index] = this->m[0][Index] + Vec3.x;
		this->m[1][Index] = this->m[1][Index] + Vec3.y;
		this->m[2][Index] = this->m[2][Index] + Vec3.z;
	}
}
void San::Mathematics::SANMATRIX3X3::iSub(const SVECTOR3 &Vec3, const uint8 Index, bool bIsRow)
{
	if (Index > 2)
	{
		return;
	}
	if (bIsRow)
	{
		this->m[Index][0] = this->m[Index][0] - Vec3.x;
		this->m[Index][1] = this->m[Index][1] - Vec3.y;
		this->m[Index][2] = this->m[Index][2] - Vec3.z;
	}
	else
	{
		this->m[0][Index] = this->m[0][Index] - Vec3.x;
		this->m[1][Index] = this->m[1][Index] - Vec3.y;
		this->m[2][Index] = this->m[2][Index] - Vec3.z;
	}
}
sfloat San::Mathematics::SANMATRIX3X3::iSum()
{
	return this->e11*this->e22*this->e33 + this->e12*this->e23*this->e31 + this->e13*this->e21*this->e32 -
		this->e13*this->e22*this->e31 - this->e12*this->e21*this->e33 - this->e11*this->e23*this->e32;
}
void San::Mathematics::SANMATRIX3X3::iSetRow(const SVECTOR3 &Vec3, const uint8 Index)
{
	if (Index <= 2)
	{
		this->m[Index][0] = Vec3.x;
		this->m[Index][1] = Vec3.y;
		this->m[Index][2] = Vec3.z;
	}
}
void San::Mathematics::SANMATRIX3X3::iSetCol(const SVECTOR3 &Vec3, const uint8 Index)
{
	if (Index <= 2)
	{
		this->m[0][Index] = Vec3.x;
		this->m[1][Index] = Vec3.y;
		this->m[2][Index] = Vec3.z;
	}
}
San::Mathematics::SVECTOR3 San::Mathematics::SANMATRIX3X3::iGetRow(const uint8 Index) const
{
	if (Index <= 2)
	{
		return SVECTOR3(this->m[Index]);
	}
	return SVECTOR3(0.0, 0.0, 0.0);
}
San::Mathematics::SVECTOR3 San::Mathematics::SANMATRIX3X3::iGetCol(const uint8 Index) const
{
	if (Index <= 2)
	{
		return SVECTOR3(this->m[0][Index], this->m[1][Index], this->m[2][Index]);
	}
	return SVECTOR3(0.0, 0.0, 0.0);
}
San::Mathematics::SANMATRIX4X4::SANMATRIX4X4()
{
	this->e11 = 0.0; this->e12 = 0.0; this->e13 = 0.0; this->e14 = 0.0;
	this->e21 = 0.0; this->e22 = 0.0; this->e23 = 0.0; this->e24 = 0.0;
	this->e31 = 0.0; this->e32 = 0.0; this->e33 = 0.0; this->e34 = 0.0;
	this->e41 = 0.0; this->e42 = 0.0; this->e43 = 0.0; this->e44 = 0.0;
}
San::Mathematics::SANMATRIX4X4::SANMATRIX4X4(const SANMATRIX4X4 &Mat4x4)
{
	this->e11=Mat4x4.e11;this->e12=Mat4x4.e12;this->e13=Mat4x4.e13;this->e14=Mat4x4.e14;
	this->e21=Mat4x4.e21;this->e22=Mat4x4.e22;this->e23=Mat4x4.e23;this->e24=Mat4x4.e24;
	this->e31=Mat4x4.e31;this->e32=Mat4x4.e32;this->e33=Mat4x4.e33;this->e34=Mat4x4.e34;
	this->e41=Mat4x4.e41;this->e42=Mat4x4.e42;this->e43=Mat4x4.e43;this->e44=Mat4x4.e44;
}
San::Mathematics::SANMATRIX4X4::SANMATRIX4X4(const SPOINT4 &Point4_1, const SPOINT4 &Point4_2, const SPOINT4 &Point4_3, const SPOINT4 &Point4_4)
{
	this->e11 = Point4_1.x; this->e12 = Point4_1.y; this->e13 = Point4_1.z; this->e14 = Point4_1.w;
	this->e21 = Point4_2.x; this->e22 = Point4_2.y; this->e23 = Point4_2.z; this->e24 = Point4_2.w;
	this->e31 = Point4_3.x; this->e32 = Point4_3.y; this->e33 = Point4_3.z; this->e34 = Point4_3.w;
	this->e41 = Point4_4.x; this->e42 = Point4_4.y; this->e43 = Point4_4.z; this->e44 = Point4_4.w;
}
San::Mathematics::SANMATRIX4X4::SANMATRIX4X4(const SVECTOR4 &Vec4_1,const SVECTOR4 &Vec4_2,const SVECTOR4 &Vec4_3,const SVECTOR4 &Vec4_4)
{
	this->e11=Vec4_1.x;this->e12=Vec4_1.y;this->e13=Vec4_1.z;this->e14=Vec4_1.w;
	this->e21=Vec4_2.x;this->e22=Vec4_2.y;this->e23=Vec4_2.z;this->e24=Vec4_2.w;
	this->e31=Vec4_3.x;this->e32=Vec4_3.y;this->e33=Vec4_3.z;this->e34=Vec4_3.w;
	this->e41=Vec4_4.x;this->e42=Vec4_4.y;this->e43=Vec4_4.z;this->e44=Vec4_4.w;
}
San::Mathematics::SANMATRIX4X4::SANMATRIX4X4(const sfloat e11,const sfloat e12,const sfloat e13,const sfloat e14,
						const sfloat e21,const sfloat e22,const sfloat e23,const sfloat e24,
						const sfloat e31,const sfloat e32,const sfloat e33,const sfloat e34,
						const sfloat e41,const sfloat e42,const sfloat e43,const sfloat e44)
{
	this->e11=e11;this->e12=e12;this->e13=e13;this->e14=e14;
	this->e21=e21;this->e22=e22;this->e23=e23;this->e24=e24;
	this->e31=e31;this->e32=e32;this->e33=e33;this->e34=e34;
	this->e41=e41;this->e42=e42;this->e43=e43;this->e44=e44;
}
San::Mathematics::SANMATRIX4X4::SANMATRIX4X4(const sfloat* pArray)
{
	::gloMemCopy((void*)this->ma,(const void*)pArray,sizeof(sfloat)*16);
}
San::Mathematics::SANMATRIX4X4& San::Mathematics::SANMATRIX4X4::operator=(const SANMATRIX4X4 &Mat4x4)
{
	this->e11=Mat4x4.e11;this->e12=Mat4x4.e12;this->e13=Mat4x4.e13;this->e14=Mat4x4.e14;
	this->e21=Mat4x4.e21;this->e22=Mat4x4.e22;this->e23=Mat4x4.e23;this->e24=Mat4x4.e24;
	this->e31=Mat4x4.e31;this->e32=Mat4x4.e32;this->e33=Mat4x4.e33;this->e34=Mat4x4.e34;
	this->e41=Mat4x4.e41;this->e42=Mat4x4.e42;this->e43=Mat4x4.e43;this->e44=Mat4x4.e44;
	return *this;
}
San::Mathematics::SANMATRIX4X4& San::Mathematics::SANMATRIX4X4::operator=(const sfloat* pArray)
{
	::gloMemCopy((void*)this->ma,(const void*)pArray,sizeof(sfloat)*16);
	return *this;
}
San::Mathematics::SANMATRIX4X4 San::Mathematics::SANMATRIX4X4::operator+(const SANMATRIX4X4 &Mat4x4) const
{
	return SANMATRIX4X4(this->e11+Mat4x4.e11,this->e12+Mat4x4.e12,this->e13+Mat4x4.e13,this->e14+Mat4x4.e14,
						this->e21+Mat4x4.e21,this->e22+Mat4x4.e22,this->e23+Mat4x4.e23,this->e24+Mat4x4.e24,
						this->e31+Mat4x4.e31,this->e32+Mat4x4.e32,this->e33+Mat4x4.e33,this->e34+Mat4x4.e34,
						this->e41+Mat4x4.e41,this->e42+Mat4x4.e42,this->e43+Mat4x4.e43,this->e44+Mat4x4.e44);
}
San::Mathematics::SANMATRIX4X4 San::Mathematics::SANMATRIX4X4::operator-(const SANMATRIX4X4 &Mat4x4) const
{
	return SANMATRIX4X4(this->e11-Mat4x4.e11,this->e12-Mat4x4.e12,this->e13-Mat4x4.e13,this->e14-Mat4x4.e14,
						this->e21-Mat4x4.e21,this->e22-Mat4x4.e22,this->e23-Mat4x4.e23,this->e24-Mat4x4.e24,
						this->e31-Mat4x4.e31,this->e32-Mat4x4.e32,this->e33-Mat4x4.e33,this->e34-Mat4x4.e34,
						this->e41-Mat4x4.e41,this->e42-Mat4x4.e42,this->e43-Mat4x4.e43,this->e44-Mat4x4.e44);
}
San::Mathematics::SANMATRIX4X4 San::Mathematics::SANMATRIX4X4::operator*(const SANMATRIX4X4 &Mat4x4) const
{
	SANMATRIX4X4 Mat;
	SVECTOR4 Vec;
	for (uint8 s_row = 0; s_row < 4; s_row = s_row + 1)
	{
		for (uint8 s_col = 0; s_col < 4; s_col = s_col + 1)
		{
			Vec.x = this->m[s_row][0] * Mat4x4.m[0][s_col];
			Vec.y = this->m[s_row][1] * Mat4x4.m[1][s_col];
			Vec.z = this->m[s_row][2] * Mat4x4.m[2][s_col];
			Vec.w = this->m[s_row][3] * Mat4x4.m[3][s_col];
			Mat.m[s_row][s_col] = Vec.x + Vec.y + Vec.z + Vec.w;
		}
	}
	return Mat;
}
San::Mathematics::SANMATRIX4X4 San::Mathematics::SANMATRIX4X4::operator*(const sfloat Value) const
{
	return SANMATRIX4X4(this->e11*Value,this->e12*Value,this->e13*Value,this->e14*Value,
						this->e21*Value,this->e22*Value,this->e23*Value,this->e24*Value,
						this->e31*Value,this->e32*Value,this->e33*Value,this->e34*Value,
						this->e41*Value,this->e42*Value,this->e43*Value,this->e44*Value);
}
bool San::Mathematics::SANMATRIX4X4::operator==(const SANMATRIX4X4 &Mat4x4) const
{
	for (uint8 seek_row = 0; seek_row < 4; seek_row = seek_row + 1)
	{
		for (uint8 seek_col = 0; seek_col < 4; seek_col = seek_col + 1)
		{
			if (this->m[seek_row][seek_col] != Mat4x4.m[seek_row][seek_col])
			{
				return false;
			}
		}
	}
	return true;
}
bool San::Mathematics::SANMATRIX4X4::operator!=(const SANMATRIX4X4 &Mat4x4) const
{
	for (uint8 seek_row = 0; seek_row < 4; seek_row = seek_row + 1)
	{
		for (uint8 seek_col = 0; seek_col < 4; seek_col = seek_col + 1)
		{
			if (this->m[seek_row][seek_col] != Mat4x4.m[seek_row][seek_col])
			{
				return true;
			}
		}
	}
	return false;
}
void San::Mathematics::SANMATRIX4X4::iIdentity()
{
	this->e11=1.0f;this->e12=0.0;this->e13=0.0;this->e14=0.0;
	this->e21=0.0;this->e22=1.0f;this->e23=0.0;this->e24=0.0;
	this->e31=0.0;this->e32=0.0;this->e33=1.0f;this->e34=0.0;
	this->e41=0.0;this->e42=0.0;this->e43=0.0;this->e44=1.0f;
}
void San::Mathematics::SANMATRIX4X4::iAdd(const SPOINT4 &Point4, const uint8 Index, bool bIsRow)
{
	if (Index>3)
	{
		return;
	}
	if (bIsRow)
	{
		this->m[Index][0] = this->m[Index][0] + Point4.x;
		this->m[Index][1] = this->m[Index][1] + Point4.y;
		this->m[Index][2] = this->m[Index][2] + Point4.z;
		this->m[Index][3] = this->m[Index][3] + Point4.w;
	}
	else
	{
		this->m[0][Index] = this->m[0][Index] + Point4.x;
		this->m[1][Index] = this->m[1][Index] + Point4.y;
		this->m[2][Index] = this->m[2][Index] + Point4.z;
		this->m[3][Index] = this->m[3][Index] + Point4.w;
	}
}
void San::Mathematics::SANMATRIX4X4::iAdd(const SVECTOR4 &Vec4,const uint8 Index,bool bIsRow)
{
	if(Index>3)
	{
		return;
	}
	if(bIsRow)
	{
		this->m[Index][0]=this->m[Index][0]+Vec4.x;
		this->m[Index][1]=this->m[Index][1]+Vec4.y;
		this->m[Index][2]=this->m[Index][2]+Vec4.z;
		this->m[Index][3]=this->m[Index][3]+Vec4.w;
	}
	else
	{
		this->m[0][Index]=this->m[0][Index]+Vec4.x;
		this->m[1][Index]=this->m[1][Index]+Vec4.y;
		this->m[2][Index]=this->m[2][Index]+Vec4.z;
		this->m[3][Index]=this->m[3][Index]+Vec4.w;
	}
}
void San::Mathematics::SANMATRIX4X4::iSub(const SPOINT4 &Point4, const uint8 Index, bool bIsRow)
{
	if (Index>3)
	{
		return;
	}
	if (bIsRow)
	{
		this->m[Index][0] = this->m[Index][0] - Point4.x;
		this->m[Index][1] = this->m[Index][1] - Point4.y;
		this->m[Index][2] = this->m[Index][2] - Point4.z;
		this->m[Index][3] = this->m[Index][3] - Point4.w;
	}
	else
	{
		this->m[0][Index] = this->m[0][Index] - Point4.x;
		this->m[1][Index] = this->m[1][Index] - Point4.y;
		this->m[2][Index] = this->m[2][Index] - Point4.z;
		this->m[3][Index] = this->m[3][Index] - Point4.w;
	}
}
void San::Mathematics::SANMATRIX4X4::iSub(const SVECTOR4 &Vec4,const uint8 Index,bool bIsRow)
{
	if(Index>3)
	{
		return;
	}
	if(bIsRow)
	{
		this->m[Index][0]=this->m[Index][0]-Vec4.x;
		this->m[Index][1]=this->m[Index][1]-Vec4.y;
		this->m[Index][2]=this->m[Index][2]-Vec4.z;
		this->m[Index][3]=this->m[Index][3]-Vec4.w;
	}
	else
	{
		this->m[0][Index]=this->m[0][Index]-Vec4.x;
		this->m[1][Index]=this->m[1][Index]-Vec4.y;
		this->m[2][Index]=this->m[2][Index]-Vec4.z;
		this->m[3][Index]=this->m[3][Index]-Vec4.w;
	}
}
sfloat San::Mathematics::SANMATRIX4X4::iSum()
{
	return 0.0;
}
void San::Mathematics::SANMATRIX4X4::iSetRow(const SPOINT4 &Point4, const uint8 Index)
{
	if (Index <= 3)
	{
		this->m[Index][0] = Point4.x;
		this->m[Index][1] = Point4.y;
		this->m[Index][2] = Point4.z;
		this->m[Index][3] = Point4.w;
	}
}
void San::Mathematics::SANMATRIX4X4::iSetRow(const SVECTOR4 &Vec4,const uint8 Index)
{
	if(Index<=3)
	{
		this->m[Index][0]=Vec4.x;
		this->m[Index][1]=Vec4.y;
		this->m[Index][2]=Vec4.z;
		this->m[Index][3]=Vec4.w;
	}
}
void San::Mathematics::SANMATRIX4X4::iSetCol(const SPOINT4 &Point4, const uint8 Index)
{
	if (Index <= 3)
	{
		this->m[0][Index] = Point4.x;
		this->m[1][Index] = Point4.y;
		this->m[2][Index] = Point4.z;
		this->m[3][Index] = Point4.w;
	}
}
void San::Mathematics::SANMATRIX4X4::iSetCol(const SVECTOR4 &Vec4,const uint8 Index)
{
	if(Index<=3)
	{
		this->m[0][Index]=Vec4.x;
		this->m[1][Index]=Vec4.y;
		this->m[2][Index]=Vec4.z;
		this->m[3][Index]=Vec4.w;
	}
}
San::Mathematics::SVECTOR4 San::Mathematics::SANMATRIX4X4::iGetRow(const uint8 Index) const
{
	if(Index<=3)
	{
		return SVECTOR4(this->m[Index]);
	}
	return SVECTOR4(0.0,0.0,0.0,0.0);
}
San::Mathematics::SVECTOR4 San::Mathematics::SANMATRIX4X4::iGetCol(const uint8 Index) const
{
	if(Index<=3)
	{
		return SVECTOR4(this->m[0][Index],this->m[1][Index],this->m[2][Index],this->m[3][Index]);
	}
	return SVECTOR4(0.0,0.0,0.0,0.0);
}
San::Mathematics::stSANSHAPE::stSANSHAPE()
	:type(eSANSHAPETYPE::SST_SPHERE),
	origion(0.0,0.0,0.0),
	direction(0.0,0.0,0.0),
	rotation(0.0,0.0,0.0),
	param(0.0,0.0,0.0)
{
	//::gloMemSet(this->paramlist,0,sizeof(this->paramlist));
}
San::Mathematics::stSANSHAPE::stSANSHAPE(const stSANSHAPE &shape)
	:type(shape.type),
	origion(shape.origion),
	direction(shape.direction),
	rotation(shape.rotation),
	param(shape.param)
{
	//::gloMemCopy(this->paramlist,shape.paramlist,sizeof(this->paramlist));
}
San::Mathematics::stSANSHAPE::stSANSHAPE(const eSANSHAPETYPE type,const lpSPOINT3 p_paramlist)
	:type(type),
	origion(p_paramlist [0]),
	direction(p_paramlist [1]),
	rotation(p_paramlist [2]),
	param(p_paramlist [3])
{
	//::gloMemCopy(this->paramlist,pParamlist,sizeof(this->paramlist));
}
San::Mathematics::stSANSHAPE::stSANSHAPE(const eSANSHAPETYPE type,const SPOINT3 &origion,const SVECTOR3 &diection,const SVECTOR3 &rotation,const SVECTOR3 &prarm)
	:type(type),
	origion(origion),
	direction(direction),
	rotation(rotation),
	param(param)
{
}
San::Mathematics::stSANSHAPE San::Mathematics::stSANSHAPE::operator+(const sfloat &value)
{
	switch(this->type)
	{
	case SST_PLANE:
		return stSANSHAPE(this->type, this->origion + value, this->direction, this->rotation, this->param);
		break;
	case SST_SPHERE:
		return stSANSHAPE(this->type, this->origion + value, this->direction, this->rotation, this->param);
		break;
	case SST_CUBOID:
		return stSANSHAPE(this->type, this->origion + value, this->direction, this->rotation, this->param);
		break;
	case SST_CYLINDER:
		return stSANSHAPE(this->type, this->origion + value, this->direction, this->rotation + value, this->param);
		break;
	default:
		return stSANSHAPE();
		break;
	}
}
San::Mathematics::stSANSHAPE San::Mathematics::stSANSHAPE::operator-(const sfloat &value)
{
	switch(this->type)
	{
	case SST_PLANE:
		return stSANSHAPE(this->type, this->origion - value, this->direction, this->rotation, this->param);
		break;
	case SST_SPHERE:
		return stSANSHAPE(this->type,this->origion-value,this->direction,this->rotation,this->param);
		break;
	case SST_CUBOID:
		return stSANSHAPE(this->type,this->origion-value,this->direction,this->rotation,this->param);
		break;
	case SST_CYLINDER:
		return stSANSHAPE(this->type,this->origion-value,this->direction,this->rotation-value,this->param);
		break;
	default:
		return stSANSHAPE();
		break;
	}
}
San::Mathematics::stSANSHAPE San::Mathematics::stSANSHAPE::operator*(const sfloat &value)
{
	stSANSHAPE shape(*this);
	SPOINT3 vector(0.0,0.0,0.0);
	switch(this->type)
	{
	case SST_PLANE:
		shape.param = ::gloAbs(this->param * value);
		break;
	case SST_SPHERE:
		shape.origion.x=::abs(this->direction.x*value);
		break;
	case SST_CUBOID:
		vector=shape.rotation-this->origion;
		vector=::gloAbs(vector*value);
		shape.direction=SPOINT3(::gloMod(vector),0.0,0.0);
		shape.rotation=this->origion+vector;
		shape.param=this->origion-vector;
		break;
	case SST_CYLINDER:
		vector=(this->origion+this->rotation)*0.5;
		shape.origion=vector+(vector-this->origion)*value;
		shape.rotation=vector+(vector-this->rotation)*value;
		shape.direction=SPOINT3(this->direction.x*value,0.0,0.0);
		shape.param=SPOINT3(this->param.x*value,0.0,0.0);
		break;
	default:
		break;
	}
	return shape;
}
San::Mathematics::stSANSHAPE San::Mathematics::stSANSHAPE::operator+(const SPOINT3 &vector)
{
	switch(this->type)
	{
	case SST_PLANE:
		return stSANSHAPE(this->type, this->origion + vector, this->direction, this->rotation, this->param);
		break;
	case SST_SPHERE:
		return stSANSHAPE(this->type, this->origion + vector, this->direction, this->rotation, this->param);
		break;
	case SST_CUBOID:
		return stSANSHAPE(this->type, this->origion + vector, this->direction, this->rotation, this->param);
		break;
	case SST_CYLINDER:
		return stSANSHAPE(this->type, this->origion + vector, this->direction, this->rotation + vector, this->param);
		break;
	default:
		return stSANSHAPE();
		break;
	}
	return *this;
}
San::Mathematics::stSANSHAPE San::Mathematics::stSANSHAPE::operator-(const SPOINT3 &vector)
{
	switch(this->type)
	{
	case SST_PLANE:
		return stSANSHAPE(this->type, this->origion - vector, this->direction, this->rotation, this->param);
		break;
	case SST_SPHERE:
		return stSANSHAPE(this->type, this->origion - vector, this->direction, this->rotation, this->param);
		break;
	case SST_CUBOID:
		return stSANSHAPE(this->type, this->origion - vector, this->direction, this->rotation, this->param);
		break;
	case SST_CYLINDER:
		return stSANSHAPE(this->type, this->origion - vector, this->direction, this->rotation - vector, this->param);
		break;
	default:
		return stSANSHAPE();
		break;
	}
	return *this;
}
San::Mathematics::stSANSHAPE San::Mathematics::stSANSHAPE::operator*(const SPOINT3 &vector)
{
	stSANSHAPE shape(*this);
	sfloat value=0.0;
	SPOINT3 vec(0.0,0.0,0.0);
	switch(this->type)
	{
	case SST_PLANE:
		break;
	case SST_SPHERE:
		value=this->direction.x;
		vec=vector;
		shape.direction=SPOINT3(::gloMod(::gloAbs(vec*value)),0.0,0.0);
		break;
	case SST_CUBOID:
		value=this->direction.x;
		vec=vector;
		vec=::gloAbs(vec*value);
		shape.direction=SPOINT3(::gloMod(vec),0.0,0.0);
		shape.rotation=this->origion+vec;
		shape.param=this->origion-vec;
		break;
	case SST_CYLINDER:
		break;
	default:
		break;
	}
	return shape;
}
SVECTOR3 San::Mathematics::gloAbs(const SVECTOR3 &vector3)
{
	return SVECTOR3(vector3.x>=0.0?(vector3.x):(-vector3.x),
					vector3.y>=0.0?(vector3.y):(-vector3.y),
					vector3.z>=0.0?(vector3.z):(-vector3.z));
}
SVECTOR4 San::Mathematics::gloAbs(const SVECTOR4 &vector4)
{
	return SVECTOR4(vector4.x>=0.0?(vector4.x):(-vector4.x),
					vector4.y>=0.0?(vector4.y):(-vector4.y),
					vector4.z>=0.0?(vector4.z):(-vector4.z),
					vector4.w>=0.0?(vector4.w):(-vector4.w));
}
void San::Mathematics::gloAbs(sfloat* pDest,const sfloat* pSrc,size_t Size)
{
	if((pDest==nullptr)||(pSrc==nullptr)||(Size==0))
	{
		return;
	}
	for(size_t seek=0;seek<Size;seek=seek+1)
	{
		pDest[seek]=(pSrc[seek]<=0.0)?(pSrc[seek]):(-pSrc[seek]);
	}
}
San::sfloat San::Mathematics::gloMod(const SPOINT3 &vector3) //mod modulus
{
	return sqrt(vector3.x*vector3.x+vector3.y*vector3.y+vector3.z*vector3.z);
}
San::sfloat San::Mathematics::gloMod(const SVECTOR4 &vector4)
{
	return sqrt(vector4.x*vector4.x+vector4.y*vector4.y+vector4.z*vector4.z+vector4.w*vector4.w);
}
SVECTOR3 San::Mathematics::gloNormalize(const SVECTOR3 &vector3)
{
	sfloat mod=::gloMod(vector3);
	if (!::gloIsFloatEqual(mod, 0.0))
	{
		return SVECTOR3(vector3.x / mod, vector3.y / mod, vector3.z / mod);
	}
	return SVECTOR3(0.0,0.0,0.0);
}
SVECTOR4 San::Mathematics::gloNormalize(const SVECTOR4 &vector4)
{
	sfloat mod=::gloMod(vector4);
	if (!::gloIsFloatEqual(mod, 0.0))
	{
		return SVECTOR4(vector4.x/mod,vector4.y/mod,vector4.z/mod,vector4.w/mod);
	}
	return SVECTOR4(0.0,0.0,0.0,0.0);
}
San::sfloat San::Mathematics::gloDot(const SVECTOR3 &vec3_1,const SVECTOR3 &vec3_2)
{
	return vec3_1.x*vec3_2.x+vec3_1.y*vec3_2.y+vec3_1.z*vec3_2.z;
}
San::sfloat San::Mathematics::gloDot(const SVECTOR4 &vec4_1,const SVECTOR4 &vec4_2)
{
	return vec4_1.x*vec4_2.x+vec4_1.y*vec4_2.y+vec4_1.z*vec4_2.z+vec4_1.w*vec4_2.w;
}
SVECTOR3 San::Mathematics::gloCross(const SVECTOR3 &vec3_1,const SVECTOR3 &vec3_2)
{
	return SVECTOR3(vec3_1.y*vec3_2.z-vec3_1.z*vec3_2.y,vec3_1.z*vec3_2.x-vec3_1.x*vec3_2.z,vec3_1.x*vec3_2.y-vec3_1.y*vec3_2.x);
}
SVECTOR4 San::Mathematics::gloCross(const SVECTOR4 &vec4_1,const SVECTOR4 &vec4_2)
{
	return SVECTOR4(vec4_1.x*vec4_2.w-vec4_1.y*vec4_2.x-vec4_1.z*vec4_2.y-vec4_1.w*vec4_2.z,
		vec4_1.y*vec4_2.w+vec4_1.x*vec4_2.x+vec4_1.z*vec4_2.z-vec4_1.w*vec4_2.y,
		vec4_1.z*vec4_2.w+vec4_1.x*vec4_2.y+vec4_1.w*vec4_2.x-vec4_1.y*vec4_2.z,
		vec4_1.w*vec4_2.w+vec4_1.z*vec4_2.x+vec4_1.y*vec4_2.y-vec4_1.x*vec4_2.z);
}
sfloat San::Mathematics::gloLength(const SVECTOR3 &vector3)
{
	return ::sqrt(vector3.x*vector3.x+vector3.y*vector3.y+vector3.z*vector3.z);
}
sfloat San::Mathematics::gloLength(const SVECTOR4 &vector4)
{
	return ::sqrt(vector4.x*vector4.x+vector4.y*vector4.y+vector4.z*vector4.z+vector4.w*vector4.w);
}
SVECTOR3 San::Mathematics::gloCos(const SVECTOR3 &vector3)
{
	return SVECTOR3(::cos(vector3.x),::cos(vector3.y),::cos(vector3.z));
}
SVECTOR4 San::Mathematics::gloCos(const SVECTOR4 &vector4)
{
	return SVECTOR4(::cos(vector4.x),::cos(vector4.y),::cos(vector4.z),::cos(vector4.w));
}
SVECTOR3 San::Mathematics::gloACos(const SVECTOR3 &vector3,const sfloat Length)
{
	SVECTOR3 Theta(0.0,0.0,0.0);
	if (::gloIsFloatEqual(Length, 0.0))
	{
		return Theta;
	}
	Theta = vector3*(1.0 / Length);
	Theta.x=::acos(Theta.x);
	Theta.y=::acos(Theta.y);
	Theta.z=::acos(Theta.z);
	return Theta;
}
SVECTOR4 San::Mathematics::gloACos(const SVECTOR4 &vector4,const sfloat Length)
{
	SVECTOR4 Theta(0.0,0.0,0.0,0.0);
	if (::gloIsFloatEqual(Length, 0.0))
	{
		return Theta;
	}
	Theta = vector4*(1.0 / Length);
	Theta.x = ::acos(Theta.x);
	Theta.y = ::acos(Theta.y);
	Theta.z = ::acos(Theta.z);
	Theta.w = ::acos(Theta.w);
	return Theta;
}
void San::Mathematics::gloSetTranslateMatrix(SANMATRIX4X4 &matrix, const SVECTOR3 &value)
{
	SANMATRIX4X4 TransMatrix;
	TransMatrix.iIdentity();
	TransMatrix.iSetCol((SVECTOR4)value, 3);
	matrix = matrix*TransMatrix;
}
void San::Mathematics::gloSetRotateMatrix(SANMATRIX4X4 &matrix, const SVECTOR3 &angle, const SVECTOR3 &axle)
{
	if (::gloIsFloatEqual(axle.x, 1.0))
	{
		SANMATRIX4X4 RotateMatrix;
		sfloat cos_theta = ::cos(angle.x);
		sfloat sin_theta = ::sin(angle.x);
		RotateMatrix.iIdentity();
		RotateMatrix.e22 = cos_theta;
		RotateMatrix.e23 = -sin_theta;
		RotateMatrix.e32 = sin_theta;
		RotateMatrix.e33 = cos_theta;
		matrix = matrix*RotateMatrix;
	}
	if (::gloIsFloatEqual(axle.y, 1.0))
	{
		SANMATRIX4X4 RotateMatrix;
		sfloat cos_theta = ::cos(angle.y);
		sfloat sin_theta = ::sin(angle.y);
		RotateMatrix.iIdentity();
		RotateMatrix.e11 = cos_theta;
		RotateMatrix.e13 = sin_theta;
		RotateMatrix.e31 = -sin_theta;
		RotateMatrix.e33 = cos_theta;
		matrix = matrix*RotateMatrix;
	}
	if (::gloIsFloatEqual(axle.z, 1.0))
	{
		SANMATRIX4X4 RotateMatrix;
		sfloat cos_theta = ::cos(angle.z);
		sfloat sin_theta = ::sin(angle.z);
		RotateMatrix.iIdentity();
		RotateMatrix.e11 = cos_theta;
		RotateMatrix.e12 = -sin_theta;
		RotateMatrix.e21 = sin_theta;
		RotateMatrix.e22 = cos_theta;
		matrix = matrix*RotateMatrix;
	}
}
void San::Mathematics::gloSetScaleMatrix(SANMATRIX4X4 &matrix, const SVECTOR3 &rate)
{
	SANMATRIX4X4 ScaleMatrix;
	ScaleMatrix.iIdentity();
	ScaleMatrix.e11 = rate.x;
	ScaleMatrix.e22 = rate.y;
	ScaleMatrix.e33 = rate.z;
	matrix = matrix*ScaleMatrix;
}
bool San::Mathematics::gloIsFloatEqual32(const float32 &f1, const float32 &f2, const float64 &bound)
{
	return (f1 <= (f2 + bound)) && (f1 >= (f2 - bound));
}
bool San::Mathematics::gloIsFloatEqual64(const float64 &f1, const float64 &f2, const float64 &bound)
{
	return (f1 <= (f2 + bound)) && (f1 >= (f2 - bound));
}