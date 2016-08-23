//=================================================================================
// UISystem.hpp
// Author: Tyler George
// Date  : April 20, 2016
//=================================================================================

#pragma once

#ifndef __included_UISystem__
#define __included_UISystem__

////---------------------------------------------------------------------------------
///
// Includes
///
////---------------------------------------------------------------------------------
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Systems/UI/UIStyle.hpp"
#include "Engine/Systems/UI/UIWindow.hpp"
#include "../../Utilities/XMLParser.h"

////---------------------------------------------------------------------------------
///
// Typedefs
///
////---------------------------------------------------------------------------------
typedef std::map< std::string, UIStyle* > StyleMap;
typedef std::map< std::string, UIWindow* > WindowMap;
typedef std::map< std::string, WidgetPropertyLayer > WidgetDefinitionMap;
typedef std::map< std::string, BaseWidget* > WidgetMap;
typedef std::vector< UIWindow* > WindowStack;

////---------------------------------------------------------------------------------
///
// class UISystem
///
////---------------------------------------------------------------------------------
class UISystem 
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------

    ///---------------------------------------------------------------------------------
    /// Initialization
    ///---------------------------------------------------------------------------------
    static void InitializeUISystem();
    static void LoadStyles();
    static void LoadWidgetDefinitions();
    static void LoadWindows();
    static BaseWidget* CreateWidgetAndChildren( const XMLNode& parentNode );

    ///---------------------------------------------------------------------------------
    /// Accessors/Queries
    ///---------------------------------------------------------------------------------
    static std::string GetWidgetStateAsString( WidgetState state );
    static std::string GetWidgetTypeAsString( WidgetType type );
    static WidgetType GetWidgetTypeAsEnum( const std::string& type );
    static WidgetState GetWidgetStateAsEnum( const std::string& state );

    static bool DoesStyleExist( const std::string& styleName );
    static UIStyle* GetStyleByName( const std::string& styleName );

    static bool DoesWidgetDefinitionExist( const std::string& widgetName );
    static WidgetPropertyLayer GetWidgetDefinitionByName( const std::string& widgetName );

    static bool DoesWindowExist( const std::string& windowName );
    static UIWindow* GetWindowByName( const std::string& windowName );

    static bool DoesWidgetExist( const std::string& widgetName );
    static BaseWidget* GetWidgetByName( const std::string& widgetName );

    static void GetWidgetPropertiesFromXMLBasedOnType( const XMLNode& node, NamedProperties& in_out_properties, WidgetType type );
    static WidgetPropertyLayer GetWidgetPropertyLayerFromXML( const XMLNode& node );

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    static BaseWidget* CreateWidget( WidgetType type, const std::string& widgetName, const WidgetPropertyLayer& instanceProperties );
    static bool AddWidgetDefinition( const std::string& widgetName, WidgetPropertyLayer properties );
    static bool AddWidget( const std::string& widgetName, BaseWidget* widget );

    static void PushWindow( const std::string& windowName );
    static void PopWindow( const std::string& popUntilWindowName = "top" );

    static void AddWidgetToDefaultWindow( BaseWidget* widget );

    static void HideWidgetByName( const std::string& widgetName );
    static void ActivateWidgetByName( const std::string& widgetName );
    static void DisableWidgetByName( const std::string& widgetName );

    ///---------------------------------------------------------------------------------
    /// Update
    ///---------------------------------------------------------------------------------
    static void ProcessInput( InputSystem* inputSystem );
    static void Update();

    ///---------------------------------------------------------------------------------
    /// Render
    ///---------------------------------------------------------------------------------
    static void Render( OpenGLRenderer* renderer );


private:
    ///---------------------------------------------------------------------------------
    /// private member variables
    ///---------------------------------------------------------------------------------
    StyleMap m_styles;
    WindowMap m_windows;
    WidgetDefinitionMap m_widgetDefinitions;

    WidgetMap m_widgetsByName;
    int m_numCreatedWidgets;

    WindowStack m_windowStack;

    ///---------------------------------------------------------------------------------
    /// private static variables
    ///---------------------------------------------------------------------------------
    static UISystem* s_theUISystem;

    ///---------------------------------------------------------------------------------
    /// private functions
    ///---------------------------------------------------------------------------------
    UISystem();


};

#endif