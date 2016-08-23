//=================================================================================
// ProgressBarWidget.cpp
// Author: Tyler George
// Date  : May 8, 2016
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "Engine/Systems/UI/ProgressBarWidget.hpp"
#include "../../Math/AABB2D.hpp"
#include "../../Utilities/Font.hpp"
#include "../../Renderer/FontRenderer.hpp"
#include "../../Utilities/XMLHelper.hpp"
#include "../../Utilities/Utilities.hpp"

////===========================================================================================
///===========================================================================================
// class ProgressBarWidget
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
ProgressBarWidget::ProgressBarWidget( const std::string& widgetName, const WidgetPropertyLayer& instanceProperties )
    : BaseWidget( widgetName, instanceProperties )
{
    m_type = WidgetType_PROGRESS_BAR;

    m_codeProperties.m_defaults.Set( "FillColor", Rgba::GREEN );
    m_codeProperties.m_defaults.Set( "FillOpacity", 1.0f );
    m_codeProperties.m_defaults.Set( "FillAmount", 0.3f );
    m_codeProperties.m_defaults.Set( "FillDirection", "right" );
    m_codeProperties.m_defaults.Set( "Size", Vector2( 400.0f, 50.0f ) );
    m_codeProperties.m_defaults.Set( "BGColor", Rgba( 0x887777 ) );


    m_codeProperties.m_defaultsByState[WidgetState_DISABLED].Set( "BGColor", Rgba( 0x808080 ) );
    m_codeProperties.m_defaultsByState[WidgetState_DISABLED].Set( "EdgeColor", Rgba( 0x777777 ) );
    m_codeProperties.m_defaultsByState[WidgetState_DISABLED].Set( "FillColor", Rgba( 0x007700 ) );
    m_codeProperties.m_defaultsByState[WidgetState_DISABLED].Set( "FillOpacity", 0.5f );

    //fill verts
    Rgba fillColor;
    GetPropertyByName( "FillColor", fillColor );
    AABB2D bounds = GetWidgetBounds();

    float fillAmount;
    GetPropertyByName( "FillAmount", fillAmount );
    std::string fillDir = "";
    GetPropertyByName( "FillDirection", fillDir );
    fillDir = ConvertToLowerCase( fillDir );

    Vector2 mins = bounds.m_mins;
    Vector2 maxs = bounds.m_maxs;

    if (fillDir == "right")
    {
        float length = maxs.x - mins.x;
        float fillMaxX = mins.x + (length * fillAmount);

        m_fillVerts.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, 1.0f ), Vector2( 0.0f, 1.0f ), fillColor ) );
        m_fillVerts.push_back( Vertex3D_PUC( Vector3( fillMaxX, mins.y, 1.0f ), Vector2( 1.0f, 1.0f ), fillColor ) );
        m_fillVerts.push_back( Vertex3D_PUC( Vector3( fillMaxX, maxs.y, 1.0f ), Vector2( 1.0f, 0.0f ), fillColor ) );

        m_fillVerts.push_back( Vertex3D_PUC( Vector3( fillMaxX, maxs.y, 1.0f ), Vector2( 1.0f, 0.0f ), fillColor ) );
        m_fillVerts.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, 1.0f ), Vector2( 0.0f, 0.0f ), fillColor ) );
        m_fillVerts.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, 1.0f ), Vector2( 0.0f, 1.0f ), fillColor ) );
    }

    else if (fillDir == "left")
    {
        float length = maxs.x - mins.x;
        float fillMinX = maxs.x - (length * fillAmount);

        m_fillVerts.push_back( Vertex3D_PUC( Vector3( fillMinX, mins.y, 1.0f ), Vector2( 0.0f, 1.0f ), fillColor ) );
        m_fillVerts.push_back( Vertex3D_PUC( Vector3( maxs.x, mins.y, 1.0f ), Vector2( 1.0f, 1.0f ), fillColor ) );
        m_fillVerts.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, 1.0f ), Vector2( 1.0f, 0.0f ), fillColor ) );

        m_fillVerts.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, 1.0f ), Vector2( 1.0f, 0.0f ), fillColor ) );
        m_fillVerts.push_back( Vertex3D_PUC( Vector3( fillMinX, maxs.y, 1.0f ), Vector2( 0.0f, 0.0f ), fillColor ) );
        m_fillVerts.push_back( Vertex3D_PUC( Vector3( fillMinX, mins.y, 1.0f ), Vector2( 0.0f, 1.0f ), fillColor ) );
    }

    else if (fillDir == "up")
    {
        float length = maxs.y - mins.y;
        float fillMaxY = mins.y + (length * fillAmount);

        m_fillVerts.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, 1.0f ), Vector2( 0.0f, 1.0f ), fillColor ) );
        m_fillVerts.push_back( Vertex3D_PUC( Vector3( maxs.x, mins.y, 1.0f ), Vector2( 1.0f, 1.0f ), fillColor ) );
        m_fillVerts.push_back( Vertex3D_PUC( Vector3( maxs.x, fillMaxY, 1.0f ), Vector2( 1.0f, 0.0f ), fillColor ) );

        m_fillVerts.push_back( Vertex3D_PUC( Vector3( maxs.x, fillMaxY, 1.0f ), Vector2( 1.0f, 0.0f ), fillColor ) );
        m_fillVerts.push_back( Vertex3D_PUC( Vector3( mins.x, fillMaxY, 1.0f ), Vector2( 0.0f, 0.0f ), fillColor ) );
        m_fillVerts.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, 1.0f ), Vector2( 0.0f, 1.0f ), fillColor ) );
    }

    else if (fillDir == "down")
    {
        float length = maxs.y - mins.y;
        float fillMinY = maxs.y - (length * fillAmount);

        m_fillVerts.push_back( Vertex3D_PUC( Vector3( mins.x, fillMinY, 1.0f ), Vector2( 0.0f, 1.0f ), fillColor ) );
        m_fillVerts.push_back( Vertex3D_PUC( Vector3( maxs.x, fillMinY, 1.0f ), Vector2( 1.0f, 1.0f ), fillColor ) );
        m_fillVerts.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, 1.0f ), Vector2( 1.0f, 0.0f ), fillColor ) );

        m_fillVerts.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, 1.0f ), Vector2( 1.0f, 0.0f ), fillColor ) );
        m_fillVerts.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, 1.0f ), Vector2( 0.0f, 0.0f ), fillColor ) );
        m_fillVerts.push_back( Vertex3D_PUC( Vector3( mins.x, fillMinY, 1.0f ), Vector2( 0.0f, 1.0f ), fillColor ) );
    }
}

////---------------------------------------------------------------------------------
///
// Accessors/Queries
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ProgressBarWidget::GetPropertiesFromXML( const XMLNode& node, NamedProperties& in_out_properties )
{
    XMLNode fillColorNode = node.getChildNode( "FillColor" );
    if (!fillColorNode.isEmpty())
    {
        Rgba fillColor = GetRgbaProperty( fillColorNode, "value", Rgba::GREEN );
        in_out_properties.Set( "FillColor", fillColor );
    }

    XMLNode fillOpacityNode = node.getChildNode( "FillOpacity" );
    if (!fillOpacityNode.isEmpty())
    {
        float fillOpacity = GetFloatProperty( fillOpacityNode, "value", 1.0f );
        in_out_properties.Set( "FillOpacity", fillOpacity );
    }

    XMLNode fillAmountNode = node.getChildNode( "FillAmount" );
    if (!fillAmountNode.isEmpty())
    {
        float fillAmount = GetFloatProperty( fillAmountNode, "value", 0.3f );
        in_out_properties.Set( "FillAmount", fillAmount );
    }


    BaseWidget::GetPropertiesFromXML( node, in_out_properties );
}

////---------------------------------------------------------------------------------
///
// Update
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ProgressBarWidget::ProcessInput( InputSystem* inputSystem )
{
    return;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ProgressBarWidget::Update()
{
    if (m_currentState == WidgetState_HIDDEN)
        return;

    float widgetOpactiy = GetWidgetOpacity();

    Rgba bgColor;
    GetPropertyByName( "BGColor", bgColor );

    Rgba edgeColor;
    GetPropertyByName( "EdgeColor", edgeColor );

    Rgba fillColor;
    GetPropertyByName( "FillColor", fillColor );

    float fillOpacity;
    GetPropertyByName( "FillOpacity", fillOpacity );

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

    for (auto iter = m_fillVerts.begin(); iter != m_fillVerts.end(); ++iter)
    {
        Vertex3D_PUC& vert = *iter;
        vert.color = fillColor;
        vert.color.SetAlpha( vert.color.Af() * fillOpacity * widgetOpactiy );
    }


    UpdateVertsFromBounds();

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ProgressBarWidget::UpdateVertsFromBounds()
{
    AABB2D bounds = GetWidgetBounds();

    float fillAmount;
    GetPropertyByName( "FillAmount", fillAmount );
    std::string fillDir = "";
    GetPropertyByName( "FillDirection", fillDir );
    fillDir = ConvertToLowerCase( fillDir );

    Vector2 mins = bounds.m_mins;
    Vector2 maxs = bounds.m_maxs;

    if (fillDir == "right")
    {
        float length = maxs.x - mins.x;
        float fillMaxX = mins.x + (length * fillAmount);
        
        m_fillVerts[0].position = Vector3( mins.x, mins.y, 1.0f );
        m_fillVerts[1].position = Vector3( fillMaxX, mins.y, 1.0f );
        m_fillVerts[2].position = Vector3( fillMaxX, maxs.y, 1.0f );
        m_fillVerts[3].position = Vector3( fillMaxX, maxs.y, 1.0f );
        m_fillVerts[4].position = Vector3( mins.x, maxs.y, 1.0f );
        m_fillVerts[5].position = Vector3( mins.x, mins.y, 1.0f );
    }

    else if (fillDir == "left")
    {
        float length = maxs.x - mins.x;
        float fillMinX = maxs.x - (length * fillAmount);

        m_fillVerts[0].position = Vector3( fillMinX, mins.y, 1.0f );
        m_fillVerts[1].position = Vector3( maxs.x, mins.y, 1.0f );
        m_fillVerts[2].position = Vector3( maxs.x, maxs.y, 1.0f );
        m_fillVerts[3].position = Vector3( maxs.x, maxs.y, 1.0f );
        m_fillVerts[4].position = Vector3( fillMinX, maxs.y, 1.0f );
        m_fillVerts[5].position = Vector3( fillMinX, mins.y, 1.0f );
    }

    else if (fillDir == "up")
    {
        float length = maxs.y - mins.y;
        float fillMaxY = mins.y + (length * fillAmount);

        m_fillVerts[0].position = Vector3( mins.x, mins.y, 1.0f );
        m_fillVerts[1].position = Vector3( maxs.x, mins.y, 1.0f );
        m_fillVerts[2].position = Vector3( maxs.x, fillMaxY, 1.0f );
        m_fillVerts[3].position = Vector3( maxs.x, fillMaxY, 1.0f );
        m_fillVerts[4].position = Vector3( mins.x, fillMaxY, 1.0f );
        m_fillVerts[5].position = Vector3( mins.x, mins.y, 1.0f );

    }

    else if (fillDir == "down")
    {
        float length = maxs.y - mins.y;
        float fillMinY = maxs.y - (length * fillAmount);

        m_fillVerts[0].position = Vector3( mins.x, fillMinY, 1.0f );
        m_fillVerts[1].position = Vector3( maxs.x, fillMinY, 1.0f );
        m_fillVerts[2].position = Vector3( maxs.x, maxs.y, 1.0f );
        m_fillVerts[3].position = Vector3( maxs.x, maxs.y, 1.0f );
        m_fillVerts[4].position = Vector3( mins.x, maxs.y, 1.0f );
        m_fillVerts[5].position = Vector3( mins.x, fillMinY, 1.0f );
    }

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
void ProgressBarWidget::Render( OpenGLRenderer* renderer )
{
    if (!renderer)
        return;

    if (m_currentState == WidgetState_HIDDEN)
        return;

    renderer->Enable( GL_BLEND );
    renderer->BlendFunct( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    renderer->DrawVertexesOrtho( NULL, m_contentVerts, GL_TRIANGLES );
    renderer->DrawVertexesOrtho( NULL, m_edgeVerts, GL_TRIANGLES );
    renderer->DrawVertexesOrtho( NULL, m_fillVerts, GL_TRIANGLES );
}