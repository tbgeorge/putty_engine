//=================================================================================
// EngineCommon.cpp
// Author: Tyler George
// Date  : June 17, 2015
//=================================================================================

#include "Engine/Common/EngineCommon.hpp"


///---------------------------------------------------------------------------------
/// Constants
///---------------------------------------------------------------------------------
const Rgba INFO_TEXT_COLOR = Rgba::YELLOW;
const Rgba WARNING_TEXT_COLOR = Rgba::ORANGE;
const Rgba ERROR_TEXT_COLOR = Rgba::RED;

bool g_shouldContinueExecution = true;
bool g_appHasEnded = false;