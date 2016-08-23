//=================================================================================
// CheckboxWidget.cpp
// Author: Tyler George
// Date  : May 8, 2016
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "Engine/Systems/UI/CheckboxWidget.hpp"
#include "../../Math/AABB2D.hpp"
#include "../../Utilities/Font.hpp"
#include "../../Renderer/FontRenderer.hpp"
#include "../../Utilities/XMLHelper.hpp"

////===========================================================================================
///===========================================================================================
// class CheckboxWidget
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
CheckboxWidget::CheckboxWidget( const std::string& widgetName, const WidgetPropertyLayer& instanceProperties )
    : ButtonWidget( widgetName, instanceProperties )
{
    m_type = WidgetType_CHECKBOX;

    m_codeProperties.m_defaults.Set( "FillColor", Rgba::BLACK );
    m_codeProperties.m_defaults.Set( "FillOpacity", 1.0f );
    m_codeProperties.m_defaults.Set( "FillEdgeThickness", 5.0f );
    m_codeProperties.m_defaults.Set( "IsChecked", false );
    m_codeProperties.m_defaults.Set( "EventFiredOnChecked", "PrintChecked" );
    m_codeProperties.m_defaults.Set( "EventFiredOnUnChecked", "PrintUnchecked" );
    
    m_codeProperties.m_defaults.Set( "Size", Vector2( 60.0f, 60.0f ) );
    m_codeProperties.m_defaults.Set( "BGColor", Rgba( 0xEEEEEE ) );

    m_codeProperties.m_defaultsByState[WidgetState_HIGHLIGHTED].Set( "FillColor", Rgba( 0x88888888 ) );
    m_codeProperties.m_defaultsByState[WidgetState_HIGHLIGHTED].Set( "BGColor", Rgba( 0xEEEEEE ) );

    m_codeProperties.m_defaultsByState[WidgetState_PRESSED].Set( "FillColor", Rgba( 0x444444 ) );
    m_codeProperties.m_defaultsByState[WidgetState_PRESSED].Set( "FillEdgeThickness", 10.0f );
    m_codeProperties.m_defaultsByState[WidgetState_PRESSED].Set( "BGColor", Rgba( 0xEEEEEE ) );

    m_codeProperties.m_defaultsByState[WidgetState_DISABLED].Set( "FillColor", Rgba( 0xAAAAAA ) );

    //fill verts
    Rgba fillColor;
    GetPropertyByName( "FillColor", fillColor );
    AABB2D bounds = GetWidgetBounds();
    float fillEdgeThickness;
    GetPropertyByName( "FillEdgeThickness", fillEdgeThickness );

    GetPropertyByName( "IsChecked", m_isChecked );
    Vector2 mins = bounds.m_mins;
    Vector2 maxs = bounds.m_maxs;

    m_fillVerts.push_back( Vertex3D_PUC( Vector3( mins.x + fillEdgeThickness, mins.y + fillEdgeThickness, 1.0f ), Vector2( 0.0f, 0.0f ), fillColor ) );                     // 0
    m_fillVerts.push_back( Vertex3D_PUC( Vector3( maxs.x - fillEdgeThickness, mins.y + fillEdgeThickness, 1.0f ), Vector2( 0.0f, 0.0f ), fillColor ) );            // 1
    m_fillVerts.push_back( Vertex3D_PUC( Vector3( maxs.x - fillEdgeThickness, maxs.y - fillEdgeThickness, 1.0f ), Vector2( 0.0f, 0.0f ), fillColor ) );                                            // 2
    
    m_fillVerts.push_back( Vertex3D_PUC( Vector3( maxs.x - fillEdgeThickness, maxs.y - fillEdgeThickness, 1.0f ), Vector2( 0.0f, 0.0f ), fillColor ) );                                            // 2
    m_fillVerts.push_back( Vertex3D_PUC( Vector3( mins.x + fillEdgeThickness, maxs.y - fillEdgeThickness, 1.0f ), Vector2( 0.0f, 0.0f ), fillColor ) );                                                     // 3
    m_fillVerts.push_back( Vertex3D_PUC( Vector3( mins.x + fillEdgeThickness, mins.y + fillEdgeThickness, 1.0f ), Vector2( 0.0f, 0.0f ), fillColor ) );                     // 0


}

////---------------------------------------------------------------------------------
///
// Accessors/Queries
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void CheckboxWidget::GetPropertiesFromXML( const XMLNode& node, NamedProperties& in_out_properties )
{
    XMLNode fillColorNode = node.getChildNode( "FillColor" );
    if (!fillColorNode.isEmpty())
    {
        Rgba fillColor = GetRgbaProperty( fillColorNode, "value", Rgba::BLACK );
        in_out_properties.Set( "FillColor", fillColor );
    }

    XMLNode fillOpacityNode = node.getChildNode( "FillOpacity" );
    if (!fillOpacityNode.isEmpty())
    {
        float fillOpacity = GetFloatProperty( fillOpacityNode, "value", 1.0f );
        in_out_properties.Set( "FillOpacity", fillOpacity );
    }

    XMLNode fillEdgeThicknessNode = node.getChildNode( "FillEdgeThickness" );
    if (!fillEdgeThicknessNode.isEmpty())
    {
        float fillEdgeThickness = GetFloatProperty( fillEdgeThicknessNode, "value", 1.0f );
        in_out_properties.Set( "FillEdgeThickness", fillEdgeThickness );
    }

    XMLNode isCheckedNode = node.getChildNode( "IsChecked" );
    if (!isCheckedNode.isEmpty())
    {
        bool isChecked = GetBooleanProperty( isCheckedNode, "value", false );
        in_out_properties.Set( "IsChecked", isChecked );
    }

    XMLNode eventCheckedNode = node.getChildNode( "EventFiredOnChecked" );
    if (!eventCheckedNode.isEmpty())
    {
        std::string eventFiredOnChecked = GetStringProperty( eventCheckedNode, "value", "PrintChecked" );
        in_out_properties.Set( "EventFiredOnChecked", eventFiredOnChecked );
    }

    XMLNode eventUncheckedNode = node.getChildNode( "EventFiredOnUnChecked" );
    if (!eventUncheckedNode.isEmpty())
    {
        std::string eventFiredOnUnchecked = GetStringProperty( eventUncheckedNode, "value", "PrintUnchecked" );
        in_out_properties.Set( "EventFiredOnUnchecked", eventFiredOnUnchecked );
    }

    ButtonWidget::GetPropertiesFromXML( node, in_out_properties );
}

////---------------------------------------------------------------------------------
///
// Mutators
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void CheckboxWidget::Clicked()
{
    NamedProperties eventProps;
    eventProps.Set( "WidgetName", m_name );

    // unchecked
    if (m_isChecked)
    {
        m_isChecked = false;
        std::string eventFiredOnUnchecked;
        GetPropertyByName( "EventFiredOnUnChecked", eventFiredOnUnchecked );
        FireEvent( eventFiredOnUnchecked, eventProps );
        m_currentState = WidgetState_HIGHLIGHTED;
    }
    else // checked
    {
        m_isChecked = true;
        std::string eventFiredOnChecked;
        GetPropertyByName( "EventFiredOnChecked", eventFiredOnChecked );
        FireEvent( eventFiredOnChecked, eventProps );
        m_currentState = WidgetState_HIGHLIGHTED;
    }
}


////---------------------------------------------------------------------------------
///
// Update
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void CheckboxWidget::Update()
{
    if (m_currentState == WidgetState_HIDDEN)
        return;

    m_instanceProperties.m_defaults.Set( "IsChecked", m_isChecked );

    if (m_isChecked || m_currentState == WidgetState_HIGHLIGHTED || m_currentState == WidgetState_PRESSED )
    {
        float widgetOpactiy = GetWidgetOpacity();

        Rgba fillColor;
        GetPropertyByName( "FillColor", fillColor );

        if (m_isChecked && m_currentState == WidgetState_HIGHLIGHTED)
        {
            m_currentState = WidgetState_IDLE;
            GetPropertyByName( "FillColor", fillColor );
            m_currentState = WidgetState_HIGHLIGHTED;
        }

        float fillOpacity;
        GetPropertyByName( "FillOpacity", fillOpacity );

        for (auto iter = m_fillVerts.begin(); iter != m_fillVerts.end(); ++iter)
        {
            Vertex3D_PUC& vert = *iter;
            vert.color = fillColor;
            vert.color.SetAlpha( vert.color.Af() * fillOpacity * widgetOpactiy );
        }

        UpdateVertsFromBounds();
    }

    ButtonWidget::Update();

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void CheckboxWidget::UpdateVertsFromBounds()
{
    //fill verts
    AABB2D bounds = GetWidgetBounds();
    float fillEdgeThickness;
    GetPropertyByName( "FillEdgeThickness", fillEdgeThickness );

    Vector2 mins = bounds.m_mins;
    Vector2 maxs = bounds.m_maxs;

    m_fillVerts[0].position = Vector3( mins.x + fillEdgeThickness, mins.y + fillEdgeThickness, 1.0f );
    m_fillVerts[1].position = Vector3( maxs.x - fillEdgeThickness, mins.y + fillEdgeThickness, 1.0f );
    m_fillVerts[2].position = Vector3( maxs.x - fillEdgeThickness, maxs.y - fillEdgeThickness, 1.0f );
              
    m_fillVerts[3].position = Vector3( maxs.x - fillEdgeThickness, maxs.y - fillEdgeThickness, 1.0f );
    m_fillVerts[4].position = Vector3( mins.x + fillEdgeThickness, maxs.y - fillEdgeThickness, 1.0f );
    m_fillVerts[5].position = Vector3( mins.x + fillEdgeThickness, mins.y + fillEdgeThickness, 1.0f );

    BaseWidget::UpdateVertsFromBounds();
}

////---------------------------------------------------------------------------------
///
// Render
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void CheckboxWidget::Render( OpenGLRenderer* renderer )
{
    if (!renderer)
        return;

    if (m_currentState == WidgetState_HIDDEN)
        return;

    renderer->Enable( GL_BLEND );
    renderer->BlendFunct( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    renderer->DrawVertexesOrtho( NULL, m_contentVerts, GL_TRIANGLES );
    renderer->DrawVertexesOrtho( NULL, m_edgeVerts, GL_TRIANGLES );

    if (m_isChecked || m_currentState == WidgetState_HIGHLIGHTED || m_currentState == WidgetState_PRESSED)
            renderer->DrawVertexesOrtho( NULL, m_fillVerts, GL_TRIANGLES );
}