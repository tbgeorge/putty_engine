//=================================================================================
// StateVectors.hpp
// Author: Tyler George
// Date  : June 12, 2015
//=================================================================================

#ifndef __included_StateVectors__
#define __included_StateVectors__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/Vector3.hpp"

struct StateVector3D
{
    Vector3 s1;
    Vector3 s2;

    StateVector3D() {}

    StateVector3D( const Vector3& s1, const Vector3& s2 )
        : s1( s1 ), s2( s2 ) {}

    StateVector3D operator*(const float& scale)
    {
        return StateVector3D( s1 * scale, s2 * scale );
    }

    StateVector3D operator+(const StateVector3D& toAdd) const
    {
        return StateVector3D( s1 + toAdd.s1, s2 + toAdd.s2 );
    }


};


#endif