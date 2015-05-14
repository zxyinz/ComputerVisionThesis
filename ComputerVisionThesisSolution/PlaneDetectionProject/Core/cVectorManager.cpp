#include"cVectorManager.h"
using namespace std;
using namespace San;
lpVECTORNODE	cVectorManager::m_pRootVector=NULL;
VECTORNODE		cVectorManager::m_VIndex[];
unsigned int	cVectorManager::m_ManagerCount=0;
bool			cVectorManager::m_bOnInit=true;
void cVectorManager::_OnInitVectorManager()
{
	if(this->m_bOnInit!=true)
	{
		this->_DestoryVectorManager();
	}
	VECTORNODE	*pElement=NULL;
	VECTORNODE	*pElement_perv=NULL;
	SString strTittle=TEXT("abcdefghigklmnopqrstuvwxyz~");
	int VectorMaxSize=-1;
	this->m_pRootVector=new VECTORNODE;
	this->m_pRootVector->str_name=TEXT("Root");
	this->m_pRootVector->str_pname=TEXT("");
	this->m_pRootVector->id=0;
	this->m_pRootVector->pid=0;
	this->m_pRootVector->v_ptr=NULL;
	this->m_pRootVector->v_seek=(SHANDLE)(this->m_pRootVector);
	this->m_pRootVector->type=VT_ROOT|VT_SYS;
	this->m_pRootVector->pnext=NULL;
	this->m_pRootVector->ppervious=NULL;
	this->m_pRootVector->size=0;
	this->m_pRootVector->maxsize=VectorMaxSize;
	pElement_perv=this->m_pRootVector;
	for(int seek=(VINDEXSIZE-27-1);seek<VINDEXSIZE-1;seek=seek+1)
	{
		pElement=(VECTORNODE*)(this->m_pRootVector->v_seek);
		pElement->pnext=new VECTORNODE;
		pElement->pnext->ppervious=pElement_perv;
		pElement=(VECTORNODE*)pElement->pnext;
		pElement_perv=pElement;
		pElement->str_name=strTittle[seek-(VINDEXSIZE-27-1)];
		this->m_pRootVector->v_seek=(SHANDLE)(pElement);
		this->m_VIndex[seek].type=VT_INDEX|VT_VAR|VT_CONST;
		this->m_VIndex[seek].str_name=strTittle[seek-(VINDEXSIZE-27-1)];
		this->m_VIndex[seek].v_ptr=(SHANDLE)(pElement);
		this->m_VIndex[seek].v_seek=NULL;
		this->m_VIndex[seek].size=0;
		this->m_VIndex[seek].maxsize=VectorMaxSize;
		this->m_pRootVector->size=this->m_pRootVector->size+1;
	}
}
bool cVectorManager::_VectorFound(SString strName,VECTORNODE **pNode) const
{
	if(!strName.empty())
	{
		uint32 Index=this->_GetIndex(strName);
		if(this->m_VIndex[Index].v_seek!=NULL)
		{
			lpVECTORNODE pSeek=(lpVECTORNODE)(this->m_VIndex[Index].v_ptr);
			while(pSeek!=NULL)
			{
				if(pSeek->str_name==strName)
				{
					if(pNode!=NULL)
					{
						(*pNode)=pSeek;
					}
					return true;
				}
				pSeek=(lpVECTORNODE)pSeek->pnext;
			}
		}
	}
	if(pNode!=NULL)
	{
		(*pNode)=NULL;
	}
	return false;
}
uint32 cVectorManager::_GetIndex(SString strName) const
{
	uint32 Index=VINDEXSIZE-27-1;
	//SString strTittleString=TEXT("abcdefghigklmnopqrstuvwxyz");
	wchar_t strTittle=strName[0];
	/*int seek=0;
	for(seek=0;seek<26;seek=seek+1)
	{
		if(strTittle==strTittleString[seek])
		{
			break;
		}
		else
		{
			seek=seek+1;
		}
	}
	if(seek==26)
	{
		strTittleString=TEXT("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		for(seek=0;seek<26;seek=seek+1)
		{
			if(strTittle==strTittleString[seek])
			{
				break;
			}
			else
			{
				seek=seek+1;
			}
		}
	}
	Index=Index+seek;*/
	switch(strTittle)
	{
	case TEXT('a'):
	case TEXT('A'):
		break;
	case TEXT('b'):
	case TEXT('B'):
		Index=Index+1;
		break;
	case TEXT('c'):
	case TEXT('C'):
		Index=Index+2;
		break;
	case TEXT('d'):
	case TEXT('D'):
		Index=Index+3;
		break;
	case TEXT('e'):
	case TEXT('E'):
		Index=Index+4;
		break;
	case TEXT('f'):
	case TEXT('F'):
		Index=Index+5;
		break;
	case TEXT('g'):
	case TEXT('G'):
		Index=Index+6;
		break;
	case TEXT('h'):
	case TEXT('H'):
		Index=Index+7;
		break;
	case TEXT('i'):
	case TEXT('I'):
		Index=Index+8;
		break;
	case TEXT('j'):
	case TEXT('J'):
		Index=Index+9;
		break;
	case TEXT('k'):
	case TEXT('K'):
		Index=Index+10;
		break;
	case TEXT('l'):
	case TEXT('L'):
		Index=Index+11;
		break;
	case TEXT('m'):
	case TEXT('M'):
		Index=Index+12;
		break;
	case TEXT('n'):
	case TEXT('N'):
		Index=Index+13;
		break;
	case TEXT('o'):
	case TEXT('O'):
		Index=Index+14;
		break;
	case TEXT('p'):
	case TEXT('P'):
		Index=Index+15;
		break;
	case TEXT('q'):
	case TEXT('Q'):
		Index=Index+16;
		break;
	case TEXT('r'):
	case TEXT('R'):
		Index=Index+17;
		break;
	case TEXT('s'):
	case TEXT('S'):
		Index=Index+18;
		break;
	case TEXT('t'):
	case TEXT('T'):
		Index=Index+19;
		break;
	case TEXT('u'):
	case TEXT('U'):
		Index=Index+20;
		break;
	case TEXT('v'):
	case TEXT('V'):
		Index=Index+21;
		break;
	case TEXT('w'):
	case TEXT('W'):
		Index=Index+22;
		break;
	case TEXT('x'):
	case TEXT('X'):
		Index=Index+23;
		break;
	case TEXT('y'):
	case TEXT('Y'):
		Index=Index+24;
		break;
	case TEXT('z'):
	case TEXT('Z'):
		Index=Index+25;
		break;
	default:
		Index=Index+26;
		break;
	}
	return Index;
}
unsigned int cVectorManager::iRegisterVector(SString Name,uint32 Type,SHANDLE Param,int MaxSize,void(*pRelease)(SHANDLE),bool bCreateIndex)
{
	if(!this->_VectorFound(Name))
	{
		VECTORNODE *pElement=NULL;
		wchar_t strTittle;
		uint32 Index=this->_GetIndex(Name);
		if(this->m_VIndex[Index].size==0)
		{
			pElement=(lpVECTORNODE)(this->m_VIndex[Index].v_ptr);
		}
		else
		{
			pElement=new VECTORNODE;
			lpVECTORNODE pseek=(lpVECTORNODE)(this->m_VIndex[Index].v_seek);
			if(pseek->pnext!=NULL)
			{
				pseek->pnext->ppervious=pElement;
				pElement->pnext=pseek->pnext;
			}
			else
			{
				pElement->pnext=NULL;
			}
			pElement->ppervious=pseek;
			pseek->pnext=pElement;
		}
		if(pElement!=NULL)
		{
			pElement->str_name=Name;
			pElement->type=Type;
			pElement->size=0;
			pElement->maxsize=MaxSize;
			if((Type&VT_VAR)==VT_VAR)
			{
				pElement->size=1;
				pElement->maxsize=1;
				pElement->v_ptr=Param;
			}
			pElement->prelease=pRelease;
			this->m_VIndex[Index].v_seek=pElement;
			this->m_VIndex[Index].size=this->m_VIndex[Index].size+1;
		}
		else
		{
			SString strError=TEXT("error: wrong pointer");
			throw strError;
			return false;
		}
		if(bCreateIndex)
		{
			int seek=0;
			while((!this->m_VIndex[seek].str_name.empty())&&(seek<=(VINDEXSIZE-27-1)))
			{
				seek=seek+1;
			}
			if(seek==(VINDEXSIZE-27-1))
			{
			    SString strError=TEXT("error:can not establish index in vector manager");
				throw strError;
			}
			this->m_VIndex[seek].str_name=Name;
			this->m_VIndex[seek].v_ptr=(SHANDLE)pElement;
			this->m_VIndex[seek].type=VT_INDEX;
			return seek;
		}
		return true;
	}
	else
	{
		SString strError=TEXT("error: can not establish vector, the vector name is already exist");
		throw strError;
		return false;
	}
}
bool cVectorManager::iDestoryVector(SString strName,uint32 Type)
{
	lpVECTORNODE pElement=NULL;
	if(!this->_VectorFound(strName,&pElement))
	{
		return false;
	}
	if(pElement==this->m_pVector)
	{
		this->m_pVector=NULL;
		this->m_pMember=NULL;
	}
	if(this->iGetVectorIndex(strName)!=0)
	{
		this->iReleaseIndex(strName);
	}
	pElement->ppervious->pnext=pElement->pnext;
	pElement->pnext->ppervious=pElement->ppervious;
	pElement->ppervious=NULL;
	pElement->pnext=NULL;
	this->m_pRootVector->size=this->m_pRootVector->size-1;
	if(pElement->type==VT_VAR)
	{
		if(pElement->prelease!=NULL)
		{
			pElement->prelease((SHANDLE)(&pElement->v_ptr));
		}
		else
		{
			pElement->v_ptr=0;
		}
	}
	else
	{
		if((pElement->size!=0)&&(pElement->v_ptr!=NULL))
		{
			lpVMEMBER pMember=(lpVMEMBER)(pElement->v_ptr);
			lpVMEMBER pSeek=NULL;
			while(pMember!=NULL)
			{
				pMember->str_name.clear();
				if(pElement->prelease!=NULL)
				{
					pElement->prelease((SHANDLE)(&pMember->value));
				}
				else
				{
					pMember->value=0;
				}
				pSeek=pMember;
				pMember=pMember->pnext;
				pSeek->pprevious=NULL;
				pSeek->pnext=NULL;
				delete pSeek;
				pSeek=NULL;
				pElement->size=pElement->size-1;
			}
		}
		if(pElement->size!=0)
		{
			return false;
		}
	}
	pElement->str_name.clear();
	pElement->str_pname.clear();
	pElement->prelease=NULL;
	pElement->v_seek=NULL;
	delete pElement;
	pElement=NULL;
	return true;
}
bool cVectorManager::iDestoryVectorByIndex(uint32 Index,uint32 Type)
{
	if(Index>=VINDEXSIZE)
	{
		return false;
	}
	if(Index>=(VINDEXSIZE-27-1))
	{
		return false;
	}
	lpVECTORNODE pNode=NULL;
	pNode=(lpVECTORNODE)(this->m_VIndex[Index].v_ptr);
	this->iReleaseIndexByIndex(Index);
	if(pNode==this->m_pVector)
	{
		this->m_pVector=NULL;
		this->m_pMember=NULL;
	}
	pNode->ppervious->pnext=pNode->pnext;
	pNode->pnext->ppervious=pNode->ppervious;
	pNode->ppervious=NULL;
	pNode->pnext=NULL;
	this->m_pRootVector->size=this->m_pRootVector->size-1;
	if(pNode->type==VT_VAR)
	{
		if(pNode->prelease!=NULL)
		{
			pNode->prelease((SHANDLE)(&pNode->v_ptr));
		}
		else
		{
			pNode->v_ptr=0;
		}
	}
	else
	{
		if((pNode->size!=0)&&(pNode->v_ptr!=NULL))
		{
			lpVMEMBER pMember=(lpVMEMBER)(pNode->v_ptr);
			lpVMEMBER pSeek=NULL;
			while(pMember!=NULL)
			{
				pMember->str_name.clear();
				if(pNode->prelease!=NULL)
				{
					pNode->prelease((SHANDLE)(&pMember->value));
				}
				else
				{
					pMember->value=0;
				}
				pSeek=pMember;
				pMember=pMember->pnext;
				pSeek->pprevious=NULL;
				pSeek->pnext=NULL;
				delete pSeek;
				pSeek=NULL;
				pNode->size=pNode->size-1;
			}
		}
		if(pNode->size!=0)
		{
			return false;
		}
	}
	pNode->str_name.clear();
	pNode->str_pname.clear();
	pNode->prelease=NULL;
	pNode->v_seek=NULL;
	delete pNode;
	pNode=NULL;
	return true;
}
bool cVectorManager::iSetVectorMaxSize(SString strName,uint32 MaxSize)
{
	lpVECTORNODE pElement=NULL;
	if(!this->_VectorFound(strName,&pElement))
	{
		return false;
	}
	if((pElement->type&VT_VAR)==VT_VAR)
	{
		return false;
	}
	if(MaxSize<pElement->maxsize)
	{
		return false;
	}
	pElement->maxsize=MaxSize;
	return true;
}
bool cVectorManager::iSetVectorMaxSizeByIndex(uint32 Index,uint32 MaxSize)
{
	if(Index>=VINDEXSIZE)
	{
		return false;
	}
	lpVECTORNODE pNode=NULL;
	pNode=(lpVECTORNODE)(this->m_VIndex[Index].v_ptr);
	if((pNode->type&VT_VAR)==VT_VAR)
	{
		return false;
	}
	if(MaxSize<pNode->maxsize)
	{
		return false;
	}
	pNode->maxsize=MaxSize;
	return true;
}
int cVectorManager::iCreateIndex(SString strName)
{
	lpVECTORNODE pElement=NULL;
	if(!this->_VectorFound(strName,&pElement))
	{
		return -1;
	}
	if(this->iGetVectorIndex(strName)!=0)
	{
		return -1;
	}
	for(int seek=0;seek<(VINDEXSIZE-27-1);seek=seek+1)
	{
		if(this->m_VIndex[seek].str_name.empty())
		{
			this->m_VIndex[seek].str_name=pElement->str_name;
			this->m_VIndex[seek].type=pElement->type|VT_INDEX;
			this->m_VIndex[seek].v_ptr=(SHANDLE)pElement;
			this->m_VIndex[seek].maxsize=1;
			this->m_VIndex[seek].size=0;
			this->m_VIndex[seek].v_seek=NULL;
			return seek;
		}
	}
	return -1;
}
bool cVectorManager::iReleaseIndex(SString strName)
{
	for(int seek=0;seek<(VINDEXSIZE-27-1);seek=seek+1)
	{
		if(this->m_VIndex[seek].str_name==strName)
		{
			this->m_VIndex[seek].str_name.clear();
			this->m_VIndex[seek].type=0;
			this->m_VIndex[seek].v_ptr=NULL;
			this->m_VIndex[seek].size=0;
			this->m_VIndex[seek].maxsize=0;
			this->m_VIndex[seek].v_seek=NULL;
			return true;
		}
	}
	return false;
}
bool cVectorManager::iReleaseIndexByIndex(uint32 Index)
{
	if(Index>=VINDEXSIZE)
	{
		return false;
	}
	if(Index>=(VINDEXSIZE-27-1))
	{
		return false;
	}
	this->m_VIndex[Index].str_name.clear();
	this->m_VIndex[Index].type=0;
	this->m_VIndex[Index].v_ptr=NULL;
	this->m_VIndex[Index].size=0;
	this->m_VIndex[Index].maxsize=0;
	this->m_VIndex[Index].v_seek=NULL;
	return true;
}
SHANDLE cVectorManager::iFound(SString strName)
{
	if(!strName.empty())
	{
		lpVECTORNODE pElement=NULL;
		if(!this->_VectorFound(strName,&pElement))
		{
			this->m_pVector=NULL;
			this->m_pMember=NULL;
			return NULL;
		}
		this->m_pVector=pElement;
		this->iGetFirstEx();
		if((pElement->type)&VT_VAR==VT_VAR)
		{
			return pElement->v_ptr;
		}
		return NULL;
	}
	else
	{
		return NULL;
	}
}
SHANDLE cVectorManager::iFoundByIndex(uint32 Index)
{
	if(Index>=VINDEXSIZE)
	{
		return NULL;
	}
	if(this->m_VIndex[Index].str_name.empty())
	{
		this->m_pVector=NULL;
		this->m_pMember=NULL;
		return NULL;
	}
	else
	{
		this->m_pVector=(lpVECTORNODE)(this->m_VIndex[Index].v_ptr);
		this->iGetFirstEx();
		if((this->m_VIndex[Index].type)&VT_VAR==VT_VAR)
		{
			return this->m_pVector->v_ptr;
		}
		else
		{
			return NULL;
		}
	}
}
int cVectorManager::iGetVectorSize(SString strName)
{
	if(strName.empty())
	{
		return -1;
	}
	lpVECTORNODE pNode=NULL;
	if(!this->_VectorFound(strName,&pNode))
	{
		return -1;
	}
	else
	{
		return pNode->size;
	}
}
int cVectorManager::iGetVectorSizeByIndex(uint32 Index)
{
	if(Index>=VINDEXSIZE)
	{
		return -1;
	}
	if(Index>=(VINDEXSIZE-27-1))
	{
		return -1;
	}
	return ((lpVECTORNODE)(this->m_VIndex[Index].v_ptr))->size;
}
int cVectorManager::iGetVectorMaxSize(SString strName)
{
	if(strName.empty())
	{
		return -1;
	}
	lpVECTORNODE pNode=NULL;
	if(!this->_VectorFound(strName,&pNode))
	{
		return -1;
	}
	else
	{
		return pNode->maxsize;
	}
}
int cVectorManager::iGetVectorMaxSizeByIndex(uint32 Index)
{
	if(Index>=VINDEXSIZE)
	{
		return -1;
	}
	if(Index>=(VINDEXSIZE-27-1))
	{
		return -1;
	}
	return ((lpVECTORNODE)(this->m_VIndex[Index].v_ptr))->maxsize;
}
int cVectorManager::iGetVectorIndex(SString strName)
{
	if(strName.empty())
	{
		return -1;
	}
	for(int seek=0;seek<(VINDEXSIZE);seek=seek+1)
	{
		if(this->m_VIndex[seek].str_name==strName)
		{
			return seek;
		}
	}
	return -1;
}
SString cVectorManager::iGetVectorName(uint32 Index)
{
	SString strName;
	strName.clear();
	if(Index>=VINDEXSIZE)
	{
		return strName;
	}
	if(Index>=(VINDEXSIZE-27-1))
	{
		return strName;
	}
	return this->m_VIndex[Index].str_name;
}
SHANDLE cVectorManager::iFoundItem(SString strName,bool bGetValue,uint32 Type)
{
	if(!this->m_bFound)
	{
		SString strError=TEXT("error: the vector was not decleared");
		throw strError;
		this->m_pMember=NULL;
		return NULL;
	}
	if((this->m_pVector->type&VT_VAR)==VT_VAR)
	{
		SString strError=TEXT("error: can not find item which the vector type was VT_VAR");
		throw strError;
		this->m_pMember=NULL;
		return NULL;
	}
	if(this->m_pVector->v_seek==NULL)
	{
		SString strError=TEXT("error: the vector was empty");
		throw strError;
		this->m_pMember=NULL;
		return NULL;
	}
	lpVMEMBER pSeek=(lpVMEMBER)(this->m_pVector->v_ptr);
	while(pSeek!=(lpVMEMBER)(this->m_pVector->v_seek))
	{
		if(pSeek->str_name==strName)
		{
			this->m_pMember=pSeek;
			if(bGetValue)
			{
				return  pSeek->value;
			}
			else
			{
				return (SHANDLE)pSeek;
			}
		}
		pSeek=pSeek->pnext;
	}
	if(pSeek->str_name==strName)
	{
		this->m_pMember=pSeek;
		if(bGetValue)
		{
			return pSeek->value;
		}
		else
		{
			return (SHANDLE)pSeek;
		}
	}
	SString strError=_SSTR("error: vector member not found in vector")+this->m_pVector->str_name;
	throw strError;
	this->m_pMember=NULL;
	return NULL;
}
bool cVectorManager::iInsertItem(SHANDLE Value,SString strName)
{
	if(this->m_pVector==NULL)
	{
		SString strError=TEXT("error: the vector was not decleared");
		throw strError;
		return false;
	}
	if((this->m_pVector->type&VT_VAR)==VT_VAR)
	{
		SString strError=TEXT("error: can not insert item whitch the vector type was VT_VAR");
		throw strError;
		return false;
	}
	if((this->m_pMember==NULL)&&(this->m_pVector->v_seek!=NULL))
	{
		SString strError=TEXT("error: the vector member was not decleared");
		throw strError;
		return false;
	}
	if(this->m_pVector->size==this->m_pVector->maxsize)
	{
		SString strError=TEXT("error: this vector were reath the max vector size");
		throw strError;
		return false;
	}
	if((this->m_pMember==NULL)&&(this->m_pVector->v_seek==NULL))
	{
		return this->iAddItem(Value,strName);
	}
	if(this->m_pMember==(lpVMEMBER)(this->m_pVector->v_ptr))
	{
		lpVMEMBER pElement=new VMEMBER;
		pElement->value=Value;
		pElement->str_name=strName;
		pElement->pprevious=NULL;
		pElement->pnext=this->m_pMember;
		this->m_pMember->pprevious=pElement;
		this->m_pVector->v_ptr=(SHANDLE)pElement;
	}
	else
	{
		lpVMEMBER pSeek=this->m_pMember->pprevious;
		lpVMEMBER pElement=new VMEMBER;
		pElement->value=Value;
		pElement->str_name=strName;
		pElement->pnext=this->m_pMember;
		this->m_pMember->pprevious=pElement;
		pSeek->pnext=pElement;
		pElement->pprevious=pSeek;
	}
	this->m_pVector->size=this->m_pVector->size+1;
	return true;
}
bool cVectorManager::iAddItem(SHANDLE Value,SString strName)
{
	if(this->m_pVector==NULL)
	{
		SString strError=TEXT("error: the vector was not decleard");
		throw strError;
		return false;
	}
	if((this->m_pVector->type&VT_VAR)==VT_VAR)
	{
		SString strError=TEXT("error: can not add item which the vector type was VT_VAR");
		throw strError;
		return false;
	}
	if(this->m_pVector->size==this->m_pVector->maxsize)
	{
		SString strError=TEXT("error: this vector were reach the max vector size");
		throw strError;
		return false;
	}
	if(this->m_pVector->v_seek==NULL)
	{
		lpVMEMBER pElement=new VMEMBER;
		pElement->value=Value;
		pElement->str_name=strName;
		pElement->pprevious=NULL;
		pElement->pnext=NULL;
		this->m_pVector->v_ptr=(SHANDLE)pElement;
		this->m_pVector->v_seek=(SHANDLE)pElement;
	}
	else
	{
		lpVMEMBER pElement=(VMEMBER*)(this->m_pVector->v_seek);
		pElement->pnext=new VMEMBER;
		pElement->pnext->value=Value;
		pElement->pnext->str_name=strName;
		pElement->pnext->pprevious=pElement;
		pElement->pnext->pnext=NULL;
		this->m_pVector->v_seek=(SHANDLE)(pElement->pnext);
	}
	this->m_pVector->size=this->m_pVector->size+1;
	return true;
}
bool cVectorManager::iSetCurrentItem(SHANDLE Value,SString strName)
{
	if(this->m_pVector==NULL)
	{
		SString strError=TEXT("error: the vector was not decleared");
		throw strError;
		return false;
	}
	if((this->m_pVector->type&VT_VAR)==VT_VAR)
	{
		SString strError=TEXT("error: can not insert item whitch the vector type was VT_VAR");
		throw strError;
		return false;
	}
	if((this->m_pMember==NULL)&&(this->m_pVector->v_seek!=NULL))
	{
		SString strError=TEXT("error: the vector member was not decleared");
		throw strError;
		return false;
	}
	this->m_pMember->value=Value;
	if(!strName.empty())
	{
		this->m_pMember->str_name=strName;
	}
	return true;
}
bool cVectorManager::iDeleteItem()
{
	if(this->m_pMember==NULL)
	{
		SString strError=TEXT("error: the vector member was not decleared");
		throw strError;
		return false;
	}
	if(this->m_pVector->v_seek==NULL)
	{
		SString strError=TEXT("error: the vactor was empty");
		throw strError;
		return true;
	}
	if(this->m_pMember->value!=NULL)
	{
		if(this->m_pVector->prelease!=NULL)
		{
			this->m_pVector->prelease((SHANDLE(&(this->m_pMember->value))));
		}
		else
		{
			this->m_pMember->value=0;
		}
	}
	if(this->m_pMember==(lpVMEMBER)(this->m_pVector->v_seek))
	{
		if(this->m_pVector->v_ptr==this->m_pVector->v_seek)
		{
			this->m_pMember->pprevious=NULL;
			this->m_pMember->pnext=NULL;
			this->m_pMember->str_name.clear();
			delete this->m_pMember;
			this->m_pVector->v_seek=NULL;
		}
		else
		{
			lpVMEMBER pSeek=this->m_pMember->pprevious;
			pSeek->pnext=NULL;
			this->m_pMember->pprevious=NULL;
			this->m_pMember->pnext=NULL;
			this->m_pMember->str_name.clear();
			delete this->m_pMember;
			this->m_pVector->v_seek=(SHANDLE)pSeek;
			this->m_pMember=pSeek;
		}
	}
	else
	{
		if(this->m_pMember==(lpVMEMBER)(this->m_pVector->v_ptr))
		{
			this->m_pVector->v_ptr=(SHANDLE)(this->m_pMember->pnext);
			this->m_pMember->pnext->pprevious=NULL;
			this->m_pMember->pnext=NULL;
			this->m_pMember->str_name.clear();
			delete this->m_pMember;
			this->m_pMember=(lpVMEMBER)(this->m_pVector->v_ptr);
		}
		else
		{
			lpVMEMBER pSeek=this->m_pMember->pprevious;
			this->m_pMember->pprevious->pnext=this->m_pMember->pnext;
			this->m_pMember->pnext->pprevious=this->m_pMember->pprevious;
			this->m_pMember->pprevious=NULL;
			this->m_pMember->pnext=NULL;
			this->m_pMember->str_name.clear();
			delete this->m_pMember;
			this->m_pMember=pSeek->pnext;
		}
	}
	this->m_pVector->size=this->m_pVector->size-1;
	return true;
}
bool cVectorManager::iMovToFirst()
{
	if(this->m_pVector==NULL)
	{
		SString strError=TEXT("error: the vector was not decleared");
		throw strError;
		return false;
	}
	if(this->m_pVector->size==0)
	{
		//SString strError=TEXT("error: the vector was empty");
		//throw strError;
		return false;
	}
	this->m_pMember=(lpVMEMBER)(this->m_pVector->v_ptr);
	return true;
}
bool cVectorManager::iMovToLast()
{
	if(this->m_pVector==NULL)
	{
		SString strError=TEXT("error: the vector was not decleared");
		throw strError;
		return false;
	}
	if(this->m_pVector->size==0)
	{
		//SString strError=TEXT("error: the vector was empty");
		//throw strError;
		return false;
	}
	this->m_pMember=(lpVMEMBER)(this->m_pVector->v_seek);
	return true;
}
bool cVectorManager::iMovToPerivous()
{
	if(this->m_pVector==NULL)
	{
		SString strError=TEXT("error: the vector was not decleared");
		throw strError;
		return false;
	}
	if(this->m_pMember==(lpVMEMBER)(this->m_pVector->v_ptr))
	{
		SString strError=TEXT("error: the member is in the end of the vector");
		throw strError;
		return false;
	}
	this->m_pMember=this->m_pMember->pprevious;
	return true;
}
bool cVectorManager::iMovToNext()
{
	if(this->m_pVector==NULL)
	{
		SString strError=TEXT("error: the vector was not decleared");
		throw strError;
		return false;
	}
	if(this->m_pMember==(lpVMEMBER)(this->m_pVector->v_seek))
	{
		SString strError=TEXT("error: the member is in the end of the vector");
		throw strError;
		return false;
	}
	this->m_pMember=this->m_pMember->pnext;
	return true;
}
SHANDLE cVectorManager::iGetFirst() const
{
	if(this->m_pVector==NULL)
	{
		SString strError=TEXT("error: the vector was not decleared");
		throw strError;
		return NULL;
	}
	if(this->m_pVector->size==0)
	{
		//SString strError=TEXT("error: the vector was empty");
		//throw strError;
		return NULL;
	}
	return ((lpVMEMBER)(this->m_pVector->v_ptr))->value;
}
SHANDLE cVectorManager::iGetLast() const
{
	if(this->m_pVector==NULL)
	{
		SString strError=TEXT("error: the vector was not decleared");
		throw strError;
		return NULL;
	}
	if(this->m_pVector->size==0)
	{
		//SString strError=TEXT("error: the vector was empty");
		//throw strError;
		return NULL;
	}
	return ((lpVMEMBER)(this->m_pVector->v_seek))->value;
}
SHANDLE cVectorManager::iGetPerivous() const
{
	if(this->m_pVector==NULL)
	{
		SString strError=TEXT("error: the vector was not decleared");
		throw strError;
		return NULL;
	}
	if(this->m_pMember==(lpVMEMBER)(this->m_pVector->v_ptr))
	{
		SString strError=TEXT("error: the member is in the end of the vector");
		throw strError;
		return NULL;
	}
	return this->m_pMember->pprevious->value;
}
SHANDLE cVectorManager::iGetNext() const
{
	if(this->m_pVector==NULL)
	{
		SString strError=TEXT("error: the vector was not decleared");
		throw strError;
		return NULL;
	}
	if(this->m_pMember==(lpVMEMBER)(this->m_pVector->v_seek))
	{
		SString strError=TEXT("error: the member is in the end of the vector");
		throw strError;
		return NULL;
	}
	return this->m_pMember->pnext->value;
}
SHANDLE cVectorManager::iGetFirstEx(bool bGetValue,bool bSetVector)
{
	if(this->m_pVector==NULL)
	{
		SString strError=TEXT("error: the vector was not decleared");
		throw strError;
		return NULL;
	}
	if(this->m_pVector->v_seek==NULL)
	{
		//SString strError=TEXT("error: the vector was empty");
		//throw strError;
		if(bSetVector)
		{
			this->m_pMember=NULL;
		}
		return NULL;
	}
	if(bSetVector)
	{
		this->m_pMember=(lpVMEMBER)(this->m_pVector->v_ptr);
	}
	if(bGetValue)
	{
		return ((lpVMEMBER)(this->m_pVector->v_ptr))->value;
	}
	else
	{
		return (SHANDLE)&(((lpVMEMBER)(this->m_pVector->v_ptr))->value);
	}
}
SHANDLE cVectorManager::iGetLastEx(bool bGetValue,bool bSetVector)
{
	if(this->m_pVector==NULL)
	{
		SString strError=TEXT("error: the vector was not decleared");
		throw strError;
		return NULL;
	}
	if(this->m_pVector->v_seek==NULL)
	{
		//SString strError=TEXT("error: the vector was empty");
		//throw strError;
		if(bSetVector)
		{
			this->m_pMember=NULL;
		}
		return NULL;
	}
	if(bSetVector)
	{
		this->m_pMember=(lpVMEMBER)(this->m_pVector->v_seek);
	}
	if(bGetValue)
	{
		return ((lpVMEMBER)(this->m_pVector->v_seek))->value;
	}
	else
	{
		return (SHANDLE)&((lpVMEMBER)(this->m_pVector->v_ptr))->value;
	}
}
SHANDLE cVectorManager::iGetPerivousEx(bool bGetValue,bool bSetVector)
{
	if(this->m_pVector==NULL)
	{
		SString strError=TEXT("error: the vector was not decleared");
		throw strError;
		return NULL;
	}
	if(this->m_pMember==(lpVMEMBER)(this->m_pVector->v_ptr))
	{
		SString strError=TEXT("error: the member is in the end of the vector");
		throw strError;
		return NULL;
	}
	if(bSetVector)
	{
		this->m_pMember=this->m_pMember->pprevious;
		if(bGetValue)
		{
			return this->m_pMember->value;
		}
		else
		{
			return (SHANDLE)&(this->m_pMember->value);
		}
	}
	else
	{
		if(bGetValue)
		{
			return this->m_pMember->pprevious->value;
		}
		else
		{
			return (SHANDLE)&(this->m_pMember->pprevious->value);
		}
	}
}
SHANDLE cVectorManager::iGetNextEx(bool bGetValue,bool bSetVector)
{
	if(this->m_pVector==NULL)
	{
		SString strError=TEXT("error: the vector was not decleared");
		throw strError;
		return NULL;
	}
	if(this->m_pMember==(lpVMEMBER)(this->m_pVector->v_seek))
	{
		SString strError=TEXT("error: the member is in the end of the vector");
		throw strError;
		return NULL;
	}
	if(bSetVector)
	{
		this->m_pMember=this->m_pMember->pnext;
		if(bGetValue)
		{
			return this->m_pMember->value;
		}
		else
		{
			return (SHANDLE)&(this->m_pMember->value);
		}
	}
	else
	{
		if(bGetValue)
		{
			return this->m_pMember->pnext->value;
		}
		else
		{
			return (SHANDLE)&(this->m_pMember->pnext->value);
		}
	}
}
bool cVectorManager::iVMBOF() const
{
	if(this->m_pVector==NULL)
	{
		SString strError=TEXT("error: the vector was not decleared");
		throw strError;
		return false;
	}
	if(this->m_pMember==NULL)
	{
		SString strError=TEXT("error: the vector member was not decleared");
		throw strError;
		return false;
	}
	if(this->m_pVector->v_seek==NULL)
	{
		//SString strError=TEXT("error: the vector was empty");
		//throw strError;
		return true;
	}
	if(this->m_pMember==(lpVMEMBER)(this->m_pVector->v_ptr))
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool cVectorManager::iVMEOF() const
{
	if(this->m_pVector==NULL)
	{
		SString strError=TEXT("error: the vector was not decleared");
		throw strError;
		return false;
	}
	if(this->m_pMember==NULL)
	{
		SString strError=TEXT("error: the vector manager was not decleared");
		throw strError;
		return false;
	}
	if(this->m_pVector->v_seek==NULL)
	{
		//SString strError=TEXT("error: the vcctor was empty");
		//throw strError;
		return true;
	}
	if(this->m_pMember==(lpVMEMBER)(this->m_pVector->v_seek))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*unsigned int San::gloRegisterVector(SString strName,uint32 Type,SHANDLE Param,int MaxSize,void(*pRelease)(SHANDLE),bool bCreateIndex)
{
	cVectorManager Manager;//=new cVectorManager();
	//if(pManager!=NULL)
	//{
		return Manager.iRegisterVector(strName,Type,Param,MaxSize,pRelease,bCreateIndex);
	//}
	//return 0;
}
bool San::gloDestoryVector(SString strName,uint32 Type)
{
	cVectorManager Manager;//=new cVectorManager();
	//if(pManager!=NULL)
	//{
		return Manager.iDestoryVector(strName,Type);
	//}
	//return false;
}
bool San::gloDestoryVectorByIndex(uint32 Index,uint32 Type)
{
	cVectorManager Manager;//=new cVectorManager();
	//if(pManager!=NULL)
	//{
		return Manager.iDestoryVectorByIndex(Index,Type);
	//}
	//return false;
}
SHANDLE San::gloFoundVector(SString strName)
{
	cVectorManager Manager;//=new cVectorManager();
	//if(pManager!=NULL)
	//{
		return Manager.iFound(strName);
	//}
	//return NULL;
}
SHANDLE San::gloFoundVectorByIndex(uint32 Index)
{
	cVectorManager Manager;//=new cVectorManager();
	//if(pManager!=NULL)
	//{
		return Manager.iFoundByIndex(Index);
	//}
	//return NULL;
}*/