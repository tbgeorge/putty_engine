//=================================================================================
// DeveloperConsoleCommands.hpp
// Author: Tyler George
// Date  : June 24, 2015
//=================================================================================

#pragma once 

#ifndef __included_DeveloperConsoleCommands__
#define __included_DeveloperConsoleCommands__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include <vector>
#include <string>
#include <map>
#include "Engine/Utilities/Rgba.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"


///---------------------------------------------------------------------------------
/// Defines
///---------------------------------------------------------------------------------
#define CONSOLE_COMMAND(name, helpMessage)  static void DeveloperConsoleCommand_##name##( DeveloperConsoleArguments* args ); \
                                            static DeveloperConsoleCommand command_##name##(#name, DeveloperConsoleCommand_##name##, ##helpMessage##); \
                                            static void DeveloperConsoleCommand_##name##(DeveloperConsoleArguments* args)  



///---------------------------------------------------------------------------------
/// Developer Console Arguments
///---------------------------------------------------------------------------------
class DeveloperConsoleArguments
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    DeveloperConsoleArguments( const std::vector<std::string>& argsList, int numArgsToSkip );

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    bool GetString( std::string& string );
    bool GetInt( int& integer );
    bool GetDouble( double& doub );
    bool GetFloat( float& flt );
    bool GetBool( bool& boolean );
    bool GetColorFromInts( Rgba& color );
    bool GetColorFromString( Rgba& color );
    bool GetColorFromHex( Rgba& color );
    bool GetVector2( Vector2& vec2 );
    bool GetVector3( Vector3& vec3 );

private:
    ///---------------------------------------------------------------------------------
    /// Private functions
    ///---------------------------------------------------------------------------------
    bool PeekString( std::string& string );

    ///---------------------------------------------------------------------------------
    /// Private Member Variables
    ///---------------------------------------------------------------------------------
    unsigned int m_currentIndex;
    std::vector< std::string> m_args;

};

typedef void( *DeveloperConsoleCommandFunc )(DeveloperConsoleArguments *args);

class DeveloperConsoleCommand;

typedef std::map<std::string, DeveloperConsoleCommand*> CommandMap;

///---------------------------------------------------------------------------------
/// Developer Console Command
///---------------------------------------------------------------------------------
class DeveloperConsoleCommand
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    DeveloperConsoleCommand( std::string name, DeveloperConsoleCommandFunc command, std::string helpMessage );

    ///---------------------------------------------------------------------------------
    /// Initialization
    ///---------------------------------------------------------------------------------
    static void Shutdown();

    ///---------------------------------------------------------------------------------
    /// Non-Static Accessors/Queries
    ///---------------------------------------------------------------------------------
    std::string GetHelpMessage();
    std::string GetName(); 
    
    ///---------------------------------------------------------------------------------
    /// Static Accessors/Queries
    ///---------------------------------------------------------------------------------
    static DeveloperConsoleCommand* GetCommand( const std::string& name );
    static CommandMap* GetAllCommands();

    ///---------------------------------------------------------------------------------
    /// Execution Functions
    ///---------------------------------------------------------------------------------
    void RunCommand( DeveloperConsoleArguments *args );

private:
    ///---------------------------------------------------------------------------------
    /// Private Member Variables
    ///---------------------------------------------------------------------------------
    std::string m_name;
    DeveloperConsoleCommandFunc m_command;
    std::string m_help;

    ///---------------------------------------------------------------------------------
    /// Private Static Member Variables
    ///---------------------------------------------------------------------------------
    static CommandMap* s_allCommands;
};

///---------------------------------------------------------------------------------
/// Structs
///---------------------------------------------------------------------------------
struct ConsoleCommand
{
    ConsoleCommand( char* commandName )
        : m_command( nullptr )
    {
        std::string command( commandName );
        m_command = DeveloperConsoleCommand::GetCommand( command );
    }

    DeveloperConsoleCommand* m_command;
    std::vector<std::string> m_args;
};

typedef std::vector< ConsoleCommand* > ConsoleCommands;

///---------------------------------------------------------------------------------
/// Execution Functions
///---------------------------------------------------------------------------------  
ConsoleCommands ParseCommands( const std::string& commandLine );
void ProcessCommands( ConsoleCommands commands );
void FreeCommands( ConsoleCommands commands );

bool RunConsoleCommand( const std::string& name, DeveloperConsoleArguments* args );
bool RunConsoleCommandFromLine( const std::string& line );

///---------------------------------------------------------------------------------
/// Helper Functions
///---------------------------------------------------------------------------------
char* GetNextToken( char** stream );
bool IsTokenNullOrEmpty( const char* token );
bool IsTokenCommand( const char* token );
void GetNextString( char** out_stream, char** stream );
char GetNextCharacter( char **stream );


#endif // DeveloperConsoleCommands