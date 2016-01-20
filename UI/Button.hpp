//=================================================================================
// Button.hpp
// Author: Tyler George
// Date  : October 26, 2015
//=================================================================================

#pragma once

#ifndef __included_Button__
#define __included_Button__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/UI/UIElement.hpp"
#include "Engine/UI/TextBox.hpp"
#include "Engine/UI/Image.hpp"


///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------


////===========================================================================================
///===========================================================================================
// Button Class
///===========================================================================================
////===========================================================================================
class Button : public UIElement
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    Button( OpenGLRenderer* renderer, UIElement* parent, Font* font, int fontSize, Texture* image, const Vector2& size,
        Anchor anchor = BOTTOM_LEFT, const Vector2& offset = Vector2::ZERO, const float& zVal = 1.0f, const Rgba& fontColor = Rgba(), const std::string& text = "", Texture* hoverImage = nullptr, const Rgba& hoverFontColor = Rgba() );

    ~Button();

    ///---------------------------------------------------------------------------------
    /// Initialization
    ///---------------------------------------------------------------------------------
    void Startup();

    ///---------------------------------------------------------------------------------
    /// Accessors/Queries
    ///---------------------------------------------------------------------------------
    bool WasJustClicked() { return m_wasJustClicked; }
    bool IsEnabled() { return m_isEnabled; }

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    void SetHoverImage( Texture* hoverImage ) { m_hoverImage = hoverImage; }
    void SetHoverFontColor( const Rgba& hoverFontColor ) { m_hoverFontColor = hoverFontColor; }
    void SetDisabledFontColor( const Rgba& disabledFontColor ) { m_disabledFontColor = disabledFontColor; }
    void Enable() { m_isEnabled = true; }
    void Disable() { m_isEnabled = false; }

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

	///---------------------------------------------------------------------------------
	/// Private Member Variables
	///---------------------------------------------------------------------------------
    TextBox* m_textbox;
    Image* m_image;

    Texture* m_defaultImage;
    Texture* m_hoverImage;

    Rgba m_fontColor;
    Rgba m_hoverFontColor;
    Rgba m_disabledFontColor;

    bool m_mouseIsHoveringOver;
    bool m_wasJustClicked;

    bool m_isEnabled;
};

///---------------------------------------------------------------------------------
/// Inline function implementations
///---------------------------------------------------------------------------------

#endif