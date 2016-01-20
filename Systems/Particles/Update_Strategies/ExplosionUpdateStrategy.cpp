//=================================================================================
// ExplosionUpdateStrategy.cpp
// Author: Tyler George
// Date  : June 18, 2015
//=================================================================================

#include "Engine/Systems/Particles/Update_Strategies/ExplosionUpdateStrategy.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/StateVectors.hpp"

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
ExplosionUpdateStrategy::ExplosionUpdateStrategy( const float& mass, const float& defaultSpeed, const float& maxScale, bool gravity )
    : UpdateStrategy( mass )
    , m_defaultSpeed( defaultSpeed )
    , m_maxScale( maxScale )
    , m_gravityEnabled( gravity )
{

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
const Vector3 ExplosionUpdateStrategy::GetVelocity()
{
    float speedmodifier = GetRandomFloatInRange( 0.0f, m_maxScale );
    
    float randX = GetRandomFloatZeroToOne() - 0.5f;
    float randY = GetRandomFloatZeroToOne() - 0.5f;
    float randZ = GetRandomFloatZeroToOne() - 0.5f;

    Vector3 direction( randX, randY, randZ );
    direction.Normalize();

    return direction * ( m_defaultSpeed * speedmodifier );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
StateVector3D ExplosionUpdateStrategy::CalculateDerivative( const StateVector3D& initialState )
{
    Vector3 pos = initialState.s1;
    Vector3 vel = initialState.s2;

    float gravityForce = -9.81f;

    if (!m_gravityEnabled)
        gravityForce = 0.0f;
    StateVector3D derivative( vel, Vector3( 0.0f, gravityForce, 0.0f ) );

    return derivative;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
StateVector3D ExplosionUpdateStrategy::RK4( const StateVector3D& initialState, const double& deltaT )
{
    StateVector3D s1 = initialState;

    StateVector3D s1Derivative = CalculateDerivative( s1 );
    StateVector3D s2 = initialState + s1Derivative * (float)(deltaT / 2.0);

    StateVector3D s2Derivative = CalculateDerivative( s2 );
    StateVector3D s3 = initialState + s2Derivative * (float)(deltaT / 2.0);

    StateVector3D s3Derivative = CalculateDerivative( s3 );
    StateVector3D s4 = initialState + s3Derivative * (float)deltaT;

    StateVector3D s4Derivative = CalculateDerivative( s4 );

    StateVector3D nextState = initialState + (s1Derivative + (s2Derivative * 2.0f) + (s3Derivative * 2.0f) + s4Derivative) * (float)(deltaT / 6.0);

    return nextState;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ExplosionUpdateStrategy::Update( double deltaSeconds, Particle* first, Particle* firstInactive )
{
    Particle* current = first;

    while (current != firstInactive)
    {
        current->state = RK4( current->state, deltaSeconds );
        current = current->next;
    }
}