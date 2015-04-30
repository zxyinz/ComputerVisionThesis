#include"cXMLLoader.h"
using namespace std;
using namespace San;
bool cXMLLoader::iLoadXMLFile(SString strFilePath)
{
	this->m_ErrorList.clear();
	if (strFilePath.empty())
	{
		this->_PushError(_SSTR("Error:	Wrong XML file path"));
		return false;
	}
	if (this->m_pFile != nullptr)
	{
		this->_PushError(_SSTR("Error: The XML file already exits"));
		return false;
	}
	if (this->m_pRoot != nullptr)
	{
		this->_PushError(_SSTR("Error:	The XML file already exits"));
		return false;
	}
	this->m_pFile = new fstream(strFilePath, ios::binary | ios::in, 0x40);
	if (this->m_pFile == nullptr)
	{
		this->_PushError(_SSTR("Error:	Faild to load XML file in path: ") + strFilePath);
		return false;
	}
	this->m_pFile->seekg(0, ios::end);
	int32 FileSize = this->m_pFile->tellg();
	if (FileSize <= 0)
	{
		this->_PushError(_SSTR("Error:	Faild to load XML file in path") + strFilePath);
		return false;
	}
	this->m_pFile->clear();
	this->m_pFile->seekg(0, ios::beg);
	FileSize = FileSize / 2;//Unicode File//
	swchar* pBuffer = new swchar[FileSize];
	::memset(pBuffer, 0, sizeof(swchar) *FileSize);
	this->m_pFile->read((char*) pBuffer, FileSize * 2);
	this->m_pRoot = new stSANTREEEX<XMLELEMENT>;
	this->m_pRoot->p_trunk = nullptr;
	this->m_pRoot->identify = 0;
	this->m_pRoot->listsize = 0;
	this->m_pRoot->p_offshootlist.clear();
	this->m_pRoot->data.Name = _SSTR("XML_ROOT");
	this->m_pRoot->data.Value = _SSTR("");
	this->m_pCurrentNode = this->m_pRoot;
	this->m_pCurrentAttribute = this->m_pCurrentNode->data.Attributes.begin();
	stack<SString> NodeStack;
	uint32 SeekBegin = 0;
	swchar pstrValue[512];
	for (uint32 seek = 0; seek<FileSize; seek = seek + 1)
	{
		switch (pBuffer[seek])
		{
		case L'<':
			switch (pBuffer[seek + 1])
			{
			case L'?':
				while ((seek<FileSize) && (pBuffer[seek] != L'>'))
				{
					seek = seek + 1;
				}
				break;
			case L'/':
				SeekBegin = seek + 2;
				while ((seek<FileSize) && (pBuffer[seek] != L'>'))
				{
					seek = seek + 1;
				}
				if (NodeStack.empty())
				{
					this->_PushError(_SSTR("Error: XML file format miss match. Node:") + NodeStack.top());
					delete[] pBuffer;
					pBuffer = nullptr;
					this->iRelease();
					return false;
				}
				pBuffer[seek] = L'\0';
				if ((sint) NodeStack.top().find(pBuffer + SeekBegin) >= 0)
				{
					NodeStack.pop();
					this->m_pCurrentNode = this->m_pCurrentNode->p_trunk;
				}
				else
				{
					this->_PushError(_SSTR("Error: XML file format miss match. Node:") + NodeStack.top());
					pBuffer[seek] = L'>';
					delete[] pBuffer;
					pBuffer = nullptr;
					this->iRelease();
					return false;
				}
				pBuffer[seek] = L'>';
				break;
			default:
				SeekBegin = seek + 1;
				while (pBuffer[seek] != L'>')
				{
					seek = seek + 1;
				}
				if (!this->_CreateNode(pBuffer + SeekBegin, seek - SeekBegin, this->m_pCurrentNode, &NodeStack))
				{
					delete[] pBuffer;
					this->iRelease();
					return false;
				}
				break;
			}
			break;
		case L'>':
			this->_PushError(_SSTR("Error: XML file format miss match. Node:") + NodeStack.top());
			delete[] pBuffer;
			pBuffer = nullptr;
			this->iRelease();
			return false;
			break;
		default:
			if ((this->m_pCurrentNode != nullptr) && (this->m_pCurrentNode != this->m_pRoot))
			{
				SeekBegin = seek;
				while ((seek<FileSize) && (pBuffer[seek] != L'<'))
				{
					if (pBuffer[seek] == L'>')
					{
						seek = seek - 1;
						break;
					}
					seek = seek + 1;
				}
				pBuffer[seek] = L'\0';
				this->m_pCurrentNode->data.Value = this->_StringTranslate(pBuffer + SeekBegin);
				pBuffer[seek] = L'<';
				seek = seek - 1;
			}
			break;
		}
	}
	delete[] pBuffer;
	pBuffer = nullptr;
	if (!NodeStack.empty())
	{
		this->_PushError(_SSTR("Error: XML file format miss match."));
		this->iRelease();
		return false;
	}
	this->m_pFile->close();
	delete this->m_pFile;
	this->m_pFile = nullptr;
	this->m_pCurrentNode = (*this->m_pRoot->p_offshootlist.begin());
	this->m_pCurrentAttribute = this->m_pCurrentNode->data.Attributes.begin();
	return true;
}
bool cXMLLoader::_CreateNode(swchar* pBuffer, uint32 BufferSize, stSANTREEEX<XMLELEMENT>* pNode, stack<SString>* pStack)
{
	if (pBuffer == nullptr)
	{
		return false;
	}
	if (BufferSize == 0)
	{
		return false;
	}
	pBuffer[BufferSize] = L'\0';
	vector<SString> ItemList = San::gloGetStringItems(pBuffer, _SSTR(" "));
	pBuffer[BufferSize] = L'>';
	if (ItemList.empty())
	{
		this->_PushError(_SSTR("Error:	Wrong XML format, can't find node name"));
		return false;
	}
	vector<SString>::iterator pItem = ItemList.begin();
	stSANTREEEX<XMLELEMENT>* pItemNode = new stSANTREEEX<XMLELEMENT>;
	pItemNode->p_trunk = pNode;
	pItemNode->identify = pNode->listsize;
	pItemNode->data.Name = *pItem;
	pItemNode->data.Value = _SSTR("");
	pItemNode->data.Attributes.clear();
	pItemNode->listsize = 0;
	pItemNode->p_offshootlist.clear();
	pItem++;
	stSANUNKNOWNEX<SString, SString> Attribute;
	sint seek;
	while (pItem != ItemList.end())
	{
		seek = (sint) pItem->find(L'=');
		if (seek<0)
		{
			this->_PushError(_SSTR("Error:	Wrong XML format, attribute format miss match.\n\tMiss: '='\n\tNode: ") + pItemNode->data.Name);
			return false;
		}
		Attribute.description = this->_StringTranslate(pItem->substr(0, seek));
		Attribute.value = pItem->substr(seek + 1, pItem->length() - seek - 1);
		if ((Attribute.value.c_str()[0] != L'"') || (Attribute.value.c_str()[Attribute.value.length() - 1] != L'"'))
		{
			this->_PushError(_SSTR("Error:	Wrong XML format, attribute format miss match.\n\tMiss: '\"'\n\tNode: ") + pItemNode->data.Name);
			return false;
		}
		Attribute.value = this->_StringTranslate(Attribute.value.substr(1, Attribute.value.length() - 2));
		pItemNode->data.Attributes.insert(pItemNode->data.Attributes.end(), Attribute);
		pItem++;
	}
	pNode->p_offshootlist.insert(pNode->p_offshootlist.end(), pItemNode);
	pNode->listsize = pNode->listsize + 1;
	this->m_pCurrentNode = pItemNode;
	pStack->push(pItemNode->data.Name);
	return true;
}
void cXMLLoader::_DestoryNode(stSANTREEEX<XMLELEMENT>* pNode)
{
	if (pNode != nullptr)
	{
		while (pNode->listsize != 0)
		{
			this->_DestoryNode(*pNode->p_offshootlist.begin());
			pNode->p_offshootlist.erase(pNode->p_offshootlist.begin());
			pNode->listsize = pNode->listsize - 1;
		}
		pNode->identify = 0;
		pNode->data.Name.clear();
		pNode->data.Value.clear();
		pNode->data.Attributes.clear();
		pNode->p_trunk = nullptr;
		delete pNode;
		pNode = nullptr;
	}
}
SStringW cXMLLoader::_StringTranslate(SStringW strSource)
{
	if (strSource.empty())
	{
		return L"";
	}
	uint32 Size = strSource.length() + 1;
	swchar* pDest = new swchar[Size];
	::memset(pDest, 0, sizeof(swchar) *Size);
	const swchar* pSrc = strSource.c_str();
	uint32 DestSeek = 0;
	for (uint32 seek = 0; seek<Size; seek = seek + 1)
	{
		if (pSrc[seek] != L'&')
		{
			pDest[DestSeek] = pSrc[seek];
		}
		else
		{
			uint32 seek_begin = seek;
			while ((seek<Size) && (pSrc[seek] != L';'))
			{
				seek = seek + 1;
			}
			if (seek == Size)
			{
				delete[] pDest;
				pDest = nullptr;
				return L"";
			}
			switch (seek - seek_begin - 1)
			{
			case 2:
				if ((pSrc[seek_begin + 1] == L'l') && (pSrc[seek_begin + 2] == L't'))
				{
					pDest[DestSeek] = L'<';
				}
				if ((pSrc[seek_begin + 1] == L'g') && (pSrc[seek_begin + 2] == L't'))
				{
					pDest[DestSeek] = L'>';
				}
				break;
			case 3:
				if ((pSrc[seek_begin + 1] == L'a') && (pSrc[seek_begin + 2] == L'm') && (pSrc[seek_begin + 3] == L'p'))
				{
					pDest[DestSeek] = L'&';
				}
				break;
			case 4:
				if ((pSrc[seek_begin + 1] == L'a') && (pSrc[seek_begin + 2] == L'p') && (pSrc[seek_begin + 3] == L'o') && (pSrc[seek_begin + 4] == L's'))
				{
					pDest[DestSeek] = L'\'';
				}
				if ((pSrc[seek_begin + 1] == L'q') && (pSrc[seek_begin + 2] == L'u') && (pSrc[seek_begin + 3] == L'o') && (pSrc[seek_begin + 4] == L't'))
				{
					pDest[DestSeek] = L'=';
				}
				break;
			default:
				delete[] pDest;
				pDest = nullptr;
				return L"";
			}
		}
		DestSeek = DestSeek + 1;
	}
	SString DestString = pDest;
	delete[] pDest;
	pDest = nullptr;
	return DestString;
}
void cXMLLoader::_PushError(SString strErrorString)
{
	this->m_ErrorList.insert(this->m_ErrorList.end(), strErrorString);
}
void cXMLLoader::iRelease()
{
	this->_DestoryNode(this->m_pRoot);
	this->m_pRoot = nullptr;
	this->m_pCurrentNode = nullptr;
	this->m_ErrorList.clear();
}
bool cXMLLoader::iSetToRoot()
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	this->m_pCurrentNode = (*this->m_pRoot->p_offshootlist.begin());
	this->m_pCurrentAttribute = this->m_pCurrentNode->data.Attributes.begin();
}
bool cXMLLoader::iSetToPreviousLevel()
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	if (this->m_pCurrentNode == nullptr)
	{
		this->_PushError(_SSTR("Error:	The current node pointer is empty"));
		return false;
	}
	if (this->m_pCurrentNode == (*this->m_pRoot->p_offshootlist.begin()))
	{
		this->_PushError(_SSTR("Error:	The current node is the top node"));
		return false;
	}
	this->m_pCurrentNode = this->m_pCurrentNode->p_trunk;
	this->m_pCurrentAttribute = this->m_pCurrentNode->data.Attributes.begin();
	return true;
}
bool cXMLLoader::iSetToNextLevel()
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	if (this->m_pCurrentNode == nullptr)
	{
		this->_PushError(_SSTR("Error:	The current node pointer is empty"));
		return false;
	}
	if (this->m_pCurrentNode->listsize == 0)
	{
		this->_PushError(_SSTR("Error:	The current node is the leaf node"));
		return false;
	}
	this->m_pCurrentNode = (*this->m_pCurrentNode->p_offshootlist.begin());
	this->m_pCurrentAttribute = this->m_pCurrentNode->data.Attributes.begin();
	return true;
}
bool cXMLLoader::iSetToFirstNode()
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	if (this->m_pCurrentNode == nullptr)
	{
		this->_PushError(_SSTR("Error:	The current node pointer is empty"));
		return false;
	}
	if (this->m_pCurrentNode == this->m_pRoot)
	{
		this->_PushError(_SSTR("Error:	The current node is the top node"));
		return false;
	}
	if (this->m_pCurrentNode->identify == 0)
	{
		return true;
	}
	this->m_pCurrentNode = (*this->m_pCurrentNode->p_trunk->p_offshootlist.begin());
	this->m_pCurrentAttribute = this->m_pCurrentNode->data.Attributes.begin();
	return true;
}
bool cXMLLoader::iSetToLastNode()
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	if (this->m_pCurrentNode == nullptr)
	{
		this->_PushError(_SSTR("Error:	The current node pointer is empty"));
		return false;
	}
	if (this->m_pCurrentNode == this->m_pRoot)
	{
		this->_PushError(_SSTR("Error:	The current node is the top node"));
		return false;
	}
	if (this->m_pCurrentNode->identify == this->m_pCurrentNode->p_trunk->listsize - 1)
	{
		return true;
	}
	this->m_pCurrentNode = (*this->m_pCurrentNode->p_trunk->p_offshootlist.end());
	this->m_pCurrentAttribute = this->m_pCurrentNode->data.Attributes.begin();
	return true;
}
bool cXMLLoader::iSetToPreviousNode()
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	if (this->m_pCurrentNode == nullptr)
	{
		this->_PushError(_SSTR("Error:	The current node pointer is empty"));
		return false;
	}
	if (this->m_pCurrentNode == this->m_pRoot)
	{
		this->_PushError(_SSTR("Error:	The current node is the top node"));
		return false;
	}
	if (this->m_pCurrentNode->identify == 0)
	{
		this->_PushError(_SSTR("Error:	BOF"));
		return false;
	}
	list<stSANTREEEX<XMLELEMENT>*>::iterator pItem = this->m_pCurrentNode->p_trunk->p_offshootlist.begin();
	for (uint32 seek = 0; seek < (this->m_pCurrentNode->identify - 1); seek = seek + 1)
	{
		pItem++;
	}
	this->m_pCurrentNode = (*pItem);
	this->m_pCurrentAttribute = this->m_pCurrentNode->data.Attributes.begin();
	return true;
}
bool cXMLLoader::iSetToNextNode()
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	if (this->m_pCurrentNode == nullptr)
	{
		this->_PushError(_SSTR("Error:	The current node pointer is empty"));
		return false;
	}
	if (this->m_pCurrentNode == this->m_pRoot)
	{
		this->_PushError(_SSTR("Error:	The current node is the top node"));
		return false;
	}
	if (this->m_pCurrentNode->identify == this->m_pCurrentNode->p_trunk->listsize - 1)
	{
		this->_PushError(_SSTR("Error:	EOF"));
		return false;
	}
	list<stSANTREEEX<XMLELEMENT>*>::iterator pItem = this->m_pCurrentNode->p_trunk->p_offshootlist.begin();
	for (uint32 seek = 0; seek <= this->m_pCurrentNode->identify; seek = seek + 1)
	{
		pItem++;
	}
	this->m_pCurrentNode = (*pItem);
	this->m_pCurrentAttribute = this->m_pCurrentNode->data.Attributes.begin();
	return true;
}
bool cXMLLoader::iSetToNode(SString strNodeName)
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	if (strNodeName.empty())
	{
		this->_PushError(_SSTR("Error:	The node name is empty"));
		return false;
	}
	if (this->m_pCurrentNode == nullptr)
	{
		this->_PushError(_SSTR("Error:	The current node pointer is empty"));
		return false;
	}
	if (this->m_pCurrentNode == this->m_pRoot)
	{
		this->_PushError(_SSTR("Error:	The current node is the top node"));
		return false;
	}
	list<stSANTREEEX<XMLELEMENT>*>::iterator pItem = this->m_pCurrentNode->p_trunk->p_offshootlist.begin();
	while (pItem != this->m_pCurrentNode->p_trunk->p_offshootlist.end())
	{
		if ((sint) (*pItem)->data.Name.find(strNodeName) == 0)
		{
			break;
		}
		pItem++;
	}
	if (pItem == this->m_pCurrentNode->p_trunk->p_offshootlist.end())
	{
		this->_PushError(_SSTR("Error:	Can't find node ") + strNodeName);
		return false;
	}
	this->m_pCurrentNode = (*pItem);
	this->m_pCurrentAttribute = this->m_pCurrentNode->data.Attributes.begin();
	return true;
}
bool cXMLLoader::iSetToFirstAttribute()
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	if (this->m_pCurrentNode == nullptr)
	{
		this->_PushError(_SSTR("Error:	The current node pointer is empty"));
		return false;
	}
	if (this->m_pCurrentNode->data.Attributes.empty())
	{
		this->_PushError(_SSTR("Error:	The current node attribute list is empty"));
		return false;
	}
	this->m_pCurrentAttribute = this->m_pCurrentNode->data.Attributes.begin();
	return true;
}
bool cXMLLoader::iSetToLastAttribute()
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	if (this->m_pCurrentNode == nullptr)
	{
		this->_PushError(_SSTR("Error:	The current node pointer is empty"));
		return false;
	}
	if (this->m_pCurrentNode->data.Attributes.empty())
	{
		this->_PushError(_SSTR("Error:	The current node attribute list is empty"));
		return false;
	}
	this->m_pCurrentAttribute = this->m_pCurrentNode->data.Attributes.end();
	this->m_pCurrentAttribute--;
	return true;
}
bool cXMLLoader::iSetToPreviousAttribute()
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	if (this->m_pCurrentNode == nullptr)
	{
		this->_PushError(_SSTR("Error:	The current node pointer is empty"));
		return false;
	}
	if (this->m_pCurrentNode->data.Attributes.empty())
	{
		this->_PushError(_SSTR("Error:	The current node attribute list is empty"));
		return false;
	}
	if (this->m_pCurrentAttribute == this->m_pCurrentNode->data.Attributes.begin())
	{
		this->_PushError(_SSTR("Error:	The attribute BOF"));
		return false;
	}
	this->m_pCurrentAttribute--;
	return true;
}
bool cXMLLoader::iSetToNextAttribute()
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	if (this->m_pCurrentNode == nullptr)
	{
		this->_PushError(_SSTR("Error:	The current node pointer is empty"));
		return false;
	}
	if (this->m_pCurrentNode->data.Attributes.empty())
	{
		this->_PushError(_SSTR("Error:	The current node attribute list is empty"));
		return false;
	}
	if (this->m_pCurrentAttribute == (this->m_pCurrentNode->data.Attributes.end()--))
	{
		this->_PushError(_SSTR("Error:	The attribute EOF"));
		return false;
	}
	this->m_pCurrentAttribute++;
	return true;
}
bool cXMLLoader::iSetToAttribute(SString strAttributeName)
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	if (strAttributeName.empty())
	{
		this->_PushError(_SSTR("Error:	The attribute name is empty"));
		return false;
	}
	if (this->m_pCurrentNode == nullptr)
	{
		this->_PushError(_SSTR("Error:	The current node pointer is empty"));
		return false;
	}
	if (this->m_pCurrentNode->data.Attributes.empty())
	{
		this->_PushError(_SSTR("Error:	The current node attribute list is empty"));
		return false;
	}
	if ((sint) (this->m_pCurrentAttribute->description.find(strAttributeName)) == 0)
	{
		return true;
	}
	list<stSANUNKNOWNEX<SString, SString>>::iterator pItem = this->m_pCurrentNode->data.Attributes.begin();
	while (pItem != this->m_pCurrentNode->data.Attributes.end())
	{
		if ((sint) (pItem->description.find(strAttributeName)) == 0)
		{
			break;
		}
		pItem++;
	}
	if (pItem == this->m_pCurrentNode->data.Attributes.end())
	{
		this->_PushError(_SSTR("Error:	Can't fine attribute ") + strAttributeName + _SSTR(" in node ") + this->m_pCurrentNode->data.Name);
		return false;
	}
	this->m_pCurrentAttribute = pItem;
	return true;
}
SString cXMLLoader::iGetCurrentNodeName()
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	if (this->m_pCurrentNode == nullptr)
	{
		this->_PushError(_SSTR("Error:	The current node pointer is empty"));
		return _SSTR("");
	}
	return this->m_pCurrentNode->data.Name;
}
SString cXMLLoader::iGetCurrentNodeValue(SString strDefault)
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	if (this->m_pCurrentNode == nullptr)
	{
		this->_PushError(_SSTR("Error:	The current node pointer is empty"));
		return strDefault;
	}
	if (this->m_pCurrentNode->data.Value.empty())
	{
		return strDefault;
	}
	return this->m_pCurrentNode->data.Value;
}
SString cXMLLoader::iGetAttributeName()
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	if (this->m_pCurrentNode == nullptr)
	{
		this->_PushError(_SSTR("Error:	The current node pointer is empty"));
		return _SSTR("");
	}
	if (this->m_pCurrentNode->data.Attributes.empty())
	{
		this->_PushError(_SSTR("Error:	The current node attribute list is empty"));
		return _SSTR("");
	}
	return this->m_pCurrentAttribute->description;
}
SString cXMLLoader::iGetAttributeValue(SString strDefault)
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	if (this->m_pCurrentNode == nullptr)
	{
		this->_PushError(_SSTR("Error:	The current node pointer is empty"));
		return strDefault;
	}
	if (this->m_pCurrentNode->data.Attributes.empty())
	{
		this->_PushError(_SSTR("Error:	The current node attribute list is empty"));
		return strDefault;
	}
	return this->m_pCurrentAttribute->value;
}
SString cXMLLoader::iGetAttributeValuebyName(SString strAttributeName, SString strDefault)
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	if (strAttributeName.empty())
	{
		this->_PushError(_SSTR("Error:	The sttribute name is empty"));
		return strDefault;
	}
	if (this->m_pCurrentNode == nullptr)
	{
		this->_PushError(_SSTR("Error:	The current node pointer is empty"));
		return strDefault;
	}
	if (this->m_pCurrentNode->data.Attributes.empty())
	{
		this->_PushError(_SSTR("Error:	The current node attribute list is empty"));
		return strDefault;
	}
	list<stSANUNKNOWNEX<SString, SString>>::iterator pItem = this->m_pCurrentNode->data.Attributes.begin();
	while (pItem != this->m_pCurrentNode->data.Attributes.end())
	{
		if ((sint) (pItem->description.find(strAttributeName)) == 0)
		{
			break;
		}
		pItem++;
	}
	if (pItem == this->m_pCurrentNode->data.Attributes.end())
	{
		this->_PushError(_SSTR("Error:	Can't find attribute ") + strAttributeName + _SSTR(" in node ") + this->m_pCurrentNode->data.Name);
		return strDefault;
	}
	return pItem->value;
}
SString cXMLLoader::iGetFileCodeType()
{
	if (this->m_pRoot == nullptr)
	{
		this->_PushError(_SSTR("Error:	The file is empty"));
		return false;
	}
	return this->m_pRoot->data.Value;
}