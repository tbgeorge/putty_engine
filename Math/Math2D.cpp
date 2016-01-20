//=================================================================================
// Math2D.cpp
// Author: Tyler George
// Date  : January 21, 2015
//=================================================================================

#include "Engine/Math/Math2D.hpp"


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool DoAABB2DsOverlap( const AABB2D& first, const AABB2D& second )
{
	if( first.m_mins.x > second.m_maxs.x )
		return false;

	else if( first.m_maxs.x < second.m_mins.x )
		return false;

	else if( first.m_mins.y > second.m_maxs.y )
		return false;

	else if( first.m_maxs.y < second.m_mins.y )
		return false;

	return true;
}

///---------------------------------------------------------------------------------
/// 
///---------------------------------------------------------------------------------
bool DoShapesOverlap( const Disc2D& disc, const AABB2D& box )
{
	if( box.IsPointInside( disc.m_center ) )
		return true;
	else if( disc.IsPointInside( Vector2( box.m_mins.x, box.m_mins.y ) ) )
		return true;
	else if( disc.IsPointInside( Vector2( box.m_mins.x, box.m_maxs.y ) ) ) 
		return true;
	else if( disc.IsPointInside( Vector2( box.m_maxs.x, box.m_mins.y ) ) ) 
		return true;
	else if( disc.IsPointInside( Vector2( box.m_maxs.x, box.m_maxs.y ) ) )
		return true;
	else if( box.IsPointInside( Vector2( disc.m_center.x + disc.m_radius, disc.m_center.y ) ) )
		return true;
	else if( box.IsPointInside( Vector2( disc.m_center.x - disc.m_radius, disc.m_center.y ) ) )
		return true;
	else if( box.IsPointInside( Vector2( disc.m_center.x, disc.m_center.y + disc.m_radius ) ) )
		return true;
	else if( box.IsPointInside( Vector2( disc.m_center.x, disc.m_center.y - disc.m_radius ) ) )
		return true;
	else 
		return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool DoShapesOverlap( const AABB2D& box, const Disc2D& disc )
{
	return DoShapesOverlap( disc, box );
}