//=================================================================================
// EngineCommon.hpp
// Author: Tyler George
// Date  : June 14, 2015
//=================================================================================

#pragma once 

#ifndef __included_EngineCommon__
#define __included_EngineCommon__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include <string>
#include <vector>
#include <map>
#include "Engine/Utilities/Rgba.hpp"
#include "Engine/Utilities/DeveloperConsoleCommands.hpp"
#include "Engine/Low-Level/Memory.hpp"
#include "../Systems/Events/EventSystem.hpp"

class NamedProperties;
class EventReceiver;

///---------------------------------------------------------------------------------
/// Defines
///---------------------------------------------------------------------------------
#define UNUSED(x) (void)(x)

///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------
typedef std::vector<std::string> Strings;
typedef void (EventCallbackFunc)( NamedProperties& args );

///---------------------------------------------------------------------------------
/// Globals
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
/// Functions
///---------------------------------------------------------------------------------
int FireEvent( const std::string& eventName );
int FireEvent( const std::string& eventName, NamedProperties& args );
void RegisterEventCallback( const std::string& eventName, EventCallbackFunc* callbackFunc );
void UnregisterEventCallback( const std::string& eventName, EventCallbackFunc* callbackFunc );

template< typename T_ObjectType, typename T_MethodType >
void RegisterEventSubscriber( const std::string& eventName, T_ObjectType& object, T_MethodType method );

template< typename T_ObjectType, typename T_MethodType >
void UnregisterEventSubscriber( const std::string& eventName, T_ObjectType& object, T_MethodType method );

void UnregisterEventSubscriberFromAllEvents( EventReceiver* object );

///---------------------------------------------------------------------------------
/// Constants
///---------------------------------------------------------------------------------
extern const Rgba INFO_TEXT_COLOR;
extern const Rgba WARNING_TEXT_COLOR;
extern const Rgba ERROR_TEXT_COLOR;

extern bool g_shouldContinueExecution;
extern bool g_appHasEnded;

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename T_ObjectType, typename T_MethodType >
void RegisterEventSubscriber( const std::string& eventName, T_ObjectType& object, T_MethodType method )
{
    EventSystem& eventSys = EventSystem::GetInstance();
    eventSys.RegisterEventSubscriber( eventName, object, method );
}

#endif