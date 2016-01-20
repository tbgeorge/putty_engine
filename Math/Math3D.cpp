//=================================================================================
// Math3D.cpp
// Author: Tyler George
// Date  : February 18, 2015
//=================================================================================

#include "Engine/Math/Math3D.hpp"


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool DoAABB3DsOverlap( const AABB3D& first, const AABB3D& second )
{
	if( first.m_mins.x > second.m_maxs.x )
		return false;

	else if( first.m_maxs.x < second.m_mins.x )
		return false;

	else if( first.m_mins.y > second.m_maxs.y )
		return false;

	else if( first.m_maxs.y < second.m_mins.y )
		return false;

	else if( first.m_mins.z > second.m_maxs.z )
		return false;

	else if( first.m_maxs.y < second.m_mins.z )
		return false;

	return true;
}