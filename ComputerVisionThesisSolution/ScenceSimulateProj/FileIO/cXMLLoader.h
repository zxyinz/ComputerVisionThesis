#include"fstream"
#include"..\Core\SanTypes.h"
#include"..\Core\SanContainer.h"
#pragma once
using namespace std;
namespace San
{
#ifndef __CXMLLOADER_H__
#define __CXMLLOADER_H__

	struct stXMLELEMENT
	{
		SString Name;
		SString Value;
		list<stSANUNKNOWNEX<SString,SString>> Attributes;

	};
	typedef stXMLELEMENT	XMLELEMENT;
	typedef stXMLELEMENT*	LPXMLELEMENT;

	class cXMLLoader
	{
	private:
		fstream* m_pFile;
		stSANTREEEX<XMLELEMENT>* m_pRoot;
		stSANTREEEX<XMLELEMENT>* m_pCurrentNode;
		list<stSANUNKNOWNEX<SString,SString>>::iterator m_pCurrentAttribute;
	public:
		list<SString> m_ErrorList;
	protected:
		bool _CreateNode(wchar_t* p_Buffer,unsigned int BufferSize,stSANTREEEX<XMLELEMENT>* pNode,stack<SString>* pStack);
		void _DestoryNode(stSANTREEEX<XMLELEMENT>* pNode);
		SStringW _StringTranslate(SStringW strSource);
		void _PushError(SString strErrorString);
	public:
		cXMLLoader()
			:m_pFile(nullptr),
			m_pRoot(nullptr),
			m_pCurrentNode(nullptr)
		{
			this->m_ErrorList.clear();
		};
		cXMLLoader(SString strFilePath)
			:m_pFile(nullptr),
			m_pRoot(nullptr),
			m_pCurrentNode(nullptr)
		{
			this->m_ErrorList.clear();
			this->iLoadXMLFile(strFilePath);
		};
		~cXMLLoader()
		{
			this->iRelease();
		};
		bool iLoadXMLFile(SString strFilePath);
		void iRelease();////////////////////////////////////////////////////////////////////
		bool iSetToRoot();
		bool iSetToPreviousLevel();
		bool iSetToNextLevel();
		bool iSetToFirstNode();
		bool iSetToLastNode();
		bool iSetToPreviousNode();
		bool iSetToNextNode();
		bool iSetToNode(SString strNodeName);
		bool iSetToFirstAttribute();
		bool iSetToLastAttribute();
		bool iSetToPreviousAttribute();
		bool iSetToNextAttribute();
		bool iSetToAttribute(SString strAttributeName);
		SString iGetCurrentNodeName();
		SString iGetCurrentNodeValue(SString strDefault=_SSTR(""));
		SString iGetAttributeName();
		SString iGetAttributeValue(SString strDefault=_SSTR(""));
		SString iGetAttributeValuebyName(SString strAttributeName,SString strDefault=_SSTR(""));
		SString iGetFileCodeType();
	};
#endif
}