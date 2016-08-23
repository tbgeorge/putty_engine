//=================================================================================
// UISystem.cpp
// Author: Tyler George
// Date  : April 25, 2016
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "Engine/Systems/UI/UISystem.hpp"
#include "Engine/Utilities/FileUtilities.hpp"
#include "Engine/Utilities/XMLHelper.hpp"
#include "../../Utilities/Error.hpp"
#include "BaseWidget.hpp"
#include "Engine/Systems/UI/ButtonWidget.hpp"
#include "GroupWidget.hpp"
#include "../../Utilities/Utilities.hpp"
#include "../../Utilities/DeveloperConsole.hpp"
#include "LabelWidget.hpp"
#include "ProgressBarWidget.hpp"
#include "CheckboxWidget.hpp"

////===========================================================================================
///===========================================================================================
// Static variable initialization
///===========================================================================================
////===========================================================================================
UISystem* UISystem::s_theUISystem = nullptr;

////===========================================================================================
///===========================================================================================
// class UISystem
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
UISystem::UISystem()
    : m_numCreatedWidgets( 0 )
{
}

////---------------------------------------------------------------------------------
///
// Initialization
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void UISystem::InitializeUISystem()
{
    if (!s_theUISystem)
    {
        s_theUISystem = new UISystem();
        LoadStyles();
        LoadWidgetDefinitions();
        LoadWindows();
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void UISystem::LoadStyles()
{
    std::string styleDirectory = "Data/UI/Styles/";

    Strings styleFiles;
    EnumerateFiles( styleDirectory, "*.Style.xml", styleFiles, true );

    int styleNum = 0;
    for(std::string& styleFile : styleFiles)
    {

        XMLNode styleRootNode = XMLNode::parseFile( styleFile.c_str(), "Style" );

        std::string styleName = GetStringProperty( styleRootNode, "name", "unnamedstyle_" + std::to_string( ++styleNum ), true );
       
        // if style exists, alert user, skip loading it, and continue 
        if (DoesStyleExist( styleName ))
        {
            RECOVERABLE_ERROR();
            continue;
        }

        UIStyle* style = new UIStyle();
        style->m_styleName = styleName;

        // get BaseWidget node, if exists
        XMLNode baseWidgetNode = styleRootNode.getChildNode( "BaseWidget" );
        if (!baseWidgetNode.isEmpty())
        {
            WidgetPropertyLayer properties = GetWidgetPropertyLayerFromXML( baseWidgetNode );
            for (int widgetType = 0; widgetType < WidgetType_NUM_WIDGET_TYPES; ++widgetType)
            {
                style->m_styleByWidgetType[widgetType].m_defaults = properties.m_defaults;
                for (int widgetState = 0; widgetState < WidgetState_NUM_STATES; ++widgetState)
                    style->m_styleByWidgetType[widgetType].m_defaultsByState[widgetState] = properties.m_defaultsByState[widgetState];
            }

//             // fill out style information for all widgetTypes based on XML for base widget
//             XMLNode defaultBaseWidgetNode = baseWidgetNode.getChildNode( "Default" );
//             
//             if (!defaultBaseWidgetNode.isEmpty())
//             {
//                 NamedProperties defaultPropertyValues;
//                 BaseWidget::GetPropertiesFromXML( defaultBaseWidgetNode, defaultPropertyValues );
// 
//                 for (int widgetType = 0; widgetType < WidgetType_NUM_WIDGET_TYPES; ++widgetType)
//                     style->m_styleByWidgetType[widgetType].m_defaults = defaultPropertyValues;
//             }
// 
//             // state specific for base widget
//             for (int widgetState = 0; widgetState < WidgetState_NUM_STATES; ++widgetState)
//             {
//                 std::string widgetStateString = GetWidgetStateAsString( (WidgetState)widgetState );
//                 XMLNode stateBaseWidgetNode = baseWidgetNode.getChildNode( widgetStateString.c_str() );
// 
//                 if (!stateBaseWidgetNode.isEmpty())
//                 {
//                     NamedProperties statePropertyValues;
//                     BaseWidget::GetPropertiesFromXML( stateBaseWidgetNode, statePropertyValues );
// 
//                     for (int widgetType = 0; widgetType < WidgetType_NUM_WIDGET_TYPES; ++widgetType)
//                         style->m_styleByWidgetType[widgetType].m_defaultsByState[widgetState] = statePropertyValues;
//                 }
//             }
        }

        // fill out style information for each different type of widget
        for (int widgetType = 0; widgetType < WidgetType_NUM_WIDGET_TYPES; ++widgetType)
        {
            // Get widgetType enum as string
            std::string widgetTypeString = GetWidgetTypeAsString( (WidgetType) widgetType );

            // find tag with widgetType
            XMLNode widgetTypeNode = styleRootNode.getChildNode( widgetTypeString.c_str() );

            // if exists
            if (!widgetTypeNode.isEmpty())
            {
                WidgetPropertyLayer properties = GetWidgetPropertyLayerFromXML( widgetTypeNode );
                style->m_styleByWidgetType[widgetType].m_defaults = properties.m_defaults;
                for (int widgetState = 0; widgetState < WidgetState_NUM_STATES; ++widgetState)
                    style->m_styleByWidgetType[widgetType].m_defaultsByState[widgetState] = properties.m_defaultsByState[widgetState];
//                 XMLNode defaultWidgetNode = widgetTypeNode.getChildNode( "Default" );
// 
//                 if (!defaultWidgetNode.isEmpty())
//                 {
//                     NamedProperties defaultPropertyValues;
//                     GetWidgetPropertiesFromXMLBasedOnType( defaultWidgetNode, defaultPropertyValues, (WidgetType) widgetType );
// 
//                     style->m_styleByWidgetType[widgetType].m_defaults = defaultPropertyValues;
//                 }
// 
//                 // state specific for widget type
//                 for (int widgetState = 0; widgetState < WidgetState_NUM_STATES; ++widgetState)
//                 {
//                     std::string widgetStateString = GetWidgetStateAsString( (WidgetState)widgetState );
//                     XMLNode stateBaseWidgetNode = widgetTypeNode.getChildNode( widgetStateString.c_str() );
// 
//                     if (!stateBaseWidgetNode.isEmpty())
//                     {
//                         NamedProperties statePropertyValues;
//                         GetWidgetPropertiesFromXMLBasedOnType( stateBaseWidgetNode, statePropertyValues, (WidgetType) widgetType );
// 
//                         style->m_styleByWidgetType[widgetType].m_defaultsByState[widgetState] = statePropertyValues;
//                     }
//                 }
            }
        }
        s_theUISystem->m_styles.insert( std::pair< std::string, UIStyle* >( styleName, style ) );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void UISystem::LoadWidgetDefinitions()
{
    std::string widgetDefinitionDirectory = "Data/UI/WidgetDefinitions/";

    Strings widgetDefinitionFiles;
    EnumerateFiles( widgetDefinitionDirectory, "*.Widget.xml", widgetDefinitionFiles, true );

    for (std::string& widgetDefinitionFile : widgetDefinitionFiles)
    {
        XMLNode xmlRoot = XMLNode::parseFile( widgetDefinitionFile.c_str() );

        int widgetDefinitionNum = 0;
        for (;;)
        {
            XMLNode widgetDefinitionNode = xmlRoot.getChildNode( widgetDefinitionNum++ );
            if (widgetDefinitionNode.isEmpty())
                break;
            
            std::string widgetDefinitionName = GetStringProperty( widgetDefinitionNode, "name", widgetDefinitionFile + "_unnamedwidget_" + std::to_string( widgetDefinitionNum ), true );
            WidgetPropertyLayer widgetProperties = GetWidgetPropertyLayerFromXML( widgetDefinitionNode );
            
            if( !AddWidgetDefinition(widgetDefinitionName, widgetProperties ) )
            {
                RECOVERABLE_ERROR();
                continue;
            }

        }
    }
}
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void UISystem::LoadWindows()
{
    std::string windowDirectory = "Data/UI/Windows/";

    Strings windowFiles;
    EnumerateFiles( windowDirectory, "*.Window.xml", windowFiles, true );

    // create default window which will be used to add code defined widgets to
    UIWindow* defaultWindow = new UIWindow();
    defaultWindow->SetName( "defaultwindow" );
    s_theUISystem->m_windows.insert( std::pair< std::string, UIWindow* >( "defaultwindow", defaultWindow ) );

    int windowNum = 0;
    for (std::string& windowFile : windowFiles)
    {

        XMLNode windowRootNode = XMLNode::parseFile( windowFile.c_str(), "Window" );
        std::string windowName = GetStringProperty( windowRootNode, "name", "unnamedwindow_" + std::to_string( ++windowNum ), true );

        // if window exists, alert user, skip loading it, and continue 
        if (DoesWindowExist( windowName ))
        {
            RECOVERABLE_ERROR();
            continue;
        }


        UIWindow* window = new UIWindow();
        window->SetName( windowName );

        // for each "root" widget in the window
        int widgetNum = 0;
        for (;;)
        {
            XMLNode childWidgetNode = windowRootNode.getChildNode( widgetNum++ );
            if (childWidgetNode.isEmpty())
                break;
            else
            {
                // create the widget itself and all of its children
                BaseWidget* childWidget = CreateWidgetAndChildren( childWidgetNode );
                window->AddChildWidget( childWidget );
            }
        }

        s_theUISystem->m_windows.insert( std::pair< std::string, UIWindow* >( windowName, window ) );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
BaseWidget* UISystem::CreateWidgetAndChildren( const XMLNode& parentNode )
{
    std::string widgetType = GetXMLNodeName( parentNode );

    std::string widgetName = GetStringProperty( parentNode, "name", "unnamedWidget_" + std::to_string(s_theUISystem->m_numCreatedWidgets), true );
    if (DoesWidgetExist( widgetName )) // widget already exists
    {
        RECOVERABLE_ERROR();
        return nullptr;
    }

    
    Strings extendsWidgetDefinitions = GetStringsProperty( parentNode, "extends", ", ", Strings() );
    Strings widgetStyles = GetStringsProperty( parentNode, "styles", ", ", Strings() );

    // get all predefined properties
    WidgetPropertyLayer extendedProperties;
    for (std::string widgetDefinition : extendsWidgetDefinitions)
    {
        WidgetPropertyLayer properties = GetWidgetDefinitionByName( widgetDefinition );
        extendedProperties.CopyFromLayer( properties );
    }

    // get all style properties
    UIStyle* extendedStyle = new UIStyle();
    extendedStyle->m_styleName = widgetName + "_instancestyle";
    for (std::string widgetStyle : widgetStyles)
    {
        UIStyle* style = GetStyleByName( widgetStyle );
        for (int typeNum = 0; typeNum < WidgetType_NUM_WIDGET_TYPES; ++typeNum)
            extendedStyle->m_styleByWidgetType[typeNum].CopyFromLayer( style->m_styleByWidgetType[typeNum] );
    }
    s_theUISystem->m_styles[extendedStyle->m_styleName] = extendedStyle; // add instanced style to style list
    
    // get widget instance properties
    WidgetPropertyLayer widgetInstanceProperties = GetWidgetPropertyLayerFromXML( parentNode );
    extendedProperties.CopyFromLayer( widgetInstanceProperties );

    // create widget
    BaseWidget* parentWidget = CreateWidget( GetWidgetTypeAsEnum(widgetType), widgetName, extendedProperties );
    parentWidget->SetStyle( extendedStyle->m_styleName ); // set style
    AddWidget( widgetName, parentWidget );

    XMLNode childrenNode = parentNode.getChildNode( "Children" );
    if (childrenNode.isEmpty())
        return parentWidget;

    int childWidgetNum = 0;
    for (;;)
    {
        XMLNode childWidgetNode = childrenNode.getChildNode( childWidgetNum++ );
        if (childWidgetNode.isEmpty())
            break;
        else
        {
            BaseWidget* child = CreateWidgetAndChildren( childWidgetNode ); 
            if (child)
            {
                bool success = parentWidget->AddChild( child );
                if (!success) // tried to add a child to a widget that doesn't support children
                    RECOVERABLE_ERROR();
            }
        }
    }
    return parentWidget;
}

////---------------------------------------------------------------------------------
///
// Accessors/Queries
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
std::string UISystem::GetWidgetStateAsString( WidgetState state )
{
    switch (state)
    {
    case WidgetState_IDLE:              return "Idle";
    case WidgetState_HIGHLIGHTED:       return "Highlighted";
    case WidgetState_PRESSED:           return "Pressed";
    case WidgetState_DISABLED:          return "Disabled";
    case WidgetState_HIDDEN:            return "Hidden";
    default:                            return "";
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
std::string UISystem::GetWidgetTypeAsString( WidgetType type )
{
    switch (type)
    {
    case WidgetType_BUTTON:         return "Button";
    case WidgetType_GROUP:          return "Group";
    case WidgetType_LABEL:          return "Label";
    case WidgetType_PROGRESS_BAR:   return "ProgressBar";
    case WidgetType_CHECKBOX:       return "Checkbox";
    default:                        return "";
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
WidgetType UISystem::GetWidgetTypeAsEnum( const std::string& type )
{
    std::string lcType = ConvertToLowerCase( type );
    if (lcType == "button")
        return WidgetType_BUTTON;
    else if (lcType == "group")
        return WidgetType_GROUP;
    else if (lcType == "label")
        return WidgetType_LABEL;
    else if (lcType == "progressbar")
        return WidgetType_PROGRESS_BAR;
    else if (lcType == "checkbox")
        return WidgetType_CHECKBOX;

    else
        return WidgetType_NUM_WIDGET_TYPES;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
WidgetState UISystem::GetWidgetStateAsEnum( const std::string& state )
{
    
    if (state == "idle")
        return WidgetState_IDLE;
    else if (state == "highlighted")
        return WidgetState_HIGHLIGHTED;
    else if (state == "pressed")
        return WidgetState_PRESSED;
    else if (state == "disabled")
        return WidgetState_DISABLED;
    else if (state == "hidden")
        return WidgetState_HIDDEN;

    else
        return WidgetState_NUM_STATES;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool UISystem::DoesStyleExist( const std::string& styleName )
{
    StyleMap::const_iterator styleIter = s_theUISystem->m_styles.find( styleName );

    if (styleIter != s_theUISystem->m_styles.end())
        return true;
    else 
        return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
UIStyle* UISystem::GetStyleByName( const std::string& styleName )
{
    StyleMap::const_iterator styleIter = s_theUISystem->m_styles.find( ConvertToLowerCase(styleName) );

    if (styleIter != s_theUISystem->m_styles.end())
    {
        return styleIter->second;
    }

    return nullptr;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool UISystem::DoesWidgetDefinitionExist( const std::string& widgetName )
{
    WidgetDefinitionMap::const_iterator widgetIter = s_theUISystem->m_widgetDefinitions.find( widgetName );

    if (widgetIter != s_theUISystem->m_widgetDefinitions.end())
        return true;
    else
        return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
WidgetPropertyLayer UISystem::GetWidgetDefinitionByName( const std::string& widgetName )
{
    WidgetDefinitionMap::const_iterator defIter = s_theUISystem->m_widgetDefinitions.find( ConvertToLowerCase(widgetName) );

    if (defIter != s_theUISystem->m_widgetDefinitions.end())
    {
        return defIter->second;
    }

    return WidgetPropertyLayer();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool UISystem::DoesWindowExist( const std::string& windowName )
{
    WindowMap::const_iterator windowIter = s_theUISystem->m_windows.find( ConvertToLowerCase(windowName) );

    if (windowIter != s_theUISystem->m_windows.end())
        return true;
    else
        return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
UIWindow* UISystem::GetWindowByName( const std::string& windowName )
{
    WindowMap::const_iterator windowIter = s_theUISystem->m_windows.find( windowName );

    if (windowIter != s_theUISystem->m_windows.end())
        return windowIter->second;

    return nullptr;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool UISystem::DoesWidgetExist( const std::string& widgetName )
{
    WidgetMap::const_iterator widgetIter = s_theUISystem->m_widgetsByName.find( ConvertToLowerCase(widgetName) );

    if (widgetIter != s_theUISystem->m_widgetsByName.end())
        return true;
    else
        return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
BaseWidget* UISystem::GetWidgetByName( const std::string& widgetName )
{
    WidgetMap::const_iterator widgetIter = s_theUISystem->m_widgetsByName.find( ConvertToLowerCase( widgetName ) );

    if (widgetIter != s_theUISystem->m_widgetsByName.end())
        return widgetIter->second;

    return nullptr;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void UISystem::GetWidgetPropertiesFromXMLBasedOnType( const XMLNode& node, NamedProperties& in_out_properties, WidgetType type )
{
    switch (type)
    {
    case WidgetType_BUTTON:
        ButtonWidget::GetPropertiesFromXML( node, in_out_properties );
        break;
    case WidgetType_GROUP:
        GroupWidget::GetPropertiesFromXML( node, in_out_properties );
        break;
    case WidgetType_LABEL:
        LabelWidget::GetPropertiesFromXML( node, in_out_properties );
    case WidgetType_PROGRESS_BAR:
        ProgressBarWidget::GetPropertiesFromXML( node, in_out_properties );
    case WidgetType_CHECKBOX:
        CheckboxWidget::GetPropertiesFromXML( node, in_out_properties );
    default:
        BaseWidget::GetPropertiesFromXML( node, in_out_properties );
        break;
    }

    return;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
WidgetPropertyLayer UISystem::GetWidgetPropertyLayerFromXML( const XMLNode& node )
{
    WidgetPropertyLayer properties;
    WidgetType type = GetWidgetTypeAsEnum( GetXMLNodeName( node ) );

    XMLNode defaultWidgetNode = node.getChildNode( "Default" );

    if (!defaultWidgetNode.isEmpty())
    {
        NamedProperties defaultPropertyValues;
        GetWidgetPropertiesFromXMLBasedOnType( defaultWidgetNode, defaultPropertyValues, type );

        properties.m_defaults = defaultPropertyValues;
    }

    // state specific for widget type
    for (int widgetState = 0; widgetState < WidgetState_NUM_STATES; ++widgetState)
    {
        std::string widgetStateString = GetWidgetStateAsString( (WidgetState)widgetState );
        XMLNode stateBaseWidgetNode = node.getChildNode( widgetStateString.c_str() );

        if (!stateBaseWidgetNode.isEmpty())
        {
            NamedProperties statePropertyValues;
            GetWidgetPropertiesFromXMLBasedOnType( stateBaseWidgetNode, statePropertyValues, type );

            properties.m_defaultsByState[widgetState] = statePropertyValues;
        }
    }
    return properties;
}

////---------------------------------------------------------------------------------
///
// Mutators
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
BaseWidget* UISystem::CreateWidget( WidgetType type, const std::string& widgetName, const WidgetPropertyLayer& instanceProperties )
{
    s_theUISystem->m_numCreatedWidgets++;

    switch (type)
    {
    case WidgetType_BUTTON:         return new ButtonWidget( widgetName, instanceProperties );
    case WidgetType_GROUP:          return new GroupWidget( widgetName, instanceProperties );
    case WidgetType_LABEL:          return new LabelWidget( widgetName, instanceProperties );
    case WidgetType_PROGRESS_BAR:   return new ProgressBarWidget( widgetName, instanceProperties );
    case WidgetType_CHECKBOX:       return new CheckboxWidget( widgetName, instanceProperties );
    default:    
        s_theUISystem->m_numCreatedWidgets--;
        return nullptr;
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool UISystem::AddWidgetDefinition( const std::string& widgetName, WidgetPropertyLayer properties ) 
{
    if (DoesWidgetDefinitionExist( widgetName ))
        return false;
            
    s_theUISystem->m_widgetDefinitions.insert( std::pair< std::string, WidgetPropertyLayer >( widgetName, properties ) );
    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------    
bool UISystem::AddWidget( const std::string& widgetName, BaseWidget* widget )
{
    if (DoesWidgetExist( widgetName ))
        return false;

    s_theUISystem->m_widgetsByName.insert( std::pair< std::string, BaseWidget* >( widgetName, widget ) );
    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void UISystem::PushWindow( const std::string& windowName )
{
    std::string lcWindowName = ConvertToLowerCase( windowName );
    if (s_theUISystem->m_windowStack.size() > 0)
    {
        UIWindow* topWindow = s_theUISystem->m_windowStack.back();

        // if top window is the one being asked to show, do nothing
        if (topWindow->GetName() == lcWindowName)
            return;
    }

    UIWindow* windowToPush = GetWindowByName( lcWindowName );
    if( windowToPush)
        s_theUISystem->m_windowStack.push_back( windowToPush );
}

///---------------------------------------------------------------------------------
/// pops windows off the stack until it reaches a window with the specified name
///---------------------------------------------------------------------------------
void UISystem::PopWindow( const std::string& popUntilWindowName /* = "top" */ )
{
    if (s_theUISystem->m_windowStack.size() > 0)
    {
        if (popUntilWindowName == "top")
            s_theUISystem->m_windowStack.pop_back();
        else
        {
            UIWindow* window = s_theUISystem->m_windowStack.back();
            std::string windowName = window->GetName();
            while (windowName != popUntilWindowName)
            {
                if (s_theUISystem->m_windowStack.size() > 0)
                {
                    s_theUISystem->m_windowStack.pop_back();
                    if (s_theUISystem->m_windowStack.size() > 0)
                    {
                        window = s_theUISystem->m_windowStack.back();
                        windowName = window->GetName();
                        continue;
                    }
                    break;
                }
                break;
            }
        }
    }
        
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void UISystem::AddWidgetToDefaultWindow( BaseWidget* widget )
{
    UIWindow* defaultWindow = s_theUISystem->GetWindowByName( "defaultwindow" );
    if( defaultWindow )
        defaultWindow->AddChildWidget( widget );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void UISystem::HideWidgetByName( const std::string& widgetName )
{
    BaseWidget* widgetToHide = GetWidgetByName( widgetName );
    if (widgetToHide)
        widgetToHide->SetState( WidgetState_HIDDEN );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void UISystem::ActivateWidgetByName( const std::string& widgetName )
{
    BaseWidget* widgetToActivate = GetWidgetByName( widgetName );
    if (widgetToActivate)
        widgetToActivate->SetState( WidgetState_IDLE );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void UISystem::DisableWidgetByName( const std::string& widgetName )
{
    BaseWidget* widgetToDisable = GetWidgetByName( widgetName );
    if (widgetToDisable)
        widgetToDisable->SetState( WidgetState_DISABLED );
}

////---------------------------------------------------------------------------------
///
// Update
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void UISystem::ProcessInput( InputSystem* inputSystem )
{
    if (s_theUISystem->m_windowStack.size() > 0)
    {
        UIWindow* topWindow = s_theUISystem->m_windowStack.back();
        if (topWindow)
            topWindow->ProcessInput( inputSystem );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void UISystem::Update()
{

    for (WindowStack::iterator windowIter = s_theUISystem->m_windowStack.begin(); windowIter != s_theUISystem->m_windowStack.end(); ++windowIter)
    {
        UIWindow* window = *windowIter;

        window->Update();
    }
}

////---------------------------------------------------------------------------------
///
// Render
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void UISystem::Render( OpenGLRenderer* renderer )
{
    if (!renderer)
        return;

    for (WindowStack::iterator windowIter = s_theUISystem->m_windowStack.begin(); windowIter != s_theUISystem->m_windowStack.end(); ++windowIter)
    {
        UIWindow* window = *windowIter;

        window->Render( renderer );
    }
}


////===========================================================================================
///===========================================================================================
// Console Commands
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
CONSOLE_COMMAND( hide_widget, "args: widgetName | desc: Sets the widget state of the widget with the specified name to Hidden" )
{
    std::string widgetToHide;
    if (args->GetString( widgetToHide ))
    {
        if (UISystem::DoesWidgetExist( widgetToHide ))
            UISystem::HideWidgetByName( widgetToHide );
        else
            DeveloperConsole::WriteLine( "Widget " + widgetToHide + " does not exist.", ERROR_TEXT_COLOR );
    }
    else
        DeveloperConsole::WriteLine( "Widget name was unspecified or of the incorrect type.", ERROR_TEXT_COLOR );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
CONSOLE_COMMAND( activate_widget, "args: widgetName | desc: Sets the widget state of the widget with the specified name to Idle (i.e. active)" )
{
    std::string widgetToActivate;
    if (args->GetString( widgetToActivate ))
    {
        if (UISystem::DoesWidgetExist( widgetToActivate ))
            UISystem::ActivateWidgetByName( widgetToActivate );
        else
            DeveloperConsole::WriteLine( "Widget " + widgetToActivate + " does not exist.", ERROR_TEXT_COLOR );
    }
    else
        DeveloperConsole::WriteLine( "Widget name was unspecified or of the incorrect type.", ERROR_TEXT_COLOR );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
CONSOLE_COMMAND( disable_widget, "args: widgetName | desc: Sets the widget state of the widget with the specified name to Disabled" )
{
    std::string widgetToDisable;
    if (args->GetString( widgetToDisable ))
    {
        if (UISystem::DoesWidgetExist( widgetToDisable ))
            UISystem::DisableWidgetByName( widgetToDisable );
        else
            DeveloperConsole::WriteLine( "Widget " + widgetToDisable + " does not exist.", ERROR_TEXT_COLOR );
    }
    else
        DeveloperConsole::WriteLine( "Widget name was unspecified or of the incorrect type.", ERROR_TEXT_COLOR );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
CONSOLE_COMMAND( push_window, "args: windowName | desc: Pushes the specified window to the top of the window stack" )
{
    std::string windowToPush;
    if (args->GetString( windowToPush ))
    {
        if (UISystem::DoesWindowExist( windowToPush ))
            UISystem::PushWindow( windowToPush );
        else
            DeveloperConsole::WriteLine( "Window " + windowToPush + " does not exist.", ERROR_TEXT_COLOR );
    }
    else
        DeveloperConsole::WriteLine( "Window name was unspecified or of the incorrect type.", ERROR_TEXT_COLOR );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
CONSOLE_COMMAND( pop_window, "args: n/a | desc: Pops the specified window to the top of the window stack" )
{

    UISystem::PopWindow();
        
}