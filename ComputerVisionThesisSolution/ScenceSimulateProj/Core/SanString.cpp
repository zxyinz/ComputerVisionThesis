#include"SanString.h"
using namespace std;
using namespace San;
swchar San::gloAToW(const sachar Data)
{
	return gloAToW((const sachar*)&Data,1)[0];
}
wstring San::gloAToW(const SStringA &strString)
{
	return ::gloAToW(strString.c_str(),strString.length());
}
wstring San::gloAToW(const sachar* pString,int StringLength)
{
	wstring strDestString;
	if (StringLength == -1)
	{
		StringLength = ::MultiByteToWideChar(CP_ACP, 0, pString, -1, nullptr, 0);
	}
	if(StringLength==0)
	{
		return strDestString;
	}
	swchar *pBuffer = new swchar[StringLength + 1];
	::gloMemSet(pBuffer, 0, sizeof(pBuffer));
	::MultiByteToWideChar(CP_ACP, 0, pString, -1, pBuffer, StringLength + 1);
	strDestString=pBuffer;
	delete[] pBuffer;
	pBuffer=nullptr;
	return strDestString;
}
sachar San::gloWToA(const swchar Data,const sachar DefaultChar)
{
	return gloWToA((const swchar*)&Data,1,DefaultChar)[0];
}
string San::gloWToA(const SStringW &strString,const sachar DefaultChar)
{
	return ::gloWToA(strString.c_str(),strString.length(),DefaultChar);
}
string San::gloWToA(const swchar* pString,int StringLength,const sachar DefaultChar)
{
	string strDestString;
	int bUseDefaultChar=true;
	if (StringLength == -1)
	{
		StringLength = ::WideCharToMultiByte(CP_ACP, 0, pString, -1, nullptr, 0, &DefaultChar, &bUseDefaultChar);
	}
	if(StringLength==0)
	{
		return strDestString;
	}
	sachar *pBuffer = new sachar[StringLength + 1];
	::gloMemSet(pBuffer,0,sizeof(pBuffer));
	::WideCharToMultiByte(CP_ACP, 0, pString, -1, pBuffer, StringLength + 1, &DefaultChar, &bUseDefaultChar);
	strDestString=pBuffer;
	delete[] pBuffer;
	pBuffer=nullptr;
	return strDestString;
}
schar San::gloAToT(const sachar Data)
{
#ifndef _UNICODE
	return Data;
#else
	return gloAToW((const sachar*)&Data,1)[0];
#endif
}
SString San::gloAToT(const SStringA &strString)
{
#ifndef _UNICODE
	return strString;
#else
	return gloAToW(strString.c_str(),strString.length());
#endif
}
SString San::gloAToT(const sachar* pString,int StringLength)
{
#ifndef _UNICODE
	return pString;
#else
	return gloAToW(pString,StringLength);
#endif
}
schar San::gloWToT(const swchar Data,const sachar DefaultChar)
{
#ifndef _UNICODE
	return gloWToA((const swchar*)&Data,1,DefaultChar)[0];
#else
	return Data;
#endif
}
SString San::gloWToT(const SStringW &strString,const sachar DefaultChar)
{
#ifndef _UNICODE
	return gloWToA(strString.c_str(),strString.length(),DefaultChar);
#else
	return strString;
#endif
}
SString San::gloWToT(const swchar* pString,int StringLength,const sachar DefaultChar)
{
#ifndef _UNICODE
	return gloWToA(pString,StringLength,DefaultChar);
#else
	return pString;
#endif
}
sachar San::gloTToA(const schar Data,const sachar DefaultChar)
{
#ifndef _UNICODE
	return Data;
#else
	return gloWToA((const schar*)Data,1,DefaultChar)[0];
#endif
}
string San::gloTToA(const SString &strString,const sachar DefaultChar)
{
#ifndef _UNICODE
	return strString;
#else
	return ::gloWToA(strString.c_str(),strString.length(),DefaultChar);
#endif
}
string San::gloTToA(const schar* pString,int StringLength,const sachar DefaultChar)
{
#ifndef _UNICODE
	return pString;
#else
	return ::gloWToA(pString,StringLength,DefaultChar);
#endif
}
swchar San::gloTToW(const schar Data)
{
#ifndef _UNICODE
	return gloAToW((const schar*)&Data,1)[0];
#else
	return Data;
#endif
}
wstring San::gloTToW(const SString &strString)
{
#ifndef _UNICODE
	return gloAToW(strString.c_str(),strString.length());
#else
	return strString;
#endif
}
wstring San::gloTToW(const schar* pString,int StringLength)
{
#ifndef _UNICODE
	return gloAToW(pString,StringLength);
#else
	return pString;
#endif
}
bool San::operator==(const SStringA &strStringA,const SStringA &strStringB)
{
	return (((int)strStringA.find(strStringB.c_str()))==0&&(strStringA.size()==strStringB.size()));
}
bool San::operator!=(const SStringA &strStringA,const SStringA &strStringB)
{
	return !(((int)strStringA.find(strStringB.c_str())==0)&&(strStringA.size()==strStringB.size()));
}
SStringA San::operator+(const SStringA &strStringA,const SStringA &strStringB)
{
	sachar *pschar=nullptr;
	size_t Size=strStringA.size()+strStringB.size();
	if(Size==0)
	{
		pschar=new sachar[1];
		pschar[0]='\0';
	}
	else
	{
		pschar=new sachar[Size+1];
		unsigned int StrASize=strStringA.size();
		for(size_t seeka=0;seeka<StrASize;seeka=seeka+1)
		{
			pschar[seeka]=strStringA[seeka];
		}
		for(size_t seekb=0;seekb<strStringB.size();seekb=seekb+1)
		{
			pschar[seekb+StrASize]=strStringB[seekb];
		}
		pschar[Size]='\0';
	}
	SStringA strDestString=pschar;
	delete[] pschar;
	return strDestString;
}
bool San::operator==(const SStringW &strStringA,const SStringW &strStringB)
{
	return (((int)strStringA.find(strStringB.c_str())==0)&&(strStringA.size()==strStringB.size()));
}
bool San::operator!=(const SStringW &strStringA,const SStringW &strStringB)
{
	int Index=strStringA.find(strStringB.c_str());
	return !(((int)strStringA.find(strStringB.c_str())==0)&&(strStringA.size()==strStringB.size()));
}
SStringW San::operator+(const SStringW &strStringA,const SStringW &strStringB)
{
	swchar *pschar=nullptr;
	size_t Size=strStringA.size()+strStringB.size();
	if(Size==0)
	{
		pschar=new swchar[1];
		pschar[0]=L'\0';
	}
	else
	{
		pschar=new swchar[Size+1];
		unsigned int StrASize=strStringA.size();
		for(size_t seeka=0;seeka<StrASize;seeka=seeka+1)
		{
			pschar[seeka]=strStringA[seeka];
		}
		for(size_t seekb=0;seekb<strStringB.size();seekb=seekb+1)
		{
			pschar[seekb+StrASize]=strStringB[seekb];
		}
		pschar[Size]=L'\0';
	}
	SStringW strDestString=pschar;
	delete[] pschar;
	return strDestString;
}
SString San::gloIToS(const long long &Data, const unsigned int Radix)
{
	schar String[512];
#ifndef _UNICODE
	::_itoa(Data, String, Radix);
#else
	::_itow(Data, String, Radix);
#endif
	return String;
}
long long San::gloSToI(const SString &strString, const unsigned int Radix)
{
	switch (Radix)
	{
	case 2:
		{
			long long Data = 0;
			for (unsigned int seek = 0; seek < strString.size(); seek = seek + 1)
			{
				if (strString[seek] == _SSTR('0'))
				{
					Data = Data << 1;
					continue;
				}
				if (strString[seek] == _SSTR('1'))
				{
					Data = Data << 1 + 1;
					continue;
				}
				Data = 0;
				return Data;
			}
			return Data;
		}
		break;
	case 10:
#ifndef _UNICODE
		return ::atoi(strString.c_str());
#else
		return ::_wtoi(strString.c_str());
#endif
		break;
	case 16:
		{
			long long Data;
			std::stringstream strStream;
			strStream << std::hex << strString.c_str();
			strStream >> Data;
			return Data;
		}
		break;
	default:
		return 0;
		break;
	}
	return 0;
}
SString San::gloFToS(const double &Data, SString strFormat)
{
	schar string[512];
	strFormat = _SSTR("%") + strFormat + _SSTR("f");
#ifndef _UNICODE
	::sprintf_s(string, 128, strFormat.c_str(), Data);
#else
	::swprintf_s(string, 128, strFormat.c_str(), Data);
#endif
	return string;
}
double San::gloSToF(const SString &strString)
{
#ifndef _UNICODE
	return ::atof(strString.c_str());
#else
	return ::_wtof(strString.c_str());
#endif
}
vector<SStringA> San::gloGetStringItemsA(const SStringA &strString, SStringA strStringMark)
{
	vector<SStringA> SubStringList;
	SubStringList.clear();
	if (strString.empty())
	{
		return SubStringList;
	}
	if (strStringMark.empty())
	{
		strStringMark = " \n\t";
	}
	SStringA strTarget = strString + strStringMark[0];
	size_t StrLength = strTarget.length();
	size_t MarkSize = strStringMark.length();
	size_t SubStringBegin = 0;
	for (size_t seek = 0; seek < StrLength; seek = seek + 1)
	{
		for (size_t seek_mark = 0; seek_mark < MarkSize; seek_mark = seek_mark + 1)
		{
			if (strTarget[seek] == strStringMark[seek_mark])
			{
				if (seek != SubStringBegin)
				{
					SubStringList.insert(SubStringList.end(), strTarget.substr(SubStringBegin, seek - SubStringBegin));
				}
				SubStringBegin = seek + 1;
				continue;
			}
		}
	}
	return SubStringList;
}
vector<SStringW> San::gloGetStringItemsW(const SStringW &strString, SStringW strStringMark)
{
	vector<SStringW> SubStringList;
	SubStringList.clear();
	if (strString.empty())
	{
		return SubStringList;
	}
	if (strStringMark.empty())
	{
		strStringMark = L" \n\t";
	}
	SStringW strTarget = strString + strStringMark[0];
	size_t StrLength = strTarget.length();
	size_t MarkSize = strStringMark.length();
	size_t SubStringBegin = 0;
	for (size_t seek = 0; seek < StrLength; seek = seek + 1)
	{
		for (size_t seek_mark = 0; seek_mark < MarkSize; seek_mark = seek_mark + 1)
		{
			if (strTarget[seek] == strStringMark[seek_mark])
			{
				if (seek != SubStringBegin)
				{
					SubStringList.insert(SubStringList.end(), strTarget.substr(SubStringBegin, seek - SubStringBegin));
				}
				SubStringBegin = seek + 1;
				continue;
			}
		}
	}
	return SubStringList;
}