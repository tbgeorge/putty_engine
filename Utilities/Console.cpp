//=================================================================================
// Console.cpp
// Author: Tyler George
// Date  : February 17, 2015
//=================================================================================

#include "Engine/Utilities/Console.hpp"
#include "Engine/Common/EngineCommon.hpp"
#include <stdio.h>
#include <stdarg.h>
#define WIN_32_LEAN_AND_MEAN
#include <Windows.h>
#include "FileUtilities.hpp"
#include "DeveloperConsole.hpp"

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ConsolePrintf( const char* messageFormat, ... )
{
	static const size_t BUFFER_SIZE = 2048;
	static char message[ BUFFER_SIZE ];

	va_list argumentList;
	va_start( argumentList, messageFormat );

	_vsnprintf_s( message, BUFFER_SIZE, BUFFER_SIZE - 1,  messageFormat, argumentList );

	va_end( argumentList );

	// windows specific
	OutputDebugStringA( message );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
MessageBoxReturnValue MessageBoxAf( MessageBoxType type, MessageBoxIcon icon, const char* title, const char* messageFormat, ... )
{
    static const size_t BUFFER_SIZE = 2048;
    static char message[BUFFER_SIZE];

    va_list argumentList;
    va_start( argumentList, messageFormat );

    _vsnprintf_s( message, BUFFER_SIZE, BUFFER_SIZE - 1, messageFormat, argumentList );

    va_end( argumentList );

    int boxReturnValue = MB_OK;

    long boxIcon = 0L;

    switch (icon)
    {
    case MBI_EXCLAMATION:
        boxIcon = MB_ICONEXCLAMATION;
        break;
    case MBI_WARNING:
        boxIcon = MB_ICONWARNING;
        break;
    case MBI_INFORMATION:
        boxIcon = MB_ICONINFORMATION;
        break;
    case MBI_ASTERISK:
        boxIcon = MB_ICONASTERISK;
        break;
    case MBI_QUESTION:
        boxIcon = MB_ICONQUESTION;
        break;
    case MBI_STOP:
        boxIcon = MB_ICONSTOP;
        break;
    case MBI_ERROR:
        boxIcon = MB_ICONERROR;
        break;
    case MBI_HAND:
        boxIcon = MB_ICONHAND;
        break;
    default:
        break;
    }

    switch (type)
    {
    case MBT_OK:
        boxReturnValue = MessageBoxA( NULL, message, title, MB_OK | boxIcon );
        break;

    case MBT_ABORTRETRYIGNORE:
        boxReturnValue = MessageBoxA( NULL, message, title, MBT_ABORTRETRYIGNORE | boxIcon );
        break;

    case MBT_CANCELTRYCONTINUE:
        boxReturnValue = MessageBoxA( NULL, message, title, MBT_CANCELTRYCONTINUE | boxIcon );
        break;

    case MBT_HELP:
        boxReturnValue = MessageBoxA( NULL, message, title, MB_HELP | boxIcon );
        break;

    case MBT_OKCANCEL:
        boxReturnValue = MessageBoxA( NULL, message, title, MB_OKCANCEL | boxIcon );
        break;

    case MBT_RETRYCANCEL:
        boxReturnValue = MessageBoxA( NULL, message, title, MB_RETRYCANCEL | boxIcon );
        break;

    case MBT_YESNO:
        boxReturnValue = MessageBoxA( NULL, message, title, MB_YESNO | boxIcon );
        break;

    case MBT_YESNOCANCEL:
        boxReturnValue = MessageBoxA( NULL, message, title, MB_YESNOCANCEL | boxIcon );
        break;

    }
    // windows specific

    switch (boxReturnValue)
    {
    case IDOK:
        return MBR_OK;  
    case IDABORT:
        return MBR_ABORT;
    case IDRETRY:
        return MBR_RETRY;
    case IDIGNORE:
        return MBR_IGNORE;
    case IDCANCEL:
        return MBR_CANCEL;
    case IDCONTINUE:
        return MBR_CONTINUE;
    case IDNO:
        return MBR_NO;
    case IDTRYAGAIN:
        return MBR_TRYAGAIN;
    case IDYES:
        return MBR_YES;
    }

    return MBR_NULL;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
CONSOLE_COMMAND( generateFiles, "args: int numFiles, int fileSize (in KB) | desc: Generates a number of files of a specified size filled with garbage." )
{
    int numFiles = 0;
    args->GetInt( numFiles );
    int fileSize = 0;
    args->GetInt( fileSize );

    GenerateFiles( numFiles, fileSize );

    DeveloperConsole::WriteLine( "Generated " + std::to_string( numFiles ) + " files of size " + std::to_string( fileSize ) + "kb.", INFO_TEXT_COLOR );
    g_shouldContinueExecution = false;
}