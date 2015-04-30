#include"iostream"
#include"fstream"
#include"../Core/SanString.h"
#include"../Graphics/SanGraphics.h"
using namespace std;
using namespace San::Graphics;
namespace San
{
#ifndef __COBJFILELOADER_H__
#define __COBJFILELOADER_H__
	struct stOBJMATERIALELEMENT
	{
		SANMATERIAL Material;
		SStringA strName;
	};
	typedef stOBJMATERIALELEMENT	OBJMATERIALELEMENT;
	typedef stOBJMATERIALELEMENT*	LPOBJMATERIALELEMENT;
	typedef list<OBJMATERIALELEMENT>	OBJMATERIALLIST;
	typedef list<OBJMATERIALELEMENT>*	LPOBJMATERIALLIST;

	class cOBJFileLoader
	{
	private:
		SStringW m_strFilePath;
		fstream *m_pFile;
		SANOBJECTLIST m_ObjList;
		OBJMATERIALLIST m_MaterialList;
	protected:
		void _LoadMaterial(SStringW strFilePath);
		bool _ReadLine(fstream *pfile,char* &pstring,UINT Size);
		void _ClearList();
	public:
		cOBJFileLoader()
		{
			this->m_strFilePath.clear();
			this->m_ObjList.clear();
			this->m_MaterialList.clear();
			this->m_pFile=nullptr;
		};
		~cOBJFileLoader()
		{
			this->_ClearList();
		};
		bool iLoadFile(SStringW strFilePath=_SSTR(""));
		SANOBJECTLIST iGetObjectList();
	};
#endif
}