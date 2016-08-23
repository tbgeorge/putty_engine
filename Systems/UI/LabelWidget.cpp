//=================================================================================
// LabelWidget.cpp
// Author: Tyler George
// Date  : May 8, 2016
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "Engine/Systems/UI/LabelWidget.hpp"
#include "../../Utilities/Font.hpp"
#include "../../Renderer/FontRenderer.hpp"
#include "../../Utilities/XMLHelper.hpp"

////===========================================================================================
///===========================================================================================
// class LabelWidget
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
LabelWidget::LabelWidget( const std::string& widgetName, const WidgetPropertyLayer& instanceProperties )
    : BaseWidget( widgetName, instanceProperties )
{
    m_type = WidgetType_BUTTON;

    m_codeProperties.m_defaults.Set( "Text", "Change Me" );
    m_codeProperties.m_defaults.Set( "TextHeight", 40.0f );
    m_codeProperties.m_defaults.Set( "TextColor", Rgba::BLACK );
    m_codeProperties.m_defaults.Set( "Opacity", 1.0f );

    m_codeProperties.m_defaultsByState[WidgetState_DISABLED].Set( "TextColor", Rgba( 0x505050 ) );
}

////---------------------------------------------------------------------------------
///
// Accessors/Queries
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void LabelWidget::GetPropertiesFromXML( const XMLNode& node, NamedProperties& in_out_properties )
{
    XMLNode textNode = node.getChildNode( "Text" );
    if (!textNode.isEmpty())
    {
        std::string text = GetStringProperty( textNode, "value", "Change Me" );
        in_out_properties.Set( "Text", text );
    }

    XMLNode textHeightNode = node.getChildNode( "TextHeight" );
    if (!textHeightNode.isEmpty())
    {
        float textHeight = GetFloatProperty( textHeightNode, "value", 1.0f );
        in_out_properties.Set( "TextHeight", textHeight );
    }

    XMLNode textColorNode = node.getChildNode( "TextColor" );
    if (!textColorNode.isEmpty())
    {
        Rgba textColor = GetRgbaProperty( textColorNode, "value", Rgba::RED );
        in_out_properties.Set( "TextColor", textColor );
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
void LabelWidget::ProcessInput( InputSystem* inputSystem )
{
    return;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void LabelWidget::Update()
{
    return;

}

////---------------------------------------------------------------------------------
///
// Render
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void LabelWidget::Render( OpenGLRenderer* renderer )
{
    if (!renderer)
        return;

    if (m_currentState == WidgetState_HIDDEN)
        return;

    static FontRenderer* fontRenderer = renderer->GetFontRenderer();
    static Font* font = Font::CreateOrGetFont( "Data/Fonts/calibri" );

    renderer->Enable( GL_BLEND );
    renderer->BlendFunct( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    std::string text;
    GetPropertyByName( "Text", text );

    float widgetOpacity = GetWidgetOpacity();

    Rgba textColor;
    GetPropertyByName( "TextColor", textColor );

    textColor.SetAlpha( textColor.Af() * widgetOpacity );

    float textHeight;
    GetPropertyByName( "TextHeight", textHeight );

    AABB2D bounds = GetWidgetBounds();

    Vector3 position = Vector3( bounds.m_mins.x, bounds.m_mins.y, 1.0f );
    fontRenderer->DrawFontTextOrtho( textHeight, *font, text, position, textColor );
}