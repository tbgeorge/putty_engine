//=================================================================================
// Profiler.hpp
// Author: Tyler George
// Date  : October 29, 2015
//=================================================================================

#ifndef __included_Profiler__
#define __included_Profiler__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------

#include <string>
#include <queue>
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Utilities/Time.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Renderer/FontRenderer.hpp"

#define PROFILE_BLOCK( category ) ProfileSection block( category );

///---------------------------------------------------------------------------------
/// ProfileData Struct
///---------------------------------------------------------------------------------
struct ProfileData
{
    ProfileData()
        : m_totalTime_us( 0LL ), m_averageTime_us( 0LL ), m_frameCount( 0LL ) {}

    long long m_totalTime_us;
    long long m_averageTime_us;
    long long m_frameCount;

    std::vector< long long > m_timesThisFrame_us;


    void UpdateTimes();

};

///---------------------------------------------------------------------------------
/// ProfileSection Class
///---------------------------------------------------------------------------------
class ProfileSection
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    ProfileSection( const std::string& categoryName );
    ~ProfileSection();

private:
    ///---------------------------------------------------------------------------------
    /// Private Member Variables
    ///---------------------------------------------------------------------------------

    long long m_startTime_us;
    long long m_startTime_ms;

    long long m_endTime_ms;
    long long m_endTime_us;

    std::string m_categoryName;
};

///---------------------------------------------------------------------------------
/// Profiler Class
///---------------------------------------------------------------------------------
class Profiler
{
public:
    ///---------------------------------------------------------------------------------
    /// Static Profiling functions
    ///---------------------------------------------------------------------------------
    static void StartProfiling( const std::string& categoryName );
    static void StopProfiling();

    static void UpdateProfiler();
    static void PrintDataToConsole();
    static void PrintDataToScreen( OpenGLRenderer* renderer );

    ///---------------------------------------------------------------------------------
    /// Static Variables
    ///---------------------------------------------------------------------------------
    static std::queue< ProfileSection* > s_startStopProfilers;
    static std::map< std::string, ProfileData > s_profileDataMap;
};

#endif // Profiler