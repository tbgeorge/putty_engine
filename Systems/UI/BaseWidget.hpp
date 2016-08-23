//=================================================================================
// BaseWidget.hpp
// Author: Tyler George
// Date  : April 20, 2016
//=================================================================================

/*
 * Used Properties include:
 *  - Offset            (Vec2)      - positional displacement from Parent widget
 *  - Size              (Vec2)      - 2D widget dimensions
 *  - BackgroundColor   (Rgba)      - fill color or texture tint for widget area
 *  - EdgeColor         (Rgba)      - edge color for widget AABB2 perimeter
 *  - EdgeThickness     (flt)       - pixel thickness for widget borders
 *  - Opacity           (flt)       - multiplied by BackgroundColor.alpha and EdgeColor.alpha to fade widget
 *
 *
 *
 *
 *
*/

#pragma once

#ifndef __included_BaseWidget__
#define __included_BaseWidget__

////---------------------------------------------------------------------------------
///
// Includes 
///
////---------------------------------------------------------------------------------
#include "Engine\Systems\Events\EventReceiver.hpp"
#include "Engine\Utilities\NamedProperties.hpp"
#include "Engine\Math\Vector2.hpp"
#include "Engine\Utilities\Rgba.hpp"
#include "Engine\Renderer\OpenGLRenderer.hpp"
#include "Engine\Input\InputSystem.hpp"
#include "Engine\Utilities\Error.hpp"
#include "Engine\Math\AABB2D.hpp"
#include "WidgetPropertyLayer.hpp"
#include "..\..\Utilities\XMLParser.h"
#include "UIStyle.hpp"


////---------------------------------------------------------------------------------
///
// class BaseWidget
///
////---------------------------------------------------------------------------------
class BaseWidget : public EventReceiver
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    BaseWidget( const std::string& widgetName, const WidgetPropertyLayer& instanceProperties = WidgetPropertyLayer() );

    ///---------------------------------------------------------------------------------
    /// Accessors/Queries
    ///---------------------------------------------------------------------------------
    template< typename T >
    void GetPropertyByName( const std::string& name, T& out_property ) const;

    AABB2D GetWidgetBounds() const;
    void GetWidgetBoundsForChild( const BaseWidget& child, AABB2D& out_childBounds ) const;

    float GetWidgetOpacity() const;

    static void GetPropertiesFromXML( const XMLNode& node, NamedProperties& in_out_properties );

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    void SetStyle( const std::string& styleName );
    void SetState( WidgetState state );
    
    template< typename T >
    void SetPropertyByName( const std::string& name, const T& propertyValue );

    void SetParent( BaseWidget* parent );
    virtual bool AddChild( BaseWidget* child );

    ///---------------------------------------------------------------------------------
    /// Update
    ///---------------------------------------------------------------------------------
    virtual void ProcessInput( InputSystem* inputSystem );
    virtual void Update();

    virtual void UpdateVertsFromBounds();

    ///---------------------------------------------------------------------------------
    ///
    ///---------------------------------------------------------------------------------
    virtual void Render( OpenGLRenderer* renderer );

protected:
    ///---------------------------------------------------------------------------------
    /// Protected member variables
    ///---------------------------------------------------------------------------------
//     NamedProperties m_defaults;                                 // Least important
//     NamedProperties m_styleDefaults;                            //      ^
//     NamedProperties m_instanceDefaults;                         //      | 
//     NamedProperties m_defaultsByState[WidgetState_NUM_STATES];  //      |
//     NamedProperties m_styleByState[WidgetState_NUM_STATES];     //      |
//     NamedProperties m_instanceByState[WidgetState_NUM_STATES];  // Most important

    WidgetPropertyLayer m_codeProperties;
    WidgetPropertyLayer m_styleProperties;
    WidgetPropertyLayer m_instanceProperties;

    WidgetState m_currentState;
    WidgetType m_type;
    std::string m_name;
    BaseWidget* m_parent;

    PUC_Vertexes m_contentVerts;
    PUC_Vertexes m_edgeVerts;



};

////---------------------------------------------------------------------------------
///
// Templated functions
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template <typename T>
void BaseWidget::GetPropertyByName( const std::string& name, T& out_property ) const
{

    // instance by state
    PropertyGetResult result = m_instanceProperties.m_defaultsByState[m_currentState].Get( name, out_property );
    if (result == RESULT_SUCCESS)
        return;

    // style by state
    result = m_styleProperties.m_defaultsByState[m_currentState].Get( name, out_property );
    if (result == RESULT_SUCCESS)
        return;

    // code defaults by state
    result = m_codeProperties.m_defaultsByState[m_currentState].Get( name, out_property );
    if (result == RESULT_SUCCESS)
        return;


    // instance default
    result = m_instanceProperties.m_defaults.Get( name, out_property );
    if (result == RESULT_SUCCESS)
        return;

    // style default
    result = m_styleProperties.m_defaults.Get( name, out_property );
    if (result == RESULT_SUCCESS)
        return;

    // code default
    result = m_codeProperties.m_defaults.Get( name, out_property );
    if (result == RESULT_SUCCESS)
        return;

    RECOVERABLE_ERROR();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename T >
void BaseWidget::SetPropertyByName( const std::string& name, const T& propertyValue )
{
    m_instanceProperties.m_defaults.Set( name, propertyValue );
}

#endif
