//=================================================================================
// TypedAnimatedKeyFrameSequence.hpp
// Author: Tyler George
// Date  : April 26, 2016
//=================================================================================

#pragma once

#ifndef __included_TypedAnimatedKeyFrameSequence__
#define __included_TypedAnimatedKeyFrameSequence__

////---------------------------------------------------------------------------------
///
// Includes
///
////---------------------------------------------------------------------------------
#include "Engine/Utilities/TypedKeyFrame.hpp"
#include <vector>

////---------------------------------------------------------------------------------
///
// Enums
///
////---------------------------------------------------------------------------------
enum AnimationType
{
    KFS_Looped,
    KFS_Once,
    KFS_PingPong,
    KFS_NumTypes
};

////---------------------------------------------------------------------------------
///
// class TypedAnimatedKeyFrameSequence
///
////---------------------------------------------------------------------------------
template< typename T >
class TypedAnimatedKeyFrameSequence : public TypedKeyFrameSequence
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    TypedAnimatedKeyFrameSequence( const std::vector< TypedKeyFrame< T > >& kfs );

    ///---------------------------------------------------------------------------------
    /// Accessors/Queries
    ///---------------------------------------------------------------------------------
    T GetCurrentValue();
    bool IsFinished();
    void Reset();
    void DeactivateAndReset();


    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    void Activate();
    void Deactivate();

    ///---------------------------------------------------------------------------------
    /// Update
    ///---------------------------------------------------------------------------------
    void Update();

private:
    ///---------------------------------------------------------------------------------
    /// Private member variables
    ///---------------------------------------------------------------------------------
    TypedKeyFrameSequence<T> m_keyFrames;
    float m_currentParametric;
    float m_duration;
    AnimationType m_type;
    bool m_isActive;
    bool m_isDone;


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
TypedAnimatedKeyFrameSequence<T>::TypedAnimatedKeyFrameSequence()
{

}
#endif