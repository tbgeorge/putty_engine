//=================================================================================
// Particle.hpp
// Author: Tyler George
// Date  : June 18, 2015
//=================================================================================
#pragma  once

#ifndef __included_Particle__
#define __included_Particle__

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/StateVectors.hpp"
#include "Engine/Math/Vector3.hpp"

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
struct Particle
{
    StateVector3D state;
    float mass;
    double spawnTime;
    double lifeTimeRemaining;

    bool isActive;

    Particle* next;
    Particle* prev;

    Particle()
        : state( StateVector3D( Vector3::ZERO, Vector3::ZERO ) )
        , mass( -1.0f )
        , spawnTime( -1.0 )
        , isActive( false )
        , next( nullptr )
        , prev( nullptr ) {}

    bool IsActive() const { return isActive; }
    void Activate() { isActive = true; }
    void Deactivate() {
        isActive = false;
        state = StateVector3D( Vector3::ZERO, Vector3::ZERO );
        spawnTime = -1.0;
        lifeTimeRemaining = -1.0f;
        mass = -1.0f;
    }
};

#endif