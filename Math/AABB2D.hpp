//=================================================================================
// AABB2D.hpp
// Author: Tyler George
// Date  : January 21, 2015
//=================================================================================

#pragma once

#ifndef __included_AABB2D__
#define __included_AABB2D__

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/Vector2.hpp"

class AABB2D
{
public:
	///---------------------------------------------------------------------------------
	/// Member Variables
	///---------------------------------------------------------------------------------
	Vector2 m_mins;
	Vector2 m_maxs;

	static const AABB2D ZERO_TO_ONE;


	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
	~AABB2D() {}

	AABB2D() {}
	AABB2D( const AABB2D& copy );
	explicit AABB2D( float initialX, float initialY );
	explicit AABB2D( float minX, float minY, float maxX, float maxY );
	explicit AABB2D( const Vector2& mins, const Vector2& maxs );
	explicit AABB2D( const Vector2& center, float radiusX, float radiusY );


	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
	void StretchToIncludePoint( const Vector2& point );
	void AddPadding( float xPaddingRadius, float yPaddingRadius );
	void Translate( const Vector2& translation );

	
	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------
	bool			IsPointInside( const Vector2& point )										const;
	const Vector2	CalcSize()																	const;
	const Vector2	CalcCenter()																const;
	const Vector2	GetPointAtNormalizedPositionWithinBox( const Vector2& normalizedPosition )	const;
	const Vector2	GetNormalizedPositionForPointWithinBox( const Vector2& point )				const;


	///---------------------------------------------------------------------------------
	/// Operators
	///---------------------------------------------------------------------------------
	const AABB2D operator + ( const Vector2& translation )		const;
	const AABB2D operator - ( const Vector2& antiTranslation )	const;
	void		operator += ( const Vector2& translation );
	void		operator -= ( const Vector2& antiTranslation );

	///---------------------------------------------------------------------------------
	/// Friend
	///---------------------------------------------------------------------------------
	friend const AABB2D Interpolate( const AABB2D& start, const AABB2D& end, float fractionFromStartToEnd );
};

///---------------------------------------------------------------------------------
/// Inline Functions
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline AABB2D::AABB2D( const AABB2D& copy )
	: m_mins( copy.m_mins )
	, m_maxs( copy.m_maxs )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline AABB2D::AABB2D( float initialX, float initialY ) 
	: m_mins( initialX, initialY )
	, m_maxs( initialX, initialY )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline AABB2D::AABB2D( float minX, float minY, float maxX, float maxY )
	: m_mins( minX, minY )
	, m_maxs( maxX, maxY )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline AABB2D::AABB2D( const Vector2& mins, const Vector2& maxs )
	: m_mins( mins )
	, m_maxs( maxs )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline AABB2D::AABB2D( const Vector2& center, float radiusX, float radiusY )
{
	m_mins.x = center.x - radiusX;
	m_mins.y = center.y - radiusY;
	m_maxs.x = center.x + radiusX;
	m_maxs.y = center.y + radiusY;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void AABB2D::AddPadding( float xPaddingRadius, float yPaddingRadius )
{
	m_mins.x -= xPaddingRadius;
	m_mins.y -= yPaddingRadius;
	m_maxs.x += xPaddingRadius;
	m_maxs.y += yPaddingRadius;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void AABB2D::Translate( const Vector2& translation )
{
	m_mins += translation;
	m_maxs += translation;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector2 AABB2D::CalcSize() const
{
	Vector2 boxSize = m_maxs - m_mins;
	return boxSize;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector2 AABB2D::CalcCenter() const
{
	Vector2 boxSize = CalcSize();
	Vector2 center( boxSize.x * 0.5f, boxSize.y * 0.5f );
	return center;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector2 AABB2D::GetPointAtNormalizedPositionWithinBox( const Vector2& normalizedPosition ) const
{
	Vector2 boxSize = CalcSize();
	float xPos = m_mins.x + ( boxSize.x * normalizedPosition.x );
	float yPos = m_mins.y + ( boxSize.y * normalizedPosition.y );
	Vector2 positionAtNormalizedPoint( xPos, yPos );
	return positionAtNormalizedPoint;
}


///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const AABB2D AABB2D::operator + ( const Vector2& translation ) const
{
	Vector2 newMins = m_mins + translation;
	Vector2 newMaxs = m_maxs + translation;
	AABB2D translatedBox( newMins, newMaxs );
	return translatedBox;

}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const AABB2D AABB2D::operator - ( const Vector2& antiTranslation ) const
{
	Vector2 newMins = m_mins - antiTranslation;
	Vector2 newMaxs = m_maxs - antiTranslation;
	AABB2D antiTranslatedBox( newMins, newMaxs );
	return antiTranslatedBox;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void AABB2D::operator += ( const Vector2& translation )
{
	Translate( translation );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void AABB2D::operator -= ( const Vector2& antiTranslation )
{
	Vector2 translation( -antiTranslation.x, -antiTranslation.y );
	Translate( translation );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const AABB2D Interpolate( const AABB2D& start, const AABB2D& end, float fractionFromStartToEnd )
{
	Vector2 minsInterpolated = Interpolate( start.m_mins, end.m_mins, fractionFromStartToEnd );
	Vector2 maxsInterpolated = Interpolate( start.m_maxs, end.m_maxs, fractionFromStartToEnd );
	return AABB2D( minsInterpolated, maxsInterpolated );
}

#endif