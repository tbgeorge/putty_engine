//=================================================================================
// IntRange.hpp
// Author: Tyler George
// Date  : September 1, 2015
//=================================================================================

#pragma once
#ifndef __included_IntRange__
#define __included_IntRange__

#include <vector>
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Common/EngineCommon.hpp"


class IntRange
{
public:
    ///---------------------------------------------------------------------------------
    /// Public member variables
    ///---------------------------------------------------------------------------------
    int m_min;
    int m_max;

    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    ~IntRange() {};
    IntRange() {};
    IntRange( const IntRange& copy );
    explicit IntRange( int min, int max ) : m_min( min ), m_max( max ) {}

    ///---------------------------------------------------------------------------------
    /// Operators
    ///---------------------------------------------------------------------------------
    bool operator==(const IntRange& rangeToEqual) const;
    bool operator!=(const IntRange& rangeToNotEqual) const;

    const IntRange operator+(const IntRange& rangeToAdd) const;
    void operator+=(const IntRange& rangeToAdd);


    ///---------------------------------------------------------------------------------
    /// Friend functions
    ///---------------------------------------------------------------------------------
    friend int GetRandomValueInIntRangeInclusive( const IntRange& range );
    friend int GetRandomValueInRangeNonInclusive( const IntRange& range );

    ///---------------------------------------------------------------------------------
    /// Static Variables
    ///---------------------------------------------------------------------------------
    static const IntRange ZERO;

};

typedef std::vector< IntRange > IntRanges;

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline IntRange::IntRange( const IntRange& copy )
{
    m_min = copy.m_min;
    m_max = copy.m_max;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline bool IntRange::operator ==(const IntRange& rangeToEqual) const
{
    if (m_min == rangeToEqual.m_min && m_max == rangeToEqual.m_max)
        return true;
    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline bool IntRange::operator!=(const IntRange& rangeToNotEqual) const
{
    if (m_min != rangeToNotEqual.m_min || m_max != rangeToNotEqual.m_max)
        return true;
    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline const IntRange IntRange::operator+(const IntRange& rangeToAdd) const
{
    return IntRange( m_min + rangeToAdd.m_min, m_max + rangeToAdd.m_max );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline void IntRange::operator+=(const IntRange& rangeToAdd)
{
    m_min += rangeToAdd.m_min;
    m_max += rangeToAdd.m_max;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline int GetRandomValueInIntRangeInclusive( const IntRange& range )
{
    return GetRandomIntInRange( range.m_min, range.m_max );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline int GetRandomValueInRangeNonInclusive( const IntRange& range )
{
    return GetRandomIntInRange( range.m_min + 1, range.m_max - 1 );
}


#endif