#include"iostream"
#include"cOBJFileLoader.h"
using namespace std;
using namespace San;
void cOBJFileLoader::_LoadMaterial(SStringW strFilePath)
{
	fstream *pFile=new fstream();
	pFile->open(strFilePath.c_str(),ios::binary|ios::in,0x40);
	if(pFile==nullptr)
	{
		return;
	}
	uint32 FileSeek=0;
	OBJMATERIALLIST::iterator pMaterial;
	while(!(pFile->eof()))
	{
		char *pString=new char[512];
		if(!this->_ReadLine(pFile,pString,512))
		{
			delete[] pString;
			pString=nullptr;
			break;
		}
		//::cout<<pString<<"\n";
		string strString=pString;
		delete[] pString;
		if((int)(strString.find("newmtl"))>=0)
		{
			strString=strString.substr(7,strString.length()-8);
			OBJMATERIALELEMENT Material;
			Material.strName=strString;
			Material.Material.Ambient = SANCOLORF(0.0f, 0.0f, 0.0f, 1.0f);
			Material.Material.Diffuse = SANCOLORF(0.0f, 0.0f, 0.0f, 1.0f);
			Material.Material.Specular = SANCOLORF(0.0f, 0.0f, 0.0f, 1.0f);
			Material.Material.Emissive = SANCOLORF(0.0f, 0.0f, 0.0f, 1.0f);
			Material.Material.Shininess = 0.0f;
			Material.Material.EffectHandle = nullptr;
			Material.Material.TexList.clear();///////////////////////////////////////////////////////
			this->m_MaterialList.insert(this->m_MaterialList.end(),Material);
			pMaterial=this->m_MaterialList.end();
			pMaterial--;
		}
		if((int)strString.find("\tKa")==0)
		{
			strString=strString.substr(5,strString.length()-5);
			pMaterial->Material.Ambient.r=::atof(strString.c_str());
			strString=strString.substr(strString.find(' ')+1,strString.length()-strString.find(' ')-1);
			pMaterial->Material.Ambient.g=::atof(strString.c_str());
			strString=strString.substr(strString.find(' ')+1,strString.length()-strString.find(' ')-1);
			pMaterial->Material.Ambient.b=::atof(strString.c_str());
			pMaterial->Material.Ambient.a=1.0;
		}
		if((int)strString.find("\tKd")==0)
		{
			strString=strString.substr(5,strString.length()-5);
			pMaterial->Material.Diffuse.r=::atof(strString.c_str());
			strString=strString.substr(strString.find(' ')+1,strString.length()-strString.find(' ')-1);
			pMaterial->Material.Diffuse.g=::atof(strString.c_str());
			strString=strString.substr(strString.find(' ')+1,strString.length()-strString.find(' ')-1);
			pMaterial->Material.Diffuse.b=::atof(strString.c_str());
			pMaterial->Material.Diffuse.a=1.0;
		}
		if((int)strString.find("\tKs")==0)
		{
			strString=strString.substr(5,strString.length()-5);
			pMaterial->Material.Specular.r=::atof(strString.c_str());
			strString=strString.substr(strString.find(' ')+1,strString.length()-strString.find(' ')-1);
			pMaterial->Material.Specular.g=::atof(strString.c_str());
			strString=strString.substr(strString.find(' ')+1,strString.length()-strString.find(' ')-1);
			pMaterial->Material.Specular.b=::atof(strString.c_str());
			pMaterial->Material.Specular.a=1.0;
		}
		if((int)strString.find("\tKe")==0)
		{
			strString=strString.substr(5,strString.length()-5);
			pMaterial->Material.Emissive.r=::atof(strString.c_str());
			strString=strString.substr(strString.find(' ')+1,strString.length()-strString.find(' ')-1);
			pMaterial->Material.Emissive.g=::atof(strString.c_str());
			strString=strString.substr(strString.find(' ')+1,strString.length()-strString.find(' ')-1);
			pMaterial->Material.Emissive.b=::atof(strString.c_str());
			pMaterial->Material.Emissive.a=1.0;
		}
		if((int)(strString.find("map_Kd "))>=0)
		{
			strString=strString.substr(8,strString.length()-9);
			stSANUNKNOWNEX<SANTEXRENDERDESC, SHANDLE> Tex;
			Tex.description.strFilePath = ::gloAToT(strString);
			Tex.description.RenderType = STRT_DIFFUSE;
			Tex.value = nullptr;
			pMaterial->Material.TexList.insert(pMaterial->Material.TexList.end(), Tex);
		}
		if((int)(strString.find("map_Kd2"))>=0)
		{
			strString=strString.substr(8,strString.length()-9);
			stSANUNKNOWNEX<SANTEXRENDERDESC, SHANDLE> Tex;
			Tex.description.strFilePath = ::gloAToT(strString);
			Tex.description.RenderType = STRT_EMISSIVE;
			Tex.value = nullptr;
			pMaterial->Material.TexList.insert(pMaterial->Material.TexList.end(), Tex);
		}
		if((int)(strString.find("map_bump "))>=0)
		{
			strString=strString.substr(10,strString.length()-11);
			stSANUNKNOWNEX<SANTEXRENDERDESC, SHANDLE> Tex;
			Tex.description.strFilePath = ::gloAToT(strString);
			Tex.description.RenderType = STRT_NORMAL;
			Tex.value = nullptr;
			pMaterial->Material.TexList.insert(pMaterial->Material.TexList.end(), Tex);
		}
	}
	pFile->close();
	delete pFile;
	pFile=nullptr;
}
bool cOBJFileLoader::_ReadLine(fstream *pfile,char* &pstring,uint32 Size)
{
	if(pfile==nullptr)
	{
		return false;
	}
	if(Size<=0)
	{
		return false;
	}
	if(pfile->eof())
	{
		return false;
	}
	uint32 file_seek=pfile->tellg();
	pfile->read(pstring,Size);
	if(pfile->gcount()==0)
	{
		return false;
	}
	for(unsigned int seek=0;seek<Size;seek=seek+1)
	{
		if(pstring[seek]=='\n')
		{
			pstring[seek]='\0';
			pfile->clear();
			pfile->seekg(file_seek+seek+1,ios::beg);
			return true;
		}
	}
	pstring[Size]='\0';
	return true;
}
void cOBJFileLoader::_ClearList()
{
	SANOBJECTLIST::iterator pObject=this->m_ObjList.begin();
	while(pObject!=this->m_ObjList.end())
	{
		list<stSANUNKNOWNEX<SANTEXRENDERDESC, SHANDLE>>::iterator pTexDesc = pObject->Material.TexList.begin();
		while (pTexDesc != pObject->Material.TexList.end())
		{
			pTexDesc->description.strFilePath.clear();
			pTexDesc++;
		}
		pObject++;
	}
	this->m_ObjList.clear();
}
bool cOBJFileLoader::iLoadFile(SStringW strFilePath)
{
	if(strFilePath.empty())
	{
		return false;
	}
	if(!this->m_strFilePath.empty())
	{
		this->_ClearList();
	}
	this->m_strFilePath=strFilePath;
	if(this->m_pFile!=nullptr)
	{
		this->m_pFile->close();
		delete (this->m_pFile);
		this->m_pFile=nullptr;
	}
	this->m_pFile=new fstream();
	this->m_pFile->open(strFilePath.c_str(),ios::binary|ios::in,0x40);
	if(this->m_pFile==nullptr)
	{
		return false;
	}
	size_t VertexSize=0;
	size_t TextureSize=0;
	size_t NormalSize=0;
	size_t FaceSize=0;
	char* pstrLine=new char[512];
	char* pstrBuffer=new char[512];
	size_t LineLength=0;
	::gloMemSet(pstrLine,0,512);
	::gloMemSet(pstrBuffer,0,512);
	while(!this->m_pFile->eof())
	{
		if(!this->_ReadLine(this->m_pFile,pstrLine,512))
		{
			break;
		}
		LineLength=::strlen(pstrLine);
		switch(pstrLine[0])
		{
		case 'v':
			if(LineLength<3)
			{
				break;
			}
			if(pstrLine[1]==' ')
			{
				VertexSize=VertexSize+1;
			}
			if((pstrLine[1]=='t')&&(pstrLine[2]==' '))
			{
				TextureSize=TextureSize+1;
			}
			if((pstrLine[1]=='n')&&(pstrLine[2]==' '))
			{
				NormalSize=NormalSize+1;
			}
			break;
		case 'm':
			if(LineLength<7)
			{
				break;
			}
			::gloMemCopy(pstrBuffer,pstrLine,7);
			pstrBuffer[7]='\0';
			if(::strcmp(pstrBuffer,"mtllib ")==0)
			{
				size_t StringSize=this->m_strFilePath.length();
				::gloMemSet(pstrBuffer,0,512);
				::gloMemCopy(pstrBuffer,(void*)(this->m_strFilePath.c_str()),StringSize*sizeof(wchar_t));
				((wchar_t*)(pstrBuffer))[StringSize-3]=L'm';
				((wchar_t*)(pstrBuffer))[StringSize-2]=L't';
				((wchar_t*)(pstrBuffer))[StringSize-1]=L'l';
				((wchar_t*)(pstrBuffer))[StringSize]=L'\0';
				this->_LoadMaterial((wchar_t*)(pstrBuffer));
			}
			break;
		case 'g':
			if(LineLength<2)
			{
				break;
			}
			if(pstrLine[1]==' ')
			{
				if(!this->m_ObjList.empty())
				{
					SANOBJECTLIST::iterator pObj=this->m_ObjList.end();
					pObj--;
					pObj->VertexListSize=FaceSize*3;
					pObj->pVertexList=new SANVERTEX[pObj->VertexListSize];
					_SDEBUG(if(pObj->pVertexList==nullptr){::cout<<"error: can not allocation memory	Class: cObjFileLoader"<<"\n";})
					::gloMemSet(&(pObj->pVertexList),0,sizeof(pObj->pVertexList));
				}
				SANOBJECT Object;
				Object.strObjName.clear();
				Object.pVertexList=nullptr;
				Object.VertexListSize=0;
				Object.ObjParam=0;
				::gloMemSet(&(Object.Material.Ambient),0,sizeof(Object.Material.Ambient));
				::gloMemSet(&(Object.Material.Diffuse),0,sizeof(Object.Material.Diffuse));
				::gloMemSet(&(Object.Material.Emissive),0,sizeof(Object.Material.Emissive));
				::gloMemSet(&(Object.Material.Specular),0,sizeof(Object.Material.Specular));
				//::gloMemSet(&(Object.CollisionObj),0,sizeof(Object.CollisionObj));
				this->m_ObjList.insert(this->m_ObjList.end(),Object);
				FaceSize=0;
			}
			break;
		case 'f':
			if(LineLength<2)
			{
				break;
			}
			if(pstrLine[1]==' ')
			{
				FaceSize=FaceSize+1;
			}
			break;
		default:
			break;
		}
	}
	if(!this->m_ObjList.empty())
	{
		SANOBJECTLIST::iterator pObj=this->m_ObjList.end();
		pObj--;
		pObj->VertexListSize=FaceSize*3;
		pObj->pVertexList=new SANVERTEX[pObj->VertexListSize];
		_SDEBUG(if(pObj->pVertexList==nullptr){::cout<<"error: can not allocation memory	Class: cObjFileLoader"<<"\n";})
		::gloMemSet((pObj->pVertexList),0,sizeof(pObj->pVertexList));
	}
	this->m_pFile->clear();
	this->m_pFile->seekg(0,ios::beg);
	lpSPOINT3 pVertexList=new SPOINT3[VertexSize];
	lpSPOINT3 pTextureList=new SPOINT3[TextureSize];
	lpSPOINT3 pNormalList=new SPOINT3[NormalSize];
	lpSPOINT3 pListBuffer=nullptr;
	uint32 VertexSeek=0;
	uint32 TextureSeek=0;
	uint32 NormalSeek=0;
	uint32 VertexListSeek=0;
	uint32 ListSeek=0;
	SANOBJECTLIST::iterator pObject;
	pObject=this->m_ObjList.end();
	bool bBeginOfList=true;
	while(!this->m_pFile->eof())
	{
		if(!this->_ReadLine(this->m_pFile,pstrLine,512))
		{
			break;
		}
		LineLength=::strlen(pstrLine);
		switch(pstrLine[0])
		{
		case 'v':
			if(LineLength<3)
			{
				break;
			}
			if(pstrLine[1]==' ')
			{
				pListBuffer=pVertexList;
				ListSeek=VertexSeek;
				VertexSeek=VertexSeek+1;
				::gloMemCopy(pstrBuffer,pstrLine+3,LineLength-3);
				pstrBuffer[LineLength-3]='\0';
			}
			if((pstrLine[1]=='t')&&(pstrLine[2]==' '))
			{
				pListBuffer=pTextureList;
				ListSeek=TextureSeek;
				TextureSeek=TextureSeek+1;
				::gloMemCopy(pstrBuffer,pstrLine+3,LineLength-3);
				pstrBuffer[LineLength-3]='\0';
			}
			if((pstrLine[1]=='n')&&(pstrLine[2]==' '))
			{
				pListBuffer=pNormalList;
				ListSeek=NormalSeek;
				NormalSeek=NormalSeek+1;
				::gloMemCopy(pstrBuffer,pstrLine+3,LineLength-3);
				pstrBuffer[LineLength-3]='\0';
			}
			if(pListBuffer!=nullptr)
			{
				size_t seek_begin=0;
				size_t seek_end=0;
				::sscanf(pstrBuffer,"%f %f %f",&pListBuffer[ListSeek].x,&pListBuffer[ListSeek].y,&pListBuffer[ListSeek].z);
				pListBuffer=nullptr;
			}
			break;
		case 'g':
			if(LineLength<2)
			{
				break;
			}
			if(pstrLine[1]==' ')
			{
				_SDEBUG(::cout<<"Load Object Name:"<<pstrLine+2<<"\n";)
				if(bBeginOfList)
				{
					pObject=this->m_ObjList.begin();
					bBeginOfList=false;
				}
				else
				{
					/*if(pObject->CollisionObj.pLV1!=nullptr)
					{
						SPOINT3 Origin=(pObject->CollisionObj.pLV1->paramlist[2]+pObject->CollisionObj.pLV1->paramlist[3])*0.5;
						pObject->CollisionObj.LV0.paramlist[0]=Origin;
						pObject->CollisionObj.pLV1->paramlist[0]=Origin;
						pObject->CollisionObj.pLV1->paramlist[1]=Origin-pObject->CollisionObj.pLV1->paramlist[2];
						SPOINT3 Radius=pObject->CollisionObj.pLV1->paramlist[1];
						Radius=Radius*Radius;
						pObject->CollisionObj.LV0.paramlist[1].x=::sqrt(Radius.x+Radius.y+Radius.y);
					}//*/
					pObject++;
				}
				_SDEBUG(if(pObject==this->m_ObjList.end()){::cout<<"error: can not have enough iterator int object list	Class: cObjFileLoader	Function: iLoadFile"<<"\n";})
				pObject->strObjName =::gloAToT(pstrLine + 2);
				pObject->pVertexList=new SANVERTEX[pObject->VertexListSize];
				::gloMemSet((pObject->pVertexList),0,sizeof(SANVERTEX)*pObject->VertexListSize);
				VertexListSeek=0;
			}
			break;
		case 'u':
			if(LineLength<7)
			{
				break;
			}
			::gloMemCopy(pstrBuffer,pstrLine,7);
			pstrBuffer[7]='\0';
			if(::strcmp(pstrBuffer,"usemtl ")==0)
			{
				::gloMemCopy(pstrBuffer,pstrLine+7,LineLength-7);
				pstrBuffer[LineLength-8]='\0';/////////////////////////////////////////////////////////////////
				OBJMATERIALLIST::iterator pMaterial=this->m_MaterialList.begin();
				while(pMaterial!=this->m_MaterialList.end())
				{
					if(::strcmp(pMaterial->strName.c_str(),pstrBuffer)==0)
					{
						break;
					}
					pMaterial++;
				}
				if(pMaterial!=this->m_MaterialList.end())
				{
					pObject->Material=pMaterial->Material;
				}
				else
				{
					SANMATERIAL Material;
					Material.Ambient=SANCOLORF(0.0,0.0,0.0,1.0);
					Material.Diffuse=SANCOLORF(0.0,0.0,0.0,1.0);
					Material.Emissive=SANCOLORF(0.0,0.0,0.0,1.0);
					Material.Specular=SANCOLORF(0.0,0.0,0.0,1.0);
					Material.EffectHandle=nullptr;
					Material.TexList.clear();
					pObject->Material=Material;
				}
			}
			break;
		case 'f':
			if(LineLength<2)
			{
				break;
			}
			if(pstrLine[1]==' ')
			{
				size_t seek_begin=2;
				size_t seek_end=2;
				size_t seek_v=0;
				size_t seek_vt=0;
				size_t seek_vn=0;
				for(int seek=0;seek<3;seek=seek+1)
				{
					pObject->pVertexList[VertexListSeek].vc=SANCOLORF(0.5,0.5,0.5,1.0);
					pObject->pVertexList[VertexListSeek].vt_size=1;
					pObject->pVertexList[VertexListSeek].p_vt=new SPOINT3[pObject->pVertexList[VertexListSeek].vt_size];
					while(pstrLine[seek_end]!=' ')
					{
						seek_end=seek_end+1;
					}
					::gloMemCopy(pstrBuffer,pstrLine+seek_begin,seek_end-seek_begin);
					pstrBuffer[seek_end-seek_begin]='\0';
					seek_end=seek_end+1;
					seek_begin=seek_end;
					::sscanf(pstrBuffer,"%d/%d/%d",&seek_v,&seek_vt,&seek_vn);
					pObject->pVertexList[VertexListSeek].v=pVertexList[seek_v-1];
					pObject->pVertexList[VertexListSeek].p_vt[0]=pTextureList[seek_vt-1];
					pObject->pVertexList[VertexListSeek].vn=pNormalList[seek_vn-1];
					/*if(pObject->CollisionObj.pLV1==nullptr)
					{
						::gloMemSet(&(pObject->CollisionObj.LV0),0,sizeof(pObject->CollisionObj.LV0));
						pObject->CollisionObj.LV0.type=San::Mathematics::SST_BALL;
						pObject->CollisionObj.pLV1=new SANSHAPE;
						pObject->CollisionObj.pLV1->type=San::Mathematics::SST_CUBOID;
						pObject->CollisionObj.pLV1->paramlist[2].x=pObject->pVertexList[VertexListSeek].v.x;
						pObject->CollisionObj.pLV1->paramlist[2].y=pObject->pVertexList[VertexListSeek].v.y;
						pObject->CollisionObj.pLV1->paramlist[2].z=pObject->pVertexList[VertexListSeek].v.z;
						pObject->CollisionObj.pLV1->paramlist[3].x=pObject->pVertexList[VertexListSeek].v.x;
						pObject->CollisionObj.pLV1->paramlist[3].y=pObject->pVertexList[VertexListSeek].v.y;
						pObject->CollisionObj.pLV1->paramlist[3].z=pObject->pVertexList[VertexListSeek].v.z;
					}
					else
					{
						if(pObject->pVertexList[VertexListSeek].v.x>pObject->CollisionObj.pLV1->paramlist[2].x)
						{
							pObject->CollisionObj.pLV1->paramlist[2].x=pObject->pVertexList[VertexListSeek].v.x;
						}
						if(pObject->pVertexList[VertexListSeek].v.y>pObject->CollisionObj.pLV1->paramlist[2].y)
						{
							pObject->CollisionObj.pLV1->paramlist[2].y=pObject->pVertexList[VertexListSeek].v.y;
						}
						if(pObject->pVertexList[VertexListSeek].v.z>pObject->CollisionObj.pLV1->paramlist[2].z)
						{
							pObject->CollisionObj.pLV1->paramlist[2].z=pObject->pVertexList[VertexListSeek].v.z;
						}
						if(pObject->pVertexList[VertexListSeek].v.x<pObject->CollisionObj.pLV1->paramlist[3].x)
						{
							pObject->CollisionObj.pLV1->paramlist[3].x=pObject->pVertexList[VertexListSeek].v.x;
						}
						if(pObject->pVertexList[VertexListSeek].v.y<pObject->CollisionObj.pLV1->paramlist[3].y)
						{
							pObject->CollisionObj.pLV1->paramlist[3].y=pObject->pVertexList[VertexListSeek].v.y;
						}
						if(pObject->pVertexList[VertexListSeek].v.z<pObject->CollisionObj.pLV1->paramlist[3].z)
						{
							pObject->CollisionObj.pLV1->paramlist[3].z=pObject->pVertexList[VertexListSeek].v.z;
						}
					}//*/
					VertexListSeek=VertexListSeek+1;
				}
			}
			break;
		default:
			break;
		}
	}
	delete[] pVertexList;
	delete[] pTextureList;
	delete[] pNormalList;
	delete[] pstrLine;
	delete[] pstrBuffer;
	pVertexList=nullptr;
	pTextureList=nullptr;
	pNormalList=nullptr;
	pstrLine=nullptr;
	pstrBuffer=nullptr;
	this->m_pFile->close();
	delete (this->m_pFile);
	this->m_pFile=nullptr;
	return true;
}
SANOBJECTLIST cOBJFileLoader::iGetObjectList()
{
	return this->m_ObjList;
}