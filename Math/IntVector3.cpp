//=================================================================================
// IntIntVector3.cpp
// Author: Tyler George
// Date  : March 31, 2015
//=================================================================================

#include "Engine/Math/IntVector3.hpp"

///---------------------------------------------------------------------------------
/// Static Variable Initialization
///---------------------------------------------------------------------------------
IntVector3 IntVector3::ZERO = IntVector3( 0, 0, 0 );

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
float IntVector3::CalcLength() const
{
	float length = sqrtf( (float) CalcLengthSquared() );

	return length;
}

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
float CalcDistance( const IntVector3& positionA, const IntVector3& positionB )
{
	float distSquared = (float) CalcDistanceSquared( positionA, positionB );
	float dist = sqrtf( distSquared );
	return dist;
}

