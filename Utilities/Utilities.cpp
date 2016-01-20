//=================================================================================
// Utilities.cpp
// Author: Tyler George
// Date  : February 10, 2015
//=================================================================================

#include <cstdlib>
#include "Engine/Utilities/Utilities.hpp"

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
std::string ConvertBackSlashesToForwardSlashes( const std::string& stringWithPossibleBackSlashes )
{
	std::string convertedString = stringWithPossibleBackSlashes;

	size_t characterIndex = 0;
	while( characterIndex != std::string::npos )
	{
		characterIndex = convertedString.find_first_of( '\\', characterIndex );
		if( characterIndex != std::string::npos )
			convertedString.replace( characterIndex, 1, "/" );
	}
	return convertedString;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
std::string ConvertToLowerCase( const std::string& stringWithPossibleUpperCaseLetters )
{
	std::string convertedString = stringWithPossibleUpperCaseLetters;

	size_t stringLength = convertedString.size();
	size_t numberOfCharactersToCut = 1;
	size_t numberOfCharactersToPaste = 1;
	for( size_t characterIndex = 0; characterIndex < stringLength; ++characterIndex )
	{
		char character = convertedString.at( characterIndex );
		convertedString.replace( characterIndex, numberOfCharactersToCut, numberOfCharactersToPaste, (char) tolower( (int) character ) );
	}

	return convertedString;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Tokenize( const std::string& string, std::vector<std::string>& tokens, const std::string& delimiters /* = " " */ )
{
    // Skip delimiters at beginning.
    size_t lastPos = string.find_first_not_of( delimiters, 0 );
    // Find first "non-delimiter".
    size_t pos = string.find_first_of( delimiters, lastPos );

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back( string.substr( lastPos, pos - lastPos ) );
        // Skip delimiters.  Note the "not_of"
        lastPos = string.find_first_not_of( delimiters, pos );
        // Find next "non-delimiter"
        pos = string.find_first_of( delimiters, lastPos );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
std::string ExtractFileNameFromPath( const std::string& filePathStr )
{
    std::string result = "";

    result = filePathStr.substr( filePathStr.find_last_of( "/\\" ) + 1 );

    return result;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
std::string RemoveFileExtension( const std::string& fileNameStr )
{
    std::string result = "";

    result = fileNameStr.substr( 0, fileNameStr.find_first_of( "." ) );

    return result;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsInt( const std::string& line )
{
    char* p;
    strtol( line.c_str(), &p, 10 );
    return *p == 0;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsUnsignedInt( const std::string& line )
{
    char* p;
    strtoul( line.c_str(), &p, 10 );
    return *p == 0;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsHexInt( const std::string& line )
{
    char* p;
    strtoul( line.c_str(), &p, 0 );
    return *p == 0;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsDouble( const std::string& line )
{
    char* p;
    strtod( line.c_str(), &p );
    return *p == 0;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsFloat( const std::string& line )
{
    char* p;
    strtof( line.c_str(), &p );
    return *p == 0;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsWhitespace( char c )
{
    switch (c) {
    case '\n':
    case '\r':
    case ' ':
    case '\t':
        return true;

    default:
        return false;
    };
}

///---------------------------------------------------------------------------------
/// Trims whitespace at the beginning and end of a c-string
///---------------------------------------------------------------------------------
char* Trim( char *s )
{
    while (IsWhitespace( *s )) {
        ++s;
    }

    char *end = s;
    while (*end != NULL) {
        ++end;
    }

    if (end > s) {
        --end;
        while ((end > s) && IsWhitespace( *end )) {
            --end;
        }

        ++end;
        *end = NULL;
    }

    return s;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void* AlignedMalloc( size_t numBytes, size_t alignment )
{
    void* p1;
    void* p2;

    p1 = (void*)malloc( numBytes + alignment + sizeof( size_t ) );

    if (!p1)
        return NULL;

    else
    {
        size_t addr = (size_t)p1 + alignment + sizeof( size_t );

        p2 = (void*)(addr - addr % alignment);

        *((size_t *)p2 - 1) = (size_t)p1;

        return p2;
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void AlignedFree( void* p )
{
    free( (void *)(*((size_t *)p - 1)) );
}