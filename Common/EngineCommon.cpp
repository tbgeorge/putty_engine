//=================================================================================
// EngineCommon.cpp
// Author: Tyler George
// Date  : June 17, 2015
//=================================================================================

#include "Engine/Common/EngineCommon.hpp"


///---------------------------------------------------------------------------------
/// Constants
///---------------------------------------------------------------------------------
const Rgba INFO_TEXT_COLOR = Rgba( 0xFFFF00, 1.0f );
const Rgba WARNING_TEXT_COLOR = Rgba( 0xFFA500, 1.0f );
const Rgba ERROR_TEXT_COLOR = Rgba( 0xFF0000, 1.0f );

bool g_shouldContinueExecution = true;
bool g_appHasEnded = false;