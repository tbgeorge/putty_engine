//=================================================================================
// ParticleEmitter.cpp
// Author: Tyler George
// Date  : June 12, 2015
//=================================================================================

#include "Engine/Systems/Particles/ParticleEmitter.hpp"
#include "Engine/Systems/Particles/ParticleManager.hpp"
#include "Engine/Utilities/Console.hpp"
#include "Engine/Utilities/Time.hpp"

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
ParticleEmitter::ParticleEmitter( const Vector3& position, const float& particlesPerSecond, const double& minLifespan, const double& maxLifespan, const double& duration, UpdateStrategy* updateStrategy, RenderStrategy* renderStrategy )
    : m_particleManager( nullptr )
    , m_updateStrategy( updateStrategy )
    , m_renderStrategy( renderStrategy )
    , m_position( position )
    , m_particlesPerSecond( particlesPerSecond )
    , m_maxLifespan( maxLifespan )
    , m_minLifespan( minLifespan )
    , m_startTime( 0.0 )
    , m_lastTime( 0.0 )
{
    if (updateStrategy == nullptr)
    {
        MessageBoxAf( MBT_OK, MBI_ERROR, "Particle Emitter Update Strategy was NULL", "The update strategy passed into the Particle Emitter constructor was equal to nullptr." );
        __debugbreak();
    }

    if (renderStrategy == nullptr)
    {
        MessageBoxAf( MBT_OK, MBI_ERROR, "Particle Emitter Render Strategy was NULL", "The render strategy passed into the Particle Emitter constructor was equal to nullptr." );
        __debugbreak();
    }

    unsigned int numParticles = (unsigned int) floorf( (particlesPerSecond * (float)maxLifespan) + 1.0f );
    m_particleManager = new ParticleManager( numParticles );

    m_startTime = GetCurrentSeconds();
    m_lastTime = GetCurrentSeconds();
    m_endTime = m_startTime + duration;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
ParticleEmitter::~ParticleEmitter()
{
    delete m_particleManager;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned int ParticleEmitter::GetNumParticles()
{
    return m_particleManager->Size();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool ParticleEmitter::IsFinished()
{
    return GetCurrentSeconds() >= m_endTime;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ParticleEmitter::Update( double deltaSeconds )
{
    bool shouldUpdate = true;

    if (m_endTime < m_startTime)
        shouldUpdate = true;

    else if (GetCurrentSeconds() >= m_endTime)
        shouldUpdate = false;

    if (shouldUpdate)
    {

        float inverseParticlesPerSec = 1.0f / m_particlesPerSecond;
        if (GetCurrentSeconds() - inverseParticlesPerSec > m_lastTime)
        {
            double timeRemaining = GetCurrentSeconds() - inverseParticlesPerSec;
            while (timeRemaining > m_lastTime)
            {
                m_particleManager->ActivateParticle( m_position, m_updateStrategy->GetVelocity(), m_updateStrategy->GetMass(), GetRandomFloatInRange( (float)m_minLifespan, (float)m_maxLifespan ) );
                timeRemaining -= inverseParticlesPerSec;
            }

            m_lastTime = GetCurrentSeconds();
        }

    }

    m_updateStrategy->Update( deltaSeconds, m_particleManager->GetFirst(), m_particleManager->GetFirstInactive() );
    m_particleManager->UpdateActiveParticles( deltaSeconds );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ParticleEmitter::SetPosition( const Vector3& position) 
{
    m_position = position;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ParticleEmitter::Render( OpenGLRenderer* renderer, const unsigned int& shaderID , const Rgba& startColor, const Rgba& endColor )
{
//     if ( !endColor.IsValid() )
//         m_renderStrategy->Render( renderer, shaderID, m_particleManager->GetFirst(), m_particleManager->GetFirstInactive(), startColor, startColor );
//     else
        m_renderStrategy->Render( renderer, shaderID, m_particleManager->GetFirst(), m_particleManager->GetFirstInactive(), startColor, endColor );

}