//=================================================================================
// EventSystem.hpp
// Author: Tyler George
// Date  : March 21, 2016
//=================================================================================

#pragma once

#ifndef __included_EventSystem__
#define __included_EventSystem__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
// #include "Engine/Common/EngineCommon.hpp"
#include "Engine/Systems/Events/EventSubscriber.hpp"
#include <vector>
#include <string>
#include <map>
class EventReceiver;


///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------
typedef void (EventCallbackFunc)( NamedProperties& args );

///---------------------------------------------------------------------------------
/// Structs
///---------------------------------------------------------------------------------
struct SubscriberList
{
    std::vector< EventCallbackFunc* > m_callbacks;
    std::vector< EventSubscriberBase* > m_subscribers;
};

///---------------------------------------------------------------------------------
/// EventSystem class
///---------------------------------------------------------------------------------
class EventSystem
{
public:
    ///---------------------------------------------------------------------------------
    ///
    ///---------------------------------------------------------------------------------
    static void CreateInstance( /*args*/ );
    static EventSystem& GetInstance();

    int FireEvent( const std::string& eventName, NamedProperties& args );
    void RegisterEventCallback( const std::string& eventName, EventCallbackFunc* callbackFunc );
    void UnregisterEventCallback( const std::string& eventName, EventCallbackFunc* callbackFunc );

    template< typename T_ObjectType, typename T_MethodType  >
    void RegisterEventSubscriber( const std::string& eventName, T_ObjectType& subscriber, T_MethodType callback );

    void UnregisterObjectFromAllEvents( EventReceiver* object );

private:
    ///---------------------------------------------------------------------------------
    ///
    ///---------------------------------------------------------------------------------
    static EventSystem* s_theEventSystem;

    EventSystem();

    std::map< std::string, SubscriberList > m_eventSubscriptions;

};

////===========================================================================================
///===========================================================================================
// Templated function implementation
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename T_ObjectType, typename T_MethodType >
void EventSystem::RegisterEventSubscriber( const std::string& eventName, T_ObjectType& subscriber, T_MethodType callback )
{
    EventSubscriberBase* sub = new EventSubscriber< T_ObjectType >( callback, subscriber );
    SubscriberList& subs = m_eventSubscriptions[eventName];
    subs.m_subscribers.push_back( sub );
}

#endif