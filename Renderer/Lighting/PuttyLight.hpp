//=================================================================================
// Light.hpp
// Author: Tyler George
// Date  : July 6, 2015
//=================================================================================

#pragma once

#ifndef __included_Light__
#define __included_Light__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include <vector>
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Utilities/Rgba.hpp"

///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
enum PuttyLightType
{
    LT_SPOT,
    LT_POINT,
    LT_DIRECTIONAL
};

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class PuttyLight
{
public:
	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
    PuttyLight();
    ~PuttyLight();

	///---------------------------------------------------------------------------------
	/// Static Initialization
	///---------------------------------------------------------------------------------
    static PuttyLight* CreatePointLight( const Vector3& lightPos, const Rgba& lightColor, const float& lightPower );
    static PuttyLight* CreateGlobalPointLight( const Vector3& lightPos, const Rgba& lightColor, const float& lightPower );
    static PuttyLight* CreateDirectionalLight( const Vector3& lightDir, const Rgba& lightColor, const float& lightIntensity );
    static PuttyLight* CreateSpotLight( const Vector3& lightPos, const Vector3& lightDir, const Rgba& lightColor, const float& lightPower, const float& penumbraInner, const float& penumbraOuter );
    static PuttyLight* CreateGlobalSpotLight( const Vector3& lightPos, const Vector3& lightDir, const Rgba& lightColor, const float& lightPower, const float& penumbraInner, const float& penumbraOuter );

	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------
    Vector3 GetLightPosition() { return m_lightPosition; }
    Vector3 GetLightDirection() { return m_lightDirection; }
    Rgba GetLightColor() { return m_lightColor; }
    int GetLightVectorWeight() { return m_lightVectorWeight; }
    float GetLightPower() { return m_lightPower; }
    float GetMinSpotFactor() { return m_minSpotFactor; }
    float GetMinIntensity() { return m_minIntensity; }
    float GetMaxIntensity() { return m_maxIntensity; }
    float GetIntensityOutsideInnerPenumbra() { return m_intensityOutsideInnerPenumbra; }
    float GetPenumbraInner() { return m_penumbraInner; }
    float GetPenumbraOuter() { return m_penumbraOuter; }
    PuttyLightType GetLightType() { return m_lightType; }

	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
    void SetLightPosition( const Vector3& lightPos );

	///---------------------------------------------------------------------------------
	/// Update
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Render
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Public Member Variables
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Static Variables
	///---------------------------------------------------------------------------------


private:
	///---------------------------------------------------------------------------------
	/// Private Functions
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Private Member Variables
	///---------------------------------------------------------------------------------
    Vector3 m_lightPosition;
    Vector3 m_lightDirection;
    Rgba m_lightColor;
    int m_lightVectorWeight;
    float m_lightPower;
    float m_minSpotFactor;
    float m_minIntensity;
    float m_maxIntensity;
    float m_intensityOutsideInnerPenumbra;
    float m_penumbraInner;
    float m_penumbraOuter;

    PuttyLightType m_lightType;

};

typedef std::vector< PuttyLight* > Lights;

///---------------------------------------------------------------------------------
/// Inline function implementations
///---------------------------------------------------------------------------------

#endif