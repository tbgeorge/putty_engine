//=================================================================================
// Console.hpp
// Author: Tyler George
// Date  : February 17, 2015
//=================================================================================

#pragma once
#ifndef __included_Console__
#define __included_Console__

#include "Engine/Common/EngineCommon.hpp"

enum MessageBoxType
{
    MBT_OK,
    MBT_ABORTRETRYIGNORE,
    MBT_CANCELTRYCONTINUE,
    MBT_HELP,
    MBT_OKCANCEL,
    MBT_RETRYCANCEL,
    MBT_YESNO,
    MBT_YESNOCANCEL

};

enum MessageBoxReturnValue
{
    MBR_ABORT,
    MBR_RETRY,
    MBR_IGNORE,
    MBR_OK,
    MBR_CANCEL,
    MBR_CONTINUE,
    MBR_NO,
    MBR_TRYAGAIN,
    MBR_YES,
    MBR_NULL

};

enum MessageBoxIcon
{
    MBI_EXCLAMATION,
    MBI_WARNING,
    MBI_INFORMATION,
    MBI_ASTERISK,
    MBI_QUESTION,
    MBI_STOP,
    MBI_ERROR,
    MBI_HAND
};

void ConsolePrintf( const char* messageFormat, ... );
MessageBoxReturnValue MessageBoxAf( MessageBoxType type, MessageBoxIcon icon, const char* title, const char* messageFormat, ... );

#endif