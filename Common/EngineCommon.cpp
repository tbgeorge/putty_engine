//=================================================================================
// EngineCommon.cpp
// Author: Tyler George
// Date  : June 17, 2015
//=================================================================================

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Systems/Events/EventSystem.hpp"
#include "Engine/Utilities/NamedProperties.hpp"
#include "Engine/Systems/Events/EventReceiver.hpp"

///---------------------------------------------------------------------------------
/// Constants
///---------------------------------------------------------------------------------
const Rgba INFO_TEXT_COLOR = Rgba( 0xFFFF00, 1.0f );
const Rgba WARNING_TEXT_COLOR = Rgba( 0xFFA500, 1.0f );
const Rgba ERROR_TEXT_COLOR = Rgba( 0xFF0000, 1.0f );

bool g_shouldContinueExecution = true;
bool g_appHasEnded = false;

///---------------------------------------------------------------------------------
/// Functions
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
int FireEvent( const std::string& eventName )
{
    NamedProperties args;
    return FireEvent( eventName, args );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
int FireEvent( const std::string& eventName, NamedProperties& args )
{
    EventSystem& eventSys = EventSystem::GetInstance();
    return eventSys.FireEvent( eventName, args );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void RegisterEventCallback( const std::string& eventName, EventCallbackFunc* callbackFunc )
{
    EventSystem& eventSys = EventSystem::GetInstance();
    eventSys.RegisterEventCallback( eventName, callbackFunc );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void UnregisterEventCallback( const std::string& eventName, EventCallbackFunc* callbackFunc )
{
    EventSystem& eventSys = EventSystem::GetInstance();
    eventSys.UnregisterEventCallback( eventName, callbackFunc );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void UnregisterEventSubscriberFromAllEvents( EventReceiver* object )
{
    EventSystem& eventSys = EventSystem::GetInstance();
    eventSys.UnregisterObjectFromAllEvents( object );
}