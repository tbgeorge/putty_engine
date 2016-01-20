//=================================================================================
// Utilities.hpp
// Author: Tyler George
// Date  : February 10, 2015
//=================================================================================

#pragma once
#ifndef __included_Utilities__
#define __included_Utilities__

#include <string>
#include <vector>
#include "Engine/Common/EngineCommon.hpp"


//===========================================================================================
//===========================================================================================
//  String modification
//===========================================================================================
//===========================================================================================
std::string ConvertBackSlashesToForwardSlashes( const std::string& stringWithPossibleBackSlashes );
std::string ConvertToLowerCase( const std::string& stringWithPossibleUpperCaseLetters );
void Tokenize( const std::string& string, std::vector<std::string>& tokens, const std::string& delimiters = " " );
std::string ExtractFileNameFromPath( const std::string& filePathStr );
std::string RemoveFileExtension( const std::string& fileNameStr );

bool IsInt( const std::string& line );
bool IsUnsignedInt( const std::string& line );
bool IsHexInt( const std::string& line );
bool IsDouble( const std::string& line );
bool IsFloat( const std::string& line );
bool IsWhitespace( char c );

//===========================================================================================
//===========================================================================================
// Memory management
//===========================================================================================
//===========================================================================================
void* AlignedMalloc( size_t numBytes, size_t alignment );
void AlignedFree( void* p );

#endif