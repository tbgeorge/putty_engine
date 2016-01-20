//=================================================================================
// PointsRenderStrategy.hpp
// Author: Tyler George
// Date  : June 18, 2015
//=================================================================================

#pragma once

#ifndef __included_PointsRenderStrategy__
#define __included_PointsRenderStrategy__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Systems/Particles/ParticleEmitter.hpp"
#include "Engine/Utilities/Time.hpp"

///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class PointsRenderStrategy : public ParticleEmitter::RenderStrategy
{
public:
	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
    PointsRenderStrategy( const float& pointSize )
        : m_pointSize( pointSize ) {}

	///---------------------------------------------------------------------------------
	/// Initialization
	///---------------------------------------------------------------------------------
	
	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Update
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Render
	///---------------------------------------------------------------------------------
    void Render( OpenGLRenderer* renderer, const unsigned int& shaderID, Particle* first, Particle* firstInactive, const Rgba& startColor, const Rgba& endColor );

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
    float m_pointSize;
};

///---------------------------------------------------------------------------------
/// Inline function implementations
///---------------------------------------------------------------------------------
inline void PointsRenderStrategy::Render( OpenGLRenderer* renderer, const unsigned int& shaderID, Particle* first, Particle* firstInactive, const Rgba& startColor, const Rgba& endColor )
{
    PUC_Vertexes points;

    Particle* current = first;

    while (current != firstInactive)
    {
        float totalLife = (float) ( (GetCurrentSeconds() - current->spawnTime) + current->lifeTimeRemaining );
        float fractionalLife = (float) (GetCurrentSeconds() - current->spawnTime ) / totalLife;

        Rgba color = Interpolate( startColor, endColor, fractionalLife );

        Vertex3D_PUC point;
        point.position = current->state.s1;
        point.color = color;

        points.push_back( point );
        current = current->next;
    }

    if (points.size() != 0)
    {
        renderer->SetPointSize( m_pointSize );
        renderer->DrawVertexes( OpenGLRenderer::CreateOrGetShader( "Data/Shaders/basic" ), points, GL_POINTS );
    }

}

#endif