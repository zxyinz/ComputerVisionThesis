#include"iostream"
#include"../RenderSystem.h"
#include"../Core/SanTypes.h"
#pragma once
using namespace std;
namespace San
{
	namespace Network
	{
#ifndef __STDSANNETWORK_H__
#define __STDSANNETWORK_H__

#ifdef _WINDOWS
#include"winsock2.h"
//#pragma comment(lib,"ws_32.lib")
#endif

#ifdef _UNIX
#include"stdio.h"
#include"stdlib.h"
#include"sys/types.h"
#include"sys/socket.h"
#include"unistd.h"
#include"linux/in.h"
		typedef int SOCKET;
#endif
		enum eSANNETFRAMETYPE
		{
			SNFT_802dot3	= 1,
		};

		enum eSANNETPROTOCOL
		{
			SNP_UNKNOWN	= 0,
			SNP_ICMP	= 1,
			SNP_IGMP	= 2,
			SNP_IPv4	= 4,
			SNP_TCP		= 6,
			SNP_UDP		= 17,
			SNP_IGRP	= 88,
			SNP_OSPF	= 89,
		};
		//extern const unsigned int	SNP_SIZE=7;
#define SNP_SIZE 8

		struct	stSANIPADDRv4
		{
			union
			{
				unsigned __int32		addr_32;
				unsigned __int8		addr_byte[4];
			};
		};
		typedef stSANIPADDRv4	SANIPADDRv4;
		typedef stSANIPADDRv4*	LPSANIPADDRv4;

		typedef SANIPADDRv4		SANIPADDR;
		typedef LPSANIPADDRv4	LPSANIPADDR;

#pragma pack(1)
		struct stSANNETFRAMEHEADER_ETHERNET
		{
			unsigned char	dest_mac_address[6];
			unsigned char	src_mac_address[6];
			unsigned short	type_length;
		};
		typedef stSANNETFRAMEHEADER_ETHERNET	SANETHERNETHEADER;
		typedef stSANNETFRAMEHEADER_ETHERNET*	LPSANETHERNETHEADER;

		struct stSANNETPACKAGEHEADER_IPv4
		{
			//unsigned char	version:4;
			//unsigned char	header_length:4;
			unsigned char	version;//:4;
			unsigned char	type_of_service;
			unsigned short	total_length;
			unsigned short	identifier;
			/*union
			{
				unsigned short	flag:3;
				struct
				{
					char		nf:1;
					char		df:1;
					char		mf:1;
				};
			};*/
			unsigned short	fragment_offset;//:13;
			/*union
			{
				unsigned short	flag:3;
				struct
				{
					char		nf:1;
					char		df:1;
					char		mf:1;
				};
			};*/
			unsigned char	ttl;
			unsigned char	protocol;
			unsigned short	header_checksum;
			unsigned long	src_address;
			unsigned long	dest_address;
		};
		typedef stSANNETPACKAGEHEADER_IPv4	SANIPv4HEADER;
		typedef stSANNETPACKAGEHEADER_IPv4*	LPSANIPv4HEADER;

		struct stSANNETPCAKAGEHEADER_TCP
		{
			unsigned short	src_port;
			unsigned short	dest_port;
			unsigned long	sequence;
			unsigned long	acknowledge;
			unsigned char	header_length:4;
			unsigned char	reserved:4;
			unsigned char	flag;
			unsigned short	window_size;
			unsigned short	checksum;
			unsigned short	urgent_pointer;
			unsigned long	options;
		};
		typedef stSANNETPCAKAGEHEADER_TCP	SANTCPHEADER;
		typedef stSANNETPCAKAGEHEADER_TCP*	LPSANTCPHEADER;

		struct stSANNETPACKAGEHEADER_UDP
		{
			unsigned short	src_port;
			unsigned short	dest_port;
			unsigned short	udp_length;
			unsigned short	checksum;
		};
		typedef stSANNETPACKAGEHEADER_UDP	SANUDPHEADER;
		typedef stSANNETPACKAGEHEADER_UDP*	LPSANUDPHEADER;
#pragma pack()

		unsigned short	glo_ntohs(unsigned short NetData);
		unsigned long	glo_ntohl(unsigned long NetData);

		unsigned long	gloStrToIPv4(SStringA strAddress);
		SString			gloIPv4ToStr(unsigned long Address);

		bool			gloIsIPv4Address(SStringA strAddress);

		void	gloShowPackageInfo(UINT Length,const unsigned char* pData);

		eSANNETPROTOCOL	gloTranslateIPv4Protocol(unsigned char Protocol);
#endif
	}
}