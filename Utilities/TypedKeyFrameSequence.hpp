//=================================================================================
// TypedKeyFrameSequence.hpp
// Author: Tyler George
// Date  : April 13, 2016
//=================================================================================

#pragma once

#ifndef __included_TypedKeyFrameSequence__
#define __included_TypedKeyFrameSequence__

////---------------------------------------------------------------------------------
///
// Includes
///
////---------------------------------------------------------------------------------
#include "Engine/Utilities/TypedKeyFrame.hpp"
#include <vector>
#include "Engine/Math/MathUtils.hpp"

////---------------------------------------------------------------------------------
///
// class TypedKeyFrameSequence
///
////---------------------------------------------------------------------------------
template< typename T >
class TypedKeyFrameSequence
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    TypedKeyFrameSequence( const std::vector< TypedKeyFrame< T > >& kfs );

    ///---------------------------------------------------------------------------------
    /// Accessors/Queries
    ///---------------------------------------------------------------------------------
    T GetLoopedParametric( float parametric );
    T GetValueAtParametric( float parametric );
    
private:
    ///---------------------------------------------------------------------------------
    /// Private member variables
    ///---------------------------------------------------------------------------------
    std::vector< TypedKeyFrame< T > > m_keyFrames;
};

////===========================================================================================
///===========================================================================================
// Templated class functions
///===========================================================================================
////===========================================================================================

////---------------------------------------------------------------------------------
///
// Constructors/Destructors
///
////---------------------------------------------------------------------------------
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename T >
TypedKeyFrameSequence<T>::TypedKeyFrameSequence( const std::vector< TypedKeyFrame< T > >& kfs )
    : m_keyFrames( kfs )
{

}

////---------------------------------------------------------------------------------
///
// Accessors/Queries
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename T >
T TypedKeyFrameSequence<T>::GetLoopedParametric( float parametric )
{
    while (parametric > 1.0f)
        parametric -= 1.0f;

    while (parametric < 0.0f)
        parametric += 1.0f;

    return GetValueAtParametric( parametric );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename T >
T TypedKeyFrameSequence<T>::GetValueAtParametric( float parametric )
{
    // if only one key frame return its key value regardless of parametric value
    if (m_keyFrames.size() == 1)
        return m_keyFrames[0].m_keyValue;


    for (int kfIndex = 0; kfIndex < m_keyFrames.size(); ++kfIndex)
    {
        float currKFparametricValue = m_keyFrames[kfIndex].m_parametricValue;
        T currKFvalue = m_keyFrames[kfIndex].m_keyValue;

        // if parametric given is less than first keyFrame parametric value
        if (parametric < currKFparametricValue && kfIndex == 0)
            return m_keyFrames[kfIndex].m_keyValue;

        // if there are two valid key frames to use
        if (kfIndex != m_keyFrames.size() - 1)
        {
            float nextKFparametricValue = m_keyFrames[kfIndex + 1].m_parametricValue;
            T nextKFValue = m_keyFrames[kfIndex + 1].m_keyValue;

            // found range
            if (parametric >= currKFparametricValue && parametric < nextKFparametricValue)
            {
                float adjustedTop = nextKFparametricValue - currKFparametricValue;
                float adjustedParametric = parametric - currKFparametricValue;

                float rangedParametric = RangeMap( adjustedParametric, 0.0f, adjustedTop, 0.0f, 1.0f );
                return Interpolate( currKFvalue, nextKFValue, rangedParametric );

            }
            else
                continue;
        }

        // if we are at the end of the key frames and still haven't found a value return the value of the last key frame
        else if (parametric >= currKFparametricValue)
            return m_keyFrames[kfIndex].m_keyValue;

    }
}

#endif
