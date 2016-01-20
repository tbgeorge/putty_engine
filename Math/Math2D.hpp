//=================================================================================
// Math2D.hpp
// Author: Tyler George
// Date  : January 21, 2015
//=================================================================================

#pragma once

#ifndef __included_Math2D__
#define __included_Math2D__

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/Disc2D.hpp"
#include "Engine/Math/AABB2D.hpp"

bool DoDiscsOverlap( const Disc2D& first, const Disc2D& second );
bool DoAABB2DsOverlap( const AABB2D& first, const AABB2D& second );
bool DoShapesOverlap( const AABB2D& box, const Disc2D& disc );
bool DoShapesOverlap( const Disc2D& disc, const AABB2D& box );

#endif

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline bool DoDiscsOverlap( const Disc2D& first, const Disc2D& second )
{
	float sumRadii = first.m_radius + second.m_radius;
	float distSquared = CalcDistanceSquared( first.m_center, second.m_center );
	bool isOverlapping = distSquared < (sumRadii * sumRadii );
	return isOverlapping;
}

