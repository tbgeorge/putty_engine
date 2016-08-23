//=================================================================================
// CheckboxWidget.hpp
// Author: Tyler George
// Date  : May 8, 2016
//=================================================================================

/*
* Used Properties include:
*  - FillColor              (Rgba)      - color of box when checked
*  - FillOpacity            (flt)       - multiplied by FillColor.alpha AND BaseWidget.Opacity for fading
*  - FillEdgeThickness      (flt)       - thickness of the inner edge
*  - IsChecked              (bool       - whether or not the box is currently checked
*  - EventFiredOnChecked    (string)    - event to fire when the checkbox gets checked
*  - EventFiredOnUnChecked  (string)    - event to fire when the checkbox gets unchecked
*
*
*
*
*/

#pragma once

#ifndef __included_CheckboxWidget__
#define __included_CheckboxWidget__

////---------------------------------------------------------------------------------
///
// Includes
///
////---------------------------------------------------------------------------------
#include "Engine/Systems/UI/ButtonWidget.hpp"
#include "Engine/Math/Vector2.hpp"

////---------------------------------------------------------------------------------
///
// class CheckboxWidget
///
////---------------------------------------------------------------------------------
class CheckboxWidget : public ButtonWidget
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    CheckboxWidget( const std::string& widgetName, const WidgetPropertyLayer& instanceProperties = WidgetPropertyLayer() );

    ///---------------------------------------------------------------------------------
    /// Accessors/Queries
    ///---------------------------------------------------------------------------------
    static void GetPropertiesFromXML( const XMLNode& node, NamedProperties& in_out_properties );

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    void Clicked();

    ///---------------------------------------------------------------------------------
    /// Update
    ///---------------------------------------------------------------------------------
    void Update();

    void UpdateVertsFromBounds();

    ///---------------------------------------------------------------------------------
    /// Render
    ///---------------------------------------------------------------------------------
    void Render( OpenGLRenderer* renderer );



protected:

private:
    PUC_Vertexes m_fillVerts;
    bool m_isChecked;

};

#endif
