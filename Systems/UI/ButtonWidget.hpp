//=================================================================================
// ButtonWidget.hpp
// Author: Tyler George
// Date  : April 20, 2016
//=================================================================================

/*
* Used Properties include:
*  - Text               (string)    - displayed centered (by default) in bounds
*  - TextScale          (flt)       - multiplier for text cell height (defaults to fixed % of widget height)
*  - TextColor          (Rgba)      - fill color for text including alpha
*  - TextOpacity        (flt)       - multiplied by TextColor.alpha AND BaseWidget.Opacity for fading
*  - EventToFire        (string)    - event to fire when the button is clicked
*
*
*
*
*
*/

#pragma once

#ifndef __included_ButtonWidget__
#define __included_ButtonWidget__

////---------------------------------------------------------------------------------
///
// Includes
///
////---------------------------------------------------------------------------------
#include "Engine/Systems/UI/BaseWidget.hpp"
#include "Engine/Math/Vector2.hpp"

////---------------------------------------------------------------------------------
///
// Constants
///
////---------------------------------------------------------------------------------
const float TEXT_HEIGHT_PERCENTAGE = 0.5f;

////---------------------------------------------------------------------------------
///
// class ButtonWidget
///
////---------------------------------------------------------------------------------
class ButtonWidget : public BaseWidget
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    ButtonWidget( const std::string& widgetName, const WidgetPropertyLayer& instanceProperties = WidgetPropertyLayer() );

    ///---------------------------------------------------------------------------------
    /// Accessors/Queries
    ///---------------------------------------------------------------------------------
    static void GetPropertiesFromXML( const XMLNode& node, NamedProperties& in_out_properties );

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    virtual void Clicked();

    ///---------------------------------------------------------------------------------
    /// Update
    ///---------------------------------------------------------------------------------
    virtual void ProcessInput( InputSystem* inputSystem );
    virtual void Update();

    ///---------------------------------------------------------------------------------
    /// Render
    ///---------------------------------------------------------------------------------
    virtual void Render( OpenGLRenderer* renderer );



protected:
    float m_textHeight;

};


#endif
