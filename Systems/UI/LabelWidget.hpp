//=================================================================================
// LabelWidget.hpp
// Author: Tyler George
// Date  : May 8, 2016
//=================================================================================

/*
* Used Properties include:
*  - Text               (string)    - displayed centered (by default) in bounds
*  - TextHeight         (flt)       - text cell height
*  - TextColor          (Rgba)      - fill color for text including alpha
*
*
*
*
*
*/

#pragma once

#ifndef __included_LabelWidget__
#define __included_LabelWidget__

////---------------------------------------------------------------------------------
///
// Includes
///
////---------------------------------------------------------------------------------
#include "Engine/Systems/UI/BaseWidget.hpp"

////---------------------------------------------------------------------------------
///
// class LabelWidget
///
////---------------------------------------------------------------------------------
class LabelWidget : public BaseWidget
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    LabelWidget( const std::string& widgetName, const WidgetPropertyLayer& instanceProperties = WidgetPropertyLayer() );

    ///---------------------------------------------------------------------------------
    /// Accessors/Queries
    ///---------------------------------------------------------------------------------
    static void GetPropertiesFromXML( const XMLNode& node, NamedProperties& in_out_properties );

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------

    ///---------------------------------------------------------------------------------
    /// Update
    ///---------------------------------------------------------------------------------
    void ProcessInput( InputSystem* inputSystem );
    void Update();

    ///---------------------------------------------------------------------------------
    /// Render
    ///---------------------------------------------------------------------------------
    void Render( OpenGLRenderer* renderer );



protected:

private:

};

#endif