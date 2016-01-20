//=================================================================================
// Light.cpp
// Author: Tyler George
// Date  : July 6, 2015
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "Engine/Renderer/Lighting/PuttyLight.hpp"

////===========================================================================================
///===========================================================================================
// Constructors/Destructors
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
PuttyLight::PuttyLight()
{

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
PuttyLight::~PuttyLight()
{

}

////===========================================================================================
///===========================================================================================
// Static Initialization
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
PuttyLight* PuttyLight::CreatePointLight( const Vector3& lightPos, const Rgba& lightColor, const float& lightPower )
{
    PuttyLight* light = new PuttyLight();

    light->m_lightPosition = lightPos;
    light->m_lightDirection = Vector3::ZERO;
    light->m_lightColor = lightColor;
    light->m_lightVectorWeight = 1;
    light->m_lightPower = lightPower;
    light->m_minSpotFactor = 1.0f;
    light->m_minIntensity = 0.0f;
    light->m_maxIntensity = 1.0f;
    light->m_intensityOutsideInnerPenumbra = 1.0f;
    light->m_penumbraInner = -1.0f;
    light->m_penumbraOuter = -1.0f;

    light->m_lightType = LT_POINT;

    return light;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
PuttyLight* PuttyLight::CreateGlobalPointLight( const Vector3& lightPos, const Rgba& lightColor, const float& lightPower )
{
    PuttyLight* light = new PuttyLight();

    light->m_lightPosition = lightPos;
    light->m_lightDirection = Vector3::ZERO;
    light->m_lightColor = lightColor;
    light->m_lightVectorWeight = 1;
    light->m_lightPower = lightPower;
    light->m_minSpotFactor = 1.0f;
    light->m_minIntensity = 1.0f;
    light->m_maxIntensity = 1.0f;
    light->m_intensityOutsideInnerPenumbra = 1.0f;
    light->m_penumbraInner = -1.0f;
    light->m_penumbraOuter = -1.0f;

    light->m_lightType = LT_POINT;

    return light;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
PuttyLight* PuttyLight::CreateDirectionalLight( const Vector3& lightDir, const Rgba& lightColor, const float& lightIntensity )
{
    PuttyLight* light = new PuttyLight();

    light->m_lightPosition = Vector3::ZERO;
    light->m_lightDirection = lightDir.Normalized();
    light->m_lightColor = lightColor;
    light->m_lightVectorWeight = 0;
    light->m_lightPower = 0.0f;
    light->m_minSpotFactor = 1.0f;
    light->m_minIntensity = lightIntensity;
    light->m_maxIntensity = lightIntensity;
    light->m_intensityOutsideInnerPenumbra = 1.0f;
    light->m_penumbraInner = -1.0f;
    light->m_penumbraOuter = -1.0f;

    light->m_lightType = LT_DIRECTIONAL;

    return light;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
PuttyLight* PuttyLight::CreateSpotLight( const Vector3& lightPos, const Vector3& lightDir, const Rgba& lightColor, const float& lightPower, const float& penumbraInner, const float& penumbraOuter )
{
    PuttyLight* light = new PuttyLight();

    light->m_lightPosition = lightPos;
    light->m_lightDirection = lightDir.Normalized();
    light->m_lightColor = lightColor;
    light->m_lightVectorWeight = 1;
    light->m_lightPower = lightPower;
    light->m_minSpotFactor = 0.0f;
    light->m_minIntensity = 0.0f;
    light->m_maxIntensity = 1.0f;
    light->m_intensityOutsideInnerPenumbra = 0.0f;
    light->m_penumbraInner = cos( ConvertDegreesToRadians( penumbraInner / 2.0f ) );
    light->m_penumbraOuter = cos( ConvertDegreesToRadians( penumbraOuter / 2.0f ) );

    light->m_lightType = LT_SPOT;

    return light;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
PuttyLight* PuttyLight::CreateGlobalSpotLight( const Vector3& lightPos, const Vector3& lightDir, const Rgba& lightColor, const float& lightPower, const float& penumbraInner, const float& penumbraOuter )
{
    PuttyLight* light = new PuttyLight();

    light->m_lightPosition = lightPos;
    light->m_lightDirection = lightDir.Normalized();
    light->m_lightColor = lightColor;
    light->m_lightVectorWeight = 1;
    light->m_lightPower = lightPower;
    light->m_minSpotFactor = 0.0f;
    light->m_minIntensity = 1.0f;
    light->m_maxIntensity = 1.0f;
    light->m_intensityOutsideInnerPenumbra = 0.0f;
    light->m_penumbraInner = cos( ConvertDegreesToRadians( penumbraInner / 2.0f ) );
    light->m_penumbraOuter = cos( ConvertDegreesToRadians( penumbraOuter / 2.0f ) );

    light->m_lightType = LT_SPOT;

    return light;
}

////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void PuttyLight::SetLightPosition( const Vector3& lightPos )
{
    m_lightPosition = lightPos;
}