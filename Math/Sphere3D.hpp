//=================================================================================
// Sphere3D.hpp
// Author: Tyler George
// Date  : February 18, 2015
//=================================================================================

#ifndef __included_Sphere3D__
#define __included_Sphere3D__

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/Vector3.hpp"

class Sphere3D
{
public:
	///---------------------------------------------------------------------------------
	/// Member Variables
	///---------------------------------------------------------------------------------
	Vector3 m_center;
	float	m_radius;

	///---------------------------------------------------------------------------------
	/// Static member variables
	///---------------------------------------------------------------------------------
	static const Sphere3D UNIT_SPHERE;


	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
	~Sphere3D() {}
	Sphere3D();
	Sphere3D( const Sphere3D& copy );
	explicit Sphere3D( float initialX, float initialY, float initialZ, float initialRadius );
	explicit Sphere3D( const Vector3& initialCenter, float initialRadius );


	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
	void MakeEmpty();
	void StretchToIncludePoint( const Vector3& point );
	void AddPadding( float paddingRadius );
	void Translate( const Vector3& translation );


	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------
	bool IsPointInside( const Vector3& point ) const;
	bool IsEmpty() const;


	///---------------------------------------------------------------------------------
	/// Operators
	///---------------------------------------------------------------------------------
	const Sphere3D	operator + ( const Vector3& translation )		const;
	const Sphere3D	operator - ( const Vector3& antiTranslation )	const;
	void			operator += ( const Vector3& translation );
	void			operator -= ( const Vector3& antiTranslation );

	///---------------------------------------------------------------------------------
	/// Friend
	///---------------------------------------------------------------------------------
	friend const Sphere3D Interpolate( const Sphere3D& start, const Sphere3D& end, float fractionFromStartToEnd );
	friend const Sphere3D Union( const Sphere3D& sphereA, const Sphere3D& sphereB );


};

///---------------------------------------------------------------------------------
/// Inline functions
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline Sphere3D::Sphere3D()
	: m_center( Vector3( 0.0f, 0.0f, 0.0f ) )
	, m_radius( -1.0f )
{

}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline Sphere3D::Sphere3D( const Sphere3D& copy )
	: m_center( copy.m_center )
	, m_radius( copy.m_radius )
{
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline Sphere3D::Sphere3D( float initialX, float initialY, float initialZ, float initialRadius )
	: m_center( Vector3( initialX, initialY, initialZ ) )
	, m_radius( initialRadius )
{
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline Sphere3D::Sphere3D( const Vector3& initialCenter, float initialRadius )
	: m_center( initialCenter )
	, m_radius( initialRadius )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline void Sphere3D::MakeEmpty()
{
	m_center = Vector3( 0.0f, 0.0f, 0.0f );
	m_radius = -1.0f;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Sphere3D::AddPadding( float paddingRadius ) 
{
	m_radius += paddingRadius;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Sphere3D::Translate( const Vector3& translation )
{
	m_center += translation;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline bool Sphere3D::IsPointInside( const Vector3& point ) const
{
	float distanceToPointSquared = CalcDistanceSquared( m_center, point );
	float radiusSquared = m_radius * m_radius;
	bool isInside = distanceToPointSquared < radiusSquared;
	return isInside;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline bool Sphere3D::IsEmpty() const
{
	if( m_center == Vector3( 0.0f, 0.0f, 0.0f ) && m_radius == -1.0f )
		return true;
	return false;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Sphere3D Sphere3D::operator+( const Vector3& translation ) const
{
	Sphere3D newDisc( m_center + translation, m_radius );
	return newDisc;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Sphere3D Sphere3D::operator-( const Vector3& antiTranslation ) const
{
	Sphere3D newDisc( m_center - antiTranslation, m_radius );
	return newDisc;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Sphere3D::operator+=( const Vector3& translation )
{
	Translate( translation );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void Sphere3D::operator-=( const Vector3& antiTranslation ) 
{
	Vector3 tranlation( -antiTranslation.x, -antiTranslation.y, -antiTranslation.z );
	Translate( tranlation );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Sphere3D Interpolate( const Sphere3D& start, const Sphere3D& end, float fractionFromStartToEnd )
{
	Vector3 centerInterpolated = Interpolate( start.m_center, end.m_center, fractionFromStartToEnd );
	float radiusInterpolated = Interpolate( start.m_radius, end.m_radius, fractionFromStartToEnd );
	return Sphere3D( centerInterpolated, radiusInterpolated );
}

#endif