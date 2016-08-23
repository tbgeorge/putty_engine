//=================================================================================
// EventSubscriberBase.hpp
// Author: Tyler George
// Date  : March 30, 2016
//=================================================================================

#pragma once

#ifndef __included_EventSubscriberBase__
#define __included_EventSubscriberBase__
 
// #include "Engine/Common/EngineCommon.hpp"
#include "Engine/Utilities/NamedProperties.hpp"

///---------------------------------------------------------------------------------
/// class EventSubscriberBase
///---------------------------------------------------------------------------------
class EventSubscriberBase
{
public: 
    virtual void Fire( NamedProperties& np ) = 0;
    virtual void* GetObject() = 0;
};

#endif