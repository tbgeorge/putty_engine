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

///---------------------------------------------------------------------------------
/// Defines
///---------------------------------------------------------------------------------
#define UNUSED(x) (void)(x)

///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------
typedef std::vector<std::string> Strings;

///---------------------------------------------------------------------------------
/// Globals
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
/// Constants
///---------------------------------------------------------------------------------
extern const Rgba INFO_TEXT_COLOR;
extern const Rgba WARNING_TEXT_COLOR;
extern const Rgba ERROR_TEXT_COLOR;

extern bool g_shouldContinueExecution;
extern bool g_appHasEnded;




#endif