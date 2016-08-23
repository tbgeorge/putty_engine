//=================================================================================
// TypedKeyFrame.hpp
// Author: Tyler George
// Date  : April 13, 2016
//=================================================================================

#pragma once

#ifndef __included_TypedKeyFrame__
#define __included_TypedKeyFrame__

////---------------------------------------------------------------------------------
///
// Includes
///
////---------------------------------------------------------------------------------

////---------------------------------------------------------------------------------
///
// struct TypedKeyFrame
///
////---------------------------------------------------------------------------------
template< typename T >
struct TypedKeyFrame
{
    TypedKeyFrame( T kV, float pV )
        : m_keyValue( kV ), m_parametricValue( pV ) {}

    T m_keyValue;
    float m_parametricValue;
};

#endif