//=================================================================================
// WidgetPropertyLayer.hpp
// Author: Tyler George
// Date  : April 26, 2016
//=================================================================================

#pragma once

#ifndef __included_WidgetPropertyLayer__
#define __included_WidgetPropertyLayer__

////---------------------------------------------------------------------------------
///
// Includes
///
////---------------------------------------------------------------------------------
#include "Engine\Utilities\NamedProperties.hpp"

////---------------------------------------------------------------------------------
///
// Enums
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
enum WidgetState
{
    WidgetState_IDLE,        // aka: Default
    WidgetState_HIGHLIGHTED, // aka: Hovered
    WidgetState_PRESSED,     // is currently being activated
    WidgetState_DISABLED,    // unclickable
    WidgetState_HIDDEN,      // invisible and unclickable
    WidgetState_NUM_STATES   // number of total states
};

////---------------------------------------------------------------------------------
///
// class WidgetPropertyLayer
///
////---------------------------------------------------------------------------------
class WidgetPropertyLayer
{
public:
    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    void CopyFromLayer( const WidgetPropertyLayer& propsToCopy );


    ///---------------------------------------------------------------------------------
    /// Public member variables
    ///---------------------------------------------------------------------------------
    NamedProperties m_defaults;
    NamedProperties m_defaultsByState[WidgetState_NUM_STATES];

};

#endif
