//=================================================================================
// AABB3D.cpp
// Author: Tyler George
// Date  : February 18, 2015
//=================================================================================

#include "Engine/Math/AABB3D.hpp"

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
const AABB3D AABB3D::ZERO_TO_ONE( 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f );


///---------------------------------------------------------------------------------
/// long
///---------------------------------------------------------------------------------
void AABB3D::StretchToIncludePoint( const Vector3& point )
{
	if( m_mins.x > point.x )
		m_mins.x = point.x;
	else if( m_maxs.x < point.x )
		m_maxs.x = point.x;

	if( m_mins.y > point.y )
		m_mins.y = point.y;
	else if( m_maxs.y < point.y )
		m_maxs.y = point.y;

	if( m_mins.z > point.z )
		m_mins.z = point.z;
	else if( m_maxs.z < point.z )
		m_maxs.z = point.z;
}

///---------------------------------------------------------------------------------
/// long
///---------------------------------------------------------------------------------
bool AABB3D::IsPointInside( const Vector3& point ) const
{
	if( point.x < m_mins.x )
		return false;
	else if( point.x > m_maxs.x )
		return false;
	else if( point.y < m_mins.y )
		return false;
	else if( point.y > m_maxs.y )
		return false;
	else if( point.z < m_mins.z )
		return false;
	else if( point.z > m_maxs.z )
		return false;
	else return true;
}

///---------------------------------------------------------------------------------
/// slow
///---------------------------------------------------------------------------------
const Vector3 AABB3D::GetNormalizedPositionForPointWithinBox( const Vector3& point ) const
{
	Vector3 boxSize = CalcSize();
	float xRelativePos = point.x - m_mins.x;
	float yRelativePos = point.y - m_mins.y;
	float zRelativePos = point.z - m_mins.z;

	float normalizedXPos = xRelativePos / boxSize.x;
	float normalizedYPos = yRelativePos / boxSize.y;
	float normalizedZPos = zRelativePos / boxSize.z;

	Vector3 normalizedPos( normalizedXPos, normalizedYPos, normalizedZPos );
	return normalizedPos;

}