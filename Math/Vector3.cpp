//=================================================================================
// Vector3.cpp
// Author: Tyler George
// Date  : February 18, 2015
//=================================================================================

#include "Engine/Math/Vector3.hpp"

///---------------------------------------------------------------------------------
/// Static variable initialization
///---------------------------------------------------------------------------------
const Vector3 Vector3::ZERO( 0.0f, 0.0f, 0.0f );

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
float Vector3::CalcLength() const
{
	float length = sqrtf( CalcLengthSquared() );

	return length;
}



///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
float Vector3::Normalize() 
{
	if( CalcLengthSquared() == 0.0f )
		return 0.0f;

	float length = CalcLength();
	if( length == 0.0f ) 
		return 0.0f;

	float inverseLength = 1.0f / length;
	x *= inverseLength;
	y *= inverseLength;
	z *= inverseLength;

	return length;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
const Vector3 Vector3::Normalized() const
{
    if (CalcLengthSquared() == 0.0f)
        return Vector3::ZERO;

    float length = CalcLength();
    if (length == 0.0f)
        return Vector3::ZERO;

    Vector3 returnVec;

    float inverseLength = 1.0f / length;
    returnVec.x = x * inverseLength;
    returnVec.y = y * inverseLength;
    returnVec.z = z * inverseLength;

    return returnVec;
}


///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
float Vector3::SetLength( float newLength )
{

	float oldLength = CalcLength();
	Normalize();

	x *= newLength;
	y *= newLength;
	z *= newLength;
	return oldLength; 
}

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
float CalcDistance( const Vector3& positionA, const Vector3& positionB )
{
	float distSquared = CalcDistanceSquared( positionA, positionB );
	float dist = sqrtf( distSquared );
	return dist;
}

