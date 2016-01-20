//=================================================================================
// ParticleManager.cpp
// Author: Tyler George
// Date  : June 12, 2015
//=================================================================================

#include "Engine/Systems/Particles/ParticleManager.hpp"
#include "Engine/Utilities/Time.hpp"

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
ParticleManager::ParticleManager( const unsigned int& numParticles )
    : m_first( nullptr )
    , m_firstInactive( nullptr )
    , m_lastActive( nullptr )
    , m_last( nullptr )
    , m_size( numParticles )
{

    for (unsigned int particleNum = 0; particleNum < numParticles; ++particleNum)
        AddParticle();

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
ParticleManager::~ParticleManager()
{
    Particle* current = m_first;
    Particle* next = m_first->next;

    while (current != m_last)
    {
        delete(current);
        current = next;
        next = current->next;
    }

    delete(m_last);

    m_first = nullptr;
    m_firstInactive = nullptr;
    m_lastActive = nullptr;
    m_last = nullptr;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ParticleManager::AddParticle()
{
    if (!m_first)
    {
        m_first = new Particle();
        m_firstInactive = m_first;
        m_last = m_first;

        return;
    }

    Particle* newParticle = new Particle();

    m_last->next = newParticle;
    newParticle->prev = m_last;
    m_last = newParticle;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool ParticleManager::ActivateParticle( const Vector3& pos, const Vector3& vel, const float& mass, const double& lifeTime )
{
    if (m_firstInactive == nullptr)
        return false;


    m_firstInactive->state = StateVector3D( pos, vel );
    m_firstInactive->mass = mass;
    m_firstInactive->spawnTime = GetCurrentSeconds();
    m_firstInactive->lifeTimeRemaining = lifeTime;
    m_firstInactive->Activate();

    m_lastActive = m_firstInactive;
    m_firstInactive = m_firstInactive->next;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Particle* ParticleManager::DeactivateParticle( Particle* particle )
{
    Particle* next = nullptr;

    if (particle == m_first)
    {
        if (particle == m_lastActive)
        {
            m_lastActive = nullptr;
            m_firstInactive = particle;
        }
        else
        {
            next = particle->next;
            m_first = next;

            particle->next = nullptr;
            m_last->next = particle;
            particle->prev = m_last;
            next->prev = nullptr;
            m_last = particle;
        }

    }

    else if (particle == m_last)
    {
        if ( m_firstInactive == nullptr )
            m_firstInactive = particle;

        if (particle == m_lastActive)
        {
            m_lastActive = particle->prev;
        }
        particle->next = nullptr;

        next = m_firstInactive;
    }

    else
    {
        next = particle->next;
        Particle* prev = particle->prev;

        prev->next = next;
        next->prev = prev;

        if (particle == m_lastActive)
            m_lastActive = prev;

        particle->next = nullptr;
        particle->prev = m_last;
        m_last->next = particle;
        m_last = particle;
    }

    particle->Deactivate();

    return next;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ParticleManager::UpdateActiveParticles( const double& deltaSeconds )
{
    if (m_lastActive != nullptr)
    {
        Particle* current = m_first;

        int count = 0;
        while ( current && current != m_firstInactive)
        {
            current->lifeTimeRemaining -= deltaSeconds;
            if (current->lifeTimeRemaining <= 0.0)
            {
                current = DeactivateParticle( current );
            }

            else
                current = current->next;
            count++;
        } 
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Particle* ParticleManager::GetFirst()
{
    return m_first;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Particle* ParticleManager::GetLast()
{
    return m_last;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Particle* ParticleManager::GetFirstInactive()
{
    return m_firstInactive;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Particle* ParticleManager::GetLastActive()
{
    return m_lastActive;
}