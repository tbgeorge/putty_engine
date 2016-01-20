//=================================================================================
// PhysicsMotion2D.hpp
// Author: Tyler George
// Date  : January 21, 2015
//=================================================================================

#pragma once
#ifndef __included_PhysicsMotion2D__
#define __included_PhysicsMotion2D__

#include "Engine/Common/EngineCommon.hpp"
#include "Vector2.hpp"

class PhysicsMotion2D
{
public:
	///---------------------------------------------------------------------------------
	/// Member Variables
	///---------------------------------------------------------------------------------
	Vector2 m_position;
	Vector2 m_velocity;
	float	m_orientationDegrees;
	float	m_angularVelocity;

	///---------------------------------------------------------------------------------
	/// Constructors
	///---------------------------------------------------------------------------------
	PhysicsMotion2D() {}
	PhysicsMotion2D( const PhysicsMotion2D& copy );
	explicit PhysicsMotion2D( Vector2 position, Vector2 velocity, float orientationDegrees, float angularVelocity );

	///---------------------------------------------------------------------------------
	/// Member Functions
	///---------------------------------------------------------------------------------
	void Update( float deltaSeconds );
	void ApplyAcceleration( const Vector2& acceleration );

};

///---------------------------------------------------------------------------------
/// inline Functions
///---------------------------------------------------------------------------------
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline PhysicsMotion2D::PhysicsMotion2D( const PhysicsMotion2D& copy ) 
	: m_position( copy.m_position )
	, m_velocity( copy.m_velocity )
	, m_orientationDegrees( copy.m_orientationDegrees )
	, m_angularVelocity( copy.m_angularVelocity )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline PhysicsMotion2D::PhysicsMotion2D( Vector2 position, Vector2 velocity, float orientationDegrees, float angularVelocity )
	: m_position( position )
	, m_velocity( velocity )
	, m_orientationDegrees( orientationDegrees )
	, m_angularVelocity( angularVelocity )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline void PhysicsMotion2D::Update( float deltaSeconds )
{
	m_position += m_velocity * deltaSeconds;
	m_orientationDegrees += m_angularVelocity * deltaSeconds;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline void PhysicsMotion2D::ApplyAcceleration( const Vector2& acceleration ) 
{
	m_velocity += acceleration;
}


#endif