//=================================================================================
// Vector2.cpp
// Author: Tyler George
// Date  : January 15, 2015
//=================================================================================

#include "Engine/Math/Vector2.hpp"

///---------------------------------------------------------------------------------
/// Static variable initialization
///---------------------------------------------------------------------------------
const Vector2 Vector2::ZERO( 0.0f, 0.0f );

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
float Vector2::CalcLength() const
{
	float length = sqrtf( CalcLengthSquared() );
	return length;
}

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
float Vector2::CalcHeadingDegrees() const
{
	float headingInDegrees = ConvertRadiansToDegrees( CalcHeadingRadians() );
	return headingInDegrees;
}

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
float Vector2::CalcHeadingRadians() const
{
	float headingInRadians = atan2( y, x );
	return headingInRadians;
}

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
void Vector2::RotateDegrees( float degrees )
{
	float length = CalcLength();
	float headingInDegrees = CalcHeadingDegrees();

	headingInDegrees += degrees;
	float headingInRadians = ConvertDegreesToRadians( headingInDegrees );

	x = length * cos( headingInRadians );
	y = length * sin( headingInRadians );
}

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
void Vector2::RotateRadians( float radians )
{
	float length = CalcLength();
	float headingInRadians = CalcHeadingRadians();

	headingInRadians += radians;

	x = length * cos( headingInRadians );
	y = length * sin( headingInRadians );
}

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
float Vector2::Normalize() 
{
	if( CalcLengthSquared() == 0.0f )
		return 0.0f;

	float length = CalcLength();
	if( length == 0.0f ) 
		return 0.0f;

	float inverseLength = 1.0f / length;
	x *= inverseLength;
	y *= inverseLength;

	return length;
}


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
const Vector2 Vector2::Normalized() const
{
    if (CalcLengthSquared() == 0.0f)
        return Vector2::ZERO;

    float length = CalcLength();
    if (length == 0.0f)
        return Vector2::ZERO;

    Vector2 returnVec;

    float inverseLength = 1.0f / length;
    returnVec.x = x * inverseLength;
    returnVec.y = y * inverseLength;

    return returnVec;
}

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
float Vector2::SetLength( float newLength )
{

	float oldLength = CalcLength();
	Normalize();

	x *= newLength;
	y *= newLength;
	return oldLength; 
}

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
void Vector2::SetUnitLengthAndHeadingDegrees( float headingDegrees )
{
	float headingRadians = ConvertDegreesToRadians( headingDegrees );
	x = cos( headingRadians );
	y = sin( headingRadians );
}

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
void Vector2::SetUnitLengthAndHeadingRadians( float headingRadians )
{
	x = cos( headingRadians );
	y = sin( headingRadians );
}

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
void Vector2::SetLengthAndHeadingDegrees( float newLength, float headingDegrees )
{
	float length = newLength;
	float headingRadians = ConvertDegreesToRadians( headingDegrees );
	x = length * cos( headingRadians );
	y = length * sin( headingRadians );
}

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
void Vector2::SetLengthAndHeadingRadians( float newLength, float headingRadians )
{
	float length = newLength;
	x = length * cos( headingRadians );
	y = length * sin( headingRadians );
}

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
float CalcDistance( const Vector2& positionA, const Vector2& positionB )
{
	float distSquared = CalcDistanceSquared( positionA, positionB );
	float dist = sqrtf( distSquared );
	return dist;
}

