//=================================================================================
// ButtonWidget.cpp
// Author: Tyler George
// Date  : April 20, 2016
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "Engine/Systems/UI/ButtonWidget.hpp"
#include "../../Math/AABB2D.hpp"
#include "../../Utilities/Font.hpp"
#include "../../Renderer/FontRenderer.hpp"
#include "../../Utilities/XMLHelper.hpp"

////===========================================================================================
///===========================================================================================
// class ButtonWidget
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
ButtonWidget::ButtonWidget( const std::string& widgetName, const WidgetPropertyLayer& instanceProperties )
    : BaseWidget( widgetName, instanceProperties )
{
    m_type = WidgetType_BUTTON;

    m_codeProperties.m_defaults.Set( "Text", "Change Me" );
    m_codeProperties.m_defaults.Set( "TextScale", 1.0f );
    m_codeProperties.m_defaults.Set( "TextColor", Rgba::BLACK );
    m_codeProperties.m_defaults.Set( "TextOpacity", 1.0f );
    m_codeProperties.m_defaults.Set( "EventToFire", "PrintClicked" );
    m_codeProperties.m_defaults.Set( "Size", Vector2( 200.0f, 60.0f ) );
    m_codeProperties.m_defaults.Set( "BGColor", Rgba( 0xCCCCCC ) );

    m_codeProperties.m_defaultsByState[WidgetState_HIGHLIGHTED].Set( "BGColor", Rgba( 0x888888 ) );
    
    m_codeProperties.m_defaultsByState[WidgetState_PRESSED].Set( "BGColor", Rgba( 0x444444 ) );
    m_codeProperties.m_defaultsByState[WidgetState_PRESSED].Set( "TextColor", Rgba::WHITE );

    m_codeProperties.m_defaultsByState[WidgetState_DISABLED].Set( "BGColor", Rgba( 0x808080) );
    m_codeProperties.m_defaultsByState[WidgetState_DISABLED].Set( "EdgeColor", Rgba( 0x777777 ) );
    m_codeProperties.m_defaultsByState[WidgetState_DISABLED].Set( "TextColor", Rgba( 0xAAAAAA ) );

    AABB2D bounds = GetWidgetBounds();

    m_textHeight = (bounds.m_maxs.y - bounds.m_mins.y) * TEXT_HEIGHT_PERCENTAGE;

}

////---------------------------------------------------------------------------------
///
// Accessors/Queries
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ButtonWidget::GetPropertiesFromXML( const XMLNode& node, NamedProperties& in_out_properties )
{
    XMLNode textNode = node.getChildNode( "Text" );
    if (!textNode.isEmpty())
    {
        std::string text = GetStringProperty( textNode, "value", "Change Me" );
        in_out_properties.Set( "Text", text );
    }

    XMLNode textScaleNode = node.getChildNode( "TextScale" );
    if (!textScaleNode.isEmpty())
    {
        float textScale = GetFloatProperty( textScaleNode, "value", 1.0f );
        in_out_properties.Set( "TextScale", textScale );
    }

    XMLNode textColorNode = node.getChildNode( "TextColor" );
    if (!textColorNode.isEmpty())
    {
        Rgba textColor = GetRgbaProperty( textColorNode, "value", Rgba::RED );
        in_out_properties.Set( "TextColor", textColor );
    }

    XMLNode textOpacityNode = node.getChildNode( "TextOpacity" );
    if (!textOpacityNode.isEmpty())
    {
        float textOpacity = GetFloatProperty( textOpacityNode, "value", 1.0f );
        in_out_properties.Set( "TextOpacity", textOpacity );
    }

    XMLNode eventNode = node.getChildNode( "EventToFire" );
    if (!eventNode.isEmpty())
    {
        std::string eventToFire = GetStringProperty( eventNode, "value", "PrintClicked" );
        in_out_properties.Set( "EventToFire", eventToFire );
    }

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
void ButtonWidget::Clicked()
{
    std::string eventToFire = "";
    GetPropertyByName( "EventToFire", eventToFire );
    NamedProperties eventProps;
    eventProps.Set( "WidgetName", m_name );
    FireEvent( eventToFire, eventProps );
    m_currentState = WidgetState_HIGHLIGHTED;
}

////---------------------------------------------------------------------------------
///
// Update
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ButtonWidget::ProcessInput( InputSystem* inputSystem )
{
    if (m_currentState == WidgetState_DISABLED || m_currentState == WidgetState_HIDDEN)
        return;

    AABB2D bounds = GetWidgetBounds();
    
    if( !bounds.IsPointInside( inputSystem->GetMousePosition() ))
        m_currentState = WidgetState_IDLE;

    if (bounds.IsPointInside( inputSystem->GetMousePosition() ) && m_currentState != WidgetState_PRESSED)
    {
        m_currentState = WidgetState_HIGHLIGHTED;
        inputSystem->SetPointerType( PT_HAND );
    }

    if (m_currentState == WidgetState_HIGHLIGHTED)
    {
        if (inputSystem->IsMouseButtonDown( MOUSE_LEFT ))
            m_currentState = WidgetState_PRESSED;
    }

    if (m_currentState == WidgetState_PRESSED)
    {
        inputSystem->SetPointerType( PT_HAND );

        if (inputSystem->WasMouseButtonJustReleased( MOUSE_LEFT ))
            Clicked();
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ButtonWidget::Update()
{
    if (m_currentState == WidgetState_HIDDEN)
        return;

    float widgetOpactiy = GetWidgetOpacity();

    Rgba bgColor;
    GetPropertyByName( "BGColor", bgColor );

    Rgba edgeColor;
    GetPropertyByName( "EdgeColor", edgeColor );

    for (auto iter = m_contentVerts.begin(); iter != m_contentVerts.end(); ++iter )
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

    float textScale;
    GetPropertyByName( "TextScale", textScale );

    AABB2D bounds = GetWidgetBounds();
    m_textHeight = ( (bounds.m_maxs.y - bounds.m_mins.y) * TEXT_HEIGHT_PERCENTAGE ) * textScale;

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
void ButtonWidget::Render( OpenGLRenderer* renderer )
{
    if (!renderer)
        return;

    if (m_currentState == WidgetState_HIDDEN)
        return;

    static FontRenderer* fontRenderer = renderer->GetFontRenderer();
    static Font* font = Font::CreateOrGetFont( "Data/Fonts/calibri" );

    renderer->Enable( GL_BLEND );
    renderer->BlendFunct( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    renderer->DrawVertexesOrtho( NULL, m_contentVerts, GL_TRIANGLES );
    renderer->DrawVertexesOrtho( NULL, m_edgeVerts, GL_TRIANGLES );

    AABB2D bounds = GetWidgetBounds();

    std::string text;
    GetPropertyByName( "Text", text );

    float textOpacity;
    GetPropertyByName( "TextOpacity", textOpacity );

    float widgetOpacity = GetWidgetOpacity();

    Rgba textColor;
    GetPropertyByName( "TextColor", textColor );

    textColor.SetAlpha( textColor.Af() * textOpacity * widgetOpacity );

    float textWidth = fontRenderer->CalcTextWidth( m_textHeight, *font, text );
    float textXOffset = ( (bounds.m_maxs.x - bounds.m_mins.x ) - textWidth) / 2.0f;

    float textYOffset = ((bounds.m_maxs.y - bounds.m_mins.y) - m_textHeight) / 2.0f;
    

    Vector3 position = Vector3( bounds.m_mins.x + textXOffset, bounds.m_mins.y + textYOffset, 1.0f );
    fontRenderer->DrawFontTextOrtho( m_textHeight, *font, text, position, textColor );
}