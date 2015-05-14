//Project: San Lab Game Engine
//Version: 2.1.1
//Debug State: Add mathematics structure and function header
#include"../RenderSystem.h"
#include"Mathematics\SanMathematicsDef.h"
#include"Mathematics\SanMathematicsVector.h"
#include"Mathematics\SanMathematicsVectorT.h"
#include"Mathematics\SanMathematicsMatrix.h"
#pragma once
using namespace std;
namespace San
{
	namespace Mathematics
	{
#ifndef __STDSANMATHEMATICS_H__
#define __STDSANMATHEMATICS_H__

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

#endif
	}
}
