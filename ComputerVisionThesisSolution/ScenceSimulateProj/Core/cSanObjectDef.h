#include"../Core/cVectorManager.h"
#include"../Core/SanString.h"
#include"../Core/SanMathematics.h"
#include"../Physics/SanPhysics.h"
#pragma once
using namespace std;
namespace San
{
#ifndef __CSANOBJECTDEF_H__
#define __CSANOBJECTDEF_H__

	enum eSANOBJECTTYPE
	{
		SOT_OBJ		= 0x00,
		SOT_MESH	= 0x01,
		SOT_CAMERA	= 0x02,
		SOT_LIGHT	= 0x04,
		SOT_SOUND	= 0x05,
	};

#endif
}