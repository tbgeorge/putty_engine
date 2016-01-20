//=================================================================================
// FltRange.hpp
// Author: Tyler George
// Date  : September 1, 2015
//=================================================================================

#pragma once
#ifndef __included_FltRange__
#define __included_FltRange__

#include <vector>
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"

class FltRange
{
public:
    ///---------------------------------------------------------------------------------
    /// Public member variables
    ///---------------------------------------------------------------------------------
    float m_min;
    float m_max;

    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    ~FltRange() {};
    FltRange() {};
    FltRange( const FltRange& copy );
    explicit FltRange( float min, float max ) : m_min( min ), m_max( max ) {};

    ///---------------------------------------------------------------------------------
    /// Operators
    ///---------------------------------------------------------------------------------
    bool operator==(const FltRange& rangeToEqual) const;
    bool operator!=(const FltRange& rangeToNotEqual) const;

    ///---------------------------------------------------------------------------------
    /// Accessors/Queries
    ///---------------------------------------------------------------------------------
    bool IsValueInRangeInclusive( float value ) const;
    bool IsValueInRangeNonInclusive( float value ) const;

    ///---------------------------------------------------------------------------------
    /// Friend functions
    ///---------------------------------------------------------------------------------
    friend float GetRandomValueInFltRangenclusive( const FltRange& range );
    friend float GetRandomValueInFltRangeNonInclusive( const FltRange& range );

    ///---------------------------------------------------------------------------------
    /// Static Variables
    ///---------------------------------------------------------------------------------
    static const FltRange ZERO;

};

typedef std::vector< FltRange > FltRanges;

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline FltRange::FltRange( const FltRange& copy )
{
    m_min = copy.m_min;
    m_max = copy.m_max;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline bool FltRange::operator ==(const FltRange& rangeToEqual) const
{
    if (m_min == rangeToEqual.m_min && m_max == rangeToEqual.m_max)
        return true;
    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline bool FltRange::operator !=(const FltRange& rangeToNotEqual) const
{
    if (m_min != rangeToNotEqual.m_min || m_max != rangeToNotEqual.m_max)
        return true;
    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline bool FltRange::IsValueInRangeInclusive( float value ) const
{
    if (value >= m_min && value <= m_max)
        return true;
    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline bool FltRange::IsValueInRangeNonInclusive( float value ) const
{
    if (value > m_min && value < m_max)
        return true;
    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline float GetRandomValueInFltRangenclusive( const FltRange& range )
{
    return GetRandomFloatInRange( range.m_min, range.m_max );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline float GetRandomValueInFltRangeNonInclusive( const FltRange& range )
{
    return GetRandomFloatInRange( range.m_min + 1, range.m_max - 1 );
}


#endif