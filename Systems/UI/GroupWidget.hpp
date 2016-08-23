//=================================================================================
// GroupWidget.hpp
// Author: Tyler George
// Date  : April 28, 2016
//=================================================================================

/*
* Used Properties include:
*  - GroupOpacity   (flt)   -used to multiply all children by so a group can fade together
*
*
*
*
*
*/

#pragma once

#ifndef __included_GroupWidget__
#define __included_GroupWidget__

////---------------------------------------------------------------------------------
///
// Includes
///
////---------------------------------------------------------------------------------
#include "Engine/Systems/UI/BaseWidget.hpp"
#include "Engine/Math/Vector2.hpp"

////---------------------------------------------------------------------------------
///
// class GroupWidget
///
////---------------------------------------------------------------------------------
class GroupWidget : public BaseWidget
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    GroupWidget( const std::string& widgetName, const WidgetPropertyLayer& instanceProperties = WidgetPropertyLayer() );

    ///---------------------------------------------------------------------------------
    /// Accessors/Queries
    ///---------------------------------------------------------------------------------
    static void GetPropertiesFromXML( const XMLNode& node, NamedProperties& in_out_properties );

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    virtual bool AddChild( BaseWidget* child );

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
    std::vector< BaseWidget* > m_children;

};


#endif
