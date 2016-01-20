//=================================================================================
// Vector3.hpp
// Author: Tyler George
// Date  : February 18, 2015
//=================================================================================

#pragma once
#ifndef __included_Vector3__
#define __included_Vector3__

#include <vector>
#include "Engine/Math/MathUtils.hpp"

class Vector3
{
public:

	///---------------------------------------------------------------------------------
	/// Member Variables
	///---------------------------------------------------------------------------------
	float x;
	float y;
	float z;


	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
	~Vector3() {}

	Vector3() {}
	Vector3( const Vector3& copy ) 
		: x( copy.x ), y( copy.y ), z( copy.z ) {}
	explicit Vector3( float initialX, float initialY, float initialZ ) 
		: x( initialX ), y( initialY ), z( initialZ ) {}


	///---------------------------------------------------------------------------------
	/// Accessors
	///---------------------------------------------------------------------------------
	void			GetXYZ( float& out_x, float& out_y, float& out_z ) const;
	const float*	GetAsFloatArray()					const;
	float*			GetAsFloatArray();
	float			CalcLength()						const;
	float			CalcLengthSquared()					const;


	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
	void	SetXYZ( float newX, float newY, float newZ );
	float	Normalize();
    const Vector3 Normalized() const;
	float	SetLength( float newLength );
	void	ScaleUniform( float scale );
	void	ScaleNonUniform( const Vector3& perAxisScaleFactors );
	void	InverseScaleNonUniform( const Vector3& perAxisDivisors );

	///---------------------------------------------------------------------------------
	/// Operators
	///---------------------------------------------------------------------------------
	bool			operator == ( const Vector3& vectorToEqual )		const;
	bool			operator != ( const Vector3& vectorToNotEqual )		const;
	const Vector3	operator + ( const Vector3& vectorToAdd )			const;
	const Vector3	operator - ( const Vector3& vectorToSubtract )		const;
	const Vector3	operator * ( float scale )							const;
	const Vector3	operator * ( const Vector3& perAxisScaleFactors )	const;
	const Vector3	operator / ( float inverseScale )					const;
	const Vector3	operator = ( const Vector3& vectorToAssign );
	void			operator *= ( float scale );
	void			operator *= ( const Vector3& perAxisScaleFactors );
	void			operator += ( const Vector3& vectorToAdd );
	void			operator -= ( const Vector3& vectorToSubtract );

	///---------------------------------------------------------------------------------
	/// Friend Functions
	///---------------------------------------------------------------------------------
	friend float			CalcDistance( const Vector3& positionA, const Vector3& positionB );
	friend float			CalcDistanceSquared( const Vector3& positionA, const Vector3& positionB );
	friend const Vector3	operator * ( float scale, const Vector3& vectorToScale );
	friend float			DotProduct( const Vector3& a, const Vector3& b );
	friend const Vector3	CrossProduct( const Vector3& a, const Vector3& b );
	friend const Vector3	Interpolate( const Vector3& start, const Vector3& end, float fractionFromStartToEnd );
    friend bool             AreVectorsEqual( const Vector3& a, const Vector3& b, float tolerance );

	///---------------------------------------------------------------------------------
	/// Static Variables
	///---------------------------------------------------------------------------------
	static const Vector3 ZERO;

};

typedef std::vector<Vector3> Vector3s;

///---------------------------------------------------------------------------------
/// Inline function implementations
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Vector3::GetXYZ( float& out_x, float& out_y, float& out_z ) const
{
	out_x = x;
	out_y = y;
	out_z = z;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const float* Vector3::GetAsFloatArray() const
{
	return &x;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline float* Vector3::GetAsFloatArray() 
{
	return &x;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline float Vector3::CalcLengthSquared() const
{
	float lengthSquared = ( x * x ) + ( y * y ) + ( z * z );
	return lengthSquared;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Vector3::SetXYZ( float newX, float newY, float newZ )
{
	x = newX;
	y = newY;
	z = newZ;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Vector3::ScaleUniform( float scale )
{
	x *= scale;
	y *= scale;
	z *= scale;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Vector3::ScaleNonUniform(const Vector3& perAxisScaleFactors ) 
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
}

///---------------------------------------------------------------------------------
/// slow
///---------------------------------------------------------------------------------
inline void Vector3::InverseScaleNonUniform( const Vector3& perAxisScaleDivisors )
{
	x /= perAxisScaleDivisors.x;
	y /= perAxisScaleDivisors.y;
	z /= perAxisScaleDivisors.z;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline bool Vector3::operator ==( const Vector3& vectorToEqual ) const
{
	if( x == vectorToEqual.x && y == vectorToEqual.y && z == vectorToEqual.z )
		return true;
	return false;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline bool Vector3::operator !=( const Vector3& vectorToNotEqual ) const 
{
	if( x != vectorToNotEqual.x || y != vectorToNotEqual.y || z != vectorToNotEqual.z )
		return true;
	return false;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector3 Vector3::operator =( const Vector3& vectorToAssign ) 
{
	x = vectorToAssign.x;
	y = vectorToAssign.y;
	z = vectorToAssign.z;
	return *this;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector3 Vector3::operator +( const Vector3& vectorToAdd ) const
{
	Vector3 newVector( x + vectorToAdd.x, y + vectorToAdd.y, z + vectorToAdd.z );
	return newVector;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector3 Vector3::operator -( const Vector3& vectorToSubtract ) const
{
	Vector3 newVector( x - vectorToSubtract.x, y - vectorToSubtract.y, z - vectorToSubtract.z );
	return newVector;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector3 Vector3::operator *( float scale ) const
{
	Vector3 newVector( x * scale, y * scale, z * scale );
	return newVector;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector3 Vector3::operator *( const Vector3& perAxisScaleFactors ) const
{
	Vector3 newVector( x * perAxisScaleFactors.x, y * perAxisScaleFactors.y, z * perAxisScaleFactors.z );
	return newVector;
}

///---------------------------------------------------------------------------------
/// slow
///---------------------------------------------------------------------------------
inline const Vector3 Vector3::operator /( float inverseScale ) const
{

	if( inverseScale == 0.0f )
		return Vector3( 0.0f, 0.0f, 0.0f );

	float scale = 1.0f / inverseScale;
	Vector3 newVector( x * scale, y * scale, z * scale );
	return newVector;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Vector3::operator *=( float scale ) 
{
	x *= scale;
	y *= scale;
	z *= scale;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Vector3::operator *=( const Vector3& perAxisScaleFactors ) 
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Vector3::operator +=( const Vector3& vectorToAdd ) 
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	z += vectorToAdd.z;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Vector3::operator -=( const Vector3& vectorToSubtract )
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline float CalcDistanceSquared( const Vector3& positionA, const Vector3& positionB )
{
	float xDist = positionB.x - positionA.x;
	float yDist = positionB.y - positionA.y;
	float zDist = positionB.z - positionA.z;
	float distSquared = ( xDist * xDist ) + ( yDist * yDist ) + ( zDist * zDist );
	return distSquared;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector3 operator * ( float scale, const Vector3& vectorToScale )
{
	float newX = vectorToScale.x * scale;
	float newY = vectorToScale.y * scale;
	float newZ = vectorToScale.z * scale;
	return Vector3( newX, newY, newZ );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline float DotProduct( const Vector3& a, const Vector3& b )
{
	return ( ( a.x * b.x ) + ( a.y * b.y ) + ( a.z * b.z ) );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline const Vector3 CrossProduct( const Vector3& a, const Vector3& b )
{
	float cx = ( a.y * b.z ) - ( a.z * b.y );
	float cy = ( a.z * b.x ) - ( a.x * b.z );
	float cz = ( a.x * b.y ) - ( a.y * b.x );
	return Vector3( cx, cy, cz );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector3 Interpolate( const Vector3& start, const Vector3& end, float fractionFromStartToEnd )
{
	float xInterpolated = Interpolate( start.x, end.x, fractionFromStartToEnd );
	float yInterpolated = Interpolate( start.y, end.y, fractionFromStartToEnd );
	float zInterpolated = Interpolate( start.z, end.z, fractionFromStartToEnd );
	return Vector3( xInterpolated, yInterpolated, zInterpolated );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline bool AreVectorsEqual( const Vector3& a, const Vector3& b, float tolerance )
{    
    if (AreFloatsEqual( a.x, b.x, tolerance ) && AreFloatsEqual( a.y, b.y, tolerance ) && AreFloatsEqual( a.z, b.z, tolerance ))
        return true;
    return false;
}

#endif