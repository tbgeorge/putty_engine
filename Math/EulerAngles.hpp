//=================================================================================
// EulerAngles.hpp
// Author: Tyler George
// Date  : March 26, 2015
//=================================================================================

#pragma once
#ifndef __included_EulerAngles__
#define __included_EulerAngles__

#include "Engine/Common/EngineCommon.hpp"

///---------------------------------------------------------------------------------
/// EulerAngles
///---------------------------------------------------------------------------------

class EulerAngles
{
public:
	EulerAngles() {};
	EulerAngles( float initialYawDegreesAboutZ, float initialPitchDegreesAroundY, float initialRollDegreesAroundX )
		: yawDegrees( initialYawDegreesAboutZ )
		, pitchDegrees( initialPitchDegreesAroundY )
		, rollDegrees( initialRollDegreesAroundX ) {}

	const Vector3 GetForwardVector() const;

	const EulerAngles operator*( const Vector3& scaleFactors ) const;
	const EulerAngles operator*( float scale ) const;
	const EulerAngles operator+( const EulerAngles& degreesToAdd ) const;
	void operator+=( const EulerAngles& degreesToAdd );
	const EulerAngles operator=( const EulerAngles& degreesToEqual );
    const bool operator==(const EulerAngles& equalTo);

public:
	float yawDegrees;
	float pitchDegrees;
	float rollDegrees;
};

///---------------------------------------------------------------------------------
/// Inline methods
///---------------------------------------------------------------------------------
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline const Vector3 EulerAngles::GetForwardVector() const
{
	float pitchRadians = ConvertDegreesToRadians( pitchDegrees );
	float yawRadians = ConvertDegreesToRadians( yawDegrees );

	float cosPitch = cos( pitchRadians );
	Vector3 cameraForward( cosPitch * cos( yawRadians ), cosPitch * sin( yawRadians ), -sin( pitchRadians ) );
	cameraForward.Normalize();
	return cameraForward;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline const EulerAngles EulerAngles::operator*( const Vector3& scaleFactors ) const
{
	float newRollDegrees = rollDegrees * scaleFactors.x;
	float newPitchDegrees = pitchDegrees * scaleFactors.y;
	float newYawDegrees = yawDegrees * scaleFactors.z;
	return EulerAngles( newYawDegrees, newPitchDegrees, newRollDegrees );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline const EulerAngles EulerAngles::operator*( float scale ) const
{
	float newRollDegrees = rollDegrees * scale;
	float newPitchDegrees = pitchDegrees * scale;
	float newYawDegrees = yawDegrees * scale;
	return EulerAngles( newYawDegrees, newPitchDegrees, newRollDegrees );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline const EulerAngles EulerAngles::operator+( const EulerAngles& degreesToAdd ) const
{
	float newRollDegrees = rollDegrees + degreesToAdd.rollDegrees;
	float newPitchDegrees = pitchDegrees + degreesToAdd.pitchDegrees;
	float newYawDegrees = yawDegrees + degreesToAdd.yawDegrees;
	return EulerAngles( newYawDegrees, newPitchDegrees, newRollDegrees );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline void EulerAngles::operator+=( const EulerAngles& degreesToAdd ) 
{
	rollDegrees += degreesToAdd.rollDegrees;
	pitchDegrees += degreesToAdd.pitchDegrees;
	yawDegrees += degreesToAdd.yawDegrees;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline const EulerAngles EulerAngles::operator=( const EulerAngles& degreesToEqual )
{
	rollDegrees = degreesToEqual.rollDegrees;
	pitchDegrees = degreesToEqual.pitchDegrees;
	yawDegrees = degreesToEqual.yawDegrees;

	return EulerAngles( rollDegrees, pitchDegrees, yawDegrees );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline const bool EulerAngles::operator==(const EulerAngles& equalTo)
{
    if (rollDegrees == equalTo.rollDegrees && yawDegrees == equalTo.yawDegrees && pitchDegrees == equalTo.pitchDegrees)
        return true;
    return false;
}
#endif

