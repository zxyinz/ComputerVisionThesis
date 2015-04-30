#include"SanGraphics.h"
#pragma once
using namespace std;
using namespace San::Mathematics;
namespace San
{
	namespace Graphics
	{
#ifndef __CSANLIGHTDEF_H__
#define __CSANLIGHTDEF_H__

		enum eSANLIGHTTYPE
		{
			SLT_GLOBAL				= 0x00,
			SLT_POINT				= 0x01,
			SLT_DIRECTIONAL			= 0x02,
			SLT_SPOT				= 0x03,
			SLT_LIGHTTYPECOUNT		= 0x04,
			SLT_MAXHARDWAERLIGHT	= 0x08,
			SLT_MAXSOFTWARELIGHT	= 0xff,
		};

		struct stSANLIGHTDESC
		{
			SPOINT3 coord;
			SPOINT3 look_at;//cSanLight use the look at point to calculate the direction
			SVECTOR3 direction;
			eSANLIGHTTYPE type;
			SANCOLORF specular;
			SANCOLORF diffuse;
			SANCOLORF ambient;
			SANCOLORF emission;
			union
			{
				sfloat radius_inside;
				sfloat angle_theta;
			};
			union
			{
				sfloat radius_outside;
				sfloat angle_phi;
			};
			sfloat range;
			sfloat falloff;
			sfloat attenuation;
			sfloat attenuation_linear;
			sfloat attenuation_quadratic;
			bool b_light_movable;
		public:
			stSANLIGHTDESC()
				:type(SLT_POINT),
				coord(0.0,0.0,0.0),
				look_at(0.0,0.0,1.0),
				direction(0.0,0.0,0.0),
				specular(1.0,1.0,1.0,1.0),
				diffuse(1.0,1.0,1.0,1.0),
				ambient(0.0,0.0,0.0,1.0),
				emission(0.0,0.0,0.0,1.0),
				attenuation(1.0),
				attenuation_linear(0.0),
				attenuation_quadratic(0.0),
				b_light_movable(true)
			{
			};
			~stSANLIGHTDESC(){};
		};
		typedef stSANLIGHTDESC	SANLIGHTDESC;
		typedef stSANLIGHTDESC*	lpSANLIGHTDESC;

#endif
	}
}