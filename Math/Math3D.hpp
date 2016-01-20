//=================================================================================
// Math3D.hpp
// Author: Tyler George
// Date  : February 18, 2015
//=================================================================================

#pragma once

#ifndef __included_Math3D__
#define __included_Math3D__

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/Sphere3D.hpp"
#include "Engine/Math/AABB3D.hpp"

bool DoSpheresOverlap( const Sphere3D& first, const Sphere3D& second );
bool DoAABB3DsOverlap( const AABB3D& first, const AABB3D& second );


#endif

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
inline bool DoSpheresOverlap( const Sphere3D& first, const Sphere3D& second )
{
	float sumRadii = first.m_radius + second.m_radius;
	float distSquared = CalcDistanceSquared( first.m_center, second.m_center );
	bool isOverlapping = distSquared < (sumRadii * sumRadii );
	return isOverlapping;
}

