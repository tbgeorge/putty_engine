//=================================================================================
// Time.hpp
// Author: Tyler George
// Date  : January 27, 2015
//=================================================================================

#pragma once

#ifndef __includede_Time__
#define __includede_Time__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include <inttypes.h>
#include <vector>
#include <map>
#include "Engine/Common/EngineCommon.hpp"


///---------------------------------------------------------------------------------
/// Time Utility Functions
///---------------------------------------------------------------------------------
void InitializeTimeSystem();
double GetCurrentSeconds();

uint64_t TimeGetOpCount();
long long TimeGet_ms(); // milliseconds
long long TimeGet_us(); // microseconds

uint64_t TimeOpCountTo_us( uint64_t opCount );
float TimeOpCountTo_ms( uint64_t opCount );

///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------
typedef void (AlarmCallback) (const void* const arg);


///---------------------------------------------------------------------------------
/// Alarm Struct
///---------------------------------------------------------------------------------
struct Alarm
{
    Alarm()
        : m_alarmName("alarm"), m_lengthSeconds(0.0), m_currentElapsedSeconds(0.0), m_callbackFunc(nullptr), m_data(nullptr) {}

    Alarm( const std::string& name, double lengthSeconds, AlarmCallback* cb, void* const data )
        : m_alarmName( name ), m_lengthSeconds( lengthSeconds ), m_callbackFunc( cb ), m_data( data ), m_currentElapsedSeconds( 0.0 ) {}


    std::string m_alarmName;
    AlarmCallback* m_callbackFunc;
    void* m_data;

    double m_currentElapsedSeconds;
    double m_lengthSeconds;
};

typedef std::map< std::string, Alarm* > AlarmMap;

///---------------------------------------------------------------------------------
/// Clock class
///---------------------------------------------------------------------------------
class Clock;
typedef std::map< unsigned int, Clock* > ClockMap;

class Clock
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    Clock( Clock* parent, double maxDeltaSeconds );
    ~Clock();

    ///---------------------------------------------------------------------------------
    /// Initialization
    ///---------------------------------------------------------------------------------
    static void InitializeMasterClock();
    static void Shutdown();

    ///---------------------------------------------------------------------------------
    /// Accessors/Queries
    ///---------------------------------------------------------------------------------
    static Clock* GetMasterClock() { return s_masterClock; }
    double GetCurrentSeconds() const { return m_currentSeconds; }
    double GetLastDeltaSeconds() const { return m_lastDeltaSeconds; }
    float GetTimeScale() const { return m_timescale; }
    bool IsPaused() const { return m_isPaused; }

    float GetAlarmPercentElapsed( const std::string& alarmName );
    float GetAlarmPercentRemaining( const std::string& alarmName );
    double GetAlarmSecondsRemaining( const std::string& alarmName );
    double GetAlarmSecondsElapsed( const std::string& alarmName );

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    void AddChild( Clock* child );
    void SetTimeScale( float timescale ) { m_timescale = timescale; }
    void Pause() { m_isPaused = true; }
    void UnPause() { m_isPaused = false; }

    void SetAlarm( const std::string& alarmName, double lengthSeconds, AlarmCallback* cb, void* const data );

    ///---------------------------------------------------------------------------------
    /// Update
    ///---------------------------------------------------------------------------------
    static void Update( double deltaseconds );
    void AdvanceTime( double deltaseconds );
    void UpdateAlarms();

private:

    ///---------------------------------------------------------------------------------
    /// Private functions
    ///---------------------------------------------------------------------------------
    Clock();

    ///---------------------------------------------------------------------------------
    /// Private variables
    ///---------------------------------------------------------------------------------
    static Clock* s_masterClock;
    static unsigned int s_nextClockID;

    unsigned int m_clockID;

    ClockMap m_children;
    Clock* m_parent;
    
    double m_currentSeconds; // seconds
    double m_lastDeltaSeconds; // seconds
    double m_maxDeltaSeconds; // seconds; value of -1 means no max
    float m_timescale;
    bool m_isPaused;

    AlarmMap m_alarms;
};

///---------------------------------------------------------------------------------
/// Timer class
///---------------------------------------------------------------------------------
class Timer
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    Timer( Clock* referenceClock );
    ~Timer();

    ///---------------------------------------------------------------------------------
    /// Accessors
    ///---------------------------------------------------------------------------------
    double GetElapsedTimeSeconds() { return m_elapsedTime; }

    ///---------------------------------------------------------------------------------
    /// Update
    ///---------------------------------------------------------------------------------
    static void UpdateAllTimers();
    void Update();

private:
    ///---------------------------------------------------------------------------------
    /// Private member variables
    ///---------------------------------------------------------------------------------
    Clock* m_referenceClock;
    double m_elapsedTime; // seconds
    int m_timerID;

    static std::map< unsigned int, Timer* > s_allTimers;
    static unsigned int s_nextTimerID;
};

#endif