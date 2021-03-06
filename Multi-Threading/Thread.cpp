//=================================================================================
// Thread.cpp
// Author: Tyler George
// Date  : November 5, 2015
//=================================================================================


////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "Engine/Multi-Threading/Thread.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

////===========================================================================================
///===========================================================================================
// Defines
///===========================================================================================
////===========================================================================================
#define MS_VC_EXCEPTION ( 0x406d1388 )

////===========================================================================================
///===========================================================================================
// Local structs
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
#pragma pack(push, 8)
struct ThreadNameInfo
{
    DWORD type;				// must be 0x1000
    const char* name;		// name
    DWORD threadID;		    // -1 for calling thread
    DWORD flags;			// must be 0, reserved for future use
};
#pragma pack(pop)

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
struct StartThreadArgs
{
    ThreadEntryCallback* m_function;
    void* m_arg;

    bool m_done;
};

////===========================================================================================
///===========================================================================================
// Local Functions
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
static DWORD WINAPI ThreadEntryPoint( void* arg )
{
    StartThreadArgs* args = (StartThreadArgs*)arg;

    ThreadEntryCallback* funct = args->m_function;
    void* volatile parg = args->m_arg;

    // Tell the calling thread we're created so it can move on
    args->m_done = true;

    // args is no longer valid beyond this point
    funct( parg );
    return 0;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ThreadClassEntry( void* arg )
{
    Thread* thread = (Thread*)arg;
    thread->Run();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
static void SetThreadName( DWORD id, const char* name )
{
    if (0 != id) {
        ThreadNameInfo info;
        info.type = 0x1000;
        info.name = name;
        info.threadID = (DWORD)id;
        info.flags = 0;

        __try
        {
            RaiseException( MS_VC_EXCEPTION, 0, sizeof( info ) / sizeof( ULONG_PTR ), (ULONG_PTR*)(&info) );
        }
        __except (EXCEPTION_CONTINUE_EXECUTION)
        {
        }
    }
}

////===========================================================================================
///===========================================================================================
// Thread Class
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
Thread::Thread()
{
    m_threadHandle = NULL;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Thread::~Thread()
{

}

////===========================================================================================
///===========================================================================================
// Initialization
///===========================================================================================
////===========================================================================================
   
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool Thread::Start( const char* name )
{
    if (m_threadHandle == NULL) 
    {
        m_threadHandle = ThreadCreate( name, ThreadClassEntry, this );
        return m_threadHandle != NULL;
    }

    return false;
}


////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================
    
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Thread::Detach()
{
    if (m_threadHandle != NULL)
    {
        ThreadDetach( m_threadHandle );
        m_threadHandle = NULL;
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Thread::Join()
{
    if (m_threadHandle != NULL)
    {
        ThreadJoin( m_threadHandle );
        m_threadHandle = NULL;
    }
}


////===========================================================================================
///===========================================================================================
// Protected Mutators
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Thread::yield()
{
    ThreadYield();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Thread::Sleep( unsigned int ms )
{
    ThreadSleep( ms );
}


////===========================================================================================
///===========================================================================================
// External Functions
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void* ThreadCreate( const char* name, ThreadEntryCallback* entry, void* arg )
{
    StartThreadArgs args;
    args.m_function = entry;
    args.m_arg = arg;
    args.m_done = false;

    DWORD id;
    HANDLE threadHandle = ::CreateThread( NULL, 0, ThreadEntryPoint, &args, 0, &id );

    if ( threadHandle != NULL ) {
        if ( name != nullptr )	{
            SetThreadName( id, name );
        }

        while (!args.m_done) {
            ThreadYield();
        }
    }

    // Return the handle
    return (void*) threadHandle;
}

///---------------------------------------------------------------------------------
/// Does not stop the thread, just releases references
///---------------------------------------------------------------------------------
void ThreadDetach( void* handle )
{
    if (handle != NULL)
        ::CloseHandle( (HANDLE)handle );
}

///---------------------------------------------------------------------------------
/// Blocks calling thread until thread referenced by handle is finished
///---------------------------------------------------------------------------------
void ThreadJoin( void* handle )
{
    if (handle != NULL)
    {
        ::WaitForSingleObject( (HANDLE)handle, INFINITE );
        ::CloseHandle( (HANDLE)handle );
    }
}

///---------------------------------------------------------------------------------
/// Yields current thread to other threads
///---------------------------------------------------------------------------------
void ThreadYield()
{
    ::SwitchToThread();
}

///---------------------------------------------------------------------------------
/// Sleep the current thread for a number of milliseconds
///---------------------------------------------------------------------------------
void ThreadSleep( unsigned int ms )
{
    ::Sleep( (DWORD)ms );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned long ThreadGetCurrentID()
{
    return ::GetCurrentThreadId();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned long ThreadGetID( void* handle )
{
    return ::GetThreadId( (HANDLE) handle );
}

