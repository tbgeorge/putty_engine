//=================================================================================
// Memory.hpp
// Author: Tyler George
// Date  : November 10, 2015
//=================================================================================

#pragma once

#ifndef __included_Memory__
#define __included_Memory__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include <string>
#include <map>
class OpenGLRenderer;

// #define CUSTOM_ALLOCATION

///---------------------------------------------------------------------------------
/// Structs
///---------------------------------------------------------------------------------
struct PointerTrackingData
{
    PointerTrackingData( char* file, int lineNum, size_t size )
        : m_file( file ), m_lineNum( lineNum ), m_size( size ) {}

    char* m_file;
    int m_lineNum;
    size_t m_size;
};

///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------
typedef std::map< void*, PointerTrackingData > PointerDataMap;

///---------------------------------------------------------------------------------
/// Overloaded New/Delete
///---------------------------------------------------------------------------------

void* Allocate( size_t count );
void* operator new(size_t count);
void* operator new[]( size_t count );
void* operator new(size_t count, char* file, int lineNum );
void* operator new[]( size_t count, char* file, int lineNum );

void Deallocate( void* ptr );
void operator delete(void* ptr);
void operator delete[]( void* ptr );
void operator delete(void* ptr, char* file, int lineNum);
void operator delete[]( void* ptr, char* file, int lineNum );


///---------------------------------------------------------------------------------
/// Tracking
///---------------------------------------------------------------------------------

void MemoryStartup( size_t requestedMemory = 0 );
void TrackAllocation( size_t count );
void UpdateTrackingData();
void RenderTrackingData( OpenGLRenderer* renderer );

void MemoryShutdown();


#define new new( __FILE__, __LINE__ )




#endif
