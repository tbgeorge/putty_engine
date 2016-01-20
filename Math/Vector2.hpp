//=================================================================================
// Vector2.hpp
// Author: Tyler George
// Date  : January 15, 2015
//=================================================================================

#pragma once
#ifndef __included_Vector2__
#define __included_Vector2__

#include <vector>
#include "Engine/Math/MathUtils.hpp"

class Vector2
{
public:

	///---------------------------------------------------------------------------------
	/// Member Variables
	///---------------------------------------------------------------------------------
	float x;
	float y;


	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
	~Vector2() {}
		
	Vector2() {}
	Vector2( const Vector2& copy ) 
		: x( copy.x ), y( copy.y ) {}
	explicit Vector2( float initialX, float initialY ) 
		: x( initialX ), y( initialY ) {}


	///---------------------------------------------------------------------------------
	/// Accessors
	///---------------------------------------------------------------------------------
	void			GetXY( float& out_x, float& out_y ) const;
	const float*	GetAsFloatArray()					const;
	float*			GetAsFloatArray();
	float			CalcLength()						const;
	float			CalcLengthSquared()					const;
	float			CalcHeadingDegrees()				const;
	float			CalcHeadingRadians()				const;


	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
	void	SetXY( float newX, float newY );
	void	Rotate90Degrees();
	void	RotateNegative90Degrees();
	void	RotateDegrees( float degrees );
	void	RotateRadians( float radians );
	float	Normalize();
    const Vector2 Normalized() const;

	float	SetLength( float newLength );
	void	ScaleUniform( float scale );
	void	ScaleNonUniform( const Vector2& perAxisScaleFactors );
	void	InverseScaleNonUniform( const Vector2& perAxisDivisors );
	void	SetUnitLengthAndHeadingDegrees( float headingDegrees );
	void	SetUnitLengthAndHeadingRadians( float headingRadians );
	void	SetLengthAndHeadingDegrees( float newLength, float headingDegrees );
	void	SetLengthAndHeadingRadians( float newLength, float headingRadians );

	///---------------------------------------------------------------------------------
	/// Operators
	///---------------------------------------------------------------------------------
	bool			operator == ( const Vector2& vectorToEqual )		const;
	bool			operator != ( const Vector2& vectorToNotEqual )		const;
	const Vector2	operator + ( const Vector2& vectorToAdd )			const;
	const Vector2	operator - ( const Vector2& vectorToSubtract )		const;
	const Vector2	operator * ( float scale )							const;
	const Vector2	operator * ( const Vector2& perAxisScaleFactors )	const;
	const Vector2	operator / ( float inverseScale )					const;
	const Vector2	operator = ( const Vector2& vectorToAssign );
	void			operator *= ( float scale );
	void			operator *= ( const Vector2& perAxisScaleFactors );
	void			operator += ( const Vector2& vectorToAdd );
	void			operator -= ( const Vector2& vectorToSubtract );

	///---------------------------------------------------------------------------------
	/// Friend Functions
	///---------------------------------------------------------------------------------
	friend float			CalcDistance( const Vector2& positionA, const Vector2& positionB );
	friend float			CalcDistanceSquared( const Vector2& positionA, const Vector2& positionB );
	friend const Vector2	operator * ( float scale, const Vector2& vectorToScale );
	friend float			DotProduct( const Vector2& a, const Vector2& b );
	friend const Vector2	Interpolate( const Vector2& start, const Vector2& end, float fractionFromStartToEnd );
    friend bool             AreVectorsEqual( const Vector2& a, const Vector2& b, float tolerance );

    ///---------------------------------------------------------------------------------
    /// Static Variables
    ///---------------------------------------------------------------------------------
    static const Vector2 ZERO;
};

typedef std::vector<Vector2> Vector2s;

///---------------------------------------------------------------------------------
/// Inline function implementations
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Vector2::GetXY( float& out_x, float& out_y ) const
{
	out_x = x;
	out_y = y;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const float* Vector2::GetAsFloatArray() const
{
	return &x;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline float* Vector2::GetAsFloatArray() 
{
	return &x;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline float Vector2::CalcLengthSquared() const
{
	float lengthSquared = ( x * x ) + ( y * y );
	return lengthSquared;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Vector2::SetXY( float newX, float newY )
{
	x = newX;
	y = newY;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Vector2::Rotate90Degrees()
{
	float oldX = x;
	float oldY = y;
	x = -oldY;
	y = oldX;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Vector2::RotateNegative90Degrees()
{
	float oldX = x;
	float oldY = y;
	x = oldY;
	y = -oldX;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Vector2::ScaleUniform( float scale )
{
	x *= scale;
	y *= scale;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Vector2::ScaleNonUniform(const Vector2& perAxisScaleFactors ) 
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}

///---------------------------------------------------------------------------------
/// slow
///---------------------------------------------------------------------------------
inline void Vector2::InverseScaleNonUniform( const Vector2& perAxisScaleDivisors )
{
	x /= perAxisScaleDivisors.x;
	y /= perAxisScaleDivisors.y;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline bool Vector2::operator ==( const Vector2& vectorToEqual ) const
{
	if( x == vectorToEqual.x && y == vectorToEqual.y )
		return true;
	return false;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline bool Vector2::operator !=( const Vector2& vectorToNotEqual ) const 
{
	if( x != vectorToNotEqual.x || y != vectorToNotEqual.y )
		return true;
	return false;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector2 Vector2::operator =( const Vector2& vectorToAssign ) 
{
	x = vectorToAssign.x;
	y = vectorToAssign.y;
	return *this;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector2 Vector2::operator +( const Vector2& vectorToAdd ) const
{
	Vector2 newVector( x + vectorToAdd.x, y + vectorToAdd.y );
	return newVector;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector2 Vector2::operator -( const Vector2& vectorToSubtract ) const
{
	Vector2 newVector( x - vectorToSubtract.x, y - vectorToSubtract.y );
	return newVector;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector2 Vector2::operator *( float scale ) const
{
	Vector2 newVector( x * scale, y * scale );
	return newVector;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector2 Vector2::operator *( const Vector2& perAxisScaleFactors ) const
{
	Vector2 newVector( x * perAxisScaleFactors.x, y * perAxisScaleFactors.y );
	return newVector;
}

///---------------------------------------------------------------------------------
/// slow
///---------------------------------------------------------------------------------
inline const Vector2 Vector2::operator /( float inverseScale ) const
{

	if( inverseScale == 0.0f )
		return Vector2( 0.0f, 0.0f );

	float scale = 1.0f / inverseScale;
	Vector2 newVector( x * scale, y * scale );
	return newVector;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Vector2::operator *=( float scale ) 
{
	x *= scale;
	y *= scale;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Vector2::operator *=( const Vector2& perAxisScaleFactors ) 
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Vector2::operator +=( const Vector2& vectorToAdd ) 
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Vector2::operator -=( const Vector2& vectorToSubtract )
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline float CalcDistanceSquared( const Vector2& positionA, const Vector2& positionB )
{
	float xDist = positionB.x - positionA.x;
	float yDist = positionB.y - positionA.y;
	float distSquared = ( xDist * xDist ) + ( yDist * yDist );
	return distSquared;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector2 operator * ( float scale, const Vector2& vectorToScale )
{
	float newX = vectorToScale.x * scale;
	float newY = vectorToScale.y * scale;
	return Vector2( newX, newY );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline float DotProduct( const Vector2& a, const Vector2& b )
{
	return ( ( a.x * b.x ) + ( a.y * b.y ) );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector2 Interpolate( const Vector2& start, const Vector2& end, float fractionFromStartToEnd )
{
	float xInterpolated = Interpolate( start.x, end.x, fractionFromStartToEnd );
	float yInterpolated = Interpolate( start.y, end.y, fractionFromStartToEnd );
	return Vector2( xInterpolated, yInterpolated );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline bool AreVectorsEqual( const Vector2& a, const Vector2& b, float tolerance )
{
    if (AreFloatsEqual( a.x, b.x, tolerance ) && AreFloatsEqual( a.y, b.y, tolerance ) )
        return true;
    return false;
}

#endif