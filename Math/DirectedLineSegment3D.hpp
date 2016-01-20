//=================================================================================
// DirectedLineSegment3D.hpp
// Author: Tyler George
// Date  : February 18, 2015
//=================================================================================

#pragma once

#ifndef __included_DirectedLineSegment3D__
#define __included_DirectedLineSegment3D__

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/Vector3.hpp"

class DirectedLineSegment3D
{
public:

	///---------------------------------------------------------------------------------
	/// Member Variables
	///---------------------------------------------------------------------------------
	Vector3 m_start;
	Vector3 m_end;

	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
	~DirectedLineSegment3D() {}

	DirectedLineSegment3D() {}
	DirectedLineSegment3D( const DirectedLineSegment3D& copy );
	explicit DirectedLineSegment3D( float startX, float startY, float startZ, float endX, float endY, float endZ );
	explicit DirectedLineSegment3D( const Vector3& start, const Vector3& end );

	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------
	float			CalcLength()										const;
	float			CalcLengthSquared()									const;
	const Vector3	GetPointAtParametricValue( float parametricValue )	const;

	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
	void SetStart( const Vector3& newStart );
	void SetEnd( const Vector3& newEnd );
	void SetStartandEnd( const Vector3& newStart, const Vector3& newEnd );
	void Translate( const Vector3& translation );

	///---------------------------------------------------------------------------------
	/// Operators
	///---------------------------------------------------------------------------------
	bool						operator ==( const DirectedLineSegment3D& segmentToEqual )		const;
	bool						operator !=( const DirectedLineSegment3D& segmentToNotEqual )	const;
	const DirectedLineSegment3D operator +( const Vector3& translation )						const;
	const DirectedLineSegment3D operator -( const Vector3& antiTranslation )					const;
	void						operator +=( const Vector3& translation );
	void						operator -=( const Vector3& antiTranslation );

	///---------------------------------------------------------------------------------
	/// Friend
	///---------------------------------------------------------------------------------
	friend const DirectedLineSegment3D Interpolate( const DirectedLineSegment3D& start, const DirectedLineSegment3D& end, float fractionFromStartToEnd );

};


///---------------------------------------------------------------------------------
/// Inline functions
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline DirectedLineSegment3D::DirectedLineSegment3D( const DirectedLineSegment3D& copy )
	: m_start( copy.m_start )
	, m_end( copy.m_end )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline DirectedLineSegment3D::DirectedLineSegment3D( float startX, float startY, float startZ, float endX, float endY, float endZ )
	: m_start( Vector3( startX, startY, startZ ) )
	, m_end( Vector3( endX, endY, endZ ) )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline DirectedLineSegment3D::DirectedLineSegment3D( const Vector3& start, const Vector3& end )
	: m_start( start )
	, m_end( end )
{
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline float DirectedLineSegment3D::CalcLengthSquared() const
{
	float xDiff = m_end.x - m_start.x;
	float yDiff = m_end.y - m_start.y;
	float zDiff = m_end.z - m_start.z;
	float lengthSquared = ( xDiff * xDiff ) + ( yDiff * yDiff ) + ( zDiff * zDiff );

	return lengthSquared;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector3 DirectedLineSegment3D::GetPointAtParametricValue( float parametricValue ) const
{
	Vector3 startEndDifference = m_end - m_start;
	Vector3 offsetParametricValue = startEndDifference * parametricValue;
	Vector3 realParametricValue = offsetParametricValue + m_start;
	return realParametricValue;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void DirectedLineSegment3D::SetStart( const Vector3& newStart )
{
	m_start = newStart;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void DirectedLineSegment3D::SetEnd( const Vector3& newEnd )
{
	m_end = newEnd;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void DirectedLineSegment3D::SetStartandEnd( const Vector3& newStart, const Vector3& newEnd )
{
	m_start = newStart;
	m_end = newEnd;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void DirectedLineSegment3D::Translate( const Vector3& translation )
{
	m_start += translation;
	m_end += translation;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline bool DirectedLineSegment3D::operator==( const DirectedLineSegment3D& segmentToEqual ) const
{
	if( m_start == segmentToEqual.m_start && m_end == segmentToEqual.m_end )
		return true;
	return false;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline bool DirectedLineSegment3D::operator!=( const DirectedLineSegment3D& segmentToNotEqual ) const
{
	if( m_start != segmentToNotEqual.m_start || m_end != segmentToNotEqual.m_end )
		return true;
	return false;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const DirectedLineSegment3D DirectedLineSegment3D::operator +( const Vector3& translation ) const
{
	Vector3 newStart = m_start + translation;
	Vector3 newEnd = m_end + translation;
	DirectedLineSegment3D newSegment( newStart, newEnd );
	return newSegment;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const DirectedLineSegment3D DirectedLineSegment3D::operator -( const Vector3& antiTranslation ) const
{
	Vector3 newStart = m_start - antiTranslation;
	Vector3 newEnd = m_end - antiTranslation;
	DirectedLineSegment3D newSegment( newStart, newEnd );
	return newSegment;
}
///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void DirectedLineSegment3D::operator +=( const Vector3& translation )
{
	Translate( translation );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void DirectedLineSegment3D::operator -=( const Vector3& antiTranslation )
{
	Vector3 translation( -antiTranslation.x, -antiTranslation.y, -antiTranslation.z );
	Translate( translation );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const DirectedLineSegment3D Interpolate( const DirectedLineSegment3D& start, const DirectedLineSegment3D& end, float fractionFromStartToEnd )
{
	Vector3 startsInterpolated = Interpolate( start.m_start, end.m_start, fractionFromStartToEnd );
	Vector3 endsInterpolated = Interpolate( start.m_end, end.m_end, fractionFromStartToEnd );
	return DirectedLineSegment3D( startsInterpolated, endsInterpolated );
}

#endif