//=================================================================================
// Sphere3D.cpp
// Author: Tyler George
// Date  : February 18, 2015
//=================================================================================

#include "Engine/Math/Sphere3D.hpp"

const Sphere3D Sphere3D::UNIT_SPHERE( Vector3( 0.0f, 0.0f, 0.0f ), 1.0f );

///---------------------------------------------------------------------------------
/// very slow ( CalcDistance uses sqrt )
///---------------------------------------------------------------------------------
void Sphere3D::StretchToIncludePoint( const Vector3& point )
{
	if( !IsPointInside( point ) )
		m_radius = CalcDistance( m_center, point );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
const Sphere3D Union( const Sphere3D& sphereA, const Sphere3D& sphereB )
{
	Sphere3D newSphere;
	newSphere.MakeEmpty();

	if( sphereA.IsEmpty() && sphereB.IsEmpty() )
		return newSphere;

	else if( sphereA.IsEmpty() && !sphereB.IsEmpty() )
		newSphere = sphereB;

	else if( sphereB.IsEmpty() && !sphereA.IsEmpty() )
		newSphere = sphereA;

	else
	{
		Vector3 newCenter;
		float newRadius = 0.0f;

		Vector3 displacement = sphereB.m_center - sphereA.m_center;
		float distance = displacement.CalcLength();

		// one sphere is inside the other
		if( distance + sphereA.m_radius < sphereB.m_radius || distance + sphereB.m_radius < sphereA.m_radius )
		{
			if( sphereA.m_radius > sphereB.m_radius )
			{
				newCenter = sphereA.m_center;
				newRadius = sphereA.m_radius;
			}
			else if( sphereB.m_radius > sphereA.m_radius )
			{
				newCenter = sphereB.m_center;
				newRadius = sphereB.m_radius;
			}
		}

		else
		{
			Vector3 dispNormalized = displacement;
			dispNormalized.Normalize();

			Vector3 edgePointA = sphereA.m_center - sphereA.m_radius * dispNormalized;
			Vector3 edgePointB = sphereB.m_center + sphereB.m_radius * dispNormalized;
			Vector3 edgePointDisplacement = edgePointB - edgePointA;

			newCenter = edgePointA + 0.5f * edgePointDisplacement;
			newRadius = ( distance + sphereA.m_radius + sphereB.m_radius ) / 2;

		}

		newSphere.m_center = newCenter;
		newSphere.m_radius = newRadius;
	}

	return newSphere;

}