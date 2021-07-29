#include "nmea/nmea_zda.h"

//Тестовая утилита
bool TestPacket(const char* chPacketData)
{

    nmea::CNmeaPacket packet(chPacketData);

    std::string sPacketCheck = packet.GetString();

    nmea::CNmeaMsgZDA packetZda;
    packetZda.Serialize(packet,true);
    packet.Clear();
    packetZda.Serialize(packet,false);

    bool bRes = true;
    if(packet.GetString() != sPacketCheck)
    {
        bRes = false;
        std::cout << "CHK>" << sPacketCheck << std::endl;
        std::cout << "RES>" << packet.GetString() << std::endl;
    }

    return bRes;
}

int main(int argc, char *argv[])
{

    std::cout << TestPacket("$GLZDA,12322254,24,03,2021,00,00") << std::endl;

    return 0;
}
