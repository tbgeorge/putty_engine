//=================================================================================
// GraphicsDebugger.cpp
// Author: Tyler George
// Date  : June 3, 2015
//=================================================================================

#include "Engine/Renderer/GraphicsDebugger.hpp"
#include "Engine/Math/IcoSphereCreator.hpp"
#include "Engine/Utilities/Time.hpp"
#include "Engine/Utilities/DeveloperConsole.hpp"
#include "Engine/Utilities/Utilities.hpp"

//---------------------------------------------------------------------------------
// Static Variables
//---------------------------------------------------------------------------------
GraphicsDebugger* GraphicsDebugger::s_theGraphicsDebugger = NULL;


////===========================================================================================
///===========================================================================================
// Command implementations
///===========================================================================================
////===========================================================================================

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void RenderPointCommand::AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes )
{
    UNUSED( quadVertexes );
    UNUSED( triVertexes );

    Vertex3D_PUC vertex;
    vertex.color = color;

    vertex.position = position;
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );
//     vertex.position = Vector3( position.x - 0.5f, position.y, position.z );
//     lineVertexes.push_back( vertex );
// 
//     vertex.position = Vector3( position.x + 0.5f, position.y, position.z );
//     lineVertexes.push_back( vertex );
// 
//     vertex.position = Vector3( position.x, position.y - 0.5f, position.z );
//     lineVertexes.push_back( vertex );
// 
//     vertex.position = Vector3( position.x, position.y + 0.5f, position.z );
//     lineVertexes.push_back( vertex );
// 
//     vertex.position = Vector3( position.x, position.y, position.z - 0.5f );
//     lineVertexes.push_back( vertex );
// 
//     vertex.position = Vector3( position.x, position.y, position.z + 0.5f );
//     lineVertexes.push_back( vertex );
// 
//     vertex.position = Vector3( position.x - 0.25f, position.y - 0.25f, position.z - 0.25f );
//     lineVertexes.push_back( vertex );
// 
//     vertex.position = Vector3( position.x + 0.25f, position.y + 0.25f, position.z + 0.25f );
//     lineVertexes.push_back( vertex );
// 
//     vertex.position = Vector3( position.x + 0.25f, position.y - 0.25f, position.z - 0.25f );
//     lineVertexes.push_back( vertex );
// 
//     vertex.position = Vector3( position.x - 0.25f, position.y + 0.25f, position.z + 0.25f );
//     lineVertexes.push_back( vertex );
// 
//     vertex.position = Vector3( position.x - 0.25f, position.y - 0.25f, position.z + 0.25f );
//     lineVertexes.push_back( vertex );
// 
//     vertex.position = Vector3( position.x + 0.25f, position.y + 0.25f, position.z - 0.25f );
//     lineVertexes.push_back( vertex );
// 
//     vertex.position = Vector3( position.x + 0.25f, position.y - 0.25f, position.z + 0.25f );
//     lineVertexes.push_back( vertex );
// 
//     vertex.position = Vector3( position.x - 0.25f, position.y + 0.25f, position.z - 0.25f );
//     lineVertexes.push_back( vertex );

}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void RenderLineCommand::AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes )
{
    UNUSED( quadVertexes );
    UNUSED( triVertexes );

    Vertex3D_PUC vertex;

    vertex.position = startPosition;
    vertex.color = startColor;
    lineVertexes.push_back( vertex );

    vertex.position = endPosition;
    vertex.color = endColor;
    lineVertexes.push_back( vertex );
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void RenderArrowCommand::AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes )
{
    UNUSED( quadVertexes );
    UNUSED( triVertexes );

    Vector3 forward = endPosition - startPosition;
    forward.Normalize();

    Vector3 perp;
    if (forward != Vector3( 0.0f, 1.0f, 0.0f ))
        perp = CrossProduct( forward, Vector3( 0.0f, 1.0f, 0.0f ) );
    else
        perp = CrossProduct( forward, Vector3( 1.0f, 0.0f, 0.0f ) );

    perp.Normalize();
    Vector3 perp2 = CrossProduct( forward, perp );
    perp2.Normalize();

    Vertex3D_PUC vertex;
    vertex.color = color;

    vertex.position = startPosition;
    lineVertexes.push_back( vertex );

    vertex.position = endPosition;
    lineVertexes.push_back( vertex );

    Vector3 arrowHeadCenter = endPosition - (0.1f * (endPosition - startPosition));
    float length = CalcDistance( startPosition, endPosition );

    PUC_Vertexes arrowBase;
    for (float degrees = 0.0f; degrees < 360.0f; degrees += 6.0f)
    {
        float radians = ConvertDegreesToRadians( degrees );
        float cosRadians = cos( radians );
        float sinRadians = sin( radians );

        float radius = 0.025f * length;
        vertex.position = Vector3(
            arrowHeadCenter.x + radius * cosRadians * perp.x + radius * sinRadians * perp2.x,
            arrowHeadCenter.y + radius * cosRadians * perp.y + radius * sinRadians * perp2.y,
            arrowHeadCenter.z + radius * cosRadians * perp.z + radius * sinRadians * perp2.z
            );

        arrowBase.push_back( vertex );
        lineVertexes.push_back( vertex );
        vertex.position = arrowHeadCenter;
        lineVertexes.push_back( vertex );
    }

    for (PUC_Vertexes::const_iterator vertexIter = arrowBase.begin(); vertexIter != arrowBase.end(); ++vertexIter)
    {
        Vertex3D_PUC vert = *vertexIter;

        lineVertexes.push_back( vert );
        vert.position = endPosition;
        lineVertexes.push_back( vert );
    }

}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void RenderWireAABBCommand::AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes )
{
    UNUSED( quadVertexes );
    UNUSED( triVertexes );

    Vertex3D_PUC vertex;
    vertex.color = lineColor;

    // far side
    vertex.position = mins;
    lineVertexes.push_back( vertex );

    vertex.position = Vector3( mins.x, maxs.y, mins.z );
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );

    vertex.position = Vector3( maxs.x, maxs.y, mins.z );
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );

    vertex.position = Vector3( maxs.x, mins.y, mins.z );
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );

    vertex.position = mins;
    lineVertexes.push_back( vertex );

    // near side
    vertex.position = Vector3( maxs.x, mins.y, maxs.z );
    lineVertexes.push_back( vertex );

    vertex.position = maxs;
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );

    vertex.position = Vector3( mins.x, maxs.y, maxs.z );
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );

    vertex.position = Vector3( mins.x, mins.y, maxs.z );
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );

    vertex.position = Vector3( maxs.x, mins.y, maxs.z );
    lineVertexes.push_back( vertex );

    // right side
    vertex.position = Vector3( maxs.x, mins.y, mins.z );
    lineVertexes.push_back( vertex );

    vertex.position = Vector3( maxs.x, maxs.y, mins.z );
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );

    vertex.position = maxs;
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );

    vertex.position = Vector3( maxs.x, mins.y, maxs.z );
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );

    vertex.position = Vector3( maxs.x, mins.y, mins.z );
    lineVertexes.push_back( vertex );

    // left side
    vertex.position = Vector3( mins.x, mins.y, maxs.z );
    lineVertexes.push_back( vertex );

    vertex.position = Vector3( mins.x, maxs.y, maxs.z );
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );

    vertex.position = Vector3( mins.x, maxs.y, mins.z );
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );

    vertex.position = mins;
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );

    vertex.position = Vector3( mins.x, mins.y, maxs.z );
    lineVertexes.push_back( vertex );

    // top side
    vertex.position = maxs;
    lineVertexes.push_back( vertex );

    vertex.position = Vector3( maxs.x, maxs.y, mins.z );
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );

    vertex.position = Vector3( mins.x, maxs.y, mins.z );
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );

    vertex.position = Vector3( mins.x, maxs.y, maxs.z );
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );

    vertex.position = maxs;
    lineVertexes.push_back( vertex );

    // bottom side
    vertex.position = mins;
    lineVertexes.push_back( vertex );

    vertex.position = Vector3( maxs.x, mins.y, mins.z );
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );

    vertex.position = Vector3( maxs.x, mins.y, maxs.z );
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );

    vertex.position = Vector3( mins.x, mins.y, maxs.z );
    lineVertexes.push_back( vertex );
    lineVertexes.push_back( vertex );

    vertex.position = mins;
    lineVertexes.push_back( vertex );
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void RenderAABBCommand::AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes )
{
    UNUSED( lineVertexes );
    UNUSED( triVertexes );

    Vertex3D_PUC vertex;
    vertex.color = faceColor;

    // far side
    vertex.position = mins;
    quadVertexes.push_back( vertex );

    vertex.position = Vector3( mins.x, maxs.y, mins.z );
    quadVertexes.push_back( vertex );

    vertex.position = Vector3( maxs.x, maxs.y, mins.z );
    quadVertexes.push_back( vertex );

    vertex.position = Vector3( maxs.x, mins.y, mins.z );
    quadVertexes.push_back( vertex );

    // near side
    vertex.position = Vector3( maxs.x, mins.y, maxs.z );
    quadVertexes.push_back( vertex );

    vertex.position = maxs;
    quadVertexes.push_back( vertex );

    vertex.position = Vector3( mins.x, maxs.y, maxs.z );
    quadVertexes.push_back( vertex );

    vertex.position = Vector3( mins.x, mins.y, maxs.z );
    quadVertexes.push_back( vertex );

    // right side
    vertex.position = Vector3( maxs.x, mins.y, mins.z );
    quadVertexes.push_back( vertex );

    vertex.position = Vector3( maxs.x, maxs.y, mins.z );
    quadVertexes.push_back( vertex );

    vertex.position = maxs;
    quadVertexes.push_back( vertex );

    vertex.position = Vector3( maxs.x, mins.y, maxs.z );
    quadVertexes.push_back( vertex );

    // left side
    vertex.position = Vector3( mins.x, mins.y, maxs.z );
    quadVertexes.push_back( vertex );

    vertex.position = Vector3( mins.x, maxs.y, maxs.z );
    quadVertexes.push_back( vertex );

    vertex.position = Vector3( mins.x, maxs.y, mins.z );
    quadVertexes.push_back( vertex );

    vertex.position = mins;
    quadVertexes.push_back( vertex );

    // top side
    vertex.position = maxs;
    quadVertexes.push_back( vertex );

    vertex.position = Vector3( maxs.x, maxs.y, mins.z );
    quadVertexes.push_back( vertex );

    vertex.position = Vector3( mins.x, maxs.y, mins.z );
    quadVertexes.push_back( vertex );

    vertex.position = Vector3( mins.x, maxs.y, maxs.z );
    quadVertexes.push_back( vertex );

    // bottom side
    vertex.position = mins;
    quadVertexes.push_back( vertex );

    vertex.position = Vector3( maxs.x, mins.y, mins.z );
    quadVertexes.push_back( vertex );

    vertex.position = Vector3( maxs.x, mins.y, maxs.z );
    quadVertexes.push_back( vertex );

    vertex.position = Vector3( mins.x, mins.y, maxs.z );
    quadVertexes.push_back( vertex );

}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void RenderWireUVSphereCommand::AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes )
{
    UNUSED( quadVertexes );
    UNUSED( triVertexes );

    Vertex3D_PUC vertex1;
    vertex1.color = color;

    Vertex3D_PUC vertex2;
    vertex2.color = color;

    Vertex3D_PUC vertex3;
    vertex3.color = color;

    Vertex3D_PUC vertex4;
    vertex4.color = color;

    for (float t = 0.0f; t < stacks; t += 1.0f) // stacks are ELEVATION so they count theta
    {
        float theta1 = PI * (t / stacks);
        float theta2 = PI * ((t + 1) / stacks);

        for (float p = 0.0f; p < slices; p += 1.0f) // slices are ORANGE SLICES so the count azimuth
        {
            float phi1 = TWO_PI * (p / slices); // azimuth goes around 0 .. 2*PI
            float phi2 = TWO_PI * ((p + 1) / slices);

            //phi2   phi1
            // |      |
            // 2------1 -- theta1
            // |\ _   |
            // |    \ |
            // 3------4 -- theta2
            //

            float sinTheta1 = sin( theta1 );
            float cosTheta1 = cos( theta1 );
            float sinTheta2 = sin( theta2 );
            float cosTheta2 = cos( theta2 );

            float sinPhi1 = sin( phi1 );
            float cosPhi1 = cos( phi1 );
            float sinPhi2 = sin( phi2 );
            float cosPhi2 = cos( phi2 );

            vertex1.position = Vector3(
                (radius * sinPhi1 * sinTheta1),
                (radius * cosTheta1),
                (radius * cosPhi1 * sinTheta1)
                ) + center;

            vertex2.position = Vector3(
                (radius * sinPhi2 * sinTheta1),
                (radius * cosTheta1),
                (radius * cosPhi2 * sinTheta1)
                ) + center;

            vertex3.position = Vector3(
                (radius * sinPhi2 * sinTheta2),
                (radius * cosTheta2),
                (radius * cosPhi2 * sinTheta2)
                ) + center;

            vertex4.position = Vector3(
                (radius * sinPhi1 * sinTheta2),
                (radius * cosTheta2),
                (radius * cosPhi1 * sinTheta2)
                ) + center;

            // facing out
            if (t == 0) // top cap
            {
                lineVertexes.push_back( vertex1 );
                lineVertexes.push_back( vertex3 );

                lineVertexes.push_back( vertex3 );
                lineVertexes.push_back( vertex4 );

                lineVertexes.push_back( vertex4 );
                lineVertexes.push_back( vertex1 );
            }
            else if (t + 1 == stacks) //end cap
            {
                lineVertexes.push_back( vertex3 );
                lineVertexes.push_back( vertex1 );

                lineVertexes.push_back( vertex1 );
                lineVertexes.push_back( vertex2 );

                lineVertexes.push_back( vertex2 );
                lineVertexes.push_back( vertex3 );
            }
            else
            {
                // body, facing OUT:
                lineVertexes.push_back( vertex1 );
                lineVertexes.push_back( vertex2 );

                lineVertexes.push_back( vertex2 );
                lineVertexes.push_back( vertex4 );

                lineVertexes.push_back( vertex4 );
                lineVertexes.push_back( vertex1 );


                lineVertexes.push_back( vertex2 );
                lineVertexes.push_back( vertex3 );

                lineVertexes.push_back( vertex3 );
                lineVertexes.push_back( vertex4 );

                lineVertexes.push_back( vertex4 );
                lineVertexes.push_back( vertex2 );
            }
        }
    }
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void RenderUVSphereCommand::AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes )
{
    UNUSED( quadVertexes );
    UNUSED( lineVertexes );

    Vertex3D_PUC vertex1;
    vertex1.color = color;

    Vertex3D_PUC vertex2;
    vertex2.color = color;

    Vertex3D_PUC vertex3;
    vertex3.color = color;

    Vertex3D_PUC vertex4;
    vertex4.color = color;

    for (float t = 0.0f; t < stacks; t += 1.0f) // stacks are ELEVATION so they count theta
    {
        float theta1 = PI * (t / stacks);
        float theta2 = PI * ((t + 1) / stacks);

        for (float p = 0.0f; p < slices; p += 1.0f) // slices are ORANGE SLICES so the count azimuth
        {
            float phi1 = TWO_PI * (p / slices); // azimuth goes around 0 .. 2*PI
            float phi2 = TWO_PI * ((p + 1) / slices);

            //phi2   phi1
            // |      |
            // 2------1 -- theta1
            // |\ _   |
            // |    \ |
            // 3------4 -- theta2
            //

            float sinTheta1 = sin( theta1 );
            float cosTheta1 = cos( theta1 );
            float sinTheta2 = sin( theta2 );
            float cosTheta2 = cos( theta2 );

            float sinPhi1 = sin( phi1 );
            float cosPhi1 = cos( phi1 );
            float sinPhi2 = sin( phi2 );
            float cosPhi2 = cos( phi2 );

            vertex1.position = Vector3(
                (radius * sinPhi1 * sinTheta1),
                (radius * cosTheta1),
                (radius * cosPhi1 * sinTheta1)
                ) + center;

            vertex2.position = Vector3(
                (radius * sinPhi2 * sinTheta1),
                (radius * cosTheta1),
                (radius * cosPhi2 * sinTheta1)
                ) + center;

            vertex3.position = Vector3(
                (radius * sinPhi2 * sinTheta2),
                (radius * cosTheta2),
                (radius * cosPhi2 * sinTheta2)
                ) + center;

            vertex4.position = Vector3(
                (radius * sinPhi1 * sinTheta2),
                (radius * cosTheta2),
                (radius * cosPhi1 * sinTheta2)
                ) + center;

            // facing out
            if (t == 0) // top cap
            {
                triVertexes.push_back( vertex1 );
                triVertexes.push_back( vertex3 );
                triVertexes.push_back( vertex4 ); //t1p1, t2p2, t2p1
            }
            else if (t + 1 == stacks) //end cap
            {
                triVertexes.push_back( vertex3 );
                triVertexes.push_back( vertex1 );
                triVertexes.push_back( vertex2 ); //t2p2, t1p1, t1p2
            }
            else
            {
                // body, facing OUT:
                triVertexes.push_back( vertex1 );
                triVertexes.push_back( vertex2 );
                triVertexes.push_back( vertex4 );

                triVertexes.push_back( vertex2 );
                triVertexes.push_back( vertex3 );
                triVertexes.push_back( vertex4 );
            }
        }
    }
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void RenderWireIcoSphereCommand::AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes )
{
    UNUSED( quadVertexes );
    UNUSED( triVertexes );

    IcoSphereCreator* icosphereCreator = IcoSphereCreator::GetInstance();
    PUC_Vertexes icoSphereVertexes = icosphereCreator->CreateIcoSphere( recursionLevel, LINES, color );

    for (PUC_Vertexes::const_iterator vertexIter = icoSphereVertexes.begin(); vertexIter != icoSphereVertexes.end(); ++vertexIter)
    {
        Vertex3D_PUC vertex = *vertexIter;
        vertex.position = center + radius * vertex.position;
        lineVertexes.push_back( vertex );

    }
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void RenderIcoSphereCommand::AddVertexes( PUC_Vertexes& lineVertexes, PUC_Vertexes& quadVertexes, PUC_Vertexes& triVertexes )
{
    UNUSED( quadVertexes );
    UNUSED( lineVertexes );

    IcoSphereCreator* icosphereCreator = IcoSphereCreator::GetInstance();
    PUC_Vertexes icoSphereVertexes = icosphereCreator->CreateIcoSphere( recursionLevel, TRIANGLES, color );

    for (PUC_Vertexes::const_iterator vertexIter = icoSphereVertexes.begin(); vertexIter != icoSphereVertexes.end(); ++vertexIter)
    {
        Vertex3D_PUC vertex = *vertexIter;
        vertex.position = center + radius * vertex.position;
        triVertexes.push_back( vertex );

    }
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
GraphicsDebugger::~GraphicsDebugger()
{
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
GraphicsDebugger* GraphicsDebugger::GetInstance()
{
    return s_theGraphicsDebugger;
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
GraphicsDebugger::GraphicsDebugger( OpenGLRenderer* renderer )
    : m_lineVBO( 0 )
    , m_quadVBO( 0 )
    , m_triVBO( 0 )
    , m_renderer( renderer )
{
    if (!s_theGraphicsDebugger)
    {
        s_theGraphicsDebugger = this;
        s_theGraphicsDebugger->m_shaderID = OpenGLRenderer::CreateOrGetShader( "Data/Shaders/basic" );
    }
}

////===========================================================================================
///===========================================================================================
// VBO functions
///===========================================================================================
////===========================================================================================

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void GraphicsDebugger::AddCommand( RenderCommand* command, const double& duration, const DepthTestMode& mode )
{
    command->callTime = GetCurrentSeconds();
    command->duration = duration;
    command->depthTestMode = mode;

    if (mode == DT_ENABLED)
        m_DepthTestEnabledCommands.push_back( command );
    else if (mode == DT_DISABLED)
        m_DepthTestDisabledCommands.push_back( command );
    else if (mode == DT_DUAL)
    {
        m_DepthTestDisabledCommands.push_back( command );
        m_DepthTestEnabledCommands.push_back( command );
    }
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void GraphicsDebugger::ExecuteCommands()
{

    for (RenderCommands::iterator dtEnabledCommandIter = m_DepthTestEnabledCommands.begin(); dtEnabledCommandIter != m_DepthTestEnabledCommands.end(); ++dtEnabledCommandIter)
    {
        RenderCommand* command = (*dtEnabledCommandIter);
        command->AddVertexes( m_lineVertexes, m_quadVertexes, m_triVertexes );
    }

    m_renderer->Enable( GL_DEPTH_TEST );
    m_renderer->SetLineSize( 5.0f );
    ///---------------------------------------------------------------------------------
    /// lines
    ///---------------------------------------------------------------------------------
    unsigned int lineBufferID = m_renderer->CreateBuffer( m_lineVertexes.data(), sizeof( Vertex3D_PUC ) * m_lineVertexes.size() );
    m_lineVBO = m_renderer->CreateVAO( m_shaderID, lineBufferID, Vertex3D_PUC::GetVertexLayout() );

    m_renderer->Draw( m_shaderID, m_lineVBO, m_lineVertexes.size(), m_renderer->GetPerspectiveMatrix(), Matrix4f::CreateIdentity(), GL_LINES );

    m_renderer->DestroyBuffer( lineBufferID );
    m_renderer->DestroyVAO( m_lineVBO );


    ///---------------------------------------------------------------------------------
    /// quads
    ///---------------------------------------------------------------------------------
    unsigned int quadBufferID = m_renderer->CreateBuffer( m_quadVertexes.data(), sizeof( Vertex3D_PUC ) * m_quadVertexes.size() );
    m_quadVBO = m_renderer->CreateVAO( m_shaderID, quadBufferID, Vertex3D_PUC::GetVertexLayout() );

    m_renderer->Draw( m_shaderID, m_quadVBO, m_quadVertexes.size(), m_renderer->GetPerspectiveMatrix(), Matrix4f::CreateIdentity(), GL_QUADS );

    m_renderer->DestroyBuffer( quadBufferID );
    m_renderer->DestroyVAO( m_quadVBO );


    ///---------------------------------------------------------------------------------
    /// tris
    ///---------------------------------------------------------------------------------
    unsigned int triBufferID = m_renderer->CreateBuffer( m_triVertexes.data(), sizeof( Vertex3D_PUC ) * m_triVertexes.size() );
    m_triVBO = m_renderer->CreateVAO( m_shaderID, triBufferID, Vertex3D_PUC::GetVertexLayout() );

    m_renderer->Draw( m_shaderID, m_triVBO, m_triVertexes.size(), m_renderer->GetPerspectiveMatrix(), Matrix4f::CreateIdentity(), GL_TRIANGLES );

    m_renderer->DestroyBuffer( triBufferID );
    m_renderer->DestroyVAO( m_triVBO );



    ClearVertexes();

    for (RenderCommands::iterator dtDisabledCommandIter = m_DepthTestDisabledCommands.begin(); dtDisabledCommandIter != m_DepthTestDisabledCommands.end(); ++dtDisabledCommandIter)
    {
        RenderCommand* command = (*dtDisabledCommandIter);
        command->AddVertexes( m_lineVertexes, m_quadVertexes, m_triVertexes );
    }

    m_renderer->Disable( GL_DEPTH_TEST );
    m_renderer->SetLineSize( 2.0f );

    ///---------------------------------------------------------------------------------
    /// lines
    ///---------------------------------------------------------------------------------
    lineBufferID = m_renderer->CreateBuffer( m_lineVertexes.data(), sizeof( Vertex3D_PUC ) * m_lineVertexes.size() );
    m_lineVBO = m_renderer->CreateVAO( m_shaderID, lineBufferID, Vertex3D_PUC::GetVertexLayout() );

    m_renderer->Draw( m_shaderID, m_lineVBO, m_lineVertexes.size(), m_renderer->GetPerspectiveMatrix(), Matrix4f::CreateIdentity(), GL_LINES );

    m_renderer->DestroyBuffer( lineBufferID );
    m_renderer->DestroyVAO( m_lineVBO );


    ///---------------------------------------------------------------------------------
    /// quads
    ///---------------------------------------------------------------------------------
    quadBufferID = m_renderer->CreateBuffer( m_quadVertexes.data(), sizeof( Vertex3D_PUC ) * m_quadVertexes.size() );
    m_quadVBO = m_renderer->CreateVAO( m_shaderID, quadBufferID, Vertex3D_PUC::GetVertexLayout() );

    m_renderer->Draw( m_shaderID, m_quadVBO, m_quadVertexes.size(), m_renderer->GetPerspectiveMatrix(), Matrix4f::CreateIdentity(), GL_QUADS );

    m_renderer->DestroyBuffer( quadBufferID );
    m_renderer->DestroyVAO( m_quadVBO );


    ///---------------------------------------------------------------------------------
    /// tris
    ///---------------------------------------------------------------------------------
    triBufferID = m_renderer->CreateBuffer( m_triVertexes.data(), sizeof( Vertex3D_PUC ) * m_triVertexes.size() );
    m_triVBO = m_renderer->CreateVAO( m_shaderID, triBufferID, Vertex3D_PUC::GetVertexLayout() );

    m_renderer->Draw( m_shaderID, m_triVBO, m_triVertexes.size(), m_renderer->GetPerspectiveMatrix(), Matrix4f::CreateIdentity(), GL_TRIANGLES );

    m_renderer->DestroyBuffer( triBufferID );
    m_renderer->DestroyVAO( m_triVBO );


    ClearVertexes();

    CleanUpCommands();
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void GraphicsDebugger::CleanUpCommands()
{
    double currentTime = GetCurrentSeconds();

    RenderCommands commandsToBeDeleted;

    for (RenderCommands::iterator dtEnabledCommandIter = m_DepthTestEnabledCommands.begin(); dtEnabledCommandIter != m_DepthTestEnabledCommands.end();)
    {
        RenderCommand* command = (*dtEnabledCommandIter);

        double commandEndTime = command->callTime + command->duration;
        if (commandEndTime < currentTime)
        {
            commandsToBeDeleted.push_back( command );
            dtEnabledCommandIter = m_DepthTestEnabledCommands.erase( dtEnabledCommandIter );
        }
        else
            ++dtEnabledCommandIter;
    }

    for (RenderCommands::iterator dtDisabledCommandIter = m_DepthTestDisabledCommands.begin(); dtDisabledCommandIter != m_DepthTestDisabledCommands.end();)
    {
        RenderCommand* command = (*dtDisabledCommandIter);

        double commandEndTime = command->callTime + command->duration;
        if (commandEndTime < currentTime)
        {
            if ( command->depthTestMode != DT_DUAL )
                commandsToBeDeleted.push_back( command );
            dtDisabledCommandIter = m_DepthTestDisabledCommands.erase( dtDisabledCommandIter );
        }
        else
            ++dtDisabledCommandIter;
    }

    for (RenderCommands::iterator cmdIter = commandsToBeDeleted.begin(); cmdIter != commandsToBeDeleted.end(); ++cmdIter)
    {
        RenderCommand* cmd = *cmdIter;
        if ( cmd )
            delete cmd;
    }
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void GraphicsDebugger::ClearVertexes()
{
    m_lineVertexes.clear();
    m_quadVertexes.clear();
    m_triVertexes.clear();
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void GraphicsDebugger::AddPoint( const Vector3& position, const double& duration, const DepthTestMode& mode, const Rgba& color /* = Rgba() */ )
{
    RenderPointCommand* newPoint = new RenderPointCommand();
    newPoint->position = position;
    newPoint->color = color;

    AddCommand( newPoint, duration, mode );
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void GraphicsDebugger::AddLineSegment( const Vector3& startPos, const Vector3& endPos, const double& duration, const DepthTestMode& mode, const Rgba& startColor /* = Rgba() */, const Rgba& endColor /* = Rgba() */ )
{
    RenderLineCommand* newLine = new RenderLineCommand();
    newLine->startPosition = startPos;
    newLine->endPosition = endPos;
    newLine->startColor = startColor;
    newLine->endColor = endColor;

    AddCommand( newLine, duration, mode );
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void GraphicsDebugger::AddArrow( const Vector3& startPos, const Vector3& endPos, const double& duration, const DepthTestMode& mode, const Rgba& color /* = Rgba() */ )
{
    RenderArrowCommand* newArrow = new RenderArrowCommand();
    newArrow->startPosition = startPos;
    newArrow->endPosition = endPos;
    newArrow->color = color;

    AddCommand( newArrow, duration, mode );
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void GraphicsDebugger::AddWireAABB( const Vector3& mins, const Vector3& maxs, const double& duration, const DepthTestMode& mode, const Rgba& lineColor /* = Rgba() */ )
{
    RenderWireAABBCommand* newWireAABB = new RenderWireAABBCommand();
    newWireAABB->mins = mins;
    newWireAABB->maxs = maxs;
    newWireAABB->lineColor = lineColor;

    AddCommand( newWireAABB, duration, mode );
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void GraphicsDebugger::AddAABB( const Vector3& mins, const Vector3& maxs, const double& duration, const DepthTestMode& mode, const Rgba& lineColor /* = Rgba() */, const Rgba& faceColor /* = Rgba() */ )
{
    RenderAABBCommand* newAABB = new RenderAABBCommand();
    newAABB->mins = mins;
    newAABB->maxs = maxs;
    newAABB->faceColor = faceColor;

    AddCommand( newAABB, duration, mode );

    AddWireAABB( mins, maxs, duration, mode, lineColor );
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void GraphicsDebugger::AddWireUVSphere( const Vector3& center, const float& radius, const int& slices, const int& stacks, const double& duration, const DepthTestMode& mode, const Rgba& color /* = Rgba() */ )
{
    RenderWireUVSphereCommand* newWireUVSphere = new RenderWireUVSphereCommand();
    newWireUVSphere->center = center;
    newWireUVSphere->radius = radius;
    newWireUVSphere->slices = slices;
    newWireUVSphere->stacks = stacks;
    newWireUVSphere->color = color;

    AddCommand( newWireUVSphere, duration, mode );
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void GraphicsDebugger::AddUVSphere( const Vector3& center, const float& radius, const int& slices, const int& stacks, const double& duration, const DepthTestMode& mode, const Rgba& color /* = Rgba() */ )
{
    RenderUVSphereCommand* newUVSphere = new RenderUVSphereCommand();
    newUVSphere->center = center;
    newUVSphere->radius = radius;
    newUVSphere->slices = slices;
    newUVSphere->stacks = stacks;
    newUVSphere->color = color;

    AddCommand( newUVSphere, duration, mode );
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void GraphicsDebugger::AddWireIcoSphere( const Vector3& center, const float& radius, const int& recursionLevel, const double& duration, const DepthTestMode& mode, const Rgba& color /* = Rgba() */ )
{
    RenderWireIcoSphereCommand* newWireIcoSphere = new RenderWireIcoSphereCommand();
    newWireIcoSphere->center = center;
    newWireIcoSphere->radius = radius;
    newWireIcoSphere->recursionLevel = recursionLevel;
    newWireIcoSphere->color = color;

    AddCommand( newWireIcoSphere, duration, mode );
}

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
void GraphicsDebugger::AddIcoSphere( const Vector3& center, const float& radius, const int& recursionLevel, const double& duration, const DepthTestMode& mode, const Rgba& color /* = Rgba() */ )
{
    RenderIcoSphereCommand* newIcoSphere = new RenderIcoSphereCommand();
    newIcoSphere->center = center;
    newIcoSphere->radius = radius;
    newIcoSphere->recursionLevel = recursionLevel;
    newIcoSphere->color = color;

    AddCommand( newIcoSphere, duration, mode );
}

CONSOLE_COMMAND( draw_icosphere, "args: Vector3 center & float radius & int recursion level & double duration & Rgba color & bool wireframe & depth test mode| desc: Draws temporary Icosphere with specified properites" )
{
    Vector3 center;
    float radius = 0.0f;
    int recursionLevel = 0;
    double duration = 0.0;
    Rgba color( Rgba::BLACK );
    bool isWireframe = false;
    std::string dtMode = "";
    DepthTestMode mode;

    if (args->GetVector3( center ) && args->GetFloat( radius ) && args->GetInt( recursionLevel ) && args->GetDouble( duration ) && ( args->GetColorFromHex( color ) || args->GetColorFromInts(color) || args->GetColorFromString(color ) ) && args->GetBool(isWireframe) && args->GetString(dtMode ) )
    {
        dtMode = ConvertToLowerCase( dtMode );
        if (dtMode == "enabled")
            mode = DT_ENABLED;
        else if (dtMode == "disabled")
            mode = DT_DISABLED;
        else if (dtMode == "dual")
            mode = DT_DUAL;
        else
        {
            DeveloperConsole::WriteLine( "draw_icosphere: Invalid Argument", ERROR_TEXT_COLOR );
            RunConsoleCommandFromLine( "help draw_icosphere" );
            return;
        }

        if (isWireframe)
            GraphicsDebugger::GetInstance()->AddWireIcoSphere( center, radius, recursionLevel, duration, DT_DUAL, color );

        else
            GraphicsDebugger::GetInstance()->AddIcoSphere( center, radius, recursionLevel, duration, DT_DUAL, color );
    }
    else
    {
        DeveloperConsole::WriteLine( "draw_icosphere: Invalid Argument", ERROR_TEXT_COLOR );
        RunConsoleCommandFromLine( "help draw_icosphere" );
    }
}

CONSOLE_COMMAND( draw_uvsphere, "args: Vector3 center & float radius & int slices & int stacks & double duration & Rgba color & bool wireframe & depth test mode | desc: Draws temporary UVsphere with specified properites" )
{
    Vector3 center;
    float radius = 0.0f;
    int slices = 0;
    int stacks = 0;
    double duration = 0.0;
    Rgba color( Rgba::BLACK );
    bool isWireframe = false;
    std::string dtMode = "";
    DepthTestMode mode;

    if (args->GetVector3( center ) && args->GetFloat( radius ) && args->GetInt( slices ) && args->GetInt( stacks ) && args->GetDouble( duration ) && (args->GetColorFromHex( color ) || args->GetColorFromInts( color ) || args->GetColorFromString( color )) && args->GetBool( isWireframe ) && args->GetString( dtMode ) )
    {
        dtMode = ConvertToLowerCase( dtMode );
        if (dtMode == "enabled")
            mode = DT_ENABLED;
        else if (dtMode == "disabled")
            mode = DT_DISABLED;
        else if (dtMode == "dual")
            mode = DT_DUAL;
        else
        {
            DeveloperConsole::WriteLine( "draw_uvsphere: Invalid Argument", ERROR_TEXT_COLOR );
            RunConsoleCommandFromLine( "help draw_uvsphere" );
            return;
        }

        if (isWireframe)
            GraphicsDebugger::GetInstance()->AddWireUVSphere( center, radius, slices, stacks, duration, mode, color );

        else
            GraphicsDebugger::GetInstance()->AddUVSphere( center, radius, slices, stacks, duration, mode, color );
    }
    else
    {
        DeveloperConsole::WriteLine( "draw_uvsphere: Invalid Argument", ERROR_TEXT_COLOR );
        RunConsoleCommandFromLine( "help draw_uvsphere" );
    }
}

CONSOLE_COMMAND( draw_aabb, "args: Vector3 mins & Vector3 maxs & double duration & Rgba line color & bool wireframe  & depth test mode & (optional) Rgba face color | desc: Draws temporary AABB with specified properites" )
{
    Vector3 mins;
    Vector3 maxs;
    double duration = 0.0;
    Rgba lineColor( Rgba::BLACK );
    Rgba faceColor( Rgba::BLACK );
    bool isWireframe = false;
    std::string dtMode = "";
    DepthTestMode mode;

    if (args->GetVector3( mins ) && args->GetVector3( maxs ) && args->GetDouble( duration ) && (args->GetColorFromHex( lineColor ) || args->GetColorFromInts( lineColor ) || args->GetColorFromString( lineColor )) && args->GetBool( isWireframe ) && args->GetString( dtMode ) )
    {
        dtMode = ConvertToLowerCase( dtMode );

        if (dtMode == "enabled")
            mode = DT_ENABLED;
        else if (dtMode == "disabled")
            mode = DT_DISABLED;
        else if (dtMode == "dual")
            mode = DT_DUAL;
        else
        {
            DeveloperConsole::WriteLine( "draw_aabb: Invalid Argument", ERROR_TEXT_COLOR );
            RunConsoleCommandFromLine( "help draw_aabb" );
            return;
        }

        if (!isWireframe)
        {
            if (args->GetColorFromHex( faceColor ) || args->GetColorFromInts( faceColor ) || args->GetColorFromString( faceColor ))
                GraphicsDebugger::GetInstance()->AddAABB( mins, maxs, duration, mode, lineColor, faceColor );
            else
                GraphicsDebugger::GetInstance()->AddAABB( mins, maxs, duration, mode, lineColor, lineColor );
        }

        else
            GraphicsDebugger::GetInstance()->AddWireAABB( mins, maxs, duration, mode, lineColor );
    }
    else
    {
        DeveloperConsole::WriteLine( "draw_aabb: Invalid Argument", ERROR_TEXT_COLOR );
        RunConsoleCommandFromLine( "help draw_aabb" );
    }
}

CONSOLE_COMMAND( draw_arrow, "args: Vector3 start pos & Vector3 end pos & double duration & Rgba color & depth test mode | desc: Draws temporary Arrow with specified properties" )
{
    Vector3 start;
    Vector3 end;
    double duration = 0.0;
    Rgba color( Rgba::BLACK );
    std::string dtMode = "";
    DepthTestMode mode;

    if (args->GetVector3( start ) && args->GetVector3( end ) && args->GetDouble( duration ) && (args->GetColorFromHex( color ) || args->GetColorFromInts( color ) || args->GetColorFromString( color )) && args->GetString( dtMode ))
    {
        dtMode = ConvertToLowerCase( dtMode );
        if (dtMode == "enabled")
            mode = DT_ENABLED;
        else if (dtMode == "disabled")
            mode = DT_DISABLED;
        else if (dtMode == "dual")
            mode = DT_DUAL;
        else
        {
            DeveloperConsole::WriteLine( "draw_arrow: Invalid Argument", ERROR_TEXT_COLOR );
            RunConsoleCommandFromLine( "help draw_arrow" );
            return;
        }

            GraphicsDebugger::GetInstance()->AddArrow( start, end, duration, mode, color );
    }
    else
    {
        DeveloperConsole::WriteLine( "draw_arrow: Invalid Argument", ERROR_TEXT_COLOR );
        RunConsoleCommandFromLine( "help draw_arrow" );
    }
}

CONSOLE_COMMAND( draw_line, "args: Vector3 start pos & Vector3 end pos & double duration & Rgba start color & depth test mode & (optional) Rgba end color | desc: Draws temporary Line Segment with specified properties" )
{
    Vector3 start;
    Vector3 end;
    double duration = 0.0;
    Rgba startColor( Rgba::BLACK );
    Rgba endColor( Rgba::BLACK );
    std::string dtMode = "";
    DepthTestMode mode;

    if (args->GetVector3( start ) && args->GetVector3( end ) && args->GetDouble( duration ) && (args->GetColorFromHex( startColor ) || args->GetColorFromInts( startColor ) || args->GetColorFromString( startColor )) && args->GetString( dtMode ))
    {
        dtMode = ConvertToLowerCase( dtMode );
        if (dtMode == "enabled")
            mode = DT_ENABLED;
        else if (dtMode == "disabled")
            mode = DT_DISABLED;
        else if (dtMode == "dual")
            mode = DT_DUAL;
        else
        {
            DeveloperConsole::WriteLine( "draw_line: Invalid Argument", ERROR_TEXT_COLOR );
            RunConsoleCommandFromLine( "help draw_line" );
            return;
        }

        if (args->GetColorFromHex( endColor ) || args->GetColorFromInts( endColor ) || args->GetColorFromString( endColor ))
            GraphicsDebugger::GetInstance()->AddLineSegment( start, end, duration, mode, startColor, endColor );
        else
            GraphicsDebugger::GetInstance()->AddLineSegment( start, end, duration, mode, startColor, startColor );
    }
    else
    {
        DeveloperConsole::WriteLine( "draw_line: Invalid Argument", ERROR_TEXT_COLOR );
        RunConsoleCommandFromLine( "help draw_line" );
    }
}

CONSOLE_COMMAND( draw_point, "args: Vector3 position & double duration & Rgba color & depth test mode | desc: Draws temporary Point with specified properties" )
{
    Vector3 position;
    double duration = 0.0;
    Rgba color( Rgba::BLACK );
    std::string dtMode = "";
    DepthTestMode mode;

    if (args->GetVector3( position ) && args->GetDouble( duration ) && (args->GetColorFromHex( color ) || args->GetColorFromInts( color ) || args->GetColorFromString( color )) && args->GetString( dtMode ))
    {
        dtMode = ConvertToLowerCase( dtMode );
        if (dtMode == "enabled")
            mode = DT_ENABLED;
        else if (dtMode == "disabled")
            mode = DT_DISABLED;
        else if (dtMode == "dual")
            mode = DT_DUAL;
        else
        {
            DeveloperConsole::WriteLine( "draw_point: Invalid Argument", ERROR_TEXT_COLOR );
            RunConsoleCommandFromLine( "help draw_point" );
            return;
        }

        GraphicsDebugger::GetInstance()->AddPoint( position, duration, mode, color );
    }
    else
    {
        DeveloperConsole::WriteLine( "draw_point: Invalid Argument", ERROR_TEXT_COLOR );
        RunConsoleCommandFromLine( "help draw_point" );
    }
}