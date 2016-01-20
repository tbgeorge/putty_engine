//=================================================================================
// Error.hpp
// Author: Tyler George
// Date  : October 29, 2015
//=================================================================================

#pragma once

#ifndef __included_Error__
#define __included_Error__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include <string>
#include "Engine/Common/EngineCommon.hpp"



///---------------------------------------------------------------------------------
/// Defines
///---------------------------------------------------------------------------------
#define FATAL_ERROR() FatalError( __FILE__, __LINE__ );
#define RECOVERABLE_ERROR() RecoverableError( __FILE__, __LINE__ );
#define FATAL_ASSERT( evaluation ) FatalAssert( __FILE__, __LINE__, evaluation, #evaluation );
#define RECOVERABLE_ASSERT( evaluation ) RecoverableAssert( __FILE__, __LINE__, evaluation, #evaluation );

///---------------------------------------------------------------------------------
/// Error Functions
///---------------------------------------------------------------------------------
void FatalError( const char* fileName, const int& lineNum );
void RecoverableError( const char* fileName, const int& lineNum );
void FatalAssert( const char* fileName, const int& lineNum, bool eval, const char* evaluation );
void RecoverableAssert( const char* fileName, const int& lineNum, bool eval, const char* evaluation );


#endif