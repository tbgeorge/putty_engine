//=================================================================================
// DeveloperConsoleCommands.cpp
// Author: Tyler George
// Date  : June 24, 2015
//=================================================================================

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Utilities/DeveloperConsoleCommands.hpp"
#include "Engine/Utilities/Utilities.hpp"
#include "Engine/Utilities/Console.hpp"
#include "DeveloperConsole.hpp"

////===========================================================================================
///===========================================================================================
// Static Variables
///===========================================================================================
////===========================================================================================

static char const COMMAND_INDICATOR = '-';
static char const QUOTE = '\"';
static char const ESCAPE = '\\';

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Developer Console Arguments
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//===========================================================================================
//===========================================================================================
// Constructors/Destructors
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
/// 
///---------------------------------------------------------------------------------
DeveloperConsoleArguments::DeveloperConsoleArguments( const std::vector<std::string>& argsList, int numArgsToSkip )
{
    m_currentIndex = 0;
    m_args = std::vector<std::string>( argsList.begin() + numArgsToSkip, argsList.end() );
}

//===========================================================================================
//===========================================================================================
// Mutators
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool DeveloperConsoleArguments::GetString( std::string& string )
{
    // TODO: Change this function to find opening and closing quotations to denote a string

    if (PeekString( string ))
    {
        m_currentIndex++;
        return true;
    }

    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool DeveloperConsoleArguments::GetInt( int& integer )
{
    std::string string = "";
    if (PeekString( string ))
    {
        if (IsInt( string ))
        {
            integer = strtol( string.c_str(), nullptr, 10 );
            m_currentIndex++;
            return true;
        }

        return false;
    }
    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool DeveloperConsoleArguments::GetDouble( double& doub )
{
    std::string string = "";
    if (PeekString( string ))
    {
        if (IsDouble( string ))
        {
            doub = strtod( string.c_str(), nullptr );
            m_currentIndex++;
            return true;
        }

        return false;
    }
    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool DeveloperConsoleArguments::GetFloat( float& flt )
{
    std::string string = "";
    if (PeekString( string ))
    {
        if (IsFloat( string ))
        {
            flt = strtof( string.c_str(), nullptr );
            m_currentIndex++;
            return true;
        }

        return false;
    }
    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool DeveloperConsoleArguments::GetBool( bool& boolean )
{
    std::string string = "";
    if (PeekString( string ))
    {
        string = ConvertToLowerCase( string );
        if (string == "true")
            boolean = true;
        else if (string == "false")
            boolean = false;

        m_currentIndex++;
        return true;
    }

    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool DeveloperConsoleArguments::GetColorFromInts( Rgba& color )
{
    std::string string = "";


    if (PeekString( string ))
    {
        if (IsInt( string ))
        {
            color.r = (char)strtol( string.c_str(), nullptr, 10 );
            m_currentIndex++;
        }
        else
            return false;
    }
    if (PeekString( string ))
    {
        if (IsInt( string ))
        {
            color.g = (char)strtol( string.c_str(), nullptr, 10 );
            m_currentIndex++;
        }
        else
            return false;
    }

    if (PeekString( string ))
    {
        if (IsInt( string ))
        {
            color.b = (char)strtol( string.c_str(), nullptr, 10 );
            m_currentIndex++;
        }
        else
            return false;
    }

    if (PeekString( string ))
    {
        if (IsInt( string ))
        {
            color.a = (char)strtol( string.c_str(), nullptr, 10 );
            m_currentIndex++;
            return true;
        }
        else
            return false;
    }
    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool DeveloperConsoleArguments::GetColorFromString( Rgba& color )
{
    std::string string = "";
    if (PeekString( string ))
    {
        string = ConvertToLowerCase( string );
        if (string == "red")
            color = Rgba::RED;

        else if (string == "blue")
            color = Rgba::BLUE;

        else if (string == "green")
            color = Rgba::GREEN;

        else if (string == "black")
            color = Rgba::BLACK;

        else if (string == "white")
            color = Rgba::WHITE;

        else if (string == "aqua")
            color = Rgba::AQUA;

        else if (string == "orange")
            color = Rgba::ORANGE;

        else if (string == "yellow")
            color = Rgba::YELLOW;

        else if (string == "grey")
            color = Rgba::GREY;

        else if (string == "magenta")
            color = Rgba::MAGENTA;

        else
            return false;

        m_currentIndex++;
        return true;
    }

    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool DeveloperConsoleArguments::GetColorFromHex( Rgba& color )
{
    std::string string = "";
    if (PeekString( string ))
    {
        if (IsHexInt( string ))
        {
            unsigned int colorVal = strtoul( string.c_str(), nullptr, 0 );
            color = Rgba( colorVal );
            m_currentIndex++;
            return true;
        }

        else
            return false;
    }

    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool DeveloperConsoleArguments::GetVector2( Vector2& vec2 )
{
    std::string string = "";
    if (PeekString( string ))
    {
        if (IsFloat( string ))
        {
            vec2.x = strtof( string.c_str(), nullptr );
            m_currentIndex++;
        }
        else
            return false;
    }

    if (PeekString( string ))
    {
        if (IsFloat( string ))
        {
            vec2.y = strtof( string.c_str(), nullptr );
            m_currentIndex++;
            return true;
        }
        else
            return false;
    }
    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool DeveloperConsoleArguments::GetVector3( Vector3& vec3 )
{
    std::string string = "";
    if (PeekString( string ))
    {
        if (IsFloat( string ))
        {
            vec3.x = strtof( string.c_str(), nullptr );
            m_currentIndex++;
        }
        else
            return false;
    }

    if (PeekString( string ))
    {
        if (IsFloat( string ))
        {
            vec3.y = strtof( string.c_str(), nullptr );
            m_currentIndex++;
        }
        else
            return false;
    }

    if (PeekString( string ))
    {
        if (IsFloat( string ))
        {
            vec3.z = strtof( string.c_str(), nullptr );
            m_currentIndex++;
            return true;
        }
        else
            return false;
    }
    return false;
}

//===========================================================================================
//===========================================================================================
//  Private Functions
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool DeveloperConsoleArguments::PeekString( std::string& string )
{
    if (m_currentIndex >= m_args.size())
        return false;

    string = m_args[m_currentIndex];
    return true;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Developer Console Command
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//===========================================================================================
//===========================================================================================
// Static Member Variable Initialization
//===========================================================================================
//===========================================================================================
CommandMap* DeveloperConsoleCommand::s_allCommands = nullptr;

//===========================================================================================
//===========================================================================================
// Constuctors/Destructors
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------    
DeveloperConsoleCommand::DeveloperConsoleCommand( std::string name, DeveloperConsoleCommandFunc command, std::string helpMessage )
{
    name = ConvertToLowerCase( name );
    m_name = name;
    m_command = command;
    m_help = helpMessage;

    if (!s_allCommands)
        s_allCommands = new std::map < std::string, DeveloperConsoleCommand* >();

    if (s_allCommands->find( name ) != s_allCommands->end())
    {
        ConsolePrintf( "Duplicate command registered: %s \n", name );
        return;
    }

    s_allCommands->insert( std::pair< std::string, DeveloperConsoleCommand*>( name, (this) ) );
}

////===========================================================================================
///===========================================================================================
// 
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void DeveloperConsoleCommand::Shutdown()
{
//     for (std::map < std::string, DeveloperConsoleCommand* >::iterator commandIter = s_allCommands->begin(); commandIter != s_allCommands->end();)
//     {
//         DeveloperConsoleCommand* command = commandIter->second;
//         commandIter = s_allCommands->erase( commandIter );
//         delete command;
//     }

    delete s_allCommands;
}

//===========================================================================================
//===========================================================================================
// Non-Static Accessors/Queries
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
std::string DeveloperConsoleCommand::GetHelpMessage()
{
    return m_help;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
std::string DeveloperConsoleCommand::GetName()
{
    return m_name;
}

//===========================================================================================
//===========================================================================================
// Static Accessors/Queries
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
DeveloperConsoleCommand* DeveloperConsoleCommand::GetCommand( const std::string& name )
{
    std::string nameLC = ConvertToLowerCase( name );

    if (s_allCommands->find( nameLC ) != s_allCommands->end())
    {
        return s_allCommands->at( nameLC );
    }

    return nullptr;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
CommandMap* DeveloperConsoleCommand::GetAllCommands()
{
    return s_allCommands;
}

//===========================================================================================
//===========================================================================================
// Execution Functions
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void DeveloperConsoleCommand::RunCommand( DeveloperConsoleArguments *args )
{
    m_command( args );
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// General
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//===========================================================================================
//===========================================================================================
// Execution Functions
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
ConsoleCommands ParseCommands( const std::string& commandLine )
{
    size_t length = commandLine.size();
    char *cmdLine = new char[length + 1];
    memcpy( cmdLine, commandLine.c_str(), length );
    cmdLine[length] = NULL;

    ConsoleCommand* command = nullptr;
    ConsoleCommands commands;


    char* pos = cmdLine;
    char* token = GetNextToken( &pos );

    while (token != nullptr) {
        if (IsTokenCommand( token )) 
        {
            if (command != nullptr) 
                commands.push_back( command );
            
            // take the '-' off the command name
            command = new ConsoleCommand( ++token );
        }
        else if ((command != nullptr) && !IsTokenNullOrEmpty( token )) 
            command->m_args.push_back( std::string( token ) );

        token = GetNextToken( &pos );
    }

    delete[] cmdLine;

    // push last command into list
    if (command != nullptr) 
        commands.push_back( command );

    return commands;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ProcessCommands( ConsoleCommands commands )
{
    for (ConsoleCommands::iterator commandIter = commands.begin(); commandIter != commands.end(); ++commandIter)
    {
        ConsoleCommand* command = *commandIter;
        DeveloperConsoleArguments args( command->m_args, 0 );

        command->m_command->RunCommand( &args );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void FreeCommands( ConsoleCommands commands )
{
    for (ConsoleCommands::iterator commandIter = commands.begin(); commandIter != commands.end(); ++commandIter)
    {
        delete *commandIter;
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool RunConsoleCommand( const std::string& name, DeveloperConsoleArguments* args )
{
    DeveloperConsoleCommand* command = DeveloperConsoleCommand::GetCommand( name );
    if (nullptr == command) {
        return false;
    }

    command->RunCommand( args );
    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool RunConsoleCommandFromLine( const std::string& line )
{
    std::string lineLC = ConvertToLowerCase( line );

    std::vector< std::string> args;
    Tokenize( lineLC, args, "( ,;)" );

    DeveloperConsoleArguments argsObj( args, 1 );
    DeveloperConsole::WriteLine( line, INFO_TEXT_COLOR );
    return RunConsoleCommand( args[0], &argsObj );
}

////===========================================================================================
///===========================================================================================
// Helper functions
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
char* GetNextToken( char** line )
{
    char *pos = *line;
    char *token = nullptr;

    // get up the first real character
    while (IsWhitespace( *pos )) {
        ++pos;
    }

    if (*pos != NULL) {
        token = pos;
        char *out = pos;

        if (*pos == QUOTE) {
            GetNextString( &out, &pos );
        }
        else {
            while (!IsWhitespace( *pos ) && (*pos != NULL)) {
                char nextChar = GetNextCharacter( &pos );
                *out = nextChar;
                ++out;
                ++pos;
            }
        }

        // move past the possible "last position" we left on
        // since we're NULLing it out for our token
        if (*pos != NULL) {
            ++pos;
        }
        *out = NULL;
    }

    *line = pos;
    return token;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsTokenNullOrEmpty( const char* token )
{
    return (token == nullptr) || (*token == NULL);
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsTokenCommand( const char* token )
{
    if (token == nullptr) 
        return false;

    return ((*token == COMMAND_INDICATOR) && (*(token + 1) != NULL) && (!IsWhitespace( *(token + 1) )));
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void GetNextString( char** out_string, char** line )
{
    char *pos = *line;
    char *out = *out_string;

    // nothing to do, doesn't start with a quote;
    if (*pos != QUOTE) {
        return;
    }

    ++pos;
    while ((*pos != NULL) && (*pos != QUOTE)) {
        char nextChar = GetNextCharacter( &pos );
        *out = nextChar;
        ++out;
        ++pos;
    }

    if ((*pos) == QUOTE) {
        ++pos;
    }

    *out_string = out;
    *line = pos;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
char GetNextCharacter( char **line )
{
    char *pos = *line;
    char nextChar = *pos;

    if (nextChar == ESCAPE) {
        ++pos;
        switch (*pos) {
        case 'n':
            nextChar = '\n';
            break;
        case 'r':
            nextChar = '\r';
            break;
        case '\\':
            nextChar = '\\';
            break;
        case 't':
            nextChar = '\t';
            break;
        case '\'':
            nextChar = '\'';
            break;
        case '\"':
            nextChar = '\"';
            break;
        case NULL:
            nextChar = ESCAPE;
            --pos; // go back one
            break;
        default:
            nextChar = *pos;
        }
    }

    *line = pos;
    return nextChar;
}