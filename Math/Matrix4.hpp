//=================================================================================
// Matrix4.hpp
// Author: Tyler George
// Date  : March 3, 2015
//=================================================================================
#pragma once

#ifndef __included_Matrix4__
#define __included_Matrix4__

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"

template< typename primitiveType >
class Matrix4
{
public:
	///---------------------------------------------------------------------------------
	/// Member Variables
	///---------------------------------------------------------------------------------
	Vector4< primitiveType > m_iBasis;
	Vector4< primitiveType > m_jBasis;
	Vector4< primitiveType > m_kBasis;
	Vector4< primitiveType > m_translation;

	///---------------------------------------------------------------------------------
	/// Constructors/Destructor
	///---------------------------------------------------------------------------------
	~Matrix4() {}
	Matrix4();
	Matrix4( const Matrix4< primitiveType >& copy ) 
		: m_iBasis( copy.m_iBasis ), m_jBasis( copy.m_jBasis ), m_kBasis( copy.m_kBasis ), m_translation( copy.m_translation ) {}

	explicit Matrix4( const Vector2& iBasis, const Vector2& jBasis );
	explicit Matrix4( const Vector2& iBasis, const Vector2& jBasis, const Vector2& translation );
	explicit Matrix4( const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis );
	explicit Matrix4( const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis, const Vector3& translation );
	explicit Matrix4( const Vector4< primitiveType >& iBasis, const Vector4< primitiveType >& jBasis, const Vector4< primitiveType >& kBasis, const Vector4< primitiveType >& translation );

	///---------------------------------------------------------------------------------
	/// Static constructors
	///---------------------------------------------------------------------------------
	static Matrix4< primitiveType > CreateTranslation( const Vector2& translation2D ); // z translation assumed to be 0
	static Matrix4< primitiveType > CreateTranslation( const Vector3& translation3D );
	static Matrix4< primitiveType > CreateScale( primitiveType uniformScale );
	static Matrix4< primitiveType > CreateScale( const Vector2& nonUniformScale2D ); // z scale assumed to be 1
	static Matrix4< primitiveType > CreateScale( const Vector3& nonUniformScale3D );
	static Matrix4< primitiveType > CreateRotationDegreesAboutX( primitiveType degrees );
	static Matrix4< primitiveType > CreateRotationDegreesAboutY( primitiveType degrees );
	static Matrix4< primitiveType > CreateRotationDegreesAboutZ( primitiveType degrees ); // a.k.a. CreateRotationDegrees2D
	static Matrix4< primitiveType > CreateRotationRadiansAboutX( primitiveType radians );
	static Matrix4< primitiveType > CreateRotationRadiansAboutY( primitiveType radians );
	static Matrix4< primitiveType > CreateRotationRadiansAboutZ( primitiveType radians ); // a.k.a. CreateRotationRadians2D
	static Matrix4< primitiveType > CreateIdentity();

	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------
	const Matrix4< primitiveType > GetTransform( const Matrix4< primitiveType >& transformationMatrix ) const;
	const primitiveType* GetAsPrimitiveTypeArray() const;
	primitiveType* GetAsPrimitiveTypeArray();

	const Vector2 TransformPosition( const Vector2& position2D ) const; // Hardcoded to assume z=0, w=1
	const Vector3 TransformPosition( const Vector3& position3D ) const; // Hardcoded to assume w=1
	const Vector2 TransformDirection( const Vector2& direction2D ) const; // Hardcoded to assume z=0, w=0
	const Vector3 TransformDirection( const Vector3& direction3D ) const; // Hardcoded to assume w=0
	const Vector4< primitiveType > TransformVector( const Vector4< primitiveType >& homogeneousVector ) const; // w is provided

	const primitiveType GetDeterminant();

	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
	void MakeIdentity();
	void Transform( const Matrix4< primitiveType >& transformationMatrix );

	void Translate( const Vector2& translation2D ); // z translation assumed to be 0
	void Translate( const Vector3& translation3D );
	void Scale( primitiveType uniformScale );
	void Scale( const Vector2& nonUniformScale2D ); // z scale assumed to be 1
	void Scale( const Vector3& nonUniformScale3D );
	void RotateDegreesAboutX( primitiveType degrees );
	void RotateDegreesAboutY( primitiveType degrees );
	void RotateDegreesAboutZ( primitiveType degrees ); // Possibly also offered as RotateDegrees2D
	void RotateRadiansAboutX( primitiveType radians );
	void RotateRadiansAboutY( primitiveType radians );
	void RotateRadiansAboutZ( primitiveType radians ); // Possibly also offered as RotateRadians2D

	void Transpose();
	void Invert();


	///---------------------------------------------------------------------------------
	/// Operators
	///---------------------------------------------------------------------------------
	void operator=( const Matrix4< primitiveType >& matrixToAssign );
    Matrix4<primitiveType> operator+ (const Matrix4<primitiveType>& matToAdd) const;

	///---------------------------------------------------------------------------------
	/// Friend Functions
	///---------------------------------------------------------------------------------
	template< typename primitiveType >
	friend const Matrix4< primitiveType > MatrixMultiply( const Matrix4< primitiveType >& lhs, const Matrix4< primitiveType >& rhs );
};

typedef Matrix4< float > Matrix4f;
typedef Matrix4< double > Matrix4d;

///---------------------------------------------------------------------------------
/// Inline Functions
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline Matrix4< primitiveType >::Matrix4()
{
	MakeIdentity();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline Matrix4< primitiveType >::Matrix4( const Vector2& iBasis, const Vector2& jBasis )
{
	MakeIdentity();
	m_iBasis.x = iBasis.x;
	m_iBasis.y = iBasis.y;
	m_jBasis.x = jBasis.x;
	m_jBasis.y = jBasis.y;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline Matrix4< primitiveType >::Matrix4( const Vector2& iBasis, const Vector2& jBasis, const Vector2& translation )
{
	MakeIdentity();
	m_iBasis.x = iBasis.x;
	m_iBasis.y = iBasis.y;
	m_jBasis.x = jBasis.x;
	m_jBasis.y = jBasis.y;

	m_translation.x = translation.x;
	m_translation.y = translation.y;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline Matrix4< primitiveType >::Matrix4( const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis )
{
	MakeIdentity();
	m_iBasis.x = iBasis.x;
	m_iBasis.y = iBasis.y;
	m_iBasis.z = iBasis.z;
	m_jBasis.x = jBasis.x;
	m_jBasis.y = jBasis.y;
	m_jBasis.z = jBasis.z;
	m_kBasis.x = kBasis.x;
	m_kBasis.y = kBasis.y;
	m_kBasis.z = kBasis.z;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline Matrix4< primitiveType >::Matrix4( const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis, const Vector3& translation )
{
	MakeIdentity();
	m_iBasis.x = iBasis.x;
	m_iBasis.y = iBasis.y;
	m_iBasis.z = iBasis.z;
	m_jBasis.x = jBasis.x;
	m_jBasis.y = jBasis.y;
	m_jBasis.z = jBasis.z;
	m_kBasis.x = kBasis.x;
	m_kBasis.y = kBasis.y;
	m_kBasis.z = kBasis.z;

	m_translation.x = translation.x;
	m_translation.y = translation.y;
	m_translation.z = translation.z;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline Matrix4< primitiveType >::Matrix4( const Vector4< primitiveType >& iBasis, const Vector4< primitiveType >& jBasis, const Vector4< primitiveType >& kBasis, const Vector4< primitiveType>& translation )
	: m_iBasis( iBasis )
	, m_jBasis( jBasis )
	, m_kBasis( kBasis )
	, m_translation( translation )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------	
template< typename primitiveType >
inline Matrix4< primitiveType > Matrix4< primitiveType >::CreateTranslation( const Vector2& translation2D ) // z translation assumed to be 0
{
	Matrix4< primitiveType > translationMatrix;
	translationMatrix.m_translation.x = translation2D.x;
	translationMatrix.m_translation.y = translation2D.y;
	translationMatrix.m_translation.z = (primitiveType) 0;
	return translationMatrix;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline Matrix4< primitiveType > Matrix4< primitiveType >::CreateTranslation( const Vector3& translation3D )
{
	Matrix4< primitiveType > translationMatrix;
	translationMatrix.m_translation.x = translation3D.x;
	translationMatrix.m_translation.y = translation3D.y;
	translationMatrix.m_translation.z = translation3D.z;
	return translationMatrix;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline Matrix4< primitiveType > Matrix4< primitiveType >::CreateScale( primitiveType uniformScale )
{
	Matrix4< primitiveType > scaleMatrix;
	scaleMatrix.m_iBasis.x = uniformScale;
	scaleMatrix.m_jBasis.y = uniformScale;
	scaleMatrix.m_kBasis.z = uniformScale;
	return scaleMatrix;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline Matrix4< primitiveType > Matrix4< primitiveType >::CreateScale( const Vector2& nonUniformScale2D ) // z scale assumed to be 1
{
	Matrix4< primitiveType > scaleMatrix;
	scaleMatrix.m_iBasis.x = nonUniformScale2D.x;
	scaleMatrix.m_jBasis.y = nonUniformScale2D.y;
	scaleMatrix.m_kBasis.z = (primitiveType) 1;
	return scaleMatrix;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline Matrix4< primitiveType > Matrix4< primitiveType >::CreateScale( const Vector3& nonUniformScale3D )
{
	Matrix4< primitiveType > scaleMatrix;
	scaleMatrix.m_iBasis.x = nonUniformScale3D.x;
	scaleMatrix.m_jBasis.y = nonUniformScale3D.y;
	scaleMatrix.m_kBasis.z = nonUniformScale3D.z;
	return scaleMatrix;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline const primitiveType* Matrix4< primitiveType >::GetAsPrimitiveTypeArray() const
{
	return &m_iBasis.x;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline primitiveType* Matrix4< primitiveType >::GetAsPrimitiveTypeArray()
{
	return &m_iBasis.x;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline const Vector2 Matrix4< primitiveType >::TransformPosition( const Vector2& position2D ) const // Hardcoded to assume z=0, w=1
{
	Vector2 transformedPosition;
	transformedPosition.x = ( m_iBasis.x * position2D.x ) + ( m_jBasis.x * position2D.y ) 
						  + ( m_kBasis.x * (primitiveType) 0 ) + ( m_translation.x * (primitiveType) 1 );
	transformedPosition.y = ( m_iBasis.y * position2D.x ) + ( m_jBasis.y * position2D.y ) 
						  + ( m_kBasis.y * (primitiveType) 0 ) + ( m_translation.y * (primitiveType) 1 );
	return transformedPosition;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline const Vector3 Matrix4< primitiveType >::TransformPosition( const Vector3& position3D ) const // Hardcoded to assume w=1
{
	Vector3 transformedPosition;
	transformedPosition.x = ( m_iBasis.x * position3D.x ) + ( m_jBasis.x * position3D.y ) 
						  + ( m_kBasis.x * position3D.z ) + ( m_translation.x * (primitiveType) 1 );
	transformedPosition.y = ( m_iBasis.y * position3D.x ) + ( m_jBasis.y * position3D.y ) 
						  + ( m_kBasis.y * position3D.z ) + ( m_translation.y * (primitiveType) 1 );
	transformedPosition.z = ( m_iBasis.z * position3D.x ) + ( m_jBasis.z * position3D.y ) 
						  + ( m_kBasis.z * position3D.z ) + ( m_translation.z * (primitiveType) 1 );
	return transformedPosition;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline const Vector2 Matrix4< primitiveType >::TransformDirection( const Vector2& direction2D ) const // Hardcoded to assume z=0, w=0
{
	Vector2 transformedDirection;
	transformedDirection.x = ( m_iBasis.x * direction2D.x ) + ( m_jBasis.x * direction2D.y ) 
						   + ( m_kBasis.x * (primitiveType) 0 ) + ( m_translation.x * (primitiveType) 0 );
	transformedDirection.y = ( m_iBasis.y * direction2D.x ) + ( m_jBasis.y * direction2D.y ) 
						   + ( m_kBasis.y * (primitiveType) 0 ) + ( m_translation.y * (primitiveType) 0 );
	return transformedDirection;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline const Vector3 Matrix4< primitiveType >::TransformDirection( const Vector3& direction3D ) const // Hardcoded to assume w=0
{
	Vector3 transformedDirection;
	transformedDirection.x = ( m_iBasis.x * direction3D.x ) + ( m_jBasis.x * direction3D.y ) 
						   + ( m_kBasis.x * direction3D.z ) + ( m_translation.x * (primitiveType) 0 );
	transformedDirection.y = ( m_iBasis.y * direction3D.x ) + ( m_jBasis.y * direction3D.y ) 
						   + ( m_kBasis.y * direction3D.z ) + ( m_translation.y * (primitiveType) 0 );
	transformedDirection.z = ( m_iBasis.z * direction3D.x ) + ( m_jBasis.z * direction3D.y ) 
						   + ( m_kBasis.z * direction3D.z ) + ( m_translation.z * (primitiveType) 0 );
	return transformedDirection;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline const Vector4< primitiveType > Matrix4< primitiveType >::TransformVector( const Vector4< primitiveType >& homogeneousVector ) const // w is provided
{
	Vector4< primitiveType > transformedVector;
	transformedVector.x = ( m_iBasis.x * homogeneousVector.x ) + ( m_jBasis.x * homogeneousVector.y ) 
		+ ( m_kBasis.x * homogeneousVector.z ) + ( m_translation.x * homogeneousVector.w );
	transformedVector.y = ( m_iBasis.y * homogeneousVector.x ) + ( m_jBasis.y * homogeneousVector.y ) 
		+ ( m_kBasis.y * homogeneousVector.z ) + ( m_translation.y * homogeneousVector.w );
	transformedVector.z = ( m_iBasis.z * homogeneousVector.x ) + ( m_jBasis.z * homogeneousVector.y ) 
		+ ( m_kBasis.z * homogeneousVector.z ) + ( m_translation.z * homogeneousVector.w );
	transformedVector.w = ( m_iBasis.w * homogeneousVector.x ) + ( m_jBasis.w * homogeneousVector.y ) 
		+ ( m_kBasis.w * homogeneousVector.z ) + ( m_translation.w * homogeneousVector.w );
	return transformedVector;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline const primitiveType Matrix4< primitiveType >::GetDeterminant()
{
	primitiveType det2_31_32 = ( m_kBasis.x * m_translation.y ) - ( m_kBasis.y * m_translation.x );
	primitiveType det2_31_33 = ( m_kBasis.x * m_translation.z ) - ( m_kBasis.z * m_translation.x );
	primitiveType det2_31_34 = ( m_kBasis.x * m_translation.w ) - ( m_kBasis.w * m_translation.x );
	primitiveType det2_32_33 = ( m_kBasis.y * m_translation.z ) - ( m_kBasis.z * m_translation.y );
	primitiveType det2_32_34 = ( m_kBasis.y * m_translation.w ) - ( m_kBasis.w * m_translation.y );
	primitiveType det2_33_34 = ( m_kBasis.z * m_translation.w ) - ( m_kBasis.w * m_translation.z );

	primitiveType det3_21_22_23 = ( m_jBasis.x * det2_32_33 ) - ( m_jBasis.y * det2_31_33 ) + ( m_jBasis.z * det2_31_32 );
	primitiveType det3_21_22_24 = ( m_jBasis.x * det2_32_34 ) - ( m_jBasis.y * det2_31_34 ) + ( m_jBasis.w * det2_31_32 );
	primitiveType det3_21_23_24 = ( m_jBasis.x * det2_33_34 ) - ( m_jBasis.z * det2_31_34 ) + ( m_jBasis.w * det2_31_33 );
	primitiveType det3_22_23_24 = ( m_jBasis.y * det2_33_34 ) - ( m_jBasis.z * det2_32_34 ) + ( m_jBasis.w * det2_32_33 );

	primitiveType det4 = ( m_iBasis.x * det3_22_23_24 ) - ( m_iBasis.y * det3_21_23_24 ) + ( m_iBasis.z * det3_21_22_24 ) - ( m_iBasis.w * det3_21_22_23 );

	return det4;	
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline void Matrix4< primitiveType >::Transpose()
{
	Vector4< primitiveType > newIBasis;
	Vector4< primitiveType > newJBasis;
	Vector4< primitiveType > newKBasis;
	Vector4< primitiveType > newTranslation;

	newIBasis.x = m_iBasis.x;
	newIBasis.y = m_jBasis.x;
	newIBasis.z = m_kBasis.x;
	newIBasis.w = m_translation.x;

	newJBasis.x = m_iBasis.y;
	newJBasis.y = m_jBasis.y;
	newJBasis.z = m_kBasis.y;
	newJBasis.w = m_translation.y;

	newKBasis.x = m_iBasis.z;
	newKBasis.y = m_jBasis.z;
	newKBasis.z = m_kBasis.z;
	newKBasis.w = m_translation.z;

	newTranslation.x = m_iBasis.w;
	newTranslation.y = m_jBasis.w;
	newTranslation.z = m_kBasis.w;
	newTranslation.w = m_translation.w;

	m_iBasis = newIBasis;
	m_jBasis = newJBasis;
	m_kBasis = newKBasis;
	m_translation = newTranslation;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline void Matrix4< primitiveType >::Invert()
{
	primitiveType inverseDeterminant = (primitiveType) 1 / GetDeterminant();

	Matrix4< primitiveType > multiplyByInverseDeterminant;
	multiplyByInverseDeterminant.m_iBasis.x = ( m_jBasis.y * m_kBasis.z * m_translation.w )
											+ ( m_jBasis.z * m_kBasis.w * m_translation.y )
											+ ( m_jBasis.w * m_kBasis.y * m_translation.z )
											- ( m_jBasis.y * m_kBasis.w * m_translation.z )
											- ( m_jBasis.z * m_kBasis.y * m_translation.w )
											- ( m_jBasis.w * m_kBasis.z * m_translation.y );

	multiplyByInverseDeterminant.m_iBasis.y = ( m_iBasis.y * m_kBasis.w * m_translation.z )
											+ ( m_iBasis.z * m_kBasis.y * m_translation.w )
											+ ( m_iBasis.w * m_kBasis.z * m_translation.y )
											- ( m_iBasis.y * m_kBasis.z * m_translation.w )
											- ( m_iBasis.z * m_kBasis.w * m_translation.y )
											- ( m_iBasis.w * m_kBasis.y * m_translation.z );

	multiplyByInverseDeterminant.m_iBasis.z = ( m_iBasis.y * m_jBasis.z * m_translation.w )
											+ ( m_iBasis.z * m_jBasis.w * m_translation.y )
											+ ( m_iBasis.w * m_jBasis.y * m_translation.z )
											- ( m_iBasis.y * m_jBasis.w * m_translation.z )
											- ( m_iBasis.z * m_jBasis.y * m_translation.w )
											- ( m_iBasis.w * m_jBasis.z * m_translation.y );

	multiplyByInverseDeterminant.m_iBasis.w = ( m_iBasis.y * m_jBasis.w * m_kBasis.z )
											+ ( m_iBasis.z * m_jBasis.y * m_kBasis.w )
											+ ( m_iBasis.w * m_jBasis.z * m_kBasis.y )
											- ( m_iBasis.y * m_jBasis.z * m_kBasis.w )
											- ( m_iBasis.z * m_jBasis.w * m_kBasis.y )
											- ( m_iBasis.w * m_jBasis.y * m_kBasis.z );

	multiplyByInverseDeterminant.m_jBasis.x = ( m_jBasis.x * m_kBasis.w * m_translation.z )
											+ ( m_jBasis.z * m_kBasis.x * m_translation.w )
											+ ( m_jBasis.w * m_kBasis.z * m_translation.x )
											- ( m_jBasis.x * m_kBasis.z * m_translation.w )
											- ( m_jBasis.z * m_kBasis.w * m_translation.x )
											- ( m_jBasis.w * m_kBasis.x * m_translation.z );

	multiplyByInverseDeterminant.m_jBasis.y = ( m_iBasis.x * m_kBasis.z * m_translation.w )
											+ ( m_iBasis.z * m_kBasis.w * m_translation.x )
											+ ( m_iBasis.w * m_kBasis.x * m_translation.z )
											- ( m_iBasis.x * m_kBasis.w * m_translation.z )
											- ( m_iBasis.z * m_kBasis.x * m_translation.w )
											- ( m_iBasis.w * m_kBasis.z * m_translation.x );

	multiplyByInverseDeterminant.m_jBasis.z = ( m_iBasis.x * m_jBasis.w * m_translation.z )
											+ ( m_iBasis.z * m_jBasis.x * m_translation.w )
											+ ( m_iBasis.w * m_jBasis.z * m_translation.x )
											- ( m_iBasis.x * m_jBasis.z * m_translation.w )
											- ( m_iBasis.z * m_jBasis.w * m_translation.x )
											- ( m_iBasis.w * m_jBasis.x * m_translation.z );

	multiplyByInverseDeterminant.m_jBasis.w = ( m_iBasis.x * m_jBasis.z * m_kBasis.w )
											+ ( m_iBasis.z * m_jBasis.w * m_kBasis.x )
											+ ( m_iBasis.w * m_jBasis.x * m_kBasis.z )
											- ( m_iBasis.x * m_jBasis.w * m_kBasis.z )
											- ( m_iBasis.z * m_jBasis.x * m_kBasis.w )
											- ( m_iBasis.w * m_jBasis.z * m_kBasis.x );

	multiplyByInverseDeterminant.m_kBasis.x = ( m_jBasis.x * m_kBasis.y * m_translation.w )
											+ ( m_jBasis.y * m_kBasis.w * m_translation.x )
											+ ( m_jBasis.w * m_kBasis.x * m_translation.y )
											- ( m_jBasis.x * m_kBasis.w * m_translation.y )
											- ( m_jBasis.y * m_kBasis.x * m_translation.w )
											- ( m_jBasis.w * m_kBasis.y * m_translation.x );

	multiplyByInverseDeterminant.m_kBasis.y = ( m_iBasis.x * m_kBasis.w * m_translation.y )
											+ ( m_iBasis.y * m_kBasis.x * m_translation.w )
											+ ( m_iBasis.w * m_kBasis.y * m_translation.x )
											- ( m_iBasis.x * m_kBasis.y * m_translation.w )
											- ( m_iBasis.y * m_kBasis.w * m_translation.x )
											- ( m_iBasis.w * m_kBasis.x * m_translation.y );

	multiplyByInverseDeterminant.m_kBasis.z = ( m_iBasis.x * m_jBasis.y * m_translation.w )
											+ ( m_iBasis.y * m_jBasis.w * m_translation.x )
											+ ( m_iBasis.w * m_jBasis.x * m_translation.y )
											- ( m_iBasis.x * m_jBasis.w * m_translation.y )
											- ( m_iBasis.y * m_jBasis.x * m_translation.w )
											- ( m_iBasis.w * m_jBasis.y * m_translation.x );

	multiplyByInverseDeterminant.m_kBasis.w = ( m_iBasis.x * m_jBasis.w * m_kBasis.y )
											+ ( m_iBasis.y * m_jBasis.x * m_kBasis.w )
											+ ( m_iBasis.w * m_jBasis.y * m_kBasis.x )
											- ( m_iBasis.x * m_jBasis.y * m_kBasis.w )
											- ( m_iBasis.y * m_jBasis.w * m_kBasis.x )
											- ( m_iBasis.w * m_jBasis.x * m_kBasis.y );

	multiplyByInverseDeterminant.m_translation.x = ( m_jBasis.x * m_kBasis.z * m_translation.y )
												 + ( m_jBasis.y * m_kBasis.x * m_translation.z )
												 + ( m_jBasis.z * m_kBasis.y * m_translation.x )
												 - ( m_jBasis.x * m_kBasis.y * m_translation.z )
												 - ( m_jBasis.y * m_kBasis.z * m_translation.x )
												 - ( m_jBasis.z * m_kBasis.x * m_translation.y );

	multiplyByInverseDeterminant.m_translation.y = ( m_iBasis.x * m_kBasis.y * m_translation.z )
												 + ( m_iBasis.y * m_kBasis.z * m_translation.x )
												 + ( m_iBasis.z * m_kBasis.x * m_translation.y )
												 - ( m_iBasis.x * m_kBasis.z * m_translation.y )
												 - ( m_iBasis.y * m_kBasis.x * m_translation.z )
												 - ( m_iBasis.z * m_kBasis.y * m_translation.x );

	multiplyByInverseDeterminant.m_translation.z = ( m_iBasis.x * m_jBasis.z * m_translation.y )
												 + ( m_iBasis.y * m_jBasis.x * m_translation.z )
												 + ( m_iBasis.z * m_jBasis.y * m_translation.x )
												 - ( m_iBasis.x * m_jBasis.y * m_translation.z )
												 - ( m_iBasis.y * m_jBasis.z * m_translation.x )
												 - ( m_iBasis.z * m_jBasis.x * m_translation.y );

	multiplyByInverseDeterminant.m_translation.w = ( m_iBasis.x * m_jBasis.y * m_kBasis.z )
												 + ( m_iBasis.y * m_jBasis.z * m_kBasis.x )
												 + ( m_iBasis.z * m_jBasis.x * m_kBasis.y )
												 - ( m_iBasis.x * m_jBasis.z * m_kBasis.y )
												 - ( m_iBasis.y * m_jBasis.x * m_kBasis.z )
												 - ( m_iBasis.z * m_jBasis.y * m_kBasis.x );

	m_iBasis = multiplyByInverseDeterminant.m_iBasis;
	m_iBasis.ScaleUniform( inverseDeterminant );

	m_jBasis = multiplyByInverseDeterminant.m_jBasis; 
	m_jBasis.ScaleUniform( inverseDeterminant );

	m_kBasis = multiplyByInverseDeterminant.m_kBasis;
	m_kBasis.ScaleUniform( inverseDeterminant );

	m_translation = multiplyByInverseDeterminant.m_translation;
	m_translation.ScaleUniform( inverseDeterminant );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------	
template< typename primitiveType >
inline void Matrix4< primitiveType >::operator=( const Matrix4< primitiveType >& matrixToAssign )
{
	m_iBasis = matrixToAssign.m_iBasis;
	m_jBasis = matrixToAssign.m_jBasis;
	m_kBasis = matrixToAssign.m_kBasis;
	m_translation = matrixToAssign.m_translation;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline Matrix4< primitiveType > Matrix4<primitiveType>::operator+(const Matrix4<primitiveType>& matToAdd) const
{
    Matrix4<primitiveType> returnMat;

    returnMat.m_iBasis = m_iBasis + matToAdd.m_iBasis;
    returnMat.m_jBasis = m_jBasis + matToAdd.m_jBasis;
    returnMat.m_kBasis = m_kBasis + matToAdd.m_kBasis;
    returnMat.m_translation = m_translation + matToAdd.m_translation;

    return returnMat;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
inline const Matrix4< primitiveType > MatrixMultiply( const Matrix4< primitiveType >& lhs, const Matrix4< primitiveType >& rhs )
{
	Matrix4< primitiveType > result;
	result.m_iBasis.x = lhs.m_iBasis.x * rhs.m_iBasis.x + lhs.m_iBasis.y * rhs.m_jBasis.x + lhs.m_iBasis.z * rhs.m_kBasis.x + lhs.m_iBasis.w * rhs.m_translation.x;
	result.m_iBasis.y = lhs.m_iBasis.x * rhs.m_iBasis.y + lhs.m_iBasis.y * rhs.m_jBasis.y + lhs.m_iBasis.z * rhs.m_kBasis.y + lhs.m_iBasis.w * rhs.m_translation.y;
	result.m_iBasis.z = lhs.m_iBasis.x * rhs.m_iBasis.z + lhs.m_iBasis.y * rhs.m_jBasis.z + lhs.m_iBasis.z * rhs.m_kBasis.z + lhs.m_iBasis.w * rhs.m_translation.z;
	result.m_iBasis.w = lhs.m_iBasis.x * rhs.m_iBasis.w + lhs.m_iBasis.y * rhs.m_jBasis.w + lhs.m_iBasis.z * rhs.m_kBasis.w + lhs.m_iBasis.w * rhs.m_translation.w;

	result.m_jBasis.x = lhs.m_jBasis.x * rhs.m_iBasis.x + lhs.m_jBasis.y * rhs.m_jBasis.x + lhs.m_jBasis.z * rhs.m_kBasis.x + lhs.m_jBasis.w * rhs.m_translation.x;
	result.m_jBasis.y = lhs.m_jBasis.x * rhs.m_iBasis.y + lhs.m_jBasis.y * rhs.m_jBasis.y + lhs.m_jBasis.z * rhs.m_kBasis.y + lhs.m_jBasis.w * rhs.m_translation.y;
	result.m_jBasis.z = lhs.m_jBasis.x * rhs.m_iBasis.z + lhs.m_jBasis.y * rhs.m_jBasis.z + lhs.m_jBasis.z * rhs.m_kBasis.z + lhs.m_jBasis.w * rhs.m_translation.z;
	result.m_jBasis.w = lhs.m_jBasis.x * rhs.m_iBasis.w + lhs.m_jBasis.y * rhs.m_jBasis.w + lhs.m_jBasis.z * rhs.m_kBasis.w + lhs.m_jBasis.w * rhs.m_translation.w;

	result.m_kBasis.x = lhs.m_kBasis.x * rhs.m_iBasis.x + lhs.m_kBasis.y * rhs.m_jBasis.x + lhs.m_kBasis.z * rhs.m_kBasis.x + lhs.m_kBasis.w * rhs.m_translation.x;
	result.m_kBasis.y = lhs.m_kBasis.x * rhs.m_iBasis.y + lhs.m_kBasis.y * rhs.m_jBasis.y + lhs.m_kBasis.z * rhs.m_kBasis.y + lhs.m_kBasis.w * rhs.m_translation.y;
	result.m_kBasis.z = lhs.m_kBasis.x * rhs.m_iBasis.z + lhs.m_kBasis.y * rhs.m_jBasis.z + lhs.m_kBasis.z * rhs.m_kBasis.z + lhs.m_kBasis.w * rhs.m_translation.z;
	result.m_kBasis.w = lhs.m_kBasis.x * rhs.m_iBasis.w + lhs.m_kBasis.y * rhs.m_jBasis.w + lhs.m_kBasis.z * rhs.m_kBasis.w + lhs.m_kBasis.w * rhs.m_translation.w;

	result.m_translation.x = lhs.m_translation.x * rhs.m_iBasis.x + lhs.m_translation.y * rhs.m_jBasis.x + lhs.m_translation.z * rhs.m_kBasis.x + lhs.m_translation.w * rhs.m_translation.x;
	result.m_translation.y = lhs.m_translation.x * rhs.m_iBasis.y + lhs.m_translation.y * rhs.m_jBasis.y + lhs.m_translation.z * rhs.m_kBasis.y + lhs.m_translation.w * rhs.m_translation.y;
	result.m_translation.z = lhs.m_translation.x * rhs.m_iBasis.z + lhs.m_translation.y * rhs.m_jBasis.z + lhs.m_translation.z * rhs.m_kBasis.z + lhs.m_translation.w * rhs.m_translation.z;
	result.m_translation.w = lhs.m_translation.x * rhs.m_iBasis.w + lhs.m_translation.y * rhs.m_jBasis.w + lhs.m_translation.z * rhs.m_kBasis.w + lhs.m_translation.w * rhs.m_translation.w;

	return result;
}

///---------------------------------------------------------------------------------
/// Non-Inline templated functions
///---------------------------------------------------------------------------------
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
Matrix4< primitiveType > Matrix4< primitiveType >::CreateRotationDegreesAboutX( primitiveType degrees )
{
	Matrix4< primitiveType > rotationMatrix;
	primitiveType radians = ConvertDegreesToRadians( degrees );
	primitiveType cosRadians = cos( radians );
	primitiveType sinRadians = sin( radians );

	rotationMatrix.m_jBasis.y = cosRadians;
	rotationMatrix.m_jBasis.z = sinRadians;
	rotationMatrix.m_kBasis.y = -sinRadians;
	rotationMatrix.m_kBasis.z = cosRadians;
	return rotationMatrix;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
Matrix4< primitiveType > Matrix4< primitiveType >::CreateRotationDegreesAboutY( primitiveType degrees )
{
	Matrix4< primitiveType > rotationMatrix;
	primitiveType radians = ConvertDegreesToRadians( degrees );
	primitiveType cosRadians = cos( radians );
	primitiveType sinRadians = sin( radians );

	rotationMatrix.m_iBasis.x = cosRadians;
	rotationMatrix.m_iBasis.z = -sinRadians;
	rotationMatrix.m_kBasis.x = sinRadians;
	rotationMatrix.m_kBasis.z = cosRadians;
	return rotationMatrix;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
Matrix4< primitiveType > Matrix4< primitiveType >::CreateRotationDegreesAboutZ( primitiveType degrees ) // a.k.a. CreateRotationDegrees2D
{
	Matrix4< primitiveType > rotationMatrix;
	primitiveType radians = ConvertDegreesToRadians( degrees );
	primitiveType cosRadians = cos( radians );
	primitiveType sinRadians = sin( radians );

	rotationMatrix.m_iBasis.x = cosRadians;
	rotationMatrix.m_iBasis.y = sinRadians;
	rotationMatrix.m_jBasis.x = -sinRadians;
	rotationMatrix.m_jBasis.y = cosRadians;
	return rotationMatrix;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
Matrix4< primitiveType > Matrix4< primitiveType >::CreateRotationRadiansAboutX( primitiveType radians )
{
	Matrix4< primitiveType > rotationMatrix;
	primitiveType cosRadians = cos( radians );
	primitiveType sinRadians = sin( radians );

	rotationMatrix.m_jBasis.y = cosRadians;
	rotationMatrix.m_jBasis.z = sinRadians;
	rotationMatrix.m_kBasis.y = -sinRadians;
	rotationMatrix.m_kBasis.z = cosRadians;
	return rotationMatrix;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
Matrix4< primitiveType > Matrix4< primitiveType >::CreateRotationRadiansAboutY( primitiveType radians )
{
	Matrix4< primitiveType > rotationMatrix;
	primitiveType cosRadians = cos( radians );
	primitiveType sinRadians = sin( radians );

	rotationMatrix.m_iBasis.x = cosRadians;
	rotationMatrix.m_iBasis.z = sinRadians;
	rotationMatrix.m_kBasis.x = -sinRadians;
	rotationMatrix.m_kBasis.z = cosRadians;
	return rotationMatrix;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
Matrix4< primitiveType > Matrix4< primitiveType >::CreateRotationRadiansAboutZ( primitiveType radians ) // a.k.a. CreateRotationRadians2D
{
	Matrix4< primitiveType > rotationMatrix;
	primitiveType cosRadians = cos( radians );
	primitiveType sinRadians = sin( radians );

	rotationMatrix.m_iBasis.x = cosRadians;
	rotationMatrix.m_iBasis.y = sinRadians;
	rotationMatrix.m_jBasis.x = -sinRadians;
	rotationMatrix.m_jBasis.y = cosRadians;
	return rotationMatrix;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
Matrix4< primitiveType > Matrix4< primitiveType >::CreateIdentity()
{
	Matrix4< primitiveType > identity;
	return identity;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
const Matrix4< primitiveType > Matrix4< primitiveType >::GetTransform( const Matrix4< primitiveType >& transformationMatrix ) const
{
	Matrix4< primitiveType > transformedMatrix;

	// first row
	transformedMatrix.m_iBasis.x = ( m_iBasis.x * transformationMatrix.m_iBasis.x )
								 + ( m_jBasis.x * transformationMatrix.m_iBasis.y )
								 + ( m_kBasis.x * transformationMatrix.m_iBasis.z )
								 + ( m_translation.x * transformationMatrix.m_iBasis.w );
	transformedMatrix.m_jBasis.x = ( m_iBasis.x * transformationMatrix.m_jBasis.x )
								 + ( m_jBasis.x * transformationMatrix.m_jBasis.y )
								 + ( m_kBasis.x * transformationMatrix.m_jBasis.z )
								 + ( m_translation.x * transformationMatrix.m_jBasis.w );
	transformedMatrix.m_kBasis.x = ( m_iBasis.x * transformationMatrix.m_kBasis.x )
								 + ( m_jBasis.x * transformationMatrix.m_kBasis.y )
								 + ( m_kBasis.x * transformationMatrix.m_kBasis.z )
								 + ( m_translation.x * transformationMatrix.m_kBasis.w );
	transformedMatrix.m_translation.x = ( m_iBasis.x * transformationMatrix.m_translation.x )
									  + ( m_jBasis.x * transformationMatrix.m_translation.y )
									  + ( m_kBasis.x * transformationMatrix.m_translation.z )
									  + ( m_translation.x * transformationMatrix.m_translation.w );

	// second row
	transformedMatrix.m_iBasis.y = ( m_iBasis.y * transformationMatrix.m_iBasis.x )
								 + ( m_jBasis.y * transformationMatrix.m_iBasis.y )
								 + ( m_kBasis.y * transformationMatrix.m_iBasis.z )
								 + ( m_translation.y * transformationMatrix.m_iBasis.w );
	transformedMatrix.m_jBasis.y = ( m_iBasis.y * transformationMatrix.m_jBasis.x )
								 + ( m_jBasis.y * transformationMatrix.m_jBasis.y )
								 + ( m_kBasis.y * transformationMatrix.m_jBasis.z )
								 + ( m_translation.y * transformationMatrix.m_jBasis.w );
	transformedMatrix.m_kBasis.y = ( m_iBasis.y * transformationMatrix.m_kBasis.x )
								 + ( m_jBasis.y * transformationMatrix.m_kBasis.y )
								 + ( m_kBasis.y * transformationMatrix.m_kBasis.z )
								 + ( m_translation.y * transformationMatrix.m_kBasis.w );
	transformedMatrix.m_translation.y = ( m_iBasis.y * transformationMatrix.m_translation.x )
								      + ( m_jBasis.y * transformationMatrix.m_translation.y )
								      + ( m_kBasis.y * transformationMatrix.m_translation.z )
								      + ( m_translation.y * transformationMatrix.m_translation.w );

	// third row
	transformedMatrix.m_iBasis.z = ( m_iBasis.z * transformationMatrix.m_iBasis.x )
								 + ( m_jBasis.z * transformationMatrix.m_iBasis.y )
								 + ( m_kBasis.z * transformationMatrix.m_iBasis.z )
								 + ( m_translation.z * transformationMatrix.m_iBasis.w );
	transformedMatrix.m_jBasis.z = ( m_iBasis.z * transformationMatrix.m_jBasis.x )
								 + ( m_jBasis.z * transformationMatrix.m_jBasis.y )
								 + ( m_kBasis.z * transformationMatrix.m_jBasis.z )
								 + ( m_translation.z * transformationMatrix.m_jBasis.w );
	transformedMatrix.m_kBasis.z = ( m_iBasis.z * transformationMatrix.m_kBasis.x )
								 + ( m_jBasis.z * transformationMatrix.m_kBasis.y )
								 + ( m_kBasis.z * transformationMatrix.m_kBasis.z )
								 + ( m_translation.z * transformationMatrix.m_kBasis.w );
	transformedMatrix.m_translation.z = ( m_iBasis.z * transformationMatrix.m_translation.x )
									  + ( m_jBasis.z * transformationMatrix.m_translation.y )
									  + ( m_kBasis.z * transformationMatrix.m_translation.z )
									  + ( m_translation.z * transformationMatrix.m_translation.w );

	// fourth row
	transformedMatrix.m_iBasis.w = ( m_iBasis.w * transformationMatrix.m_iBasis.x )
								 + ( m_jBasis.w * transformationMatrix.m_iBasis.y )
								 + ( m_kBasis.w * transformationMatrix.m_iBasis.z )
								 + ( m_translation.w * transformationMatrix.m_iBasis.w );
	transformedMatrix.m_jBasis.w = ( m_iBasis.w * transformationMatrix.m_jBasis.x )
								 + ( m_jBasis.w * transformationMatrix.m_jBasis.y )
								 + ( m_kBasis.w * transformationMatrix.m_jBasis.z )
								 + ( m_translation.w * transformationMatrix.m_jBasis.w );
	transformedMatrix.m_kBasis.w = ( m_iBasis.w * transformationMatrix.m_kBasis.x )
								 + ( m_jBasis.w * transformationMatrix.m_kBasis.y )
								 + ( m_kBasis.w * transformationMatrix.m_kBasis.z )
								 + ( m_translation.w * transformationMatrix.m_kBasis.w );
	transformedMatrix.m_translation.w = ( m_iBasis.w * transformationMatrix.m_translation.x )
									  + ( m_jBasis.w * transformationMatrix.m_translation.y )
									  + ( m_kBasis.w * transformationMatrix.m_translation.z )
									  + ( m_translation.w * transformationMatrix.m_translation.w );

	return transformedMatrix;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
void Matrix4< primitiveType >::MakeIdentity()
{
	m_iBasis.x = (primitiveType) 1;
	m_iBasis.y = (primitiveType) 0;
	m_iBasis.z = (primitiveType) 0;
	m_iBasis.w = (primitiveType) 0;
				 
	m_jBasis.x = (primitiveType) 0;
	m_jBasis.y = (primitiveType) 1;
	m_jBasis.z = (primitiveType) 0;
	m_jBasis.w = (primitiveType) 0;
				 
	m_kBasis.x = (primitiveType) 0;
	m_kBasis.y = (primitiveType) 0;
	m_kBasis.z = (primitiveType) 1;
	m_kBasis.w = (primitiveType) 0;

	m_translation.x = (primitiveType) 0;
	m_translation.y = (primitiveType) 0;
	m_translation.z = (primitiveType) 0;
	m_translation.w = (primitiveType) 1;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
void Matrix4< primitiveType >::Transform( const Matrix4< primitiveType >& transformationMatrix )
{
	// first row
	m_iBasis.x = ( m_iBasis.x * transformationMatrix.m_iBasis.x )
			   + ( m_jBasis.x * transformationMatrix.m_iBasis.y )
			   + ( m_kBasis.x * transformationMatrix.m_iBasis.z )
			   + ( m_translation.x * transformationMatrix.m_iBasis.w );
	m_jBasis.x = ( m_iBasis.x * transformationMatrix.m_jBasis.x )
			   + ( m_jBasis.x * transformationMatrix.m_jBasis.y )
			   + ( m_kBasis.x * transformationMatrix.m_jBasis.z )
			   + ( m_translation.x * transformationMatrix.m_jBasis.w );
	m_kBasis.x = ( m_iBasis.x * transformationMatrix.m_kBasis.x )
			   + ( m_jBasis.x * transformationMatrix.m_kBasis.y )
			   + ( m_kBasis.x * transformationMatrix.m_kBasis.z )
			   + ( m_translation.x * transformationMatrix.m_kBasis.w );
	m_translation.x = ( m_iBasis.x * transformationMatrix.m_translation.x )
					+ ( m_jBasis.x * transformationMatrix.m_translation.y )
					+ ( m_kBasis.x * transformationMatrix.m_translation.z )
					+ ( m_translation.x * transformationMatrix.m_translation.w );

	// second row
	m_iBasis.y = ( m_iBasis.y * transformationMatrix.m_iBasis.x )
			   + ( m_jBasis.y * transformationMatrix.m_iBasis.y )
			   + ( m_kBasis.y * transformationMatrix.m_iBasis.z )
			   + ( m_translation.y * transformationMatrix.m_iBasis.w );
	m_jBasis.y = ( m_iBasis.y * transformationMatrix.m_jBasis.x )
			   + ( m_jBasis.y * transformationMatrix.m_jBasis.y )
			   + ( m_kBasis.y * transformationMatrix.m_jBasis.z )
			   + ( m_translation.y * transformationMatrix.m_jBasis.w );
	m_kBasis.y = ( m_iBasis.y * transformationMatrix.m_kBasis.x )
			   + ( m_jBasis.y * transformationMatrix.m_kBasis.y )
			   + ( m_kBasis.y * transformationMatrix.m_kBasis.z )
			   + ( m_translation.y * transformationMatrix.m_kBasis.w );
	m_translation.y = ( m_iBasis.y * transformationMatrix.m_translation.x )
					+ ( m_jBasis.y * transformationMatrix.m_translation.y )
					+ ( m_kBasis.y * transformationMatrix.m_translation.z )
					+ ( m_translation.y * transformationMatrix.m_translation.w );

	// third row
	m_iBasis.z = ( m_iBasis.z * transformationMatrix.m_iBasis.x )
			   + ( m_jBasis.z * transformationMatrix.m_iBasis.y )
			   + ( m_kBasis.z * transformationMatrix.m_iBasis.z )
			   + ( m_translation.z * transformationMatrix.m_iBasis.w );
	m_jBasis.z = ( m_iBasis.z * transformationMatrix.m_jBasis.x )
			   + ( m_jBasis.z * transformationMatrix.m_jBasis.y )
			   + ( m_kBasis.z * transformationMatrix.m_jBasis.z )
			   + ( m_translation.z * transformationMatrix.m_jBasis.w );
	m_kBasis.z = ( m_iBasis.z * transformationMatrix.m_kBasis.x )
			   + ( m_jBasis.z * transformationMatrix.m_kBasis.y )
			   + ( m_kBasis.z * transformationMatrix.m_kBasis.z )
			   + ( m_translation.z * transformationMatrix.m_kBasis.w );
	m_translation.z = ( m_iBasis.z * transformationMatrix.m_translation.x )
					+ ( m_jBasis.z * transformationMatrix.m_translation.y )
					+ ( m_kBasis.z * transformationMatrix.m_translation.z )
					+ ( m_translation.z * transformationMatrix.m_translation.w );

	// fourth row
	m_iBasis.w = ( m_iBasis.w * transformationMatrix.m_iBasis.x )
			   + ( m_jBasis.w * transformationMatrix.m_iBasis.y )
			   + ( m_kBasis.w * transformationMatrix.m_iBasis.z )
			   + ( m_translation.w * transformationMatrix.m_iBasis.w );
	m_jBasis.w = ( m_iBasis.w * transformationMatrix.m_jBasis.x )
			   + ( m_jBasis.w * transformationMatrix.m_jBasis.y )
			   + ( m_kBasis.w * transformationMatrix.m_jBasis.z )
			   + ( m_translation.w * transformationMatrix.m_jBasis.w );
	m_kBasis.w = ( m_iBasis.w * transformationMatrix.m_kBasis.x )
			   + ( m_jBasis.w * transformationMatrix.m_kBasis.y )
			   + ( m_kBasis.w * transformationMatrix.m_kBasis.z )
			   + ( m_translation.w * transformationMatrix.m_kBasis.w );
	m_translation.w = ( m_iBasis.w * transformationMatrix.m_translation.x )
					+ ( m_jBasis.w * transformationMatrix.m_translation.y )
					+ ( m_kBasis.w * transformationMatrix.m_translation.z )
					+ ( m_translation.w * transformationMatrix.m_translation.w );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
void Matrix4< primitiveType >::Translate( const Vector2& translation2D ) // z translation assumed to be 0
{
	Matrix4< primitiveType > translation = CreateTranslation( translation2D );
	Transform( translation );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
void Matrix4< primitiveType >::Translate( const Vector3& translation3D )
{
	Matrix4< primitiveType > translation = CreateTranslation( translation3D );
	Transform( translation );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
void Matrix4< primitiveType >::Scale( primitiveType uniformScale )
{
	Matrix4< primitiveType > translation = CreateScale( uniformScale );
	Transform( translation );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
void Matrix4< primitiveType >::Scale( const Vector2& nonUniformScale2D ) // z scale assumed to be 1
{
	Matrix4< primitiveType > translation = CreateScale( nonUniformScale2D );
	Transform( translation );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
void Matrix4< primitiveType >::Scale( const Vector3& nonUniformScale3D )
{
	Matrix4< primitiveType > translation = CreateScale( nonUniformScale3D );
	Transform( translation );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
void Matrix4< primitiveType >::RotateDegreesAboutX( primitiveType degrees )
{
	Matrix4< primitiveType > translation = CreateRotationDegreesAboutX( degrees );
	Transform( translation );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
void Matrix4< primitiveType >::RotateDegreesAboutY( primitiveType degrees )
{
	Matrix4< primitiveType > translation = CreateRotationDegreesAboutY( degrees );
	Transform( translation );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
void Matrix4< primitiveType >::RotateDegreesAboutZ( primitiveType degrees ) // Possibly also offered as RotateDegrees2D
{
	Matrix4< primitiveType > translation = CreateRotationDegreesAboutZ( degrees );
	Transform( translation );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
void Matrix4< primitiveType >::RotateRadiansAboutX( primitiveType radians )
{
	Matrix4< primitiveType > translation = CreateRotationRadiansAboutX( radians );
	Transform( translation );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
void Matrix4< primitiveType >::RotateRadiansAboutY( primitiveType radians )
{
	Matrix4< primitiveType > translation = CreateRotationRadiansAboutY( radians );
	Transform( translation );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename primitiveType >
void Matrix4< primitiveType >::RotateRadiansAboutZ( primitiveType radians ) // Possibly also offered as RotateRadians2D
{
	Matrix4< primitiveType > translation = CreateRotationRadiansAboutZ( radians );
	Transform( translation );
}
#endif