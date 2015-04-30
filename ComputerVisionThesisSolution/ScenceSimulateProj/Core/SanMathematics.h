#include"cmath"
//#include"xmmintrin.h"
#include"../RenderSystem.h"
#include"SanContainer.h"
#pragma once
using namespace std;
namespace San
{
	namespace Mathematics
	{
#ifndef __STDSANMATHEMATICS_H__
#define __STDSANMATHEMATICS_H__

		static const float64 S_PI = 3.14159265358979323846;//3.14159265357;
		static const float64 S_PI_2 = S_PI / 2.0;

#ifdef _FLOAT_TYPE_DOUBLE
		static const float64 DEFAULT_FLOAT_COMPARE_BOUNDARY = 0.000000000000001;
#else
		static const float64 DEFAULT_FLOAT_COMPARE_BOUNDARY = 0.000001;
#endif

		struct SPOINT4;
		struct SVECTOR4;

		struct stSPOINT3
		{
		public:
			union
			{
				struct
				{
					sfloat x;
					sfloat y;
					sfloat z;
				};
				sfloat p[3];
			};
		public:
			stSPOINT3();
			stSPOINT3(const stSPOINT3 &Point3);
			stSPOINT3(const SPOINT4 &Point4);
			stSPOINT3(const SVECTOR4 &Vec4);
			stSPOINT3(const sfloat x, const sfloat y, const sfloat z);
			stSPOINT3(const sfloat* pArray);

			//assignment operators
			stSPOINT3& operator = (const POINT &Point2);
			stSPOINT3& operator = (const stSPOINT3 &Point3);
			stSPOINT3& operator = (const SPOINT4 &Point4);
			stSPOINT3& operator = (const SVECTOR4 &Vec4);

			//binary operators
			stSPOINT3 operator * (const stSPOINT3 &Point3) const;
			stSPOINT3 operator + (const stSPOINT3 &Point3) const;
			stSPOINT3 operator - (const stSPOINT3 &Point3) const;
			stSPOINT3 operator * (const sfloat Value) const;
			stSPOINT3 operator + (const sfloat Value) const;
			stSPOINT3 operator - (const sfloat Value) const;

			bool operator == (const stSPOINT3 &Point3) const;
			bool operator != (const stSPOINT3 &Point3) const;
		};
		typedef stSPOINT3	SPOINT3;
		typedef stSPOINT3*	lpSPOINT3;

		typedef SPOINT3		SVECTOR3;
		typedef lpSPOINT3	lpSVECTOR3;

		struct stSPOINT4
		{
		public:
			union
			{
				struct
				{
					sfloat x;
					sfloat y;
					sfloat z;
					sfloat w;
				};
				sfloat p[4];
			};
		public:
			stSPOINT4(const sfloat &x = 0.0, const sfloat &y = 0.0, const sfloat &z = 0.0, const sfloat &w = 0.0)
				:x(x), y(y), z(z), w(w)
			{};
			~stSPOINT4(){};
		};

		struct SPOINT4 :public stSPOINT4
		{
		public:
			SPOINT4();
			SPOINT4(const SPOINT3 &Point3);
			SPOINT4(const SPOINT4 &Point4);
			SPOINT4(const SVECTOR4 &Vec4);
			SPOINT4(const sfloat x, const sfloat y, const sfloat z, const sfloat w = 1.0);
			SPOINT4(const sfloat* pArray);

			SPOINT4& operator = (const SPOINT3 &Point3);
			SPOINT4& operator = (const SPOINT4 &Point4);
			SPOINT4& operator = (const SVECTOR4 &Vec4);

			SPOINT4 operator + (const SPOINT4 &Point4) const;
			SPOINT4 operator - (const SPOINT4 &Point4) const;
			SPOINT4 operator * (const SPOINT4 &Point4) const;
			SPOINT4 operator + (const sfloat Val) const;
			SPOINT4 operator - (const sfloat Val) const;
			SPOINT4 operator * (const sfloat Val) const;

			bool operator == (const SPOINT4 &Point4) const;
			bool operator != (const SPOINT4 &Point4) const;

			void iHomogenzation();
		};
		typedef SPOINT4*	lpSPOINT4;

		struct SVECTOR4 :public stSPOINT4
		{
		public:
			SVECTOR4();
			SVECTOR4(const SPOINT3 &Point3);
			SVECTOR4(const SPOINT4 &Point4);
			SVECTOR4(const SVECTOR4 &Vec4);
			SVECTOR4(const sfloat x, const sfloat y, const sfloat z, const sfloat w = 0.0);
			SVECTOR4(const sfloat* pArray);

			SVECTOR4& operator = (const SPOINT3 &Point3);
			SVECTOR4& operator = (const SPOINT4 &Point4);
			SVECTOR4& operator = (const SVECTOR4 &Vec4);

			SVECTOR4 operator + (const SVECTOR4 &Vec4) const;
			SVECTOR4 operator - (const SVECTOR4 &Vec4) const;
			SVECTOR4 operator * (const SVECTOR4 &Vec4) const;
			SVECTOR4 operator + (const sfloat Val) const;
			SVECTOR4 operator - (const sfloat Val) const;
			SVECTOR4 operator * (const sfloat Val) const;

			bool operator == (const SVECTOR4 &Vec4) const;
			bool operator != (const SVECTOR4 &Vec4) const;

			void iHomogenzation();
		};
		typedef SVECTOR4*	lpSVECTOR4;

		struct stSANMATRIX3X3
		{
			union
			{
				struct
				{
					sfloat e11, e12, e13;
					sfloat e21, e22, e23;
					sfloat e31, e32, e33;
				};
				sfloat	m[3][3];
				sfloat	ma[9];
			};
		};

		struct SANMATRIX3X3 : public stSANMATRIX3X3
		{
		public:
			SANMATRIX3X3(){};
			SANMATRIX3X3(const SANMATRIX3X3 &Mat3x3);
			SANMATRIX3X3(const SVECTOR3 &Vec3_1,
				const SVECTOR3 &Vec3_2,
				const SVECTOR3 &Vec3_3);
			SANMATRIX3X3(const sfloat e11, const sfloat e12, const sfloat e13,
				const sfloat e21, const sfloat e22, const sfloat e23,
				const sfloat e31, const sfloat e32, const sfloat e33);
			SANMATRIX3X3(const sfloat* pArray);

			SANMATRIX3X3& operator = (const SANMATRIX3X3 &Mat3x3);
			SANMATRIX3X3& operator = (const sfloat* pArray);

			SANMATRIX3X3 operator + (const SANMATRIX3X3 &Mat3x3) const;
			SANMATRIX3X3 operator - (const SANMATRIX3X3 &Mat3x3) const;
			SANMATRIX3X3 operator * (const SANMATRIX3X3 &Mat3x3) const;
			SANMATRIX3X3 operator * (const sfloat Value) const;

			bool operator == (const SANMATRIX3X3 &Mat3x3) const;
			bool operator != (const SANMATRIX3X3 &Mat3x3) const;

			void iIdentity();
			void iAdd(const SVECTOR3 &Vec3, const uint8 Index, bool bIsRow = false);
			void iSub(const SVECTOR3 &Vec3, const uint8 Index, bool bIsRow = false);
			sfloat iSum();
			void iSetRow(const SVECTOR3 &Vec3, const uint8 Index);
			void iSetCol(const SVECTOR3 &Vec3, const uint8 Index);
			SVECTOR3 iGetRow(const uint8 Index) const;
			SVECTOR3 iGetCol(const uint8 Index) const;
		};
		typedef SANMATRIX3X3*	lpSANMATRIX3X3;

		struct stSANMATRIX4X4
		{
			union
			{
				struct
				{
					sfloat e11,e12,e13,e14;
					sfloat e21,e22,e23,e24;
					sfloat e31,e32,e33,e34;
					sfloat e41,e42,e43,e44;
				};
				sfloat	m[4][4];
				sfloat	ma[16];
			};
		};

		struct SANMATRIX4X4 : public stSANMATRIX4X4
		{
		public:
			SANMATRIX4X4();
			SANMATRIX4X4(const SANMATRIX4X4 &Mat4x4);
			SANMATRIX4X4(const SPOINT4 &Point4_1,
				const SPOINT4 &Point4_2,
				const SPOINT4 &Point4_3,
				const SPOINT4 &Point4_4);
			SANMATRIX4X4(const SVECTOR4 &Vec4_1,
						const SVECTOR4 &Vec4_2,
						const SVECTOR4 &Vec4_3,
						const SVECTOR4 &Vec4_4);
			SANMATRIX4X4(const sfloat e11,const sfloat e12,const sfloat e13,const sfloat e14,
						const sfloat e21,const sfloat e22,const sfloat e23,const sfloat e24,
						const sfloat e31,const sfloat e32,const sfloat e33,const sfloat e34,
						const sfloat e41,const sfloat e42,const sfloat e43,const sfloat e44);
			SANMATRIX4X4(const sfloat* pArray);

			SANMATRIX4X4& operator = (const SANMATRIX4X4 &Mat4x4);
			SANMATRIX4X4& operator = (const sfloat* pArray);

			SANMATRIX4X4 operator + (const SANMATRIX4X4 &Mat4x4) const;
			SANMATRIX4X4 operator - (const SANMATRIX4X4 &Mat4x4) const;
			SANMATRIX4X4 operator * (const SANMATRIX4X4 &Mat4x4) const;
			SANMATRIX4X4 operator * (const sfloat Value) const;

			bool operator == (const SANMATRIX4X4 &Mat4x4) const;
			bool operator != (const SANMATRIX4X4 &Mat4x4) const;

			void iIdentity();
			void iAdd(const SPOINT4 &Point4, const uint8 Index, bool bIsRow = false);
			void iAdd(const SVECTOR4 &Vec4,const uint8 Index,bool bIsRow=false);
			void iSub(const SPOINT4 &Point4, const uint8 Index, bool bIsRow = false);
			void iSub(const SVECTOR4 &Vec4,const uint8 Index,bool bIsRow=false);
			sfloat iSum();
			void iSetRow(const SPOINT4 &Point4, const uint8 Index);
			void iSetRow(const SVECTOR4 &Vec4,const uint8 Index);
			void iSetCol(const SPOINT4 &Point4, const uint8 Index);
			void iSetCol(const SVECTOR4 &Vec4,const uint8 Index);
			SVECTOR4 iGetRow(const uint8 Index) const;
			SVECTOR4 iGetCol(const uint8 Index) const;
		};
		typedef SANMATRIX4X4*	lpSANMATRIX4X4;

		SVECTOR3 operator*(const SVECTOR3 &Vec3,const SANMATRIX4X4 &Mat16);
		SVECTOR4 operator*(const SVECTOR4 &Vec4,const SANMATRIX4X4 &Mat16);

		template<class ST>
		struct stSPOINT3EX
		{
			union
			{
				struct
				{
					ST x;
					ST y;
					ST z;
				};
				ST p[3];
			};
		};

		template<class ST>
		struct stSANVECTOR3EX
		{
			union
			{
				struct
				{
					ST x;
					ST y;
					ST z;
				};
				ST vec[3];
			};
		};

		template<class ST>
		struct stSPOINT4EX
		{
			union
			{
				struct
				{
					ST x;
					ST y;
					ST z;
					ST w;
				};
				ST p[4];
			};
		};

		template<class ST>
		struct stSANVECTOR4EX
		{
			union
			{
				struct
				{
					ST x;
					ST y;
					ST z;
					ST w;
				};
				ST vec[4];
			};
		};

		template<class ST>
		struct _stSANMATRIX4X4EX
		{
			union
			{
				struct
				{
					ST e11,e12,e13,e14;
					ST e21,e22,e23,e24;
					ST e31,e32,e33,e34;
					ST e41,e42,e43,e44;
				};
				ST m[4][4];
				ST ma[16];
			};
		};

		enum eSANSHAPETYPE
		{
			SST_LINE		= 0x01,
			SST_PLANE		= 0x02,
			SST_SPHERE		= 0x03,
			SST_CUBOID		= 0x04,// AABB OBB
			SST_CYLINDER	= 0x05,// not support yet
		};

		struct stSANSHAPE
		{
		public:
			eSANSHAPETYPE	type;
			SPOINT3 origion;
			SVECTOR3 direction;
			SVECTOR3 rotation;
			SVECTOR3 param;
		public:
			stSANSHAPE();
			stSANSHAPE(const stSANSHAPE &shape);
			stSANSHAPE(eSANSHAPETYPE type,const lpSVECTOR3 p_paramlist);
			stSANSHAPE(eSANSHAPETYPE type,const SPOINT3 &origion,const SPOINT3 &direction,const SPOINT3 &rotation,const SPOINT3 &param);

			stSANSHAPE	operator+(const sfloat &value);
			stSANSHAPE	operator-(const sfloat &value);
			stSANSHAPE	operator*(const sfloat &value);

			stSANSHAPE	operator+(const SPOINT3 &vector);
			stSANSHAPE	operator-(const SPOINT3 &vector);
			stSANSHAPE	operator*(const SPOINT3 &vector);

			bool	operator==(const stSANSHAPE &shape);
			bool	operator!=(const stSANSHAPE &shape);
		};
		typedef stSANSHAPE	SANSHAPE;
		typedef stSANSHAPE*	lpSANSHAPE;
		typedef	list<SANSHAPE>	SANSHAPELIST;
		typedef list<SANSHAPE>*	lpSANSHAPELIST;
		
		SVECTOR3	gloAbs(const SVECTOR3 &vector3);
		SVECTOR4	gloAbs(const SVECTOR4 &vector3);
		void		gloAbs(sfloat* pDest,const sfloat* pSrc,size_t size);

		sfloat		gloMod(const SVECTOR3 &vector3);//mod modulus
		sfloat		gloMod(const SVECTOR4 &vector4);

		SVECTOR3	gloNormalize(const SVECTOR3 &vector3);
		SVECTOR4	gloNormalize(const SVECTOR4 &vector4);

		sfloat		gloDot(const SVECTOR3 &vec3_1,const SVECTOR3 &vec3_2);
		sfloat		gloDot(const SVECTOR4 &vec4_1,const SVECTOR4 &vec4_2);

		SVECTOR3	gloCross(const SVECTOR3 &vec3_1,const SVECTOR3 &vec3_2);
		SVECTOR4	gloCross(const SVECTOR4 &vec4_1,const SVECTOR4 &vec4_2);

		sfloat		gloLength(const SVECTOR3 &vector3);
		sfloat		gloLength(const SVECTOR4 &vector4);

		SVECTOR3	gloCos(const SVECTOR3 &vector3);
		SVECTOR4	gloCos(const SVECTOR4 &vector4);

		SVECTOR3	gloACos(const SVECTOR3 &vector3,const sfloat Length);
		SVECTOR4	gloACos(const SVECTOR4 &vector4,const sfloat Length);

		void		gloSetTranslateMatrix(SANMATRIX4X4 &matrix, const SVECTOR3 &value);
		void		gloSetRotateMatrix(SANMATRIX4X4 &matrix, const SVECTOR3 &angle, const SVECTOR3 &axle);
		void		gloSetScaleMatrix(SANMATRIX4X4 &matrix, const SVECTOR3 &rate);

		bool		gloIsFloatEqual32(const float32 &f1, const float32 &f2, const float64 &bound = DEFAULT_FLOAT_COMPARE_BOUNDARY);
		bool		gloIsFloatEqual64(const float64 &f1, const float64 &f2, const float64 &bound = DEFAULT_FLOAT_COMPARE_BOUNDARY);

#ifdef _FLOAT_TYPE_DOUBLE
#define gloIsFloatEqual gloIsFloatEqual64
#else
#define gloIsFloatEqual gloIsFloatEqual32
#endif

#endif
	}
}
