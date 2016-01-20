//=================================================================================
// Memory.cpp
// Author: Tyler George
// Date  : November 10, 2015
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================

#include <list>
#include "Engine/Low-Level/Memory.hpp"
#include "Engine/Utilities/Console.hpp"
#include "Engine/Utilities/Error.hpp"
#include "Engine/Low-Level/SignPostAllocator.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Renderer/FontRenderer.hpp"
#include "Engine/Utilities/Font.hpp"
#include "Engine/Multi-Threading/Locks.hpp"
#undef new

////===========================================================================================
///===========================================================================================
// Static variable initialization
///===========================================================================================
////===========================================================================================

PointerDataMap* g_pointerData = nullptr;

#ifdef CUSTOM_ALLOCATION
SignPostAllocator g_allocator;
#endif

CriticalSection* g_mapLock = nullptr;

unsigned int g_totalAllocations = 0;
unsigned int g_unpairedAllocations = 0;
unsigned int g_totalBytesAllocated = 0;
unsigned int g_largestAllocation = 0;
float g_averageAllocation = 0.0f;

unsigned int g_numCurrentBytes = 0;

unsigned int g_maxNumAllocationsAtOnce = 0;
unsigned int g_maxBytesAllocatedAtOnce = 0;

std::list< unsigned int > g_past5FramesAllocations;
std::list< unsigned int > g_past5FramesBytes;



////===========================================================================================
///===========================================================================================
// Overloaded New/Delete
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void* Allocate( size_t count )
{
    void* ptr = nullptr;
#ifdef CUSTOM_ALLOCATION
    if (g_allocator.GetBufferSize() == 0)
        ptr = malloc( count );
    else
        ptr = g_allocator.Alloc( count );
#else
    ptr = malloc( count );
#endif
    FATAL_ASSERT( ptr != nullptr );

    return ptr;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void* operator new(size_t count)
{
    TrackAllocation( count );

    return Allocate( count );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void* operator new[]( size_t count )
{
    TrackAllocation( count );

    return Allocate( count );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void* operator new(size_t count, char* file, int lineNum)
{
    TrackAllocation( count );
    void* ptr = Allocate( count );

    if (!g_mapLock && !g_appHasEnded)
        g_mapLock = new CriticalSection();

    if ( g_mapLock )
        g_mapLock->Enter();

    if (!g_pointerData)
        g_pointerData = new PointerDataMap();

    g_numCurrentBytes += count;
    PointerTrackingData data( file, lineNum, count );
    g_pointerData->insert( std::pair< void*, PointerTrackingData >( ptr, data ) );

    if ( g_mapLock )
        g_mapLock->Exit();

    return ptr;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void* operator new[]( size_t count, char* file, int lineNum )
{
    TrackAllocation( count );

    void* ptr = Allocate( count );

    if (!g_mapLock && !g_appHasEnded)
        g_mapLock = new CriticalSection();

    if (g_mapLock)
        g_mapLock->Enter();

    if (!g_pointerData)
        g_pointerData = new PointerDataMap();

    g_numCurrentBytes += count;
    PointerTrackingData data( file, lineNum, count );
    g_pointerData->insert( std::pair< void*, PointerTrackingData >( ptr, data ) );

    if (g_mapLock)
        g_mapLock->Exit();

    return ptr;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Deallocate( void* ptr )
{
#ifdef CUSTOM_ALLOCATION
    if (g_allocator.GetBufferSize() == 0)
        return free( ptr );

    void* buf = g_allocator.GetBuffer();
    size_t size = g_allocator.GetBufferSize();
    if ( ( ptr < buf ) || ( ptr > ( (Byte*)(buf) + size ) ) )
        return free( ptr );

    return g_allocator.DeAlloc( ptr );
#else    
    free( ptr );
#endif
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void operator delete(void* ptr)
{
    if (ptr != g_mapLock)
    {
        if (!g_mapLock && !g_appHasEnded)
            g_mapLock = new CriticalSection();

        if (g_mapLock)
            g_mapLock->Enter();

        if (!g_pointerData)
            g_pointerData = new PointerDataMap();

        if (!g_pointerData->empty())
        {
            PointerDataMap::iterator iter = g_pointerData->find( ptr );

            if (iter != g_pointerData->end())
            {
                g_numCurrentBytes -= iter->second.m_size;
                g_pointerData->erase( iter );

            }

        }
        if (g_mapLock)
            g_mapLock->Exit();
    }

    g_unpairedAllocations--;


    Deallocate( ptr );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void operator delete[]( void* ptr )
{
    if (!g_mapLock && !g_appHasEnded)
        g_mapLock = new CriticalSection();

    if (g_mapLock)
        g_mapLock->Enter();

    if (!g_pointerData)
        g_pointerData = new PointerDataMap();

    if (!g_pointerData->empty())
    {
        PointerDataMap::iterator iter = g_pointerData->find( ptr );

        if (iter != g_pointerData->end())
        {
            g_numCurrentBytes -= iter->second.m_size;
            g_pointerData->erase( iter );
        }
    }

    if (g_mapLock)
        g_mapLock->Exit();

    g_unpairedAllocations--;

    Deallocate( ptr );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void operator delete(void* ptr, char* file, int lineNum)
{
    UNUSED( file );
    UNUSED( lineNum );

    g_unpairedAllocations--;

    Deallocate( ptr );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void operator delete[]( void* ptr, char* file, int lineNum )
{
    UNUSED( file );
    UNUSED( lineNum );

    g_unpairedAllocations--;
    
    Deallocate( ptr );
}

////===========================================================================================
///===========================================================================================
// Tracking
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void MemoryStartup( size_t requestedMemory /* = 0 */ )
{
#ifdef CUSTOM_ALLOCATION
    g_allocator.Startup( requestedMemory );
#else
    UNUSED( requestedMemory );
#endif 

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void TrackAllocation( size_t count )
{
    g_totalAllocations++;

    if (g_unpairedAllocations > g_maxNumAllocationsAtOnce)
        g_maxNumAllocationsAtOnce = g_unpairedAllocations;

    g_unpairedAllocations++;

    g_totalBytesAllocated += count;

    if (g_numCurrentBytes > g_maxBytesAllocatedAtOnce)
        g_maxBytesAllocatedAtOnce = g_numCurrentBytes;

    if (count > g_largestAllocation)
        g_largestAllocation = count;

    if (g_averageAllocation > 0.0f )
        g_averageAllocation = (0.9f * g_averageAllocation) + (0.1f * (float)count);
    else
        g_averageAllocation = (float)count;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void UpdateTrackingData()
{
    if (g_past5FramesAllocations.size() == 10)
    {
        g_past5FramesAllocations.pop_back();
        g_past5FramesAllocations.push_front( g_unpairedAllocations );
        
        g_past5FramesBytes.pop_back();
        g_past5FramesBytes.push_front( g_numCurrentBytes );
    }
    else
    {
        g_past5FramesAllocations.push_front( g_unpairedAllocations );
        g_past5FramesBytes.push_front( g_numCurrentBytes );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void RenderTrackingData( OpenGLRenderer* renderer )
{
    renderer->Disable( GL_CULL_FACE );
    renderer->Disable( GL_DEPTH_TEST );
    renderer->Enable( GL_BLEND );
    renderer->Enable( GL_TEXTURE_2D );
    renderer->BlendFunct( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    FontRenderer* fRenderer = renderer->GetFontRenderer();

    Vector3 zeroPos( 10.0f, 0.0f, 1.0f );
    fRenderer->DrawFontTextOrtho( 32, *Font::CreateOrGetFont( "Data/Fonts/Calibri" ), "0", zeroPos, Rgba::WHITE );

    std::string maxAlloc = "Allocations: " + std::to_string( g_maxNumAllocationsAtOnce ) + " (max) ";
    std::string maxBytes = "Bytes Allocated: " + std::to_string( g_maxBytesAllocatedAtOnce ) + " (max) ";

    Vector3 maxAllocPos( 0.0f, 330.0f, 1.0f );
    Vector3 maxBytesPos( 0.0f, 305.0f, 1.0f );
    fRenderer->DrawFontTextOrtho( 32, *Font::CreateOrGetFont( "Data/Fonts/Calibri" ), maxAlloc, maxAllocPos, Rgba::RED );
    fRenderer->DrawFontTextOrtho( 32, *Font::CreateOrGetFont( "Data/Fonts/Calibri" ), maxBytes, maxBytesPos, Rgba::GREEN );

    renderer->DrawColoredQuad( NULL, Vector2( 0.0f, 300.0f ), Vector2( 700.0f, 305.0f ), 1.0f, Rgba::BLACK );
    renderer->DrawColoredQuad( NULL, Vector2( 40.0f, 0.0f ), Vector2( 45.0f, 300.0f ), 1.0f, Rgba::BLACK );


    Vector2 allocPos( 50.0f, 0.0f );

    for (std::list< unsigned int >::const_iterator dataIter = g_past5FramesAllocations.begin(); dataIter != g_past5FramesAllocations.end(); ++dataIter)
    {
        unsigned int allocations = *dataIter;

        float maxY = RangeMap( (float)allocations, 0.0f, (float)g_maxNumAllocationsAtOnce, 0.0f, 300.0f );
        renderer->DrawColoredQuad( NULL, allocPos, Vector2( allocPos.x + 30.0f, maxY ), 1.0f, Rgba::RED );

        Vector3 textPos( allocPos.x, maxY - 20.0f, 1.0f );
        fRenderer->DrawFontTextOrtho( 16, *Font::CreateOrGetFont( "Data/Fonts/Calibri" ), std::to_string( allocations ), textPos, Rgba::WHITE );
        allocPos.x += 65.0f;
    }

    Vector2 bytePos( 80.0f, 0.0f );

    for (std::list< unsigned int >::const_iterator dataIter = g_past5FramesBytes.begin(); dataIter != g_past5FramesBytes.end(); ++dataIter)
    {
        unsigned int bytes = *dataIter;

        float maxY = RangeMap( (float)bytes, 0.0f, (float)g_maxBytesAllocatedAtOnce, 0.0f, 300.0f );
        renderer->DrawColoredQuad( NULL, bytePos, Vector2( bytePos.x + 30.0f, maxY ), 1.0f, Rgba::GREEN );

        Vector3 textPos( bytePos.x, maxY - 20.0f, 1.0f );
        fRenderer->DrawFontTextOrtho( 16, *Font::CreateOrGetFont( "Data/Fonts/Calibri" ), std::to_string( bytes ), textPos, Rgba::BLACK );

        bytePos.x += 65.0f;
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void MemoryShutdown()
{
    ConsolePrintf( "=============================================================================================\n" );
    ConsolePrintf( "============================== Memory Tracking Information ==================================\n\n" );
    ConsolePrintf( "Total Allocations:     %i\n", g_totalAllocations );
    ConsolePrintf( "Unpaired Allocations:  %i\n", g_unpairedAllocations );
    ConsolePrintf( "Total Bytes Allocated: %i\n", g_totalBytesAllocated );
    ConsolePrintf( "Largest Allocation:    %i\n", g_largestAllocation );
    ConsolePrintf( "Average Allocation:    %.02f\n\n", g_averageAllocation );

    if (!g_pointerData->empty())
    {
        for (PointerDataMap::iterator iter = g_pointerData->begin(); iter != g_pointerData->end(); ++iter )
        {
            //void* ptr = iter->first;
            PointerTrackingData data = iter->second;

            // inform user
            ConsolePrintf( "%s(%i): Memory leak of %i bytes \n", data.m_file, data.m_lineNum, data.m_size );

            // clean up memory
            //iter = s_pointerData->erase( iter );
            //free( ptr );

        }
    }

    ConsolePrintf( "\n=============================================================================================\n" );

#ifdef CUSTOM_ALLOCATION
    g_allocator.Shutdown();
#endif

   delete g_mapLock;
   g_mapLock = nullptr;
}


