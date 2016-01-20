//=================================================================================
// TextBox.hpp
// Author: Tyler George
// Date  : October 26, 2015
//=================================================================================

#pragma once

#ifndef __included_TextBox__
#define __included_TextBox__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Utilities/Font.hpp"
#include "Engine/UI/UIElement.hpp"


///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------


////===========================================================================================
///===========================================================================================
// TextBox Class
///===========================================================================================
////===========================================================================================
class TextBox : public UIElement
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    TextBox( OpenGLRenderer* renderer, UIElement* parent, Font* font, int fontSize, 
        Anchor anchor = BOTTOM_LEFT, const Vector2& offset = Vector2::ZERO, const Rgba& fontColor = Rgba(), const std::string& text = "", const float& zVal = 1.0f );

    ~TextBox();

    ///---------------------------------------------------------------------------------
    /// Initialization
    ///---------------------------------------------------------------------------------

    ///---------------------------------------------------------------------------------
    /// Accessors/Queries
    ///---------------------------------------------------------------------------------

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    void SetFontColor( const Rgba& fontColor ) { m_fontColor = fontColor; }
    void SetFont( Font* font ) { m_font = font; }

    void SetText( const std::string& text );
    void SetFontSize( int fontSize );


    ///---------------------------------------------------------------------------------
    /// Update
    ///---------------------------------------------------------------------------------
    void ProcessInput( InputSystem* inputSystem );
    void Update( bool debugModeEnabled );

    ///---------------------------------------------------------------------------------
    /// Render
    ///---------------------------------------------------------------------------------
    void Render( bool debugModeEnabled );

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
    void UpdateVariables();

    ///---------------------------------------------------------------------------------
    /// Private Member Variables
    ///---------------------------------------------------------------------------------
    Font* m_font;
    Rgba m_fontColor;
    std::string m_text;
    int m_fontSize;


};

///---------------------------------------------------------------------------------
/// Inline function implementations
///---------------------------------------------------------------------------------

#endif