#include"cVectorManagerDef.h"
#pragma once
using namespace std;
namespace San
{
#ifndef __CVECTORMANAGER_H__
#define __CVECTORMANAGER_H__
	class cVectorManager
	{
	private:
		static lpVECTORNODE		m_pRootVector;
		static VECTORNODE		m_VIndex[VINDEXSIZE];
		static unsigned int		m_ManagerCount;
		lpVECTORNODE			m_pVector;
		lpVMEMBER				m_pMember;
		bool					m_bFound;
	protected:
		void _OnInitVectorManager();
		void _DestoryVectorManager(){};////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool _VectorFound(SString Name,VECTORNODE **pNode=NULL) const;
		bool _VectorFound(uint32 ID,VECTORNODE **pNode=NULL) const;
		uint32 _GetIndex(SString strName) const;
	public:
		static bool m_bOnInit;
		cVectorManager()
			:m_pVector(NULL),
			m_pMember(NULL),
			m_bFound(false)
		{
			if(this->m_bOnInit)
			{
				this->m_bOnInit=false;
				this->_OnInitVectorManager();
			}
			this->m_ManagerCount=this->m_ManagerCount+1;
		};
		~cVectorManager()
		{
			this->_DestoryVectorManager();
		};
		unsigned int iRegisterVector(SString Name,uint32 Type=VT_USER,SHANDLE Param=0,int MaxSize=-1,void(*pRelease)(SHANDLE)=NULL,bool bCreateIndex=false);
		unsigned int iRegisterVector(uint32 ID,uint32 Type=VT_USER,SHANDLE Param=0,int MaxSize=-1,void(*pRelease)(SHANDLE)=NULL,bool bCreateIndex=false);//////////////////////
		bool iDestoryVector(SString Name,uint32 Type=VF_LOCAL);
		bool iDestoryVector(uint32 ID,uint32 Type=VF_LOCAL);///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool iDestoryVectorByIndex(uint32 Index,uint32 Type=VF_LOCAL);
		bool iSetVectorMaxSize(SString strName,uint32 Size);
		bool iSetVectorMaxSize(uint32 ID,uint32 Size);
		bool iSetVectorMaxSizeByIndex(uint32 Index,uint32 Size);
		int iCreateIndex(SString strName);
		int iCreateIndex(uint32 ID);
		bool iReleaseIndex(SString strName);
		bool iReleaseIndex(uint32 ID);
		bool iReleaseIndexByIndex(uint32 Index);
		SHANDLE iFound(SString strName);
		SHANDLE iFound(uint32 ID);///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		SHANDLE iFoundByIndex(uint32 Index);
		int iGetVectorSize(SString strName);
		int iGetVectorSize(uint32 ID);
		int iGetVectorSizeByIndex(uint32 Index);
		int iGetVectorMaxSize(SString strName);
		int iGetVectorMaxSize(uint32 ID);
		int iGetVectorMaxSizeByIndex(uint32 Index);
		int iGetVectorIndex(SString strName);
		int iGetVectorIndex(uint32 ID);
		SString iGetVectorName(uint32 Index);
		uint32 iGetVectorID(uint32 Index);
		SHANDLE iFoundItem(SString strName,bool bGetValue=true,uint32 Type=VF_LOCAL);
		bool iInsertItem(SHANDLE Value,SString strName=TEXT(""));
		bool iAddItem(SHANDLE Value,SString strName=TEXT(""));
		bool iSetCurrentItem(SHANDLE Value,SString strName=TEXT(""));
		bool iDeleteItem();
		bool iMovToFirst();
		bool iMovToLast();
		bool iMovToPerivous();
		bool iMovToNext();
		SHANDLE iGetFirst() const;
		SHANDLE iGetLast() const;
		SHANDLE iGetPerivous() const;
		SHANDLE iGetNext() const;
		SHANDLE iGetFirstEx(bool bGetValue=true,bool bSetVector=true);
		SHANDLE iGetLastEx(bool bGetValue=true,bool bSetVector=true);
		SHANDLE iGetPerivousEx(bool bGetValue=true,bool bSetVector=true);
		SHANDLE iGetNextEx(bool bGetValue=true,bool bSetVector=true);
		bool iVMBOF() const;
		bool iVMEOF()const;
		SRESULT iCommandLine(SVALUE wParam,SVALUE lParam);
	};
	inline unsigned int gloRegisterVector(SString Name,uint32 Type=VT_USER,SHANDLE Param=0,int MaxSize=-1,void(*pRelease)(SHANDLE)=NULL,bool bCreateIndex=false)
			{ cVectorManager Manager; return Manager.iRegisterVector(Name,Type,Param,MaxSize,pRelease,bCreateIndex);};

	inline bool gloDestoryVector(SString Name,uint32 Type=VF_LOCAL)
			{cVectorManager Manager; return Manager.iDestoryVector(Name,Type);};

	inline bool gloDestoryVectorByIndex(uint32 Index,uint32 Type=VF_LOCAL)
			{cVectorManager Manager; return Manager.iDestoryVectorByIndex(Index,Type);};

	inline SHANDLE gloFoundVector(SString Name)
			{cVectorManager Manager; return Manager.iFound(Name);};

	inline SHANDLE gloFoundVectorByIndex(uint32 Index)
			{cVectorManager Manager; return Manager.iFoundByIndex(Index);};

#endif
}
