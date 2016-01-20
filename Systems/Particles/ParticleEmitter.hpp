//=================================================================================
// ParticleEmitter.hpp
// Author: Tyler George
// Date  : June 12, 2015
//=================================================================================

#pragma once

#ifndef __included_ParticleEmitter__
#define __included_ParticleEmitter__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Systems/Particles/ParticleManager.hpp"

///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class ParticleEmitter
{
public:
    struct UpdateStrategy
    {
        UpdateStrategy( const float& mass )
            : m_mass( mass ) {}

        virtual void Update( double deltaSeconds, Particle* first, Particle* firstInactive ) = 0;
        virtual const Vector3 GetVelocity() = 0;

        const float GetMass() { return m_mass; }

    private:
        float m_mass;
    };                                                

    struct RenderStrategy
    {
        virtual void Render( OpenGLRenderer* renderer, const unsigned int& shaderID, Particle* first, Particle* firstInactive, const Rgba& startColor, const Rgba& endColor ) = 0;
    };

	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
    ParticleEmitter( const Vector3& position, const float& particlesPerSecond, const double& minLifespan, const double& maxLifespan, const double& duration, UpdateStrategy* updateStrategy, RenderStrategy* renderStrategy );
    ~ParticleEmitter();

	///---------------------------------------------------------------------------------
	/// Initialization
	///---------------------------------------------------------------------------------
	
	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------
    unsigned int GetNumParticles();
    bool IsFinished();

	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
    void SetPosition( const Vector3& position );

	///---------------------------------------------------------------------------------
	/// Update
	///---------------------------------------------------------------------------------
    void Update( double deltaSeconds );

	///---------------------------------------------------------------------------------
	/// Render
	///---------------------------------------------------------------------------------
    void Render( OpenGLRenderer* renderer, const unsigned int& shaderID, const Rgba& startColor, const Rgba& endColor = Rgba() );

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
    ParticleManager* m_particleManager;
    UpdateStrategy* m_updateStrategy;
    RenderStrategy* m_renderStrategy;

    double m_minLifespan;
    double m_maxLifespan;
    float m_particlesPerSecond;
    Vector3 m_position;
    double m_startTime;
    double m_endTime;
    double m_lastTime;
};

///---------------------------------------------------------------------------------
/// Inline function implementations
///---------------------------------------------------------------------------------

#endif