//=================================================================================
// IntVector2.hpp
// Author: Tyler George
// Date  : February 9, 2015
//=================================================================================

#pragma once
#ifndef __included_IntVector2__
#define __included_IntVector2__

#include <vector>
#include "Engine/Common/EngineCommon.hpp"

class IntVector2 
{
public:
	///---------------------------------------------------------------------------------
	/// Public member variables
	///---------------------------------------------------------------------------------
	int x;
	int y;

	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
	~IntVector2() {};
	IntVector2() {};
	IntVector2( const IntVector2& copy );
	explicit IntVector2( int x, int y ) : x( x ), y( y ) {}

	///---------------------------------------------------------------------------------
	/// Operators
	///---------------------------------------------------------------------------------
	const IntVector2 operator-( const IntVector2& intVectorToSubtract ) const;
	const IntVector2 operator+( const IntVector2& intVectorToAdd ) const;
	bool operator==( const IntVector2& vectorToEqual ) const;
	bool operator!=( const IntVector2& vectorToNotEqual ) const;
	bool operator<( const IntVector2& vectorToCompare ) const;
	const IntVector2 operator/( const IntVector2& intVectorToDivideBy ) const;
	const IntVector2 operator/( int divisor ) const;

	///---------------------------------------------------------------------------------
	/// Friend functions
	///---------------------------------------------------------------------------------
	friend int CalcDistanceSquared( const IntVector2& positionA, const IntVector2& positionB );
	friend float CalcDistance( const IntVector2& positionA, const IntVector2& positionB );

    ///---------------------------------------------------------------------------------
    /// Static Variables
    ///---------------------------------------------------------------------------------
    static const IntVector2 ZERO;

};

typedef std::vector< IntVector2 > IntVector2s;

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline IntVector2::IntVector2( const IntVector2& copy )
{
	x = copy.x;
	y = copy.y;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline const IntVector2 IntVector2::operator -( const IntVector2& intVectorToSubtract ) const
{
	IntVector2 vectorToReturn;
	vectorToReturn.x = x - intVectorToSubtract.x;
	vectorToReturn.y = y - intVectorToSubtract.y;

	return vectorToReturn;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline const IntVector2 IntVector2::operator +( const IntVector2& intVectorToAdd ) const
{
	IntVector2 vectorToReturn;
	vectorToReturn.x = x + intVectorToAdd.x;
	vectorToReturn.y = y + intVectorToAdd.y;

	return vectorToReturn;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline bool IntVector2::operator ==( const IntVector2& vectorToEqual ) const 
{
	if( x == vectorToEqual.x && y == vectorToEqual.y )
		return true;
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline bool IntVector2::operator !=( const IntVector2& vectorToNotEqual ) const 
{
	if( x != vectorToNotEqual.x || y != vectorToNotEqual.y )
		return true;
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline bool IntVector2::operator <( const IntVector2& vectorToCompare ) const
{
	if( y < vectorToCompare.y )
		return true;
	else if( y > vectorToCompare.y )
		return false;
	else 
		return ( x < vectorToCompare.x );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline const IntVector2 IntVector2::operator /( const IntVector2& intVectorToDivideBy ) const
{
	return IntVector2( x / intVectorToDivideBy.x, y / intVectorToDivideBy.y );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline const IntVector2 IntVector2::operator /( int divisor ) const
{
	return IntVector2( x / divisor, y / divisor );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline int CalcDistanceSquared( const IntVector2& positionA, const IntVector2& positionB )
{
	int xDist = positionB.x - positionA.x;
	int yDist = positionB.y - positionA.y;
	int distSquared = ( xDist * xDist ) + ( yDist * yDist );
	return distSquared;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline float CalcDistance( const IntVector2& positionA, const IntVector2& positionB )
{
	return sqrtf( (float) CalcDistanceSquared( positionA, positionB ) );
}

#endif