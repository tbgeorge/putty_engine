//=================================================================================
// UIElement.cpp
// Author: Tyler George
// Date  : November 8, 2015
//=================================================================================


////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================

#include "Engine/UI/UIElement.hpp"


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
UIElement::UIElement( OpenGLRenderer* renderer, UIElement* parent /* = nullptr */ )
    : m_renderer( renderer )
    , m_parent( parent )
    , m_size( Vector2::ZERO )
    , m_anchor( BOTTOM_LEFT )
    , m_offset( Vector2::ZERO )
    , m_zVal( 0.0f )
    , m_bounds( Vector2::ZERO, Vector2::ZERO )
{

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
UIElement::UIElement( OpenGLRenderer* renderer, const Vector2& size, UIElement* parent /* = nullptr */, Anchor anchor /* = BOTTOM_LEFT */, const Vector2& offset /* = Vector2::ZERO */ )
    : m_renderer( renderer )
    , m_parent( parent )
    , m_size( size )
    , m_anchor( anchor )
    , m_offset( offset )
{
    Vector2 mins = CalculatePosition();
    m_bounds = AABB2D( mins, mins + m_size );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
UIElement::~UIElement()
{

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

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void UIElement::CalcVerts()
{
    m_verts.clear();
    m_verts.resize( 0 );

    Vector3 lowerLeft( m_bounds.m_mins.x, m_bounds.m_mins.y, m_zVal );
    Vector3 lowerRight( m_bounds.m_maxs.x, m_bounds.m_mins.y, m_zVal );
    Vector3 upperRight( m_bounds.m_maxs.x, m_bounds.m_maxs.y, m_zVal );
    Vector3 upperLeft( m_bounds.m_mins.x, m_bounds.m_maxs.y, m_zVal );

    m_verts.push_back( Vertex3D_PUC( lowerLeft, Vector2( 0.0f, 1.0f ), Rgba() ) );
    m_verts.push_back( Vertex3D_PUC( lowerRight, Vector2( 1.0f, 1.0f ), Rgba() ) );
    m_verts.push_back( Vertex3D_PUC( upperRight, Vector2( 1.0f, 0.0f ), Rgba() ) );

    m_verts.push_back( Vertex3D_PUC( upperRight, Vector2( 1.0f, 0.0f ), Rgba() ) );
    m_verts.push_back( Vertex3D_PUC( upperLeft, Vector2( 0.0f, 0.0f ), Rgba() ) );
    m_verts.push_back( Vertex3D_PUC( lowerLeft, Vector2( 0.0f, 1.0f ), Rgba() ) );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Vector2 UIElement::CalculatePosition()
{
    Vector2 position( 0.0f, 0.0f );

    float xPos = 0.0f;
    float yPos = 0.0f;

    AABB2D parentBounds;
    Vector2 parentSize;

    if (!m_parent)
    {
        parentBounds.m_mins = Vector2::ZERO;
        parentBounds.m_maxs = Vector2( m_renderer->GetDisplayWidth(), m_renderer->GetDisplayHeight() );

        parentSize = parentBounds.m_maxs;
    }
    else
    {
        parentBounds = m_parent->m_bounds;
        parentSize = m_parent->m_size;
    }


    switch (m_anchor)
    {
    case TOP_LEFT:
        xPos = parentBounds.m_mins.x + m_offset.x;
        yPos = parentBounds.m_maxs.y - m_offset.y - m_size.y;
        break;
    case TOP_CENTER:
        xPos = parentBounds.m_mins.x + ((parentSize.x - m_size.x) / 2.0f) + m_offset.x;
        yPos = parentBounds.m_maxs.y - m_offset.y - m_size.y;
        break;
    case TOP_RIGHT:
        xPos = parentBounds.m_maxs.x - m_offset.x - m_size.x;
        yPos = parentBounds.m_maxs.y - m_offset.y - m_size.y;
        break;
    case CENTER_LEFT:
        xPos = parentBounds.m_mins.x + m_offset.x;
        yPos = parentBounds.m_mins.y + ((parentSize.y - m_size.y) / 2.0f) + m_offset.y;
        break;
    case CENTER_CENTER:
        xPos = parentBounds.m_mins.x + ((parentSize.x - m_size.x) / 2.0f) + m_offset.x;
        yPos = parentBounds.m_mins.y + ((parentSize.y - m_size.y) / 2.0f) + m_offset.y;
        break;
    case CENTER_RIGHT:
        xPos = parentBounds.m_maxs.x - m_offset.x - m_size.x;
        yPos = parentBounds.m_mins.y + ((parentSize.y - m_size.y) / 2.0f) + m_offset.y;
        break;
    case BOTTOM_LEFT:
        xPos = parentBounds.m_mins.x + m_offset.x;
        yPos = parentBounds.m_mins.y + m_offset.y;
        break;
    case BOTTOM_CENTER:
        xPos = parentBounds.m_mins.x + ((parentSize.x - m_size.x) / 2.0f) + m_offset.x;
        yPos = parentBounds.m_mins.y + m_offset.y;
        break;
    case BOTTOM_RIGHT:
        xPos = parentBounds.m_maxs.x - m_size.x - m_offset.x;
        yPos = parentBounds.m_mins.y + m_offset.y;
        break;
    default:
        break;
    }
    position = Vector2( xPos, yPos );
    return position;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void UIElement::SetAnchor( Anchor alignment )
{ 
    m_anchor = alignment; 
    CalculatePosition();
    CalcVerts();
}

////===========================================================================================
///===========================================================================================
// Update
///===========================================================================================
////===========================================================================================

////===========================================================================================
///===========================================================================================
// Render
///===========================================================================================
////===========================================================================================

////===========================================================================================
///===========================================================================================
// Private Functions
///===========================================================================================
////===========================================================================================

