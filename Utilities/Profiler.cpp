//=================================================================================
// Profiler.cpp
// Author: Tyler George
// Date  : October 29, 2015
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "Engine/Utilities/Profiler.hpp"
#include "Console.hpp"
#include "../UI/TextBox.hpp"
#include "Error.hpp"


////===========================================================================================
///===========================================================================================
// Static Variable Initialization
///===========================================================================================
////===========================================================================================

std::queue< ProfileSection* > Profiler::s_startStopProfilers;
std::map< std::string, ProfileData > Profiler::s_profileDataMap;

////===========================================================================================
///===========================================================================================
// ProfileData Struct
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ProfileData::UpdateTimes()
{
    m_totalTime_us = 0LL;

    for (std::vector< long long >::const_iterator timeIter_us = m_timesThisFrame_us.begin(); timeIter_us != m_timesThisFrame_us.end(); ++timeIter_us)
        m_totalTime_us += *timeIter_us;

    m_averageTime_us = ((m_frameCount * m_averageTime_us) + m_totalTime_us) / (m_frameCount + 1);
    m_frameCount++;

    m_timesThisFrame_us.clear();
}

////===========================================================================================
///===========================================================================================
// ProfileSection Class
///===========================================================================================
////===========================================================================================

////===========================================================================================
///===========================================================================================
// Constructors/Destructors
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
ProfileSection::ProfileSection( const std::string& categoryName )
    : m_categoryName( categoryName )
{
    m_startTime_us = TimeGet_us();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
ProfileSection::~ProfileSection()
{
    m_endTime_us = TimeGet_us();

    long long elapsedTime_us = m_endTime_us - m_startTime_us;

    std::map< std::string, ProfileData >::iterator dataIter = Profiler::s_profileDataMap.find( m_categoryName );

    if (dataIter != Profiler::s_profileDataMap.end())
    {
        ProfileData& data = dataIter->second;
        data.m_timesThisFrame_us.push_back( elapsedTime_us );
    }

    else
    {
        ProfileData data;
        data.m_timesThisFrame_us.push_back( elapsedTime_us );

        Profiler::s_profileDataMap[m_categoryName] = data;
    }
}

////===========================================================================================
///===========================================================================================
// Static Profiling Functions
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Profiler::StartProfiling( const std::string& categoryName )
{
    s_startStopProfilers.push( new ProfileSection( categoryName ) );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Profiler::StopProfiling()
{
    delete s_startStopProfilers.back();
    s_startStopProfilers.pop();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Profiler::UpdateProfiler()
{
    for (std::map< std::string, ProfileData >::iterator categoryIter = s_profileDataMap.begin(); categoryIter != s_profileDataMap.end(); ++categoryIter)
    {
        ProfileData& data = categoryIter->second;
        data.UpdateTimes();

    }

    FATAL_ASSERT( s_startStopProfilers.empty() );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Profiler::PrintDataToConsole()
{
    for (std::map< std::string, ProfileData >::const_iterator categoryIter = s_profileDataMap.begin(); categoryIter != s_profileDataMap.end(); ++categoryIter)
    {
        const std::string& categoryName = categoryIter->first;
        const ProfileData& data = categoryIter->second;

        ConsolePrintf( "%s total: %lld ms, %lld us \n", categoryName.c_str(), data.m_totalTime_us / 1000LL, data.m_totalTime_us );
        ConsolePrintf( "%s average: %lld ms, %lld us \n", categoryName.c_str(), data.m_averageTime_us / 1000LL, data.m_averageTime_us );

    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Profiler::PrintDataToScreen( OpenGLRenderer* renderer )
{
    if (!renderer)
        return;

    FontRenderer* fontRenderer = renderer->GetFontRenderer();
    Font* font = Font::CreateOrGetFont( "Data/Fonts/Calibri" );

    Vector3 offset( 10.0f, 870.0f, 1.0f );
    float offsetModifier = -35.0f;
    int fontSize = 32;


    for (std::map< std::string, ProfileData >::const_iterator categoryIter = s_profileDataMap.begin(); categoryIter != s_profileDataMap.end(); ++categoryIter)
    {
        const std::string& categoryName = categoryIter->first;
        const ProfileData& data = categoryIter->second;

        std::string totalText = categoryName + " total: " + std::to_string( data.m_totalTime_us / 1000LL ) + " ms, " + std::to_string( data.m_totalTime_us ) + " us - average: " + std::to_string( data.m_averageTime_us / 1000LL ) + " ms, " + std::to_string( data.m_averageTime_us ) + " us";

        Vector3 pos = offset;
        fontRenderer->DrawFontTextOrtho( fontSize, *font, totalText, pos, Rgba::WHITE );
        offset.y += offsetModifier;
    }
}
