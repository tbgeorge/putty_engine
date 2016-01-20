//=================================================================================
// Camera3D.hpp
// Author: Tyler George
// Date  : March 26, 2015
//=================================================================================

#pragma once
#ifndef __included_Camera3D__
#define __included_Camera3D__

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/EulerAngles.hpp"

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class Camera3D
{
public:
	Camera3D() {}
	Camera3D( const Vector3& initialPosition, const EulerAngles& initialOrientation )
		: m_position( initialPosition )
		, m_orientation( initialOrientation ) {}

	const Vector3 GetForwardVector() const;
    const Vector3 GetRightVector() const;
    const Vector3 GetUpVector() const;

public:
	EulerAngles m_orientation;
	Vector3 m_position;
};

///---------------------------------------------------------------------------------
/// Inline function implementations
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline const Vector3 Camera3D::GetForwardVector() const 
{
	float pitchRadians = ConvertDegreesToRadians( m_orientation.pitchDegrees );
	float yawRadians = ConvertDegreesToRadians( m_orientation.yawDegrees );

	float cosPitch = cos( pitchRadians );

	Vector3 cameraForward( cosPitch * sin( yawRadians ), -sin( pitchRadians ), cosPitch * cos( yawRadians ) );

	cameraForward.Normalize();
	return cameraForward;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline const Vector3 Camera3D::GetUpVector() const
{
    Vector3 cameraUp = CrossProduct( GetRightVector(), GetForwardVector() );
    cameraUp.Normalize();

    return cameraUp;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline const Vector3 Camera3D::GetRightVector() const
{
    float rollRadians = ConvertDegreesToRadians( m_orientation.rollDegrees );
    float yawRadians = ConvertDegreesToRadians( m_orientation.yawDegrees - 90.0f );

    float cosRoll = cos( rollRadians );

    Vector3 cameraRight(cosRoll * sin( yawRadians ), -sin( rollRadians ), cosRoll * cos( yawRadians ));

    cameraRight.Normalize();
    return cameraRight;

}
#endif