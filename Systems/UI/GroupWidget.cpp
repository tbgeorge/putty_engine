//=================================================================================
// GroupWidget.cpp
// Author: Tyler George
// Date  : April 20, 2016
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "Engine/Systems/UI/GroupWidget.hpp"
#include "../../Math/AABB2D.hpp"
#include "../../Utilities/Font.hpp"
#include "../../Renderer/FontRenderer.hpp"
#include "../../Utilities/XMLHelper.hpp"

////===========================================================================================
///===========================================================================================
// class GroupWidget
///===========================================================================================
////===========================================================================================

////---------------------------------------------------------------------------------
///
// Constructors/Destructors
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
GroupWidget::GroupWidget( const std::string& widgetName, const WidgetPropertyLayer& instanceProperties )
    : BaseWidget( widgetName, instanceProperties )
{
    m_type = WidgetType_GROUP;

    m_codeProperties.m_defaults.Set( "Opacity", 1.0f );

    m_codeProperties.m_defaultsByState[WidgetState_DISABLED].Set( "Opacity", 0.5f );
}

////---------------------------------------------------------------------------------
///
// Accessors/Queries
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void GroupWidget::GetPropertiesFromXML( const XMLNode& node, NamedProperties& in_out_properties )
{
//     XMLNode groupOpacityNode = node.getChildNode( "GroupOpacity" );
//     if (!groupOpacityNode.isEmpty())
//     {
//         float groupOpacity = GetFloatProperty( groupOpacityNode, "value", 1.0f );
//         in_out_properties.Set( "GroupOpacity", groupOpacity );
//     }

    BaseWidget::GetPropertiesFromXML( node, in_out_properties );
}

////---------------------------------------------------------------------------------
///
// Mutators
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool GroupWidget::AddChild( BaseWidget* child )
{
    if (!child)
        return false;

    m_children.push_back( child );
    child->SetParent( this );
    return true;
}

////---------------------------------------------------------------------------------
///
// Update
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void GroupWidget::ProcessInput( InputSystem* inputSystem )
{
    if (m_currentState == WidgetState_DISABLED || m_currentState == WidgetState_HIDDEN)
        return;

    for (BaseWidget* child : m_children)
    {
        child->ProcessInput( inputSystem );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void GroupWidget::Update()
{
    if (m_currentState == WidgetState_HIDDEN)
        return;

    float widgetOpactiy = GetWidgetOpacity();

    Rgba bgColor;
    GetPropertyByName( "BGColor", bgColor );

    Rgba edgeColor;
    GetPropertyByName( "EdgeColor", edgeColor );

    for (auto iter = m_contentVerts.begin(); iter != m_contentVerts.end(); ++iter)
    {
        Vertex3D_PUC& vert = *iter;
        vert.color = bgColor;
        vert.color.SetAlpha( vert.color.Af() * widgetOpactiy );
    }

    for (auto iter = m_edgeVerts.begin(); iter != m_edgeVerts.end(); ++iter)
    {
        Vertex3D_PUC& vert = *iter;
        vert.color = edgeColor;
        vert.color.SetAlpha( vert.color.Af() * widgetOpactiy );
    }

    BaseWidget::UpdateVertsFromBounds();

    for (BaseWidget* child : m_children)
        child->Update();

}

////---------------------------------------------------------------------------------
///
// Render
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void GroupWidget::Render( OpenGLRenderer* renderer )
{
    if (!renderer)
        return;

    if (m_currentState == WidgetState_HIDDEN)
        return;

    renderer->Enable( GL_BLEND );
    renderer->BlendFunct( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    renderer->DrawVertexesOrtho( NULL, m_contentVerts, GL_TRIANGLES );
    renderer->DrawVertexesOrtho( NULL, m_edgeVerts, GL_TRIANGLES );

    for (BaseWidget* child : m_children)
    {
        child->Render( renderer );
    }
}