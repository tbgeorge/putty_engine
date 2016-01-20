//=================================================================================
// ProgressBar.cpp
// Author: Tyler George
// Date  : November 16, 2015
//=================================================================================


////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================

#include "Engine/UI/ProgressBar.hpp"
#include "../Utilities/DeveloperConsole.hpp"


////===========================================================================================
///===========================================================================================
// Static Variable Initialization
///===========================================================================================
////===========================================================================================



////===========================================================================================
///===========================================================================================
// Constructors/Destructors
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
ProgressBar::ProgressBar( OpenGLRenderer* renderer, UIElement* parent, const IntRange& values, Font* font, const Vector2& size, const Rgba& backgroundColor, const Rgba& foregroundColor,
    Anchor anchor /* = BOTTOM_LEFT */, const Vector2& offset /* = Vector2::ZERO */, const float& zVal /* = 1.0f */, const Rgba& fontColor /* = Rgba() */ )
    : UIElement( renderer, parent )
    , m_fontColor( fontColor )
    , m_backgroundColor( backgroundColor )
    , m_foregroundColor( foregroundColor )
    , m_percentage( 1.0f )
    , m_values( values )
{
    m_zVal = zVal;
    m_anchor = anchor;
    m_offset = offset;
    m_size = size;

    Vector2 mins = CalculatePosition();

    m_bounds.m_mins = mins;
    m_bounds.m_maxs = mins + m_size;
    CalcVerts();

    m_textbox = new TextBox( renderer, this, font, 12, CENTER_CENTER, Vector2::ZERO, fontColor, "", zVal - 0.3f );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
ProgressBar::~ProgressBar()
{
    delete m_textbox;
}

////===========================================================================================
///===========================================================================================
// Initialization
///===========================================================================================
////===========================================================================================


////===========================================================================================
///===========================================================================================
// Accessors/Queries
///===========================================================================================
////===========================================================================================

////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================

////===========================================================================================
///===========================================================================================
// Update
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ProgressBar::ProcessInput( InputSystem* inputSystem )
{ 
    UNUSED( inputSystem );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ProgressBar::Update( bool debugModeEnabled )
{
    UNUSED( debugModeEnabled );
    m_textbox->SetText( std::to_string( (int)floorf( RangeMap( m_percentage, 0.0f, 1.0f, (float)m_values.m_min, (float)m_values.m_max ) ) ) + "/" + std::to_string( m_values.m_max ) );

}


////===========================================================================================
///===========================================================================================
// Render
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ProgressBar::Render( bool debugModeEnabled )
{
    if (!m_renderer)
        return;

    if ( debugModeEnabled )
    {
        PUC_Vertexes verts;

        verts.push_back( Vertex3D_PUC( Vector3( m_bounds.m_mins.x, m_bounds.m_mins.y, m_zVal - 0.1f ), Vector2::ZERO, Rgba::BLACK ) ); // 0
        verts.push_back( Vertex3D_PUC( Vector3( m_bounds.m_maxs.x, m_bounds.m_mins.y, m_zVal - 0.1f ), Vector2::ZERO, Rgba::BLACK ) ); // 1
        verts.push_back( Vertex3D_PUC( Vector3( m_bounds.m_maxs.x, m_bounds.m_maxs.y, m_zVal - 0.1f ), Vector2::ZERO, Rgba::BLACK ) ); // 2
        verts.push_back( Vertex3D_PUC( Vector3( m_bounds.m_mins.x, m_bounds.m_maxs.y, m_zVal - 0.1f ), Vector2::ZERO, Rgba::BLACK ) ); // 3

        m_renderer->SetLineSize( 5.0f );
        m_renderer->DrawVertexesOrtho( NULL, verts, GL_LINE_LOOP );
    }

    PUC_Vertexes verts;

    verts.push_back( Vertex3D_PUC( Vector3( m_bounds.m_mins.x, m_bounds.m_mins.y, m_zVal - 0.1f ), Vector2::ZERO, m_backgroundColor ) ); // 0
    verts.push_back( Vertex3D_PUC( Vector3( m_bounds.m_maxs.x, m_bounds.m_mins.y, m_zVal - 0.1f ), Vector2::ZERO, m_backgroundColor ) ); // 1
    verts.push_back( Vertex3D_PUC( Vector3( m_bounds.m_maxs.x, m_bounds.m_maxs.y, m_zVal - 0.1f ), Vector2::ZERO, m_backgroundColor ) ); // 2
    verts.push_back( Vertex3D_PUC( Vector3( m_bounds.m_mins.x, m_bounds.m_maxs.y, m_zVal - 0.1f ), Vector2::ZERO, m_backgroundColor ) ); // 3
    m_renderer->DrawVertexesOrtho( NULL, verts, GL_QUADS );

    for (unsigned int vertNum = 0; vertNum < verts.size(); ++vertNum)
    {
        verts[vertNum].color = m_foregroundColor;
        verts[vertNum].position.z -= 0.1f;
    }

    verts[1].position.x = m_bounds.m_mins.x + ( m_size.x * m_percentage );
    verts[2].position.x = m_bounds.m_mins.x + (m_size.x * m_percentage);

    m_renderer->DrawVertexesOrtho( NULL, verts, GL_QUADS );

    m_textbox->Render( debugModeEnabled );



}

////===========================================================================================
///===========================================================================================
// Private Functions
///===========================================================================================
////===========================================================================================

