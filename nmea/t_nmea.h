/******************************************************************************
File     : t_nmea.h
Author   : sdy
Date     : 31.01.2019
Comments : nmea messages testing
******************************************************************************/
#pragma once

#include <iostream>
#include <stdio.h>
#include "nmea/nmea_packet.h"
#include "nmea/nmea_vbw.h"
#include "nmea/nmea_vtg.h"
#include "nmea/nmea_xdr.h"
#include "nmea/nmea_hpr.h"

void StringToMessage( nmea::CNmeaMsgBase& msg, const char* pPacketString )
{
	nmea::CNmeaPacket packet;
	packet << pPacketString;
	msg << packet;

	std::string sPacket;
	packet >> sPacket;
	std::cout << "packet << pPacketString; packet >> sPacket; CheckNumRes: " << packet.GetCheckSumRes() << std::endl;
    std::cout << sPacket << std::endl;
}

void MessageToString( nmea::CNmeaMsgBase& msg, std::string& sPacketString )
{
    nmea::CNmeaPacket packetTmp;
    msg >> packetTmp;
	packetTmp >> sPacketString;
	
    std::cout << "msg << packet; msg >> packetTmp; packetTmp >> sPacketString;" << std::endl;
    std::cout << sPacketString << std::endl;
    std::cout << std::endl;
}

void TestMsg( nmea::CNmeaMsgBase& msg, const char* pPacketString )
{
	std::cout << "Initial string:" << std::endl;
    std::cout << pPacketString << std::endl;

	StringToMessage( msg, pPacketString );

	std::string sPacketString;
	MessageToString( msg, sPacketString );
}

void TestAll()
{
	nmea::CNmeaMsgVbw msgVbw;
	TestMsg( msgVbw,	"$VMVBW,1.50,,A,,,,,,,*03\r\n" );
	nmea::CNmeaMsgVtg msgVtg;
	TestMsg( msgVtg,	"$INVTG,1.2,T,2.3,M,4.5,N,6.7,K,A*31\r\n" );
	TestMsg( msgVtg,	"$INVTG,1.2,,2.3,,4.5,N,6.7,,A*31\r\n" );
    nmea::CNmeaPacket packet;
	msgVtg >> packet;
	std::string sRes; 
	sRes = packet.GetSentenceId();
	sRes = packet.GetTalkerId();
	bool bRes = packet.IsQuerySentence();
	std::cout << "IsQuerySentence: " << bRes << std::endl;
	
	nmea::CNmeaMsgXdr msgXdr;
	TestMsg( msgXdr,	"$PIXDR,8.8,358,8.7,358,8.6,359,8.5,359,1029.2,-7.9,52,,4756*6B\r\n" );
	nmea::CNmeaMsgHpr msgHpr;
	TestMsg( msgHpr,	"$INHPR,017.73,039.73,154.21,002.63,-10.56,012.78,-02.44,-02.44,012.34,001.18,001.18,-22.43,005.12,012.59,043.50,057.9994,0056.0029*46\r\n" );
}

