//=================================================================================
// ProgressBarWidget.hpp
// Author: Tyler George
// Date  : May 8, 2016
//=================================================================================

/*
* Used Properties include:
*  - FillColor          (Rgba)      - fill color for bar including alpha
*  - FillOpacity        (flt)       - multiplied by FillColor.alpha AND BaseWidget.Opacity for fading
*  - FillAmount         (flt)       - 0.0f - 1.0f to determine how filled the bar is
*  - FillDirection      (string)    - direction to fill bar ( left, right, up, down )
*
*
*
*/

#pragma once

#ifndef __included_ProgressBarWidget__
#define __included_ProgressBarWidget__

////---------------------------------------------------------------------------------
///
// Includes
///
////---------------------------------------------------------------------------------
#include "Engine/Systems/UI/BaseWidget.hpp"
#include "Engine/Math/Vector2.hpp"

////---------------------------------------------------------------------------------
///
// class ProgressBarWidget
///
////---------------------------------------------------------------------------------
class ProgressBarWidget : public BaseWidget
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    ProgressBarWidget( const std::string& widgetName, const WidgetPropertyLayer& instanceProperties = WidgetPropertyLayer() );

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

    void UpdateVertsFromBounds();

    ///---------------------------------------------------------------------------------
    /// Render
    ///---------------------------------------------------------------------------------
    void Render( OpenGLRenderer* renderer );



protected:

private:
    PUC_Vertexes m_fillVerts;

};

#endif
