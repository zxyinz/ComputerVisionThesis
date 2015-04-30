#include"SanNetwork.h"
using namespace std;
using namespace San;
using namespace San::Network;
unsigned short San::Network::glo_ntohs(unsigned short NetData)
{
	return ((NetData&0x00ff)*256+(NetData&0xff00)/256);
}
unsigned long San::Network::glo_ntohl(unsigned long NetData)
{
	return ((NetData&0x000000ff)*16777216+(NetData&0x0000ff00)*256+(NetData&0x00ff0000)/256+(NetData&0xff000000)/16777216);
}
unsigned long San::Network::gloStrToIPv4(SStringA strAddress)
{
	SANIPADDRv4 Address;
	Address.addr_32=0;
	UINT AddrByte=0;
	for(int seek=3;seek>=0;seek=seek-1)
	{
		if(strAddress.empty())
		{
			Address.addr_32=0;
			return Address.addr_32;
		}
		SStringA strByte=strAddress.substr(0,strAddress.find('.'));
		AddrByte=::atoi(strByte.c_str());
		if(AddrByte>255)
		{
			Address.addr_32=0;
			return Address.addr_32;
		}
		Address.addr_byte[seek]=AddrByte;
		if(Address.addr_byte[seek]==0)
		{
			for(int seekbit=0;seekbit<strByte.size();seekbit=seekbit+1)
			{
				if(strByte[seekbit]!='0')
				{
					Address.addr_32=0;
					return Address.addr_32;
				}
			}
		}
		else
		{
			if(Address.addr_byte[seek]<::pow((sfloat)10,(int)strByte.size()-1))
			{
				Address.addr_32=0;
				return Address.addr_32;
			}
		}
		strAddress=strAddress.substr(strAddress.find('.')+1,strAddress.size());
	}
	return Address.addr_32;
}
SString San::Network::gloIPv4ToStr(unsigned long Address)
{
	SString strAddress;
	unsigned seek=0;
	schar String[16];
	::memset(String,0,sizeof(schar)*16);
#ifndef _UNICODE
	::sprintf(String,"%d.%d.%d.%d",(Address&0xff000000)/16777216,(Address&0x00ff0000)/65536,(Address&0x0000ff00)/256,(Address&0x000000ff));
#else
	::wsprintfW(String,_SSTR("%d.%d.%d.%d"),(Address&0xff000000)/16777216,(Address&0x00ff0000)/65536,(Address&0x0000ff00)/256,(Address&0x000000ff));
#endif
	strAddress=String;
	return strAddress;
}
bool San::Network::gloIsIPv4Address(SStringA strAddress)
{
	SANIPADDRv4 Address;
	Address.addr_32=0;
	UINT AddrByte=0;
	for(int seek=3;seek>=0;seek=seek-1)
	{
		if(strAddress.empty())
		{
			return false;
		}
		SStringA strByte=strAddress.substr(0,strAddress.find('.'));
		AddrByte=::atoi(strByte.c_str());
		if(AddrByte>255)
		{
			return false;
		}
		Address.addr_byte[seek]=AddrByte;
		if(Address.addr_byte[seek]==0)
		{
			for(int seekbit=0;seekbit<strByte.size();seekbit=seekbit+1)
			{
				if(strByte[seekbit]!='0')
				{
					return false;
				}
			}
		}
		else
		{
			if(Address.addr_byte[seek]<::pow((sfloat)10,(int)strByte.size()))
			{
				return false;
			}
		}
		strAddress=strAddress.substr(strAddress.find('.')+1,strAddress.size());
	}
	return true;
}
void San::Network::gloShowPackageInfo(UINT Length,const unsigned char *pData)
{
	UINT address=0x00000000;
	UINT Step=16;
	for(int seek=0;seek<Length/Step;seek=seek+1)
	{
		for(int seekl=0;seekl<Step;seekl=seekl+1)
		{
			unsigned char Byte=pData[seek*10+seekl];
			if(Byte/16>9)
			{
				::cout<<(char)('A'+Byte/16-10);
			}
			else
			{
				::cout<<(char)('0'+Byte/16);
			}
			Byte=(int)(Byte%16);
			if(Byte>9)
			{
				::cout<<(char)('A'+Byte-10);
			}
			else
			{
				::cout<<(char)('0'+Byte);
			}
			::cout<<" ";
		}
		for(unsigned int seekstr=0;seekstr<Step;seekstr=seekstr+1)
		{
			char Byte=pData[seek*10+seekstr];
			if(((Byte>='0')&&(Byte<='9'))||((Byte>='a')&&(Byte<='z'))||((Byte>='A')&&(Byte<='Z')))
			{
				::cout<<(char)(Byte);
			}
			else
			{
				switch(Byte)
				{
				case '!':
				case '@':
				case '#':
				case '$':
				case '%':
				case '^':
				case '&':
				case '*':
				case '(':
				case ')':
				case '-':
				case '_':
				case '=':
				case '+':
				case '[':
				case ']':
				case '{':
				case '}':
				case '\\':
				case '|':
				case ';':
				case ':':
				case '\'':
				case '"':
				case ',':
				case '.':
				case '<':
				case '>':
				case '/':
				case '?':
				case '`':
				case '~':
					::cout<<(char)Byte;
					break;
				default:
					::cout<<".";
				}
			}
		}
		::cout<<"\n";
	}
}
eSANNETPROTOCOL San::Network::gloTranslateIPv4Protocol(unsigned char Protocol)
{
	eSANNETPROTOCOL IPv4Protocol;
	switch(Protocol)
	{
	case 1:
		IPv4Protocol=SNP_ICMP;
		break;
	case 2:
		IPv4Protocol=SNP_IGMP;
		break;
	case 4:
		IPv4Protocol=SNP_IPv4;
		break;
	case 6:
		IPv4Protocol=SNP_TCP;
		break;
	case 17:
		IPv4Protocol=SNP_UDP;
		break;
	case 88:
		IPv4Protocol=SNP_IGRP;
		break;
	case 89:
		IPv4Protocol=SNP_OSPF;
		break;
	default:
		IPv4Protocol=SNP_UNKNOWN;
		break;
	}
	return IPv4Protocol;
}