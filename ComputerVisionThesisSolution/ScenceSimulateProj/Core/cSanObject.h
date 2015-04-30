#include"cSanObjectDef.h"
#pragma once
using namespace std;
using namespace San::Mathematics;
namespace San
{
#ifndef __CSANOBJECT_H__
#define __CSANOBJECT_H__
	class cSanObject
	{
	protected:
		static uint32	m_Time;
		SPOINT3			m_Coord;
		SPOINT3			m_Rotation;
		SPOINT3			m_ReleativeRotation;
		lpSPOINT3		m_pBindNode;
		lpSPOINT3		m_pBindRotation;
		SPOINT3			m_CoordOffset;
		eSANOBJECTTYPE	m_Type;
	protected:
		cSanObject(eSANOBJECTTYPE Type,SPOINT3 Coord=SPOINT3(0.0,0.0,0.0),SPOINT3 Rotation=SPOINT3(0.0,0.0,0.0))
			:m_Coord(Coord),
			m_Rotation(Rotation),
			m_ReleativeRotation(SPOINT3(0.0f,0.0f,0.0f)),
			m_pBindNode(nullptr),
			m_pBindRotation(nullptr),
			m_CoordOffset(0,0,0),
			m_Type(Type)
		{
		};
	public:
		cSanObject()
			:m_Coord(0.0,0.0,0.0),
			m_Rotation(0.0,0.0,0.0),
			m_ReleativeRotation(SPOINT3(0.0f, 0.0f, 0.0f)),
			m_pBindNode(nullptr),
			m_pBindRotation(nullptr),
			m_CoordOffset(0.0,0.0,0.0),
			//m_bBendToNode(false),
			m_Type(SOT_OBJ)
		{};
		cSanObject(SPOINT3 Coord,SPOINT3 Rotation=SPOINT3(0.0,0.0,0.0))
			:m_Coord(Coord),
			m_Rotation(Rotation),
			m_ReleativeRotation(SPOINT3(0.0f, 0.0f, 0.0f)),
			m_pBindNode(nullptr),
			m_pBindRotation(nullptr),
			m_CoordOffset(0,0,0),
			//m_bBendToNode(false),
			m_Type(SOT_OBJ)
		{};
		~cSanObject(){};
		virtual bool iBindToNode(lpSPOINT3 pNode,lpSPOINT3 pRotation=nullptr);
		virtual void iUnBindToNode();
		virtual bool iSetCoord(SPOINT3 Coord);
		virtual bool iSetRotation(SPOINT3 Rotation);
		virtual bool iSetCoordOffset(SPOINT3 CoordOffset);
		virtual SPOINT3 iGetCoord();
		virtual lpSPOINT3 iGetCoordPtr();
		virtual const lpSPOINT3 iGetCoordPtrConst();
		virtual SPOINT3 iGetRotation();
		virtual lpSPOINT3 iGetRotationPtr();
		virtual const lpSPOINT3 iGetRotationPtrConst();
		virtual SPOINT3 iGetCoordOffset();
		virtual lpSPOINT3 iGetCoordOffsetPtr();
		virtual const lpSPOINT3 iGetCoordOffsetPtrConst();
		virtual bool iGetBindState() const;
		virtual lpSPOINT3 iGetBindNodePtr() const;
		virtual const lpSPOINT3 iGetBindNodePtrConst() const;
		virtual lpSPOINT3 iGetBindRotationPtr() const;
		virtual const lpSPOINT3 iGetBindRotationPtrConst() const;
		virtual eSANOBJECTTYPE iGetType() const;
		virtual void iUpdateCoord();
	};
#endif
}