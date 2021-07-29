/******************************************************************************
 * File     : nmea_zda.h
 * Author   : spa
 * Date     : 10.07.2019
 * Comments : сообщение системы единого времени (стандарт IEC 61162-1:2000)
 * ******************************************************************************/

#ifndef __NMEA_ZDA_H
#define __NMEA_ZDA_H

#include "nmea_packet.h"

namespace nmea
{

/// @brief перечисление с полями сообщения
enum ENU_ZDA
{
    ENU_ZDA_01_Header                       = 0,    /// < заголовок сообщения = $(GP|GL|GN)ZDA
    ENU_ZDA_02_GreenwichTime,                       /// < время по гринвичу в формате hhmmss.ss
    ENU_ZDA_03_GreenwichDayOfMonth,                 /// < день месяца по гринвичу
    ENU_ZDA_04_GreenwichMonth,                      /// < месяц по гринвичу
    ENU_ZDA_05_GreenwichYear,                       /// < год по гринвичу
    ENU_ZDA_06_LocalTimeHours,                      /// < пояс местного времени, часы (величина, которую надо прибавить к местному времени для получения Гринвичского)
    ENU_ZDA_07_LocalTimeMinutes,                    /// < пояс местного времени, минуты (величина, которую надо прибавить к местному времени для получения Гринвичского)
};

const char c_szSentenceIdZDA[] = "ZDA";

/// @brief класс для работы с сообщениями типа VHW
class CNmeaMsgZDA : public CNmeaMsgBase
{
public:
    // конструктор
    CNmeaMsgZDA() { Clear(); }
    // методы
    void Clear();
    void Serialize(CNmeaPacket &packet, bool bFromString);

    /// @brief метод, возвращающий предложение-идентификатор типа сообщения
    static const char* GetSentenceId() { return c_szSentenceIdZDA; }

    /// @brief метод, устанавливающий идентификатор источника информации GPS
    inline void SetGpsHeader() { m_strHeader = "$GPZDA"; }
    /// @brief метод, устанавливающий идентификатор источника информации ГЛОНАСС
    inline void SetGlonassHeader() { m_strHeader = "$GLZDA"; }
    /// @brief метод, устанавливающий идентификатор источника информации GPS + ГЛОНАСС
    inline void SetGpsGlonassHeader() { m_strHeader = "$GNZDA"; }
    // поля данных
    std::string         m_strHeader;
    std::string         m_strGreenwichTime;
    unsigned short int  m_uGreenwichDayOfMonth;
    unsigned short int  m_uGreenwichMonth;
    unsigned short int  m_uGreenwichYear;
    short int           m_iLocalTimeHours;
    unsigned short int  m_uLocalTimeMinutes;
};

// сброс значений полей класса CNmeaMsgZDA
inline void CNmeaMsgZDA::Clear()
{
    SetGpsHeader();
    m_strGreenwichTime = "000000.00";
    m_uGreenwichDayOfMonth = 1;
    m_uGreenwichMonth = 1;
    m_uGreenwichYear = 1970;
    m_iLocalTimeHours = 0;
    m_uLocalTimeMinutes = 0;
}

// сериализация
inline void CNmeaMsgZDA::Serialize(CNmeaPacket &packet, bool bFromString)
{
    packet.Serialize(ENU_ZDA_01_Header, m_strHeader, bFromString);
    packet.Serialize(ENU_ZDA_02_GreenwichTime, m_strGreenwichTime, bFromString);
    packet.Serialize(ENU_ZDA_03_GreenwichDayOfMonth, "%02u", m_uGreenwichDayOfMonth, bFromString, false);
    packet.Serialize(ENU_ZDA_04_GreenwichMonth, "%02u", m_uGreenwichMonth, bFromString, false);
    packet.Serialize(ENU_ZDA_05_GreenwichYear, "%04u", m_uGreenwichYear, bFromString, false);
    packet.Serialize(ENU_ZDA_06_LocalTimeHours, "%02i", m_iLocalTimeHours, bFromString, false);
    packet.Serialize(ENU_ZDA_07_LocalTimeMinutes, "%02u", m_uLocalTimeMinutes, bFromString, false);
}

} // nmea

#endif // __NMEA_ZDA_H
