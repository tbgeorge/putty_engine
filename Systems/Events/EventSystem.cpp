//=================================================================================
// EventSystem.cpp
// Author: Tyler George
// Date  : March 23, 2016
//=================================================================================

#include "Engine/Systems/Events/EventSystem.hpp"
#include "Engine/Systems/Events/EventReceiver.hpp"

////===========================================================================================
///===========================================================================================
// Static Variable initialization
///===========================================================================================
////===========================================================================================
EventSystem* EventSystem::s_theEventSystem = nullptr;

////===========================================================================================
///===========================================================================================
// Functions
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
EventSystem::EventSystem()
{
    m_eventSubscriptions.clear();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void EventSystem::CreateInstance()
{
    if (s_theEventSystem)
        return;// freak out

    s_theEventSystem = new EventSystem();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
EventSystem& EventSystem::GetInstance()
{
    if (!s_theEventSystem)
        ;//freak out
    return *s_theEventSystem;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
int EventSystem::FireEvent( const std::string& eventName, NamedProperties& args )
{
    std::map< std::string, SubscriberList >::iterator subscription = m_eventSubscriptions.find( eventName );

    if (subscription == m_eventSubscriptions.end())
        return 0;

    SubscriberList subscribers = subscription->second;
    for (EventCallbackFunc* callback : subscribers.m_callbacks)
    {
        callback( args );
    }

    //return (int)subscribers.m_callbacks.size();

    for (EventSubscriberBase* subscriber : subscribers.m_subscribers)
    {
        subscriber->Fire( args );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void EventSystem::RegisterEventCallback( const std::string& eventName, EventCallbackFunc* callbackFunc )
{
    m_eventSubscriptions[eventName].m_callbacks.push_back( callbackFunc );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void EventSystem::UnregisterEventCallback( const std::string& eventName, EventCallbackFunc* callbackFunc )
{
    std::map< std::string, SubscriberList >::iterator subscription = m_eventSubscriptions.find( eventName );

    if (subscription == m_eventSubscriptions.end())
        return;

    SubscriberList& subscribers = subscription->second;
    for (std::vector< EventCallbackFunc* >::iterator subscriberIter = subscribers.m_callbacks.begin(); subscriberIter != subscribers.m_callbacks.end(); ++subscriberIter)
    {
        EventCallbackFunc* subscriberCallbackFunc = *subscriberIter;
        if (subscriberCallbackFunc == callbackFunc)
        {
            subscribers.m_callbacks.erase( subscriberIter );
            break;
        }
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void EventSystem::UnregisterObjectFromAllEvents( EventReceiver* object )
{
    for (auto subListIter = m_eventSubscriptions.begin(); subListIter != m_eventSubscriptions.end(); ++subListIter)
    {
        SubscriberList& list = subListIter->second;

        for (auto subIter = list.m_subscribers.begin(); subIter != list.m_subscribers.end(); )
        {
            EventSubscriberBase* sub = *subIter;
            if( sub->GetObject() == object )
            {
                delete sub;
                subIter = list.m_subscribers.erase( subIter );
            }
            else
                ++subIter;
        }
    }
    
}