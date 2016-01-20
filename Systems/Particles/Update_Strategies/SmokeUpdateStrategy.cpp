//=================================================================================
// SmokeUpdateStrategy.cpp
// Author: Tyler George
// Date  : June 18, 2015
//=================================================================================

#include "Engine/Systems/Particles/Update_Strategies/SmokeUpdateStrategy.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/StateVectors.hpp"

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
SmokeUpdateStrategy::SmokeUpdateStrategy( const float& mass, const float& defaultSpeed, const Vector3& normal, const float& maxRadius, const float& minVelScale, const float& maxVelScale, const bool& gravity )
    : UpdateStrategy( mass )
    , m_defaultSpeed( defaultSpeed )
    , m_normal( normal )
    , m_maxRadius( maxRadius )
    , m_minVelScale( minVelScale )
    , m_maxVelScale( maxVelScale )
    , m_gravityEnabled( gravity )
    , m_windForce( Vector3::ZERO )
{

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
const Vector3 SmokeUpdateStrategy::GetVelocity()
{
    float coneRadius = GetRandomFloatInRange( 0.0f, m_maxRadius );
    float coneAngle = GetRandomFloatInRange( 0.0f, 360.0f );

    Vector2 velocityOffset( 1.0f, 0.0f );
    velocityOffset.RotateDegrees( coneAngle );
    velocityOffset.ScaleUniform( coneRadius );

    Vector3 defaultVelocity = m_normal * m_defaultSpeed;

    Vector3 perp1;
    Vector3 perp2;
    if (m_normal != Vector3( 0.0f, 1.0f, 0.0f ))
    {
        perp1 = CrossProduct( m_normal, Vector3( 0.0f, 1.0f, 0.0f ) );
        perp1.Normalize();
        perp2 = CrossProduct( m_normal, perp1 );
        perp2.Normalize();
    }
    else
    {
        perp1 = CrossProduct( m_normal, Vector3( 0.0f, 0.0f, 1.0f ) );
        perp1.Normalize();
        perp2 = CrossProduct( m_normal, perp1 );
        perp2.Normalize();
    }

    float velocityScale = GetRandomFloatInRange( m_minVelScale, m_maxVelScale );
    return velocityScale * defaultVelocity + (velocityOffset.x * perp1) + (velocityOffset.y * perp2);
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
StateVector3D SmokeUpdateStrategy::CalculateDerivative( const StateVector3D& initialState )
{
    Vector3 pos = initialState.s1;
    Vector3 vel = initialState.s2;

    Vector3 gravityForce( 0.0f, -9.81f, 0.0f );
    if (!m_gravityEnabled)
        gravityForce = Vector3::ZERO;

    Vector3 wind = -m_windC * (vel - m_windForce);

    Vector3 force = gravityForce + wind;

    StateVector3D derivative( vel, force );

    return derivative;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
StateVector3D SmokeUpdateStrategy::RK4( const StateVector3D& initialState, const double& deltaT )
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
void SmokeUpdateStrategy::SetWindForce( const float& c, const Vector3& wind )
{
    m_windC = c;
    m_windForce = wind;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SmokeUpdateStrategy::Update( double deltaSeconds, Particle* first, Particle* firstInactive )
{
    Particle* current = first;

    while (current != firstInactive)
    {
        current->state = RK4( current->state, deltaSeconds );
        current = current->next;
    }
}