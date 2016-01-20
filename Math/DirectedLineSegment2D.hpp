//=================================================================================
// DirectedLineSegment2D.hpp
// Author: Tyler George
// Date  : January 23, 2015
//=================================================================================

#pragma once

#ifndef __included_DirectedLineSegment2D__
#define __included_DirectedLineSegment2D__

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/Vector2.hpp"

class DirectedLineSegment2D
{
public:

	///---------------------------------------------------------------------------------
	/// Member Variables
	///---------------------------------------------------------------------------------
	Vector2 m_start;
	Vector2 m_end;

	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
	~DirectedLineSegment2D() {}

	DirectedLineSegment2D() {}
	DirectedLineSegment2D( const DirectedLineSegment2D& copy );
	explicit DirectedLineSegment2D( float startX, float startY, float endX, float endY );
	explicit DirectedLineSegment2D( const Vector2& start, const Vector2& end );

	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------
	float			CalcLength()										const;
	float			CalcLengthSquared()									const;
	float			CalcHeadingDegrees()								const;
	float			CalcHeadingRadians()								const;
	const Vector2	GetPointAtParametricValue( float parametricValue )	const;

	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
	void SetStart( const Vector2& newStart );
	void SetEnd( const Vector2& newEnd );
	void SetStartandEnd( const Vector2& newStart, const Vector2& newEnd );
	void Translate( const Vector2& translation );

	///---------------------------------------------------------------------------------
	/// Operators
	///---------------------------------------------------------------------------------
	bool						operator ==( const DirectedLineSegment2D& segmentToEqual )		const;
	bool						operator !=( const DirectedLineSegment2D& segmentToNotEqual )	const;
	const DirectedLineSegment2D operator +( const Vector2& translation )						const;
	const DirectedLineSegment2D operator -( const Vector2& antiTranslation )					const;
	void						operator +=( const Vector2& translation );
	void						operator -=( const Vector2& antiTranslation );

	///---------------------------------------------------------------------------------
	/// Friend
	///---------------------------------------------------------------------------------
	friend const DirectedLineSegment2D Interpolate( const DirectedLineSegment2D& start, const DirectedLineSegment2D& end, float fractionFromStartToEnd );

};


///---------------------------------------------------------------------------------
/// Inline functions
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline DirectedLineSegment2D::DirectedLineSegment2D( const DirectedLineSegment2D& copy )
	: m_start( copy.m_start )
	, m_end( copy.m_end )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline DirectedLineSegment2D::DirectedLineSegment2D( float startX, float startY, float endX, float endY )
	: m_start( Vector2( startX, startY ) )
	, m_end( Vector2( endX, endY ) )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline DirectedLineSegment2D::DirectedLineSegment2D( const Vector2& start, const Vector2& end )
	: m_start( start )
	, m_end( end )
{
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline float DirectedLineSegment2D::CalcLengthSquared() const
{
	float xDiff = m_end.x - m_start.x;
	float yDiff = m_end.y - m_start.y;
	float lengthSquared = ( xDiff * xDiff ) + ( yDiff * yDiff );

	return lengthSquared;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const Vector2 DirectedLineSegment2D::GetPointAtParametricValue( float parametricValue ) const
{
	Vector2 startEndDifference = m_end - m_start;
	Vector2 offsetParametricValue = startEndDifference * parametricValue;
	
	Vector2 realParametricValue = offsetParametricValue + m_start;
	
	return realParametricValue;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void DirectedLineSegment2D::SetStart( const Vector2& newStart )
{
	m_start = newStart;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void DirectedLineSegment2D::SetEnd( const Vector2& newEnd )
{
	m_end = newEnd;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void DirectedLineSegment2D::SetStartandEnd( const Vector2& newStart, const Vector2& newEnd )
{
	m_start = newStart;
	m_end = newEnd;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void DirectedLineSegment2D::Translate( const Vector2& translation )
{
	m_start += translation;
	m_end += translation;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline bool DirectedLineSegment2D::operator==( const DirectedLineSegment2D& segmentToEqual ) const
{
	if( m_start == segmentToEqual.m_start && m_end == segmentToEqual.m_end )
		return true;
	return false;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline bool DirectedLineSegment2D::operator!=( const DirectedLineSegment2D& segmentToNotEqual ) const
{
	if( m_start != segmentToNotEqual.m_start || m_end != segmentToNotEqual.m_end )
		return true;
	return false;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const DirectedLineSegment2D DirectedLineSegment2D::operator +( const Vector2& translation ) const
{
	Vector2 newStart = m_start + translation;
	Vector2 newEnd = m_end + translation;
	DirectedLineSegment2D newSegment( newStart, newEnd );
	return newSegment;
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const DirectedLineSegment2D DirectedLineSegment2D::operator -( const Vector2& antiTranslation ) const
{
	Vector2 newStart = m_start - antiTranslation;
	Vector2 newEnd = m_end - antiTranslation;
	DirectedLineSegment2D newSegment( newStart, newEnd );
	return newSegment;
}
///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void DirectedLineSegment2D::operator +=( const Vector2& translation )
{
	Translate( translation );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline void DirectedLineSegment2D::operator -=( const Vector2& antiTranslation )
{
	Vector2 translation( -antiTranslation.x, -antiTranslation.y );
	Translate( translation );
}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline const DirectedLineSegment2D Interpolate( const DirectedLineSegment2D& start, const DirectedLineSegment2D& end, float fractionFromStartToEnd )
{
	Vector2 startsInterpolated = Interpolate( start.m_start, end.m_start, fractionFromStartToEnd );
	Vector2 endsInterpolated = Interpolate( start.m_end, end.m_end, fractionFromStartToEnd );
	return DirectedLineSegment2D( startsInterpolated, endsInterpolated );
}

#endif