//=================================================================================
// AABB3D.hpp
// Author: Tyler George
// Date  : February 18, 2015
//=================================================================================

#pragma once

#ifndef __included_AABB3D__
#define __included_AABB3D__

#include "Engine/Common/EngineCommon.hpp"
#include "Vector3.hpp"

class AABB3D
{
public:
	///---------------------------------------------------------------------------------
	/// Member Variables
	///---------------------------------------------------------------------------------
	Vector3 m_mins;
	Vector3 m_maxs;

	static const AABB3D ZERO_TO_ONE;


	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
	~AABB3D() {}

	AABB3D() {}
	AABB3D( const AABB3D& copy );
	explicit AABB3D( float initialX, float initialY, float initialZ );
	explicit AABB3D( float minX, float minY, float minZ, float maxX, float maxY, float maxZ );
	explicit AABB3D( const Vector3& mins, const Vector3& maxs );
	explicit AABB3D( const Vector3& center, float radiusX, float radiusY, float radiusZ );


	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
	void StretchToIncludePoint( const Vector3& point );
	void AddPadding( float xPaddingRadius, float yPaddingRadius, float zPaddingRadius );
	void Translate( const Vector3& translation );


	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------
	bool			IsPointInside( const Vector3& point )										const;
	const Vector3	CalcSize()																	const;
	const Vector3	CalcCenter()																const;
	const Vector3	GetPointAtNormalizedPositionWithinBox( const Vector3& normalizedPosition )	const;
	const Vector3	GetNormalizedPositionForPointWithinBox( const Vector3& point )				const;


	///---------------------------------------------------------------------------------
	/// Operators
	///---------------------------------------------------------------------------------
	const AABB3D operator + ( const Vector3& translation )		const;
	const AABB3D operator - ( const Vector3& antiTranslation )	const;
	void		operator += ( const Vector3& translation );
	void		operator -= ( const Vector3& antiTranslation );

	///---------------------------------------------------------------------------------
	/// Friend
	///---------------------------------------------------------------------------------
	friend const AABB3D Interpolate( const AABB3D& start, const AABB3D& end, float fractionFromStartToEnd );
};

///---------------------------------------------------------------------------------
/// Inline Functions
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline AABB3D::AABB3D( const AABB3D& copy )
	: m_mins( copy.m_mins )
	, m_maxs( copy.m_maxs )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline AABB3D::AABB3D( float initialX, float initialY, float initialZ ) 
	: m_mins( initialX, initialY, initialZ )
	, m_maxs( initialX, initialY, initialZ )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline AABB3D::AABB3D( float minX, float minY, float minZ, float maxX, float maxY, float maxZ )
	: m_mins( minX, minY, minZ )
	, m_maxs( maxX, maxY, maxZ )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline AABB3D::AABB3D( const Vector3& mins, const Vector3& maxs )
	: m_mins( mins )
	, m_maxs( maxs )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline AABB3D::AABB3D( const Vector3& center, float radiusX, float radiusY, float radiusZ )
{
	m_mins.x = center.x - radiusX;
	m_mins.y = center.y - radiusY;
	m_mins.z = center.z - radiusZ;
	m_maxs.x = center.x + radiusX;
	m_maxs.y = center.y + radiusY;
	m_maxs.z = center.z + radiusZ;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void AABB3D::AddPadding( float xPaddingRadius, float yPaddingRadius, float zPaddingRadius )
{
	m_mins.x -= xPaddingRadius;
	m_mins.y -= yPaddingRadius;
	m_mins.z -= zPaddingRadius;
	m_maxs.x += xPaddingRadius;
	m_maxs.y += yPaddingRadius;
	m_maxs.z += zPaddingRadius;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void AABB3D::Translate( const Vector3& translation )
{
	m_mins += translation;
	m_maxs += translation;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector3 AABB3D::CalcSize() const
{
	Vector3 boxSize = m_maxs - m_mins;
	return boxSize;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector3 AABB3D::CalcCenter() const
{
	Vector3 boxSize = CalcSize();
	Vector3 center( boxSize.x * 0.5f, boxSize.y * 0.5f, boxSize.z * 0.5f );
	return center;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector3 AABB3D::GetPointAtNormalizedPositionWithinBox( const Vector3& normalizedPosition ) const
{
	Vector3 boxSize = CalcSize();
	float xPos = m_mins.x + ( boxSize.x * normalizedPosition.x );
	float yPos = m_mins.y + ( boxSize.y * normalizedPosition.y );
	float zPos = m_mins.z + ( boxSize.z * normalizedPosition.z );
	Vector3 positionAtNormalizedPoint( xPos, yPos, zPos );
	return positionAtNormalizedPoint;
}


///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const AABB3D AABB3D::operator + ( const Vector3& translation ) const
{
	Vector3 newMins = m_mins + translation;
	Vector3 newMaxs = m_maxs + translation;
	AABB3D translatedBox( newMins, newMaxs );
	return translatedBox;

}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const AABB3D AABB3D::operator - ( const Vector3& antiTranslation ) const
{
	Vector3 newMins = m_mins - antiTranslation;
	Vector3 newMaxs = m_maxs - antiTranslation;
	AABB3D antiTranslatedBox( newMins, newMaxs );
	return antiTranslatedBox;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void AABB3D::operator += ( const Vector3& translation )
{
	Translate( translation );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void AABB3D::operator -= ( const Vector3& antiTranslation )
{
	Vector3 translation( -antiTranslation.x, -antiTranslation.y, -antiTranslation.z );
	Translate( translation );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const AABB3D Interpolate( const AABB3D& start, const AABB3D& end, float fractionFromStartToEnd )
{
	Vector3 minsInterpolated = Interpolate( start.m_mins, end.m_mins, fractionFromStartToEnd );
	Vector3 maxsInterpolated = Interpolate( start.m_maxs, end.m_maxs, fractionFromStartToEnd );
	return AABB3D( minsInterpolated, maxsInterpolated );
}

#endif