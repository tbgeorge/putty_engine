//=================================================================================
// EventSubscriber.hpp
// Author: Tyler George
// Date  : March 30, 2016
//=================================================================================

#pragma once

#ifndef __included_EventSubscriber__
#define __included_EventSubscriber__

#include "Engine/Systems/Events/EventSubscriberBase.hpp"
#include "Engine/Utilities/NamedProperties.hpp"
#include "Engine/Utilities/Console.hpp"

///---------------------------------------------------------------------------------
/// templated class EventSubscriber
///---------------------------------------------------------------------------------
template< typename T_ObjectType >
class EventSubscriber : public EventSubscriberBase
{
public:
    typedef void(T_ObjectType::*MethodCallbackPtr)(NamedProperties&);

    EventSubscriber( MethodCallbackPtr callback, T_ObjectType& object )
        : m_objectMethod( callback )
        , m_objectInstance( object )
    {
//         ConsolePrintf( "Object: %s\nMethod:    %s\nObjMethod: %s\n", typeid(T_ObjectType).name(), typeid(T_MethodType).name(), typeid(callback).name() );
    }
    
    virtual void Fire( NamedProperties& np );
    virtual void* GetObject();

    MethodCallbackPtr m_objectMethod;
    T_ObjectType& m_objectInstance;

private:

};

template< typename T_ObjectType >
void EventSubscriber< T_ObjectType >::Fire( NamedProperties& np )
{
     (m_objectInstance.*m_objectMethod)( np );
}

template< typename T_ObjectType >
void* EventSubscriber< T_ObjectType >::GetObject()
{
    return &m_objectInstance;
}

#endif