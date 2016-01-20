//=================================================================================
// GraphicsDebugger.hpp
// Author: Tyler George
// Date  : June 3, 2015
//=================================================================================

#pragma once

#ifndef __included_GraphicsDebugger__
#define __included_GraphicsDebugger__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Renderer/Vertex3D.hpp"
#include "Engine/Common/EngineCommon.hpp"

struct RenderCommand
{
    double callTime;
    double duration;
    DepthTestMode depthTestMode;
    virtual void AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes ) = 0;
};

typedef std::vector< RenderCommand* > RenderCommands;

struct RenderPointCommand : public RenderCommand
{
    Vector3 position;
    Rgba color;
    void AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes );
};

struct RenderLineCommand : public RenderCommand
{
    Vector3 startPosition;
    Vector3 endPosition;
    Rgba startColor;
    Rgba endColor;
    void AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes );
};

struct RenderArrowCommand : public RenderCommand
{
    Vector3 startPosition;
    Vector3 endPosition;
    Rgba color;
    void AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes );
};

struct RenderWireAABBCommand : public RenderCommand
{
    Vector3 mins;
    Vector3 maxs;
    Rgba lineColor;
    void AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes );
};

struct RenderAABBCommand : public RenderCommand
{
    Vector3 mins;
    Vector3 maxs;
    Rgba faceColor;
    void AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes );
};

struct RenderWireUVSphereCommand : public RenderCommand
{
    Vector3 center;
    float radius;
    int slices;
    int stacks;
    Rgba color;
    void AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes );
};

struct RenderUVSphereCommand : public RenderCommand
{
    Vector3 center;
    float radius;
    int slices;
    int stacks;
    Rgba color;
    void AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes );
};

struct RenderWireIcoSphereCommand : public RenderCommand
{
    Vector3 center;
    float radius;
    int recursionLevel;
    Rgba color;
    void AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes );
};

struct RenderIcoSphereCommand : public RenderCommand
{
    Vector3 center;
    float radius;
    int recursionLevel;
    Rgba color;
    void AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes );
};

///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class GraphicsDebugger
{
public:
	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
    GraphicsDebugger( OpenGLRenderer* renderer );
    ~GraphicsDebugger();

	///---------------------------------------------------------------------------------
	/// Initialization
	///---------------------------------------------------------------------------------
	
	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------
    static GraphicsDebugger* GetInstance();

	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
    void AddCommand( RenderCommand* command, const double& duration, const DepthTestMode& mode );
    void ExecuteCommands();

    void AddPoint( const Vector3& position, const double& duration, const DepthTestMode& mode, const Rgba& color = Rgba() );
    void AddLineSegment( const Vector3& startPos, const Vector3& endPos, const double& duration, const DepthTestMode& mode, const Rgba& startColor = Rgba(), const Rgba& endColor = Rgba() );
    void AddArrow( const Vector3& startPos, const Vector3& endPos, const double& duration, const DepthTestMode& mode, const Rgba& color = Rgba() );
    void AddWireAABB( const Vector3& mins, const Vector3& maxs, const double& duration, const DepthTestMode& mode, const Rgba& lineColor = Rgba() );
    void AddAABB( const Vector3& mins, const Vector3& maxs, const double& duration, const DepthTestMode& mode, const Rgba& lineColor = Rgba(), const Rgba& faceColor = Rgba() );
    void AddWireUVSphere( const Vector3& center, const float& radius, const int& slices, const int& stacks, const double& duration, const DepthTestMode& mode, const Rgba& color = Rgba() );
    void AddUVSphere( const Vector3& center, const float& radius, const int& slices, const int& stacks, const double& duration, const DepthTestMode& mode, const Rgba& color = Rgba() );
    void AddWireIcoSphere( const Vector3& center, const float& radius, const int& recursionLevel, const double& duration, const DepthTestMode& mode, const Rgba& color = Rgba() );
    void AddIcoSphere( const Vector3& center, const float& radius, const int& recursionLevel, const double& duration, const DepthTestMode& mode, const Rgba& color = Rgba() );

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
    void CleanUpCommands();
    void ClearVertexes();

	///---------------------------------------------------------------------------------
	/// Private Member Variables
	///---------------------------------------------------------------------------------
    RenderCommands m_DepthTestEnabledCommands;
    RenderCommands m_DepthTestDisabledCommands;

    PUC_Vertexes m_lineVertexes;
    PUC_Vertexes m_quadVertexes;
    PUC_Vertexes m_triVertexes;

    unsigned int m_lineVBO;
    unsigned int m_quadVBO;
    unsigned int m_triVBO;

    OpenGLRenderer* m_renderer;
    unsigned int m_shaderID;

    ///---------------------------------------------------------------------------------
    /// Private Static Variables
    ///---------------------------------------------------------------------------------
    static GraphicsDebugger* s_theGraphicsDebugger;
};

///---------------------------------------------------------------------------------
/// Inline function implementations
///---------------------------------------------------------------------------------

#endif