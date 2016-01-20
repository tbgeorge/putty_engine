//=================================================================================
// PhysicsMotion3D.hpp
// Author: Tyler George
// Date  : April 23, 2015
//=================================================================================

#pragma once
#ifndef __included_PhysicsMotion3D__
#define __included_PhysicsMotion3D__

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/EulerAngles.hpp"

class PhysicsMotion3D
{
public:
	///---------------------------------------------------------------------------------
	/// Member Variables
	///---------------------------------------------------------------------------------
	Vector3 m_position;
	Vector3 m_velocity;
	EulerAngles	m_orientationDegrees;
	EulerAngles m_orientationVelocityDegreesPerSecond;

	///---------------------------------------------------------------------------------
	/// Constructors
	///---------------------------------------------------------------------------------
	PhysicsMotion3D() {}
	PhysicsMotion3D( const PhysicsMotion3D& copy );
	explicit PhysicsMotion3D( const Vector3& position, const Vector3& velocity, EulerAngles orientationDegrees, EulerAngles orientationVelocityDegreesPerSecond );

	///---------------------------------------------------------------------------------
	/// Member Functions
	///---------------------------------------------------------------------------------
	void Update( double deltaSeconds );
	void ApplyAcceleration( const Vector3& acceleration );

};

///---------------------------------------------------------------------------------
/// inline Functions
///---------------------------------------------------------------------------------
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline PhysicsMotion3D::PhysicsMotion3D( const PhysicsMotion3D& copy ) 
	: m_position( copy.m_position )
	, m_velocity( copy.m_velocity )
	, m_orientationDegrees( copy.m_orientationDegrees )
	, m_orientationVelocityDegreesPerSecond( copy.m_orientationVelocityDegreesPerSecond )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline PhysicsMotion3D::PhysicsMotion3D( const Vector3& position, const Vector3& velocity, EulerAngles orientationDegrees, EulerAngles orientationVelocityDegreesPerSecond )
	: m_position( position )
	, m_velocity( velocity )
	, m_orientationDegrees( orientationDegrees )
	, m_orientationVelocityDegreesPerSecond( orientationVelocityDegreesPerSecond )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline void PhysicsMotion3D::Update( double deltaSeconds )
{
	m_position += m_velocity * (float) deltaSeconds;
	m_orientationDegrees += m_orientationVelocityDegreesPerSecond * (float) deltaSeconds;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline void PhysicsMotion3D::ApplyAcceleration( const Vector3& acceleration ) 
{
	m_velocity += acceleration;
}


#endif