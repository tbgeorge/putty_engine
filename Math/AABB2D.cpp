//=================================================================================
// AABB2.cpp
// Author: Tyler George
// Date  : January 21, 2015
//=================================================================================
#include "Engine/Math/AABB2D.hpp"

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
const AABB2D AABB2D::ZERO_TO_ONE( 0.0f, 0.0f, 1.0f, 1.0f );


///---------------------------------------------------------------------------------
/// long
///---------------------------------------------------------------------------------
void AABB2D::StretchToIncludePoint( const Vector2& point )
{
	if( m_mins.x > point.x )
		m_mins.x = point.x;
	else if( m_maxs.x < point.x )
		m_maxs.x = point.x;

	if( m_mins.y > point.y )
		m_mins.y = point.y;
	else if( m_maxs.y < point.y )
		m_maxs.y = point.y;
}

///---------------------------------------------------------------------------------
/// long
///---------------------------------------------------------------------------------
bool AABB2D::IsPointInside( const Vector2& point ) const
{
	if( point.x < m_mins.x )
		return false;
	else if( point.x > m_maxs.x )
		return false;
	else if( point.y < m_mins.y )
		return false;
	else if( point.y > m_maxs.y )
		return false;
	else return true;
}

///---------------------------------------------------------------------------------
/// slow
///---------------------------------------------------------------------------------
const Vector2 AABB2D::GetNormalizedPositionForPointWithinBox( const Vector2& point ) const
{
	Vector2 boxSize = CalcSize();
	float xRelativePos = point.x - m_mins.x;
	float yRelativePos = point.y - m_mins.y;

	Vector2 normalizedPos( xRelativePos / boxSize.x, yRelativePos / boxSize.y );
	return normalizedPos;

}