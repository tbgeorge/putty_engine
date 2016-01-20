//=================================================================================
// Error.cpp
// Author: Tyler George
// Date  : October 29, 2015
//=================================================================================


////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================

#include "Engine/Utilities/Error.hpp"
#include "Console.hpp"
// #include "../../SoundSystem.hpp"


////===========================================================================================
///===========================================================================================
// Error Functions
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
/// 
///---------------------------------------------------------------------------------
void FatalError( const char* fileName, const int& lineNum )
{
    MessageBoxAf( MBT_OK, MBI_ERROR, "Fatal Error", "Fatal error in %s(%d) \n", fileName, lineNum );
//     s_theSoundSystem->Shutdown();
    exit( EXIT_FAILURE );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void RecoverableError( const char* fileName, const int& lineNum )
{
    MessageBoxReturnValue userChoice = MessageBoxAf( MBT_YESNO, MBI_WARNING, "Recoverable Error", "Recoverable error in %s(%d) \n Click 'Yes' to debug, 'No' to continue execution \n", fileName, lineNum );
    if (userChoice == MBR_YES)
        __debugbreak();

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void FatalAssert( const char* fileName, const int& lineNum, bool eval, const char* evaluation ) 
{
    if (!eval)
    {
        MessageBoxAf( MBT_OK, MBI_ERROR, "Fatal Assert", "Fatal assert in %s(%d) \n\n %s has failed. \n", fileName, lineNum, evaluation );
//         s_theSoundSystem->Shutdown();
        exit( EXIT_FAILURE );

    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void RecoverableAssert( const char* fileName, const int& lineNum, bool eval, const char* evaluation )
{
    if (!eval)
    {
        MessageBoxReturnValue userChoice = MessageBoxAf( MBT_YESNO, MBI_WARNING, "Recoverable Assert", "Recoverable assert in %s(%d) \n\n %s has failed. \n\n Click 'Yes' to debug, 'No' to continue execution \n", fileName, lineNum, evaluation );
        if (userChoice == MBR_YES)
            __debugbreak();

    }
}