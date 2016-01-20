//=================================================================================
// Image.cpp
// Author: Tyler George
// Date  : October 26, 2015
//=================================================================================


////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================

#include "Engine/UI/Image.hpp"
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
Image::Image( OpenGLRenderer* renderer, UIElement* parent, Texture* image, const Vector2& size,
    Anchor anchor /* = BOTTOM_LEFT */, const Vector2& offset /* = Vector2::ZERO */, const float& zVal /* = 1.0f */ )
    : UIElement( renderer, parent )
    , m_image( image )
{
    m_zVal = zVal;
    m_anchor = anchor;
    m_offset = offset;
    m_size = size;

    Vector2 mins = CalculatePosition();

    m_bounds.m_mins = mins;
    m_bounds.m_maxs = mins + m_size;
    CalcVerts();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Image::~Image()
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

////===========================================================================================
///===========================================================================================
// Update
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Image::ProcessInput( InputSystem* inputSystem )
{
    UNUSED( inputSystem );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Image::Update( bool debugModeEnabled )
{
    UNUSED( debugModeEnabled );

}


////===========================================================================================
///===========================================================================================
// Render
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Image::Render( bool debugModeEnabled )
{
    UNUSED( debugModeEnabled );
    if (!m_renderer)
        return;

    unsigned int shaderID = OpenGLRenderer::CreateOrGetShader( "Data/Shaders/textured" );
    if (m_verts.size() > 0)
    {
        m_renderer->Enable( GL_BLEND );
        m_renderer->BlendFunct( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        if (m_image)
            m_renderer->DrawWithTextureOrtho( shaderID, Vertex3D_PUC::GetVertexLayout( m_verts ), m_verts.size(), m_renderer->GetDefaultSampler(), m_image );
    }


}

////===========================================================================================
///===========================================================================================
// Private Functions
///===========================================================================================
////===========================================================================================

