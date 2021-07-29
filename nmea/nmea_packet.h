/******************************************************************************
File     : nmea_packet.h
Author   : sdy
Date     : 09.12.2018
Comments : nmea packets processing
******************************************************************************/

#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <algorithm>
#include <map>

#ifdef WIN32
#if (_MSC_VER <= 1500)
#define snprintf _snprintf_s
#endif//(_MSC_VER <= 1500)
#endif//WIN32

namespace nmea
{

////////////////////////////////////////////////////////////////////

inline unsigned int GetCheckSum( const std::string& sPacket )
{
	unsigned int uSum = 0;
	for( size_t i=0; i<sPacket.length(); i++ )
	{
		if( sPacket[i] == '*' )
			break;
		uSum ^= sPacket[i];
		if( sPacket[i] == '$' )
			uSum = 0;
	}
	return uSum;
}

inline bool ConvertStringHex2Num( const std::string& sCheckSum, unsigned int& uCheckSum )
{
	bool bRes = false;
	if( sCheckSum.length() > 0 )
	{
		std::istringstream iss( "0x" + sCheckSum );
		iss >> std::hex >> uCheckSum;
		bRes = true;
	}
	return bRes;
}

inline std::string ConvertNum2StringHex( unsigned int uNum )
{
	std::ostringstream oss; 
	oss << std::hex << std::setfill('0') << std::setw(2) << std::uppercase << uNum;
	return oss.str();
}

inline bool DivideByAsterisk( std::string& s, std::string& sValue, std::string& sCheckSum )
{
	bool bIsAsteriskFound = false;
	size_t szPos = s.find('*');
	if( szPos != std::string::npos )
	{
		bIsAsteriskFound = true;
		sValue = s.substr(0, szPos);
		sCheckSum = s.substr( szPos + 1 );
	}
	return bIsAsteriskFound;
}

inline bool SplitString( std::vector<std::string>& vSplitted, const char* pPacketToSplit, std::string& sCheckSum )
{
    vSplitted.clear();

    std::stringstream ss(pPacketToSplit);
    std::string sItem;
    
    while (std::getline(ss, sItem, ','))
    {
		std::string sValue;
		bool bDivided = DivideByAsterisk( sItem, sValue, sCheckSum );
		if( bDivided )
		{
			vSplitted.push_back( sValue );
			break;
		}
		else
		{
			vSplitted.push_back( sItem );
		}
    }

    return vSplitted.size() > 0;
}

inline std::string GetString( const std::vector<std::string>& vSplitted )
{
    std::stringstream ss;

    for(std::size_t i=0; i<vSplitted.size(); i++)
    {
        ss << vSplitted[i];
        if( (i+1) < vSplitted.size() )
            ss << ',';
    }
    
    return ss.str();
}

//////////////////////////////////////////////////////////////////
// 
class CNmeaPacket
{
protected:
    std::vector<std::string>	m_vFields;
private:
    std::string		m_sCheckSum;
    bool			m_bCheckSumRes;

public:
    CNmeaPacket()
    {
        m_bCheckSumRes = true;
    }
    CNmeaPacket( const std::string& sPacket )
    {
        m_bCheckSumRes = true;
        Split( sPacket );
    }
    virtual ~CNmeaPacket()
    {
    }

public:
    std::string GetHeader() const;
    virtual std::string GetSentenceId() const;
    virtual std::string GetTalkerId() const;
    virtual bool IsQuerySentence() const;
    virtual void Clear();

    CNmeaPacket& operator<< ( const std::string& sPacket );
    const CNmeaPacket& operator>> ( std::string& sPacket ) const;
    bool HasField(std::size_t sizeIndex) const;
    bool SetField(std::size_t sizeIndex, const std::string& sValue);
    bool GetField(std::size_t sizeIndex, std::string& sValue) const;
    std::size_t GetSize() const;
    bool FromString( const std::string& sPacket );
    bool ToString( std::string& sPacket ) const;
    std::string GetString() const;
    template <class T_DATA_TYPE>
    void Serialize( std::size_t sizeIndex, const char* pFormat, T_DATA_TYPE& val, bool bFromString, bool bIgnoreFieldValue = false );
    void Serialize( std::size_t sizeIndex, std::string& sVal, bool bFromString );
    bool Split( const std::string& sPacket );
    bool GetCheckSumRes() const;
protected:
};

inline std::string CNmeaPacket::GetHeader() const
{
	std::string sRes;
	if( m_vFields.size() > 0 )
		sRes = m_vFields[0];
	return sRes;
}

inline std::string CNmeaPacket::GetSentenceId() const
{
	std::string sRes;
	std::string sHeader = GetHeader();
	if( sHeader.length() == 6 )
	{
		sRes = sHeader.substr(3);
	}
	return sRes;
}

inline std::string CNmeaPacket::GetTalkerId() const
{
	std::string sRes;
	std::string sHeader = GetHeader();
	if( sHeader.length() == 6 )
	{
		sRes = sHeader.substr(1,2);
	}
	return sRes;
}

inline bool CNmeaPacket::IsQuerySentence() const
{
	bool bRes = false;
	std::string sHeader = GetHeader();
	if( sHeader.length() == 6 )
	{
		bRes = ('Q' == sHeader[5]);
	}
	return bRes;
}

inline bool CNmeaPacket::GetCheckSumRes() const
{
	return m_bCheckSumRes;
}

inline CNmeaPacket& CNmeaPacket::operator<< ( const std::string& sPacket )
{
	FromString( sPacket );
	return *this;
}

inline const CNmeaPacket& CNmeaPacket::operator>> ( std::string& sPacket ) const
{
	ToString( sPacket );
	return *this;
}

inline void CNmeaPacket::Clear()
{
	m_vFields.clear();
	m_sCheckSum = "";
	m_bCheckSumRes = true;
}

inline std::string CNmeaPacket::GetString() const
{
	std::string sRes;
	ToString(sRes);
	return sRes;
}

inline void CNmeaPacket::Serialize( std::size_t sizeIndex, std::string& sVal, bool bFromString )
{
    if( bFromString )
        GetField( sizeIndex, sVal );
    else
        SetField( sizeIndex, sVal );
}

template <class T_DATA_TYPE>
inline void CNmeaPacket::Serialize( std::size_t sizeIndex, const char* pFormat, T_DATA_TYPE& val, bool bFromString, bool bIgnoreFieldValue )
{
    if( bFromString )
    {
        if( sizeIndex < m_vFields.size() )
        {
            std::stringstream ss;
            ss << m_vFields[sizeIndex];
            ss >> val;
        }
    }
    else
    {
        std::string sTmp;
        if( !bIgnoreFieldValue )
		{
            char szTmp[80] = "";
            snprintf( szTmp, sizeof(szTmp), pFormat, val );
            sTmp = szTmp;
            std::replace( sTmp.begin(), sTmp.end(), ',', '.' );
        }
        SetField( sizeIndex, sTmp );
    }
}

inline std::size_t CNmeaPacket::GetSize() const
{
    return m_vFields.size();
}

inline bool CNmeaPacket::HasField(std::size_t sizeIndex) const
{
    if(sizeIndex >= m_vFields.size() )
        return false;
    return !m_vFields[sizeIndex].empty();
}

inline bool CNmeaPacket::SetField(std::size_t sizeIndex, const std::string& sValue)
{
    while(m_vFields.size() <= sizeIndex)
        m_vFields.push_back(std::string(""));
    m_vFields[sizeIndex] = sValue;
    return true;
}

inline bool CNmeaPacket::GetField(std::size_t sizeIndex, std::string& sValue) const
{
    bool bRes = false;
    if( sizeIndex < m_vFields.size() )
    {
        sValue = m_vFields[sizeIndex];
        bRes = true;
    }
    return bRes;
}

inline bool CNmeaPacket::Split( const std::string& sPacket )
{
	bool bRes = nmea::SplitString( m_vFields, sPacket.c_str(), m_sCheckSum );

    if( !m_sCheckSum.empty() )
	{
		unsigned int uCheckSum = 0;
		if( nmea::ConvertStringHex2Num( m_sCheckSum, uCheckSum ) )
		{
			unsigned int uCheckSumCalculated = nmea::GetCheckSum( sPacket );
			m_bCheckSumRes = ( uCheckSum == uCheckSumCalculated );
		}
	}
    return bRes;
}

inline bool CNmeaPacket::FromString( const std::string& sPacket )
{
	Clear();
	bool bRes = Split( sPacket );
    return bRes;
}

inline bool CNmeaPacket::ToString( std::string& sPacket ) const
{
    bool bRes = true;
	sPacket = nmea::GetString(m_vFields);
	unsigned uCheckSum = nmea::GetCheckSum( sPacket );
	const_cast<CNmeaPacket*>(this)->m_sCheckSum = nmea::ConvertNum2StringHex( uCheckSum );
	if( m_sCheckSum.length() )
	{
		sPacket += "*" + m_sCheckSum;
	}
    if(sPacket.empty())
        bRes = false;
    return bRes;
}

////////////////////////////////////////////////////////////////////
// CNmeaMsgBase
class CNmeaMsgBase
{
public:
    virtual ~CNmeaMsgBase(){}
	CNmeaMsgBase& operator<< ( CNmeaPacket& packet );
	const CNmeaMsgBase& operator>> ( CNmeaPacket& packet ) const;
protected:
	virtual void Clear() = 0;
	virtual void Serialize( CNmeaPacket& packet, bool bFromString ) = 0;
};

inline CNmeaMsgBase& CNmeaMsgBase::operator<< ( CNmeaPacket& packet )
{
	Clear();
    Serialize( *const_cast<CNmeaPacket*>(&packet), true );
	return *this;
}

inline const CNmeaMsgBase& CNmeaMsgBase::operator>> ( CNmeaPacket& packet ) const
{
    const_cast<CNmeaMsgBase*>(this)->Serialize( packet, false );
	return *this;
}

}//namespace nmea
