//Project: San Lab Game Engine
//Version: 2.1.1
//Debug State: Add functions and need test [utf8, optimize]
#include"list"
#include"vector"
#include"string"
#include"sstream"
#include"../RenderSystem.h"
#include"SanMemory.h"
#pragma once
using namespace std;
namespace San
{
#ifndef __STDSANSTRING_H__
#define __STDSANSTRING_H__

	typedef char		sachar;
	typedef wchar_t		swchar;
	typedef string		SStringA;
	typedef wstring		SStringW;

#ifndef _UNICODE
	typedef sachar		schar;
	typedef SStringA	SString;
#else
	typedef swchar		schar;
	typedef SStringW	SString;
#endif

	typedef SString	SANSYSTEMID;

#ifdef _UNICODE
#define _SSTR(__String__) L##__String__
#else
#define _SSTR(__String__) __String__
#endif

	swchar	gloAToW(const sachar Data);
	wstring	gloAToW(const SStringA &strString);
	wstring	gloAToW(const sachar* pString, int StringLength = -1);
	//swchar	gloUToW(const swchar Data, const size_t UnicodeID = CP_UTF8);
	//wstring	gloAToW(const SStringW &strString, const size_t UnicodeID = CP_UTF8);
	//wstring	gloUToW(const swchar* pString, int StringLength = -1, const size_t UnicodeID = CP_UTF8);
	sachar	gloWToA(const swchar Data, const sachar DefaultChar = '*');
	string	gloWToA(const SStringW &strString, const sachar DefaultChar = '*');
	string	gloWToA(const swchar* pString, int StringLength = -1, const sachar DefaultChar = '*');
	//sachar	gloWToU(const swchar Data, const sachar DefaultChar = '*', const size_t UnicodeID = CP_UTF8);
	//string	gloWToU(const SStringW &strString, const sachar DefaultChar = '*', const size_t UnicodeID = CP_UTF8);
	//string	gloWToU(const swchar* pString, int StringLength = -1, const sachar DefaultChar = '*', const size_t UnicodeID = CP_UTF8);
	schar	gloAToT(const sachar Data);
	SString	gloAToT(const SStringA &strString);
	SString gloAToT(const sachar* pString, int StringLength = -1);
	schar	gloWToT(const swchar Data, const sachar DefaultChar = '*');
	SString gloWToT(const SStringW &strString, const sachar DefaultChar = '*');
	SString gloWToT(const swchar* pString, int StringLength = -1, const sachar DefaultChar = '*');
	//schar	gloUToT(const swchar Data, const sachar DefaultChar = '*', const size_t UnicodeID = CP_UTF8);
	//SString gloUToT(const SStringW &strString, const sachar DefaultChar = '*', const size_t UnicodeID = CP_UTF8);
	//SString gloUToT(const swchar* pString, int StringLength = -1, const sachar DefaultChar = '*', const size_t UnicodeID = CP_UTF8);
	sachar	gloTToA(const schar Data, const sachar DefaultChar = '*');
	string	gloTToA(const SString &strString, const sachar DefaultChar = '*');
	string	gloTToA(const schar* pString, int StringLength = -1, const sachar DefaultChar = '*');
	swchar	gloTToW(schar Data);
	wstring	gloTToW(const SString &strString);
	wstring	gloTToW(const schar* pString,int StringLength = -1);
	//sachar	gloTToU(const schar Data, const sachar DefaultChar = '*', const size_t UnicodeID = CP_UTF8);
	//string	gloTToU(const SString &strString, const sachar DefaultChar = '*', const size_t UnicodeID = CP_UTF8);
	//string	gloTToU(const schar* pString, int StringLength = -1, const sachar DefaultChar = '*', const size_t UnicodeID = CP_UTF8);

	bool	operator==(const SStringA &strStringA,const SStringA &strStringB);
	bool	operator!=(const SStringA &strStringA,const SStringA &strStringB);
	bool	operator==(const SStringW &strStringA,const SStringW &strStringB);
	bool	operator!=(const SStringW &strStringA,const SStringW &strStringB);

	//bool	operator==(const SString &strStringA,const SString &strStringB);
	//bool	operator!=(const SString &strStringA,const SString &strStringB);

	SStringA operator+(const SStringA &strStringA,const SStringA &strStringB);
	SStringW operator+(const SStringW &strStringA,const SStringW &strStringB);
	//SString	operator+(const SString &strStringA,const SString &strStringB);

	//SStringA gloStringFormat(SStringA Format,...);
	//SStringW gloStringFormat(SStringW Format,...);
#ifndef _UNICODE
//#define gloStringFormat std::sscanf
#else
//#define gloStringFormat std::wscanf
#endif
	SString	gloStringFormat(SString Format,...);

	SString		gloIToS(const long long &Data, const unsigned int Radix = 10);
	long long	gloSToI(const SString &strString, const unsigned int Radix = 10);
	SString		gloFToS(const double &Data, SString strFormat = _SSTR(""));
	double		gloSToF(const SString &strString);

	vector<SStringA> gloGetStringItemsA(const SStringA &strString, SStringA strStringMark = "\0");
	vector<SStringW> gloGetStringItemsW(const SStringW &strString, SStringW strStringMark = L"\0");
#ifndef _UNICODE
#define gloGetStringItems gloGetStringItemsA
#else
#define gloGetStringItems gloGetStringItemsW
#endif

	inline void gloPrintWideString(const SStringW &strWString){std::wprintf(L"%s",strWString.c_str());/*std::wcout<<strWString.c_str();*/};

	template<class ST>
	inline void gloPrintArray(const ST* pArray,size_t Size,size_t RowSize=0,schar Block=_SSTR('\t'),schar RowBlock=_SSTR('\n'),SString strAdditionString=_SSTR("\n"))
	{
		if(RowSize==0){RowSize=Size;}
		for(size_t seek=0;seek<Size;seek=seek+1)
		{
#ifndef _UNICODE
			::cout<<pArray[seek]<<Block;
#else
			::wcout<<pArray[seek]<<Block;
#endif
			if(((seek+1)%RowSize)==0)
			{
#ifndef _UNICODE
				::cout<<RowBlock;
#else
				::wcout<<RowBlock;
#endif
			}
		}
#ifndef _UNICODE
		::cout<<strAdditionString.c_str();
#else
		::wcout<<strAdditionString.c_str();
#endif
	};
#endif
}
