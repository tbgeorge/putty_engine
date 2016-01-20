//=================================================================================
// Disc2D.hpp
// Author: Tyler George
// Date  : January 21, 2015
//=================================================================================

#ifndef __included_Disc2D__
#define __included_Disc2D__

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/Vector2.hpp"

class Disc2D
{
public:
	///---------------------------------------------------------------------------------
	/// Member Variables
	///---------------------------------------------------------------------------------
	Vector2 m_center;
	float	m_radius;

	///---------------------------------------------------------------------------------
	/// Static member variables
	///---------------------------------------------------------------------------------
	static const Disc2D UNIT_CIRCLE;


	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
	~Disc2D() {}
	Disc2D() {}
	Disc2D( const Disc2D& copy );
	explicit Disc2D( float initialX, float initialY, float initialRadius );
	explicit Disc2D( const Vector2& initialCenter, float initialRadius );


	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
	void StretchToIncludePoint( const Vector2& point );
	void AddPadding( float paddingRadius );
	void Translate( const Vector2& translation );


	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------
	bool IsPointInside( const Vector2& point ) const;


	///---------------------------------------------------------------------------------
	/// Operators
	///---------------------------------------------------------------------------------
	const Disc2D	operator + ( const Vector2& translation )		const;
	const Disc2D	operator - ( const Vector2& antiTranslation )	const;
	void			operator += ( const Vector2& translation );
	void			operator -= ( const Vector2& antiTranslation );

	///---------------------------------------------------------------------------------
	/// Friend
	///---------------------------------------------------------------------------------
	friend const Disc2D Interpolate( const Disc2D& start, const Disc2D& end, float fractionFromStartToEnd );


};

///---------------------------------------------------------------------------------
/// Inline functions
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline Disc2D::Disc2D( const Disc2D& copy )
	: m_center( copy.m_center )
	, m_radius( copy.m_radius )
{
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline Disc2D::Disc2D( float initialX, float initialY, float initialRadius )
	: m_center( Vector2( initialX, initialY ) )
	, m_radius( initialRadius )
{
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline Disc2D::Disc2D( const Vector2& initialCenter, float initialRadius )
	: m_center( initialCenter )
	, m_radius( initialRadius )
{
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Disc2D::AddPadding( float paddingRadius ) 
{
	m_radius += paddingRadius;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Disc2D::Translate( const Vector2& translation )
{
	m_center += translation;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline bool Disc2D::IsPointInside( const Vector2& point ) const
{
	float distanceToPointSquared = CalcDistanceSquared( m_center, point );
	float radiusSquared = m_radius * m_radius;
	bool isInside = distanceToPointSquared < radiusSquared;
	return isInside;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Disc2D Disc2D::operator+( const Vector2& translation ) const
{
	Disc2D newDisc( m_center + translation, m_radius );
	return newDisc;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Disc2D Disc2D::operator-( const Vector2& antiTranslation ) const
{
	Disc2D newDisc( m_center - antiTranslation, m_radius );
	return newDisc;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Disc2D::operator+=( const Vector2& translation )
{
	Translate( translation );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Disc2D::operator-=( const Vector2& antiTranslation ) 
{
	Vector2 tranlation( -antiTranslation.x, -antiTranslation.y );
	Translate( tranlation );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Disc2D Interpolate( const Disc2D& start, const Disc2D& end, float fractionFromStartToEnd )
{
	Vector2 centerInterpolated = Interpolate( start.m_center, end.m_center, fractionFromStartToEnd );
	float radiusInterpolated = Interpolate( start.m_radius, end.m_radius, fractionFromStartToEnd );
	return Disc2D( centerInterpolated, radiusInterpolated );
}

#endif