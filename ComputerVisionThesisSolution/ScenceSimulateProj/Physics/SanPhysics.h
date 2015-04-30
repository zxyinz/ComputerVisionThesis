#include"iostream"
#include"../RenderSystem.h"
#include"list"
#include"../Core/SanString.h"
#include"../Core/SanMathematics.h"
#pragma once
using namespace std;
using namespace San::Mathematics;
namespace San
{
	namespace Physics
	{
#ifndef __STDSANPHYSICS_H__
#define __STDSANPHYSICS_H__

		enum eSANCOLLISIONLEVEL
		{
			SCL_NULL	= 0x00000000,
			SCL_LV0		= 0x00000001,
			SCL_LV1		= 0x00000002,
			SCL_LV2		= 0x00000003,
			SCL_LV3		= 0x00000004,
		};

		struct stSANCOLLISIONELEMENT
		{
			SANSHAPE		LV0;
			lpSANSHAPE		pLV1;
			lpSANSHAPELIST	pLV2;
			lpSANSHAPELIST	pLV3;
		};
		typedef stSANCOLLISIONELEMENT	SANCOLLISIONOBJ;
		typedef stSANCOLLISIONELEMENT*	lpSANCOLLISIONOBJ;

		enum eSANPHYSSTYLE
		{
			SPT_ETHER		= 0,
			SPT_PARTICLE	= 1,
			SPT_RIGID		= 2,
			SPT_DEFORMABLE	= 3,
			SPT_FLUID		= 4,
			SPT_PASTICLES	= 5,
		};

		struct stSANFORCEDESC
		{
			SPOINT3	forcevector;
			SPOINT3	actionpoint;
			eSANPHYSSTYLE	type;
		};
		typedef stSANFORCEDESC	SANFORCE;
		typedef stSANFORCEDESC*	lpSANFORCE;
		typedef list<SANFORCE>	FORCELIST;
		typedef list<SANFORCE>*	lpFORCELIST;
		
		struct stSANPHYSMATERIAL
		{
			sfloat		mass;
			sfloat		energy;
			SPOINT3	speed;
			sfloat		staticfriction;
			sfloat		dynamicfriction;
			sfloat		restitution;
			SPOINT3	dirof_anisotropy;////////////////////?
			sfloat		spring;
			sfloat		minrigidity;
			sfloat		maxrigidity;
			sfloat		elasticity;
			FORCELIST	forcelist;
		};
		typedef stSANPHYSMATERIAL	SANPHYSMATERIAL;
		typedef stSANPHYSMATERIAL*	lpSANPHYSMATERIAL;
#endif
	}
}