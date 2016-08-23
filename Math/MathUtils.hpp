//=================================================================================
// MathUtils.hpp
// Author: Tyler George
// Date  : January 16, 2015
//=================================================================================

#pragma once

#ifndef __included_MathUtils__
#define __included_MathUtils__

#include <Math.h>
#include <random>
#include <time.h>


///---------------------------------------------------------------------------------
/// Constant Math Variables
///---------------------------------------------------------------------------------
#ifndef PI
const float PI = 3.1415926535897932384626433832795f;
#endif
const float TWO_PI = PI * 2.0f;
const float DEGREES_PER_RADIAN = 180.0f / PI;
const float RADIANS_PER_DEGREE = PI / 180.0f;
const float COMPASS_DIRECTION_NORTH = 90.0f;
const float COMPASS_DIRECTION_SOUTH = 270.0f;
const float COMPASS_DIRECTION_WEST = 180.0f;
const float COMPASS_DIRECTION_EAST = 0.0f;
const float COMPASS_DIRECTION_NORTHEAST = 45.0f;
const float COMPASS_DIRECTION_NORTHWEST = 135.0f;
const float COMPASS_DIRECTION_SOUTHEAST = 315.0f;
const float COMPASS_DIRECTION_SOUTHWEST = 225.0f;
const float INVERSE_RAND_MAX = 1.0f / (float) RAND_MAX;

///---------------------------------------------------------------------------------
/// Methods
///---------------------------------------------------------------------------------
template< typename primitiveType >
primitiveType ConvertRadiansToDegrees( primitiveType radians );

template< typename primitiveType >
primitiveType ConvertDegreesToRadians( primitiveType degrees );

float   DegreeZeroTo360( float degrees );
int		GetRandomIntLessThan( int maxValueNotInclusive );
int		GetRandomIntInRange( int minValueInclusive, int maxValueInclusive );
float	GetRandomFloatZeroToOne();
float	GetRandomFloatInRange( float minimumInclusive, float maximumInclusive );
int		TripleRandomInt();

float	CalcShortestAngularDisplacement( float startAngle, float destinationAngle );
bool	IsPowerOfTwo( int x );
float	RangeMap( float inValue, float inStart, float inEnd, float outStart, float outEnd );
float	Clamp( float inValue, float min, float max );
float	SmoothStart( float normalizedValue );
float	SmoothStop( float normalizedValue );
float	SmoothStep( float normalizedValue );
float	AsymptoticAdd( float normalizedValueA, float normalizedValueB );
float	Interpolate( float start, float end, float fractionFromStartToEnd );
double  Interpolate( double start, double end, double fractionFromStartToEnd );
int		Interpolate( int start, int end, int fractionFromStartToEnd );

template< typename T >
T Interpolate( const T& start, const T& end, float fractionFromStartToEnd );

bool	AreFloatsEqual( float lhs, float rhs, float tolerance = 0.001 );

#endif

///---------------------------------------------------------------------------------
/// inline methods
///---------------------------------------------------------------------------------
///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline primitiveType ConvertRadiansToDegrees( primitiveType radians ) 
{
	primitiveType degrees = radians * (primitiveType) DEGREES_PER_RADIAN;
	return degrees;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline primitiveType ConvertDegreesToRadians( primitiveType degrees ) 
{
	primitiveType radians = degrees * (primitiveType) RADIANS_PER_DEGREE;
	return radians;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline float AsymptoticAdd( float normalizedValueA, float normalizedValueB )
{
	return 1.0f - ( ( 1.0f - normalizedValueA ) * ( 1.0f - normalizedValueB ) );
}

///---------------------------------------------------------------------------------
/// slow
///---------------------------------------------------------------------------------
inline float RangeMap( float inValue, float inStart, float inEnd, float outStart, float outEnd )
{
    if (inEnd - inStart == 0.0f)
        return 0.0f;

    float ratio = (inValue - inStart) / (inEnd - inStart);
    float outValue = ratio * (outEnd - outStart) + outStart;
    return outValue;
}


///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline bool IsPowerOfTwo( int x )
{
    if (x != 0 && x & (x - 1))
        return true;
    return false;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline float Clamp( float inValue, float min, float max )
{
    if (inValue < min)
        return min;
    else if (inValue > max)
        return max;
    else
        return inValue;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline float SmoothStart( float normalizedValue )
{
    return (normalizedValue * normalizedValue);
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline float SmoothStop( float normalizedValue )
{
    return 1 - ((1 - normalizedValue) * (1 - normalizedValue));
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline float SmoothStep( float normalizedValue )
{
    //3t^2 - 2t^3
    float normalizedValueSquared = normalizedValue * normalizedValue;
    float firstPart = 3 * normalizedValueSquared;
    float secondPart = 2 * (normalizedValueSquared * normalizedValue);
    return firstPart - secondPart;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline float Interpolate( float start, float end, float fractionFromStartToEnd )
{
    float difference = end - start;
    float fractionOfWhole = difference * fractionFromStartToEnd;
    return start + fractionOfWhole;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline double Interpolate( double start, double end, double fractionFromStartToEnd )
{
    double difference = end - start;
    double fractionOfWhole = difference * fractionFromStartToEnd;
    return start + fractionOfWhole;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline int Interpolate( int start, int end, int fractionFromStartToEnd )
{
    int difference = end - start;
    int fractionOfWhole = difference * fractionFromStartToEnd;
    return start + fractionOfWhole;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename T >
inline T Interpolate ( const T& start, const T& end, float fractionFromStartToEnd )
{
    return (( start * (1 - fractionFromStartToEnd)) + (end * fractionFromStartToEnd));
//     return (start + ((end - start) * fractionFromStartToEnd));
}