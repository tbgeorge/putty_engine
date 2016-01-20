//=================================================================================
// Vector4.hpp
// Author: Tyler George
// Date  : March 3, 2015
//=================================================================================

#pragma once
#ifndef __included_Vector4__
#define __included_Vector4__

#include <vector>
#include "Engine/Math/MathUtils.hpp"

template< typename primitiveType > 
class Vector4
{
public:

	///---------------------------------------------------------------------------------
	/// Member Variables
	///---------------------------------------------------------------------------------
	primitiveType x;
	primitiveType y;
	primitiveType z;
	primitiveType w;


	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
	~Vector4() {}

	Vector4() {}
	Vector4( const Vector4< primitiveType >& copy ) : x( copy.x ), y( copy.y ), z( copy.z ), w( copy.w ) {}
	explicit Vector4( primitiveType initialX, primitiveType initialY, primitiveType initialZ, primitiveType initialW ) 
		: x( initialX ), y( initialY ), z( initialZ ), w( initialW ) {}


	///---------------------------------------------------------------------------------
	/// Accessors
	///---------------------------------------------------------------------------------
	void					GetXYZW( primitiveType& out_x, primitiveType& out_y, primitiveType& out_z, primitiveType& out_w ) const;
	const primitiveType*	GetAsPrimitiveTypeArray()					const;
	primitiveType*			GetAsPrimitiveTypeArray();
	primitiveType			CalcLength()						const;
	primitiveType			CalcLengthSquared()					const;


	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
	void			SetXYZW( primitiveType newX, primitiveType newY, primitiveType newZ, primitiveType newW );
	primitiveType	Normalize3D();
	primitiveType	Normalize4D();
	primitiveType	SetLength( primitiveType newLength );
	void			ScaleUniform( primitiveType scale );
	void			ScaleNonUniform( const Vector4< primitiveType >& perAxisScaleFactors );
	void			InverseScaleNonUniform( const Vector4< primitiveType >& perAxisDivisors );

	///---------------------------------------------------------------------------------
	/// Operators
	///---------------------------------------------------------------------------------
	bool							operator == ( const Vector4< primitiveType >& vectorToEqual )		const;
	bool							operator != ( const Vector4< primitiveType >& vectorToNotEqual )	const;
	const Vector4< primitiveType >	operator + ( const Vector4< primitiveType >& vectorToAdd )			const;
	const Vector4< primitiveType >	operator - ( const Vector4< primitiveType >& vectorToSubtract )		const;
	const Vector4< primitiveType >	operator * ( primitiveType scale )									const;
	const Vector4< primitiveType >	operator * ( const Vector4< primitiveType >& perAxisScaleFactors )	const;
	const Vector4< float >	operator / ( float inverseScale )											const;
	const Vector4< double >	operator / ( double inverseScale )											const;
	const Vector4< int >	operator / ( int inverseScale )												const;
	const Vector4< primitiveType >	operator = ( const Vector4< primitiveType >& vectorToAssign );
	void			operator *= ( primitiveType scale );
	void			operator *= ( const Vector4< primitiveType >& perAxisScaleFactors );
	void			operator += ( const Vector4< primitiveType >& vectorToAdd );
	void			operator -= ( const Vector4< primitiveType >& vectorToSubtract );

	///---------------------------------------------------------------------------------
	/// Friend Functions
	///---------------------------------------------------------------------------------
	friend primitiveType					CalcDistance( const Vector4< primitiveType >& positionA, const Vector4< primitiveType >& positionB );
	friend primitiveType					CalcDistanceSquared( const Vector4< primitiveType >& positionA, const Vector4< primitiveType >& positionB );
	friend const Vector4< primitiveType >	operator * ( primitiveType scale, const Vector4< primitiveType >& vectorToScale );
	friend primitiveType					DotProduct( const Vector4< primitiveType >& a, const Vector4< primitiveType >& b );
	friend const Vector4< primitiveType >	Interpolate( const Vector4< primitiveType >& start, const Vector4< primitiveType >& end, primitiveType fractionFromStartToEnd );

};

typedef Vector4< float > Vector4f;
typedef Vector4< double > Vector4d;
typedef std::vector< Vector4< float > > Vector4fs;
typedef std::vector< Vector4< double > > Vector4ds;


///---------------------------------------------------------------------------------
/// Inline function implementations
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline void Vector4< primitiveType >::GetXYZW( primitiveType& out_x, primitiveType& out_y, primitiveType& out_z, primitiveType& out_w ) const
{
	out_x = x;
	out_y = y;
	out_z = z;
	out_w = w;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline const primitiveType* Vector4< primitiveType >::GetAsPrimitiveTypeArray() const
{
	return &x;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline primitiveType* Vector4< primitiveType >::GetAsPrimitiveTypeArray() 
{
	return &x;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline primitiveType Vector4< primitiveType >::CalcLengthSquared() const
{
	primitiveType lengthSquared = ( x * x ) + ( y * y ) + ( z * z ) + ( w * w );
	return lengthSquared;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline void Vector4< primitiveType >::SetXYZW( primitiveType newX, primitiveType newY, primitiveType newZ, primitiveType newW )
{
	x = newX;
	y = newY;
	z = newZ;
	w = newW;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline void Vector4< primitiveType >::ScaleUniform( primitiveType scale )
{
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline void Vector4< primitiveType >::ScaleNonUniform(const Vector4< primitiveType >& perAxisScaleFactors ) 
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
	w *= perAxisScaleFactors.w;
}

///---------------------------------------------------------------------------------
/// slow
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline void Vector4< primitiveType >::InverseScaleNonUniform( const Vector4< primitiveType >& perAxisScaleDivisors )
{
	x /= perAxisScaleDivisors.x;
	y /= perAxisScaleDivisors.y;
	z /= perAxisScaleDivisors.z;
	w /= perAxisScaleDivisors.w;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline bool Vector4< primitiveType >::operator ==( const Vector4< primitiveType >& vectorToEqual ) const
{
	if( x == vectorToEqual.x && y == vectorToEqual.y && z == vectorToEqual.z && w == vectorToEqual.w )
		return true;
	return false;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline bool Vector4< primitiveType >::operator !=( const Vector4< primitiveType >& vectorToNotEqual ) const 
{
	if( x != vectorToNotEqual.x || y != vectorToNotEqual.y || z != vectorToNotEqual.z || z != vectorToNotEqual.w )
		return true;
	return false;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline const Vector4< primitiveType > Vector4< primitiveType >::operator =( const Vector4< primitiveType >& vectorToAssign ) 
{
	x = vectorToAssign.x;
	y = vectorToAssign.y;
	z = vectorToAssign.z;
	w = vectorToAssign.w;
	return *this;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline const Vector4< primitiveType > Vector4< primitiveType >::operator +( const Vector4< primitiveType >& vectorToAdd ) const
{
	Vector4< primitiveType > newVector( x + vectorToAdd.x, y + vectorToAdd.y, z + vectorToAdd.z, w + vectorToAdd.w );
	return newVector;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline const Vector4< primitiveType > Vector4< primitiveType >::operator -( const Vector4< primitiveType >& vectorToSubtract ) const
{
	Vector4< primitiveType > newVector( x - vectorToSubtract.x, y - vectorToSubtract.y, z - vectorToSubtract.z, w - vectorToSubtract.w );
	return newVector;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline const Vector4< primitiveType > Vector4< primitiveType >::operator *( primitiveType scale ) const
{
	Vector4 newVector( x * scale, y * scale, z * scale, w * scale );
	return newVector;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline const Vector4< primitiveType > Vector4< primitiveType >::operator *( const Vector4< primitiveType >& perAxisScaleFactors ) const
{
	Vector4 newVector( x * perAxisScaleFactors.x, y * perAxisScaleFactors.y, z * perAxisScaleFactors.z, w * perAxisScaleFactors.w );
	return newVector;
}

///---------------------------------------------------------------------------------
/// slow
///---------------------------------------------------------------------------------
inline const Vector4< float > Vector4< float >::operator /( float inverseScale ) const
{

	if( inverseScale == 0.0f )
		return Vector4f( 0.0f, 0.0f, 0.0f, 0.0f );

	float scale = 1.0f / inverseScale;
	Vector4f newVector( x * scale, y * scale, z * scale, w * scale );
	return newVector;
}

///---------------------------------------------------------------------------------
/// slow
///---------------------------------------------------------------------------------
inline const Vector4< double > Vector4< double >::operator /( double inverseScale ) const
{

	if( inverseScale == 0.0 )
		return Vector4d( 0.0, 0.0, 0.0, 0.0 );

	double scale = 1.0 / inverseScale;
	Vector4d newVector( x * scale, y * scale, z * scale, w * scale );
	return newVector;
}

///---------------------------------------------------------------------------------
/// slow
///---------------------------------------------------------------------------------
inline const Vector4< int > Vector4< int >::operator /( int inverseScale ) const
{

	if( inverseScale == 0 )
		return Vector4< int >( 0, 0, 0, 0 );

	Vector4< int > newVector( x / inverseScale, y / inverseScale, z / inverseScale, w / inverseScale );
	return newVector;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline void Vector4< primitiveType >::operator *=( primitiveType scale ) 
{
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline void Vector4< primitiveType >::operator *=( const Vector4< primitiveType >& perAxisScaleFactors ) 
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
	w *= perAxisScaleFactors.w;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline void Vector4< primitiveType >::operator +=( const Vector4< primitiveType >& vectorToAdd ) 
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	z += vectorToAdd.z;
	w += vectorToAdd.w;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline void Vector4< primitiveType >::operator -=( const Vector4< primitiveType >& vectorToSubtract )
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
	w -= vectorToSubtract.w;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline primitiveType CalcDistanceSquared( const Vector4< primitiveType >& positionA, const Vector4< primitiveType >& positionB )
{
	primitiveType xDist = positionB.x - positionA.x;
	primitiveType yDist = positionB.y - positionA.y;
	primitiveType zDist = positionB.z - positionA.z;
	primitiveType wDist = positionB.w - positionA.w;
	primitiveType distSquared = ( xDist * xDist ) + ( yDist * yDist ) + ( zDist * zDist ) + ( wDist * wDist );
	return distSquared;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline const Vector4< primitiveType > operator * ( primitiveType scale, const Vector4< primitiveType >& vectorToScale )
{
	primitiveType newX = vectorToScale.x * scale;
	primitiveType newY = vectorToScale.y * scale;
	primitiveType newZ = vectorToScale.z * scale;
	primitiveType newW = vectorToScale.w * scale;
	return Vector4< primitiveType >( newX, newY, newZ, newW );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline primitiveType DotProduct( const Vector4< primitiveType >& a, const Vector4< primitiveType >& b )
{
	return ( ( a.x * b.x ) + ( a.y * b.y ) + ( a.z * b.z ) + ( a.w * b.w ) );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
template< typename primitiveType > 
inline const Vector4< primitiveType > Interpolate( const Vector4< primitiveType >& start, const Vector4< primitiveType >& end, primitiveType fractionFromStartToEnd )
{
	primitiveType xInterpolated = Interpolate( start.x, end.x, fractionFromStartToEnd );
	primitiveType yInterpolated = Interpolate( start.y, end.y, fractionFromStartToEnd );
	primitiveType zInterpolated = Interpolate( start.z, end.z, fractionFromStartToEnd );
	primitiveType wInterpolated = Interpolate( start.w, end.w, fractionFromStartToEnd );
	return Vector4< primitiveType >( xInterpolated, yInterpolated, zInterpolated, wInterpolated );
}

///---------------------------------------------------------------------------------
/// Non-Inline functions
///---------------------------------------------------------------------------------
///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
template< typename primitiveType > 
primitiveType Vector4< primitiveType >::CalcLength() const
{
	primitiveType length = sqrt( CalcLengthSquared() );

	return length;
}



///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
template< typename primitiveType > 
primitiveType Vector4< primitiveType >::Normalize3D() 
{
	if( CalcLengthSquared() == (primitiveType) 0 )
		return (primitiveType) 0;

	float length = CalcLength();
	if( length == (primitiveType) 0 ) 
		return (primitiveType) 0;

	float inverseLength = ( (primitiveType) 1 ) / length;
	x *= inverseLength;
	y *= inverseLength;
	z *= inverseLength;

	return length;
}

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
template< typename primitiveType > 
primitiveType Vector4< primitiveType >::Normalize4D() 
{
	if( CalcLengthSquared() == (primitiveType) 0 )
		return (primitiveType) 0;

	float length = CalcLength();
	if( length == (primitiveType) 0 ) 
		return (primitiveType) 0;

	float inverseLength = ( (primitiveType) 1 ) / length;
	x *= inverseLength;
	y *= inverseLength;
	z *= inverseLength;
	w *= inverseLength;

	return length;
}

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
template< typename primitiveType > 
primitiveType Vector4< primitiveType >::SetLength( primitiveType newLength )
{

	primitiveType oldLength = CalcLength();
	Normalize4D();

	x *= newLength;
	y *= newLength;
	z *= newLength;
	w *= newLength;

	return oldLength; 
}

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
template< typename primitiveType > 
primitiveType CalcDistance( const Vector4< primitiveType >& positionA, const Vector4< primitiveType >& positionB )
{
	primitiveType distSquared = CalcDistanceSquared( positionA, positionB );
	primitiveType dist = sqrt( distSquared );
	return dist;
}



#endif