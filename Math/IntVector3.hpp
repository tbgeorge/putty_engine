//=================================================================================
// IntVector3.hpp
// Author: Tyler George
// Date  : March 31, 2015
//=================================================================================

#pragma once
#ifndef __included_IntVector3__
#define __included_IntVector3__

#include <vector>
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"

class IntVector3
{
public:

	///---------------------------------------------------------------------------------
	/// Member Variables
	///---------------------------------------------------------------------------------
	int x;
	int y;
	int z;


	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
	~IntVector3() {}

	IntVector3() {}
	IntVector3( const IntVector3& copy ) 
		: x( copy.x ), y( copy.y ), z( copy.z ) {}
	explicit IntVector3( int initialX, int initialY, int initialZ ) 
		: x( initialX ), y( initialY ), z( initialZ ) {}


	///---------------------------------------------------------------------------------
	/// Accessors
	///---------------------------------------------------------------------------------
	void		GetXYZ( int& out_x, int& out_y, int& out_z ) const;
	const int*	GetAsintArray()					const;
	int*		GetAsintArray();
	float		CalcLength()						const;
	int			CalcLengthSquared()					const;


	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
	void	SetXYZ( int newX, int newY, int newZ );
	void	ScaleUniform( int scale );
	void	ScaleNonUniform( const IntVector3& perAxisScaleFactors );

	///---------------------------------------------------------------------------------
	/// Operators
	///---------------------------------------------------------------------------------
	bool			operator == ( const IntVector3& vectorToEqual )		const;
	bool			operator != ( const IntVector3& vectorToNotEqual )		const;
	const IntVector3	operator + ( const IntVector3& vectorToAdd )			const;
	const IntVector3	operator - ( const IntVector3& vectorToSubtract )		const;
	const IntVector3	operator * ( int scale )							const;
	const IntVector3	operator * ( const IntVector3& perAxisScaleFactors )	const;
	const IntVector3	operator = ( const IntVector3& vectorToAssign );
	void			operator *= ( int scale );
	void			operator *= ( const IntVector3& perAxisScaleFactors );
	void			operator += ( const IntVector3& vectorToAdd );
	void			operator -= ( const IntVector3& vectorToSubtract );

	///---------------------------------------------------------------------------------
	/// Friend Functions
	///---------------------------------------------------------------------------------
	friend float		CalcDistance( const IntVector3& positionA, const IntVector3& positionB );
	friend int			CalcDistanceSquared( const IntVector3& positionA, const IntVector3& positionB );
	friend const IntVector3	operator * ( int scale, const IntVector3& vectorToScale );
	friend int			DotProduct( const IntVector3& a, const IntVector3& b );

    ///---------------------------------------------------------------------------------
    /// Static Variables
    ///---------------------------------------------------------------------------------
    static IntVector3 ZERO;
};

typedef std::vector<IntVector3> IntVector3s;

///---------------------------------------------------------------------------------
/// Inline function implementations
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void IntVector3::GetXYZ( int& out_x, int& out_y, int& out_z ) const
{
	out_x = x;
	out_y = y;
	out_z = z;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const int* IntVector3::GetAsintArray() const
{
	return &x;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline int* IntVector3::GetAsintArray() 
{
	return &x;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline int IntVector3::CalcLengthSquared() const
{
	int lengthSquared = ( x * x ) + ( y * y ) + ( z * z );
	return lengthSquared;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void IntVector3::SetXYZ( int newX, int newY, int newZ )
{
	x = newX;
	y = newY;
	z = newZ;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void IntVector3::ScaleUniform( int scale )
{
	x *= scale;
	y *= scale;
	z *= scale;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void IntVector3::ScaleNonUniform(const IntVector3& perAxisScaleFactors ) 
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline bool IntVector3::operator ==( const IntVector3& vectorToEqual ) const
{
	if( x == vectorToEqual.x && y == vectorToEqual.y && z == vectorToEqual.z )
		return true;
	return false;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline bool IntVector3::operator !=( const IntVector3& vectorToNotEqual ) const 
{
	if( x != vectorToNotEqual.x || y != vectorToNotEqual.y || z != vectorToNotEqual.z )
		return true;
	return false;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const IntVector3 IntVector3::operator =( const IntVector3& vectorToAssign ) 
{
	x = vectorToAssign.x;
	y = vectorToAssign.y;
	z = vectorToAssign.z;
	return *this;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const IntVector3 IntVector3::operator +( const IntVector3& vectorToAdd ) const
{
	IntVector3 newVector( x + vectorToAdd.x, y + vectorToAdd.y, z + vectorToAdd.z );
	return newVector;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const IntVector3 IntVector3::operator -( const IntVector3& vectorToSubtract ) const
{
	IntVector3 newVector( x - vectorToSubtract.x, y - vectorToSubtract.y, z - vectorToSubtract.z );
	return newVector;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const IntVector3 IntVector3::operator *( int scale ) const
{
	IntVector3 newVector( x * scale, y * scale, z * scale );
	return newVector;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const IntVector3 IntVector3::operator *( const IntVector3& perAxisScaleFactors ) const
{
	IntVector3 newVector( x * perAxisScaleFactors.x, y * perAxisScaleFactors.y, z * perAxisScaleFactors.z );
	return newVector;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void IntVector3::operator *=( int scale ) 
{
	x *= scale;
	y *= scale;
	z *= scale;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void IntVector3::operator *=( const IntVector3& perAxisScaleFactors ) 
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void IntVector3::operator +=( const IntVector3& vectorToAdd ) 
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	z += vectorToAdd.z;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void IntVector3::operator -=( const IntVector3& vectorToSubtract )
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline int CalcDistanceSquared( const IntVector3& positionA, const IntVector3& positionB )
{
	int xDist = positionB.x - positionA.x;
	int yDist = positionB.y - positionA.y;
	int zDist = positionB.z - positionA.z;
	int distSquared = ( xDist * xDist ) + ( yDist * yDist ) + ( zDist * zDist );
	return distSquared;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const IntVector3 operator * ( int scale, const IntVector3& vectorToScale )
{
	int newX = vectorToScale.x * scale;
	int newY = vectorToScale.y * scale;
	int newZ = vectorToScale.z * scale;
	return IntVector3( newX, newY, newZ );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline int DotProduct( const IntVector3& a, const IntVector3& b )
{
	return ( ( a.x * b.x ) + ( a.y * b.y ) + ( a.z * b.z ) );
}

#endif