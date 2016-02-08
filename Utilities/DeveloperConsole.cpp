//=================================================================================
// DeveloperConsole.cpp
// Author: Tyler George
// Date  : June 10, 2015
//=================================================================================

//===========================================================================================
//===========================================================================================
// Includes
//===========================================================================================
//===========================================================================================
#include "Engine/Utilities/DeveloperConsole.hpp"
#include "Engine/Utilities/Time.hpp"

//===========================================================================================
//===========================================================================================
// Static Variable Initialization
//===========================================================================================
//===========================================================================================
DeveloperConsole* DeveloperConsole::s_theDeveloperConsole = nullptr;

//===========================================================================================
//===========================================================================================
// Public Constructors/Destructors
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
DeveloperConsole::~DeveloperConsole()
{
}

//===========================================================================================
//===========================================================================================
// Private Constructors/Destructors
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
DeveloperConsole::DeveloperConsole( OpenGLRenderer* renderer, GraphicsDebugger* graphicsDebugger )
    : m_renderer( renderer )
    , m_fontRenderer( renderer->GetFontRenderer() )
    , m_graphicsDebugger( graphicsDebugger )
    , m_font( nullptr )
    , m_currentLine( "" )
    , m_historyIndex( -1 )
    , m_currentLineCursorIndex( 0 )
    , m_bottomLogLineIndex( 0 )
    , m_enabled( false )
    , m_isReadyToQuit( false )
    , m_textShadowEnabled( false )
    , m_textShadowColor( Rgba::BLACK )
{
    StartUp();
}

//===========================================================================================
//===========================================================================================
// Private Initialization
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void DeveloperConsole::StartUp()
{
    m_shaderID = OpenGLRenderer::CreateOrGetShader( "Data/Shaders/devConsole" );
    m_renderer->GLCheckError();

    m_fontShaderID = OpenGLRenderer::CreateOrGetShader( "Data/Shaders/textured" );
    m_renderer->GLCheckError();

    m_font = Font::CreateOrGetFont( "Data/Fonts/Calibri" );

    m_currentLineCursorIndex = 0;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void DeveloperConsole::Shutdown()
{
    delete s_theDeveloperConsole;
}

//===========================================================================================
//===========================================================================================
// Public Static Accessors/Queries
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
DeveloperConsole* DeveloperConsole::GetInstance( OpenGLRenderer* renderer, GraphicsDebugger* graphicsDebugger  )
{
    if (!s_theDeveloperConsole)
        s_theDeveloperConsole = new DeveloperConsole( renderer, graphicsDebugger );
    
    return s_theDeveloperConsole;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Rgba DeveloperConsole::GetTextColor()
{
    if (!s_theDeveloperConsole)
        return Rgba();

    return s_theDeveloperConsole->m_textColor;
}

//===========================================================================================
//===========================================================================================
// Private Static Accessors/Queries
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Vector3 DeveloperConsole::GetCursorPos( const unsigned int& fontHeight ) 
{
    if (!s_theDeveloperConsole)
        return Vector3::ZERO;

    Vector3 cursorPos = Vector3::ZERO;

    cursorPos.x = s_theDeveloperConsole->m_fontRenderer->CalcTextWidth( fontHeight, 
                                                                        *s_theDeveloperConsole->m_font, 
                                                                        s_theDeveloperConsole->m_currentLine, 
                                                                        s_theDeveloperConsole->m_currentLineCursorIndex 
                                                                      );

    return cursorPos;
 
}

//===========================================================================================
//===========================================================================================
// Non-Static Mutators
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void DeveloperConsole::SetEnabled()
{
    if (!s_theDeveloperConsole)
        return;

    s_theDeveloperConsole->m_enabled = true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void DeveloperConsole::SetDisabled()
{
    if (!s_theDeveloperConsole)
        return;

    s_theDeveloperConsole->m_enabled = false;
    s_theDeveloperConsole->m_historyIndex = -1;
    s_theDeveloperConsole->m_currentLine = "";
    s_theDeveloperConsole->m_currentLineCursorIndex = 0;
    s_theDeveloperConsole->m_isReadyToQuit = false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool DeveloperConsole::IsEnabled()
{
    if (!s_theDeveloperConsole)
        return false;

    return s_theDeveloperConsole->m_enabled;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool DeveloperConsole::IsReadyToDisable()
{
    if (!s_theDeveloperConsole)
        return false;

    return s_theDeveloperConsole->m_isReadyToQuit;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void DeveloperConsole::TextShadowEnabled( bool enabled )
{
    if (!s_theDeveloperConsole)
        return;

    s_theDeveloperConsole->m_textShadowEnabled = enabled;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void DeveloperConsole::SetTextShadowColor( const Rgba& color )
{
    if (!s_theDeveloperConsole)
        return;

    s_theDeveloperConsole->m_textShadowColor = color;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void DeveloperConsole::SetTextColor( const Rgba& color )
{
    if (!s_theDeveloperConsole)
        return;

    s_theDeveloperConsole->m_textColor = color;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void DeveloperConsole::ClearLog()
{
    if (!s_theDeveloperConsole)
        return;

    s_theDeveloperConsole->m_log.clear();
    s_theDeveloperConsole->m_bottomLogLineIndex = 0;

    WriteLine( "Log Cleared", GetTextColor() );
}

//===========================================================================================
//===========================================================================================
// Static Mutators
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void DeveloperConsole::WriteLine( const std::string& text, const Rgba& color )
{
    if (!s_theDeveloperConsole)
    {
        ConsolePrintf( text.c_str() );
        return;
    }

    Line newLine;
    newLine.text = text;
    newLine.color = color;

    s_theDeveloperConsole->m_log.insert( s_theDeveloperConsole->m_log.begin(), newLine );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void DeveloperConsole::WriteLine( const Rgba& color, const char* messageFormat, ... )
{
    static const size_t BUFFER_SIZE = 2048;
    static char message[BUFFER_SIZE];

    va_list argumentList;
    va_start( argumentList, messageFormat );

    _vsnprintf_s( message, BUFFER_SIZE, BUFFER_SIZE - 1, messageFormat, argumentList );

    va_end( argumentList );

    if (!s_theDeveloperConsole)
    {
        ConsolePrintf( message );
        return;
    }

    Line newLine;
    newLine.text = std::string( message );
    newLine.color = color;

    s_theDeveloperConsole->m_log.insert( s_theDeveloperConsole->m_log.begin(), newLine );
}

//===========================================================================================
//===========================================================================================
// Process Input
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
/// Needs to be refactored to allow the holding of keys --REFACTOR--
///---------------------------------------------------------------------------------
void DeveloperConsole::ProcessInput( InputSystem* inputSystem )
{
    if (!s_theDeveloperConsole)
        return;

    std::string& currentLine = s_theDeveloperConsole->m_currentLine;
    int& currentLineCursorIndex = s_theDeveloperConsole->m_currentLineCursorIndex;
    int& historyIndex = s_theDeveloperConsole->m_historyIndex;
    Strings& commands = s_theDeveloperConsole->m_commands;
    Lines& log = s_theDeveloperConsole->m_log;
    int& bottomLogLineIndex = s_theDeveloperConsole->m_bottomLogLineIndex;
    bool& isReadyToQuit = s_theDeveloperConsole->m_isReadyToQuit;

    if (inputSystem->WasKeyJustReleased( VK_SPACE ))
        currentLine.insert( currentLineCursorIndex++, " " );

    if (inputSystem->IsCapsLockEnabled())
    {
        // letters
        if (inputSystem->WasKeyJustReleased( 'A' ))
            currentLine.insert( currentLineCursorIndex++, "A" );
        if (inputSystem->WasKeyJustReleased( 'B' ))
            currentLine.insert( currentLineCursorIndex++, "B" );
        if (inputSystem->WasKeyJustReleased( 'C' ))
            currentLine.insert( currentLineCursorIndex++, "C" );
        if (inputSystem->WasKeyJustReleased( 'D' ))
            currentLine.insert( currentLineCursorIndex++, "D" );
        if (inputSystem->WasKeyJustReleased( 'E' ))
            currentLine.insert( currentLineCursorIndex++, "E" );
        if (inputSystem->WasKeyJustReleased( 'F' ))
            currentLine.insert( currentLineCursorIndex++, "F" );
        if (inputSystem->WasKeyJustReleased( 'G' ))
            currentLine.insert( currentLineCursorIndex++, "G" );
        if (inputSystem->WasKeyJustReleased( 'H' ))
            currentLine.insert( currentLineCursorIndex++, "H" );
        if (inputSystem->WasKeyJustReleased( 'I' ))
            currentLine.insert( currentLineCursorIndex++, "I" );
        if (inputSystem->WasKeyJustReleased( 'J' ))
            currentLine.insert( currentLineCursorIndex++, "J" );
        if (inputSystem->WasKeyJustReleased( 'K' ))
            currentLine.insert( currentLineCursorIndex++, "K" );
        if (inputSystem->WasKeyJustReleased( 'L' ))
            currentLine.insert( currentLineCursorIndex++, "L" );
        if (inputSystem->WasKeyJustReleased( 'M' ))
            currentLine.insert( currentLineCursorIndex++, "M" );
        if (inputSystem->WasKeyJustReleased( 'N' ))
            currentLine.insert( currentLineCursorIndex++, "N" );
        if (inputSystem->WasKeyJustReleased( 'O' ))
            currentLine.insert( currentLineCursorIndex++, "O" );
        if (inputSystem->WasKeyJustReleased( 'P' ))
            currentLine.insert( currentLineCursorIndex++, "P" );
        if (inputSystem->WasKeyJustReleased( 'Q' ))
            currentLine.insert( currentLineCursorIndex++, "Q" );
        if (inputSystem->WasKeyJustReleased( 'R' ))
            currentLine.insert( currentLineCursorIndex++, "R" );
        if (inputSystem->WasKeyJustReleased( 'S' ))
            currentLine.insert( currentLineCursorIndex++, "S" );
        if (inputSystem->WasKeyJustReleased( 'T' ))
            currentLine.insert( currentLineCursorIndex++, "T" );
        if (inputSystem->WasKeyJustReleased( 'U' ))
            currentLine.insert( currentLineCursorIndex++, "U" );
        if (inputSystem->WasKeyJustReleased( 'V' ))
            currentLine.insert( currentLineCursorIndex++, "V" );
        if (inputSystem->WasKeyJustReleased( 'W' ))
            currentLine.insert( currentLineCursorIndex++, "W" );
        if (inputSystem->WasKeyJustReleased( 'X' ))
            currentLine.insert( currentLineCursorIndex++, "X" );
        if (inputSystem->WasKeyJustReleased( 'Y' ))
            currentLine.insert( currentLineCursorIndex++, "Y" );
        if (inputSystem->WasKeyJustReleased( 'Z' ))
            currentLine.insert( currentLineCursorIndex++,  "Z" );
    }

    if (inputSystem->IsKeyDown( VK_SHIFT ))
    {
        // letters
        if (inputSystem->WasKeyJustReleased( 'A' ))
            currentLine.insert( currentLineCursorIndex++, "A" );
        if (inputSystem->WasKeyJustReleased( 'B' ))
            currentLine.insert( currentLineCursorIndex++, "B" );
        if (inputSystem->WasKeyJustReleased( 'C' ))
            currentLine.insert( currentLineCursorIndex++, "C" );
        if (inputSystem->WasKeyJustReleased( 'D' ))
            currentLine.insert( currentLineCursorIndex++, "D" );
        if (inputSystem->WasKeyJustReleased( 'E' ))
            currentLine.insert( currentLineCursorIndex++, "E" );
        if (inputSystem->WasKeyJustReleased( 'F' ))
            currentLine.insert( currentLineCursorIndex++, "F" );
        if (inputSystem->WasKeyJustReleased( 'G' ))
            currentLine.insert( currentLineCursorIndex++, "G" );
        if (inputSystem->WasKeyJustReleased( 'H' ))
            currentLine.insert( currentLineCursorIndex++, "H" );
        if (inputSystem->WasKeyJustReleased( 'I' ))
            currentLine.insert( currentLineCursorIndex++, "I" );
        if (inputSystem->WasKeyJustReleased( 'J' ))
            currentLine.insert( currentLineCursorIndex++, "J" );
        if (inputSystem->WasKeyJustReleased( 'K' ))
            currentLine.insert( currentLineCursorIndex++, "K" );
        if (inputSystem->WasKeyJustReleased( 'L' ))
            currentLine.insert( currentLineCursorIndex++, "L" );
        if (inputSystem->WasKeyJustReleased( 'M' ))
            currentLine.insert( currentLineCursorIndex++, "M" );
        if (inputSystem->WasKeyJustReleased( 'N' ))
            currentLine.insert( currentLineCursorIndex++, "N" );
        if (inputSystem->WasKeyJustReleased( 'O' ))
            currentLine.insert( currentLineCursorIndex++, "O" );
        if (inputSystem->WasKeyJustReleased( 'P' ))
            currentLine.insert( currentLineCursorIndex++, "P" );
        if (inputSystem->WasKeyJustReleased( 'Q' ))
            currentLine.insert( currentLineCursorIndex++, "Q" );
        if (inputSystem->WasKeyJustReleased( 'R' ))
            currentLine.insert( currentLineCursorIndex++, "R" );
        if (inputSystem->WasKeyJustReleased( 'S' ))
            currentLine.insert( currentLineCursorIndex++, "S" );
        if (inputSystem->WasKeyJustReleased( 'T' ))
            currentLine.insert( currentLineCursorIndex++, "T" );
        if (inputSystem->WasKeyJustReleased( 'U' ))
            currentLine.insert( currentLineCursorIndex++, "U" );
        if (inputSystem->WasKeyJustReleased( 'V' ))
            currentLine.insert( currentLineCursorIndex++, "V" );
        if (inputSystem->WasKeyJustReleased( 'W' ))
            currentLine.insert( currentLineCursorIndex++, "W" );
        if (inputSystem->WasKeyJustReleased( 'X' ))
            currentLine.insert( currentLineCursorIndex++, "X" );
        if (inputSystem->WasKeyJustReleased( 'Y' ))
            currentLine.insert( currentLineCursorIndex++, "Y" );
        if (inputSystem->WasKeyJustReleased( 'Z' ))
            currentLine.insert( currentLineCursorIndex++, "Z" );

        // numbers
        if (inputSystem->WasKeyJustReleased( '1' ))
            currentLine.insert( currentLineCursorIndex++, "!" );
        if (inputSystem->WasKeyJustReleased( '2' ))
            currentLine.insert( currentLineCursorIndex++, "@" );
        if (inputSystem->WasKeyJustReleased( '3' ))
            currentLine.insert( currentLineCursorIndex++, "#" );
        if (inputSystem->WasKeyJustReleased( '4' ))
            currentLine.insert( currentLineCursorIndex++, "$" );
        if (inputSystem->WasKeyJustReleased( '5' ))
            currentLine.insert( currentLineCursorIndex++, "%" );
        if (inputSystem->WasKeyJustReleased( '6' ))
            currentLine.insert( currentLineCursorIndex++, "^" );
        if (inputSystem->WasKeyJustReleased( '7' ))
            currentLine.insert( currentLineCursorIndex++, "&" );
        if (inputSystem->WasKeyJustReleased( '8' ))
            currentLine.insert( currentLineCursorIndex++, "*" );
        if (inputSystem->WasKeyJustReleased( '9' ))
            currentLine.insert( currentLineCursorIndex++, "(" );
        if (inputSystem->WasKeyJustReleased( '0' ))
            currentLine.insert( currentLineCursorIndex++, ")" );


        // punctuation
        if (inputSystem->WasKeyJustReleased( VK_OEM_MINUS ))
            currentLine.insert( currentLineCursorIndex++, "_" );
        if (inputSystem->WasKeyJustReleased( VK_OEM_PLUS ))
            currentLine.insert( currentLineCursorIndex++, "+" );
        if (inputSystem->WasKeyJustReleased( VK_OEM_4 ))
            currentLine.insert( currentLineCursorIndex++, "{" );
        if (inputSystem->WasKeyJustReleased( VK_OEM_6 ))
            currentLine.insert( currentLineCursorIndex++, "}" );
        if (inputSystem->WasKeyJustReleased( VK_OEM_5 ))
            currentLine.insert( currentLineCursorIndex++, "|" );
        if (inputSystem->WasKeyJustReleased( VK_OEM_1 ))
            currentLine.insert( currentLineCursorIndex++, ":" );
        if (inputSystem->WasKeyJustReleased( VK_OEM_7 ))
            currentLine.insert( currentLineCursorIndex++, "\"" );
        if (inputSystem->WasKeyJustReleased( VK_OEM_COMMA ))
            currentLine.insert( currentLineCursorIndex++, "<" );
        if (inputSystem->WasKeyJustReleased( VK_OEM_PERIOD ))
            currentLine.insert( currentLineCursorIndex++, ">" );
        if (inputSystem->WasKeyJustReleased( VK_OEM_2 ))
            currentLine.insert( currentLineCursorIndex++, "?" );
    }

    else
    {
        if (!inputSystem->IsCapsLockEnabled())
        {
            // letters
            if (inputSystem->WasKeyJustReleased( 'A' ))
                currentLine.insert( currentLineCursorIndex++,  "a" );
            if (inputSystem->WasKeyJustReleased( 'B' ))
                currentLine.insert( currentLineCursorIndex++,  "b" );
            if (inputSystem->WasKeyJustReleased( 'C' ))
                currentLine.insert( currentLineCursorIndex++,  "c" );
            if (inputSystem->WasKeyJustReleased( 'D' ))
                currentLine.insert( currentLineCursorIndex++,  "d" );
            if (inputSystem->WasKeyJustReleased( 'E' ))
                currentLine.insert( currentLineCursorIndex++,  "e" );
            if (inputSystem->WasKeyJustReleased( 'F' ))
                currentLine.insert( currentLineCursorIndex++,  "f" );
            if (inputSystem->WasKeyJustReleased( 'G' ))
                currentLine.insert( currentLineCursorIndex++,  "g" );
            if (inputSystem->WasKeyJustReleased( 'H' ))
                currentLine.insert( currentLineCursorIndex++,  "h" );
            if (inputSystem->WasKeyJustReleased( 'I' ))
                currentLine.insert( currentLineCursorIndex++,  "i" );
            if (inputSystem->WasKeyJustReleased( 'J' ))
                currentLine.insert( currentLineCursorIndex++,  "j" );
            if (inputSystem->WasKeyJustReleased( 'K' ))
                currentLine.insert( currentLineCursorIndex++,  "k" );
            if (inputSystem->WasKeyJustReleased( 'L' ))
                currentLine.insert( currentLineCursorIndex++,  "l" );
            if (inputSystem->WasKeyJustReleased( 'M' ))
                currentLine.insert( currentLineCursorIndex++,  "m" );
            if (inputSystem->WasKeyJustReleased( 'N' ))
                currentLine.insert( currentLineCursorIndex++,  "n" );
            if (inputSystem->WasKeyJustReleased( 'O' ))
                currentLine.insert( currentLineCursorIndex++,  "o" );
            if (inputSystem->WasKeyJustReleased( 'P' ))
                currentLine.insert( currentLineCursorIndex++,  "p" );
            if (inputSystem->WasKeyJustReleased( 'Q' ))
                currentLine.insert( currentLineCursorIndex++,  "q" );
            if (inputSystem->WasKeyJustReleased( 'R' ))
                currentLine.insert( currentLineCursorIndex++,  "r" );
            if (inputSystem->WasKeyJustReleased( 'S' ))
                currentLine.insert( currentLineCursorIndex++,  "s" );
            if (inputSystem->WasKeyJustReleased( 'T' ))
                currentLine.insert( currentLineCursorIndex++,  "t" );
            if (inputSystem->WasKeyJustReleased( 'U' ))
                currentLine.insert( currentLineCursorIndex++,  "u" );
            if (inputSystem->WasKeyJustReleased( 'V' ))
                currentLine.insert( currentLineCursorIndex++,  "v" );
            if (inputSystem->WasKeyJustReleased( 'W' ))
                currentLine.insert( currentLineCursorIndex++,  "w" );
            if (inputSystem->WasKeyJustReleased( 'X' ))
                currentLine.insert( currentLineCursorIndex++,  "x" );
            if (inputSystem->WasKeyJustReleased( 'Y' ))
                currentLine.insert( currentLineCursorIndex++,  "y" );
            if (inputSystem->WasKeyJustReleased( 'Z' ))
                currentLine.insert( currentLineCursorIndex++,  "z" );
        }

        // numbers
        if (inputSystem->WasKeyJustReleased( '1' ))
            currentLine.insert( currentLineCursorIndex++,  "1" );
        if (inputSystem->WasKeyJustReleased( '2' ))
            currentLine.insert( currentLineCursorIndex++,  "2" );
        if (inputSystem->WasKeyJustReleased( '3' ))
            currentLine.insert( currentLineCursorIndex++,  "3" );
        if (inputSystem->WasKeyJustReleased( '4' ))
            currentLine.insert( currentLineCursorIndex++,  "4" );
        if (inputSystem->WasKeyJustReleased( '5' ))
            currentLine.insert( currentLineCursorIndex++,  "5" );
        if (inputSystem->WasKeyJustReleased( '6' ))
            currentLine.insert( currentLineCursorIndex++,  "6" );
        if (inputSystem->WasKeyJustReleased( '7' ))
            currentLine.insert( currentLineCursorIndex++,  "7" );
        if (inputSystem->WasKeyJustReleased( '8' ))
            currentLine.insert( currentLineCursorIndex++,  "8" );
        if (inputSystem->WasKeyJustReleased( '9' ))
            currentLine.insert( currentLineCursorIndex++,  "9" );
        if (inputSystem->WasKeyJustReleased( '0' ))
            currentLine.insert( currentLineCursorIndex++,  "0" );

        // punctuation
        if (inputSystem->WasKeyJustReleased( VK_OEM_MINUS ))
            currentLine.insert( currentLineCursorIndex++,  "-" );
        if (inputSystem->WasKeyJustReleased( VK_OEM_PLUS ))
            currentLine.insert( currentLineCursorIndex++,  "=" );
        if (inputSystem->WasKeyJustReleased( VK_OEM_4 ))
            currentLine.insert( currentLineCursorIndex++,  "[" );
        if (inputSystem->WasKeyJustReleased( VK_OEM_6 ))
            currentLine.insert( currentLineCursorIndex++,  "]" );
        if (inputSystem->WasKeyJustReleased( VK_OEM_5 ))
            currentLine.insert( currentLineCursorIndex++,  "\\" );
        if (inputSystem->WasKeyJustReleased( VK_OEM_1 ))
            currentLine.insert( currentLineCursorIndex++,  ";" );
        if (inputSystem->WasKeyJustReleased( VK_OEM_7 ))
            currentLine.insert( currentLineCursorIndex++,  "\'" );
        if (inputSystem->WasKeyJustReleased( VK_OEM_COMMA ))
            currentLine.insert( currentLineCursorIndex++,  "," );
        if (inputSystem->WasKeyJustReleased( VK_OEM_PERIOD ))
            currentLine.insert( currentLineCursorIndex++,  "." );
        if (inputSystem->WasKeyJustReleased( VK_OEM_2 ))
            currentLine.insert( currentLineCursorIndex++,  "/" );
    }

    if (inputSystem->WasKeyJustReleased( VK_BACK ))
    {
        if (currentLine.length() != 0 && currentLineCursorIndex != 0)
        {
            currentLine = currentLine.substr( 0, currentLineCursorIndex - 1 ) + currentLine.substr( currentLineCursorIndex, currentLine.length() );
            currentLineCursorIndex--;
        }
    }

    if (inputSystem->WasKeyJustReleased( VK_DELETE ))
    {
        if (currentLine.length() != 0 && (unsigned int) currentLineCursorIndex != currentLine.length() )
            currentLine = currentLine.substr( 0, currentLineCursorIndex ) + currentLine.substr( currentLineCursorIndex + 1, currentLine.length() );
    }

    if (inputSystem->WasKeyJustReleased( VK_LEFT ))
    {
        currentLineCursorIndex--;
        if (currentLineCursorIndex < 0)
            currentLineCursorIndex = 0;
    }

    if (inputSystem->WasKeyJustReleased( VK_RIGHT ))
    {
        currentLineCursorIndex++;
        if ( (unsigned int) currentLineCursorIndex > currentLine.length())
            currentLineCursorIndex = currentLine.length();

        if (currentLine.length() == 0)
            currentLineCursorIndex = 0;
    }

    if (inputSystem->WasKeyJustReleased( VK_UP ))
    {
        historyIndex++;
        if ((unsigned int) historyIndex == commands.size())
            historyIndex = 0;

        if (commands.size() != 0 )
            currentLine = commands[historyIndex];

        currentLineCursorIndex = currentLine.length();
    }

    if (inputSystem->WasKeyJustReleased( VK_DOWN ))
    {
        historyIndex--;
        if (historyIndex < 0)
            historyIndex = commands.size() - 1;

        if (commands.size() != 0 )
            currentLine = commands[historyIndex];

        currentLineCursorIndex = currentLine.length();

    }

    if (inputSystem->WasKeyJustReleased( VK_HOME ))
        currentLineCursorIndex = 0;

    if (inputSystem->WasKeyJustReleased( VK_END ))
        currentLineCursorIndex = currentLine.length();

    if (inputSystem->WasKeyJustReleased( VK_PRIOR ))
    {
        bottomLogLineIndex++;

        if (log.size() == (unsigned int) ( bottomLogLineIndex + MAX_NUM_VISIBLE_LOG_LINES - 1 ) )
            bottomLogLineIndex--;
    }

    if (inputSystem->WasKeyJustReleased( VK_NEXT ))
    {
        bottomLogLineIndex--;

        if (bottomLogLineIndex < 0)
            bottomLogLineIndex = 0;
    }

    if (inputSystem->WasKeyJustReleased( VK_RETURN ))
    {
        if (currentLine == "")
            isReadyToQuit = true;

        if (!isReadyToQuit)
        {
            bool commandExists = RunConsoleCommandFromLine( currentLine );
            if (!commandExists)
            {
                Line newLine;
                newLine.text = "Invalid Command";
                newLine.color = ERROR_TEXT_COLOR;
                log.insert( log.begin(), newLine );
            }

            commands.insert( commands.begin(), currentLine );
            currentLine = "";

            historyIndex = -1;


            bottomLogLineIndex = 0;
            currentLineCursorIndex = 0;
        }
    }

    if (inputSystem->WasKeyJustReleased( VK_ESCAPE ))
    {
        if (currentLine != "")
        {
            currentLine = "";
            currentLineCursorIndex = 0;
        }

        else if (currentLine == "")
            isReadyToQuit = true;
        
    }
}

//===========================================================================================
//===========================================================================================
// Render
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void DeveloperConsole::Render( const bool& debugModeEnabled )
{
    UNUSED( debugModeEnabled );

    if (!s_theDeveloperConsole)
        return;

    OpenGLRenderer* renderer = s_theDeveloperConsole->m_renderer;
    FontRenderer* fontRenderer = renderer->GetFontRenderer();
    Font font = *s_theDeveloperConsole->m_font;

    renderer->Disable( GL_CULL_FACE );
    renderer->Disable( GL_DEPTH_TEST );
    renderer->Enable( GL_BLEND );
    renderer->Enable( GL_TEXTURE_2D );
    renderer->BlendFunct( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    unsigned int shaderID = s_theDeveloperConsole->m_shaderID;
    Rgba textShadowColor = s_theDeveloperConsole->m_textShadowColor;
    bool textShadowEnabled = s_theDeveloperConsole->m_textShadowEnabled;
    std::string currentLine = s_theDeveloperConsole->m_currentLine;
    Lines log = s_theDeveloperConsole->m_log;
    int bottomLogLineIndex = s_theDeveloperConsole->m_bottomLogLineIndex;

    Vector3& cursorPos = s_theDeveloperConsole->m_cursorPos;

    renderer->Disable( GL_DEPTH_TEST );

    renderer->DrawColoredQuad( shaderID, Vector2( 0.0f, renderer->GetDisplayHeight() / 3.0f ), Vector2( renderer->GetDisplayWidth(), (renderer->GetDisplayHeight() / 3.0f) + 3.0f ), 1.0f, Rgba::WHITE );
    renderer->DrawColoredQuad( shaderID, Vector2( 0.0f, 0.0f ), Vector2( renderer->GetDisplayWidth(), renderer->GetDisplayHeight() / 3.0f ), 1.0f, Rgba( 0.0f, 0.0f, 0.0f, 0.7f ) );
    renderer->DrawColoredQuad( shaderID, Vector2( 0.0f, 0.0f ), Vector2( renderer->GetDisplayWidth(), 32.0f ), 1.0f, Rgba( 0.0f, 0.0f, 0.0f, 0.5f ) );
    renderer->DrawColoredQuad( shaderID, Vector2( 0.0f, 32.0f ), Vector2( renderer->GetDisplayWidth(), 32.0f + 3.0f ), 1.0f, Rgba::WHITE );


    Vector3 startPosition( 5.0f, 2.0f, 1.0f );
    Vector3 position = startPosition;

    Vector3 shadowPos;

    if (textShadowEnabled)
    {
        shadowPos = Vector3( startPosition.x - 1.0f, startPosition.y - 1.0f, startPosition.z );
        fontRenderer->DrawFontTextOrtho( font.fontData.size, font, ">", shadowPos, textShadowColor );
    }
    fontRenderer->DrawFontTextOrtho( font.fontData.size, font, ">", position, GetTextColor() );


    cursorPos = position;

    unsigned int desiredFontHeight = font.fontData.size;

    while (fontRenderer->CalcTextWidth( desiredFontHeight, font, currentLine, currentLine.length() ) + cursorPos.x > renderer->GetDisplayWidth())
        --desiredFontHeight;

    Vector3 cursorPosBefore = cursorPos;

    if (textShadowEnabled)
    {
        shadowPos = Vector3( cursorPosBefore.x - 1.0f, cursorPosBefore.y - 1.0f, cursorPosBefore.z );
        fontRenderer->DrawFontTextOrtho( desiredFontHeight, font, currentLine, shadowPos, textShadowColor );
    }
    fontRenderer->DrawFontTextOrtho( desiredFontHeight, font, currentLine, cursorPos, GetTextColor() );
    

    if ((int)floorf( sin( 6.0f * (float)GetCurrentSeconds() ) ) == 0)
    {
        Vector3 curPos = GetCursorPos( desiredFontHeight );
        curPos += position;
        renderer->DrawColoredQuad( shaderID, Vector2( curPos.x, curPos.y - 3.0f ), Vector2( curPos.x + 2.0f, curPos.y + font.fontData.size - 3.0f ), 1.0f, GetTextColor() );

    }

    // draw previous lines
    Vector3 defaultPosition( 5.0f, 34.0f, 1.0f );
    position = defaultPosition;
    float lineYDiff = (float) font.fontData.size;
    int count = 0;

    if (log.size() < MAX_NUM_VISIBLE_LOG_LINES)
        bottomLogLineIndex = 0;

    for (std::vector<Line>::const_iterator lineIter = log.begin() + bottomLogLineIndex; lineIter != log.end(); ++lineIter)
    {
        if (count == MAX_NUM_VISIBLE_LOG_LINES)
            break;

        Line line = *lineIter;

        desiredFontHeight = font.fontData.size;

        while (fontRenderer->CalcTextWidth( desiredFontHeight, font, line.text, line.text.length() ) > renderer->GetDisplayWidth())
            --desiredFontHeight;

        Vector3 posBefore = position;

        if (textShadowEnabled)
        {
            shadowPos = Vector3( posBefore.x - 1.0f, posBefore.y - 1.0f, posBefore.z );
            fontRenderer->DrawFontTextOrtho( desiredFontHeight, font, line.text, shadowPos, textShadowColor );
        }
        fontRenderer->DrawFontTextOrtho( desiredFontHeight, font, line.text, position, line.color * 0.8f );

        count++;

        position = defaultPosition;
        position.y += count * lineYDiff;
    }
}

//===========================================================================================
//===========================================================================================
// Developer Console Commands
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
CONSOLE_COMMAND( clear, "args: none | desc: clears console log" )
{
    UNUSED( args );
    DeveloperConsole::ClearLog();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
CONSOLE_COMMAND( quit, "args: none | desc: quits application"  )
{
    UNUSED( args );
    s_theInputSystem->SetSystemQuit();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
CONSOLE_COMMAND( help, "args: command name for specific help, none for all help" )
{
    std::string cmdStr = "";
    if (args->GetString( cmdStr ))
    {
        DeveloperConsoleCommand* command = DeveloperConsoleCommand::GetCommand( cmdStr );
        if (command)
        {
            DeveloperConsole::WriteLine( cmdStr + ":", DeveloperConsole::GetTextColor() );
            DeveloperConsole::WriteLine( "     " + command->GetHelpMessage(), DeveloperConsole::GetTextColor() );
        }
           
        else
            DeveloperConsole::WriteLine( "\"" + cmdStr + "\" is not a registered command", ERROR_TEXT_COLOR );
    }

    else
    {
        std::map < std::string, DeveloperConsoleCommand*>* allCommands = DeveloperConsoleCommand::GetAllCommands();

        for (CommandMap::const_iterator commandIter = allCommands->begin(); commandIter != allCommands->end(); ++commandIter )
        {
            DeveloperConsoleCommand* command = commandIter->second;
            DeveloperConsole::WriteLine( command->GetName() + ":", DeveloperConsole::GetTextColor() );
            DeveloperConsole::WriteLine( "     " + command->GetHelpMessage(), DeveloperConsole::GetTextColor() );

        }
    }

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
CONSOLE_COMMAND( textshadow, "args: true, false | desc: enables and disables text shadow int the console" )
{
    bool textShadow = false;
    if (args->GetBool( textShadow ))
    {
        DeveloperConsole::TextShadowEnabled( textShadow );
        std::string textShadowEnabled = "";
        if (textShadow)
            textShadowEnabled = "true";
        else
            textShadowEnabled = "false";
        DeveloperConsole::WriteLine( "textshadow enabled: " + textShadowEnabled, DeveloperConsole::GetTextColor() );

    }

    else
    {
        DeveloperConsole::WriteLine( "textshadow: Invalid Argument", ERROR_TEXT_COLOR );
        RunConsoleCommandFromLine( "help textshadow" );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
CONSOLE_COMMAND( textshadowcolor, "args: string color, 0-255 rgba color, 0xrrggbbaa color | desc: changes the text shadow color in the console" )
{
    Rgba textShadowColor = Rgba::BLACK;

    if (args->GetColorFromInts( textShadowColor ) || args->GetColorFromString( textShadowColor ) || args->GetColorFromHex( textShadowColor ))
    {
        DeveloperConsole::SetTextShadowColor( textShadowColor );
        DeveloperConsole::WriteLine( "Text Shadow Color changed", DeveloperConsole::GetTextColor() );
    }
    else
    {
        DeveloperConsole::WriteLine( "textshadowcolor: Invalid Argument", ERROR_TEXT_COLOR );
        RunConsoleCommandFromLine( "help textshadowcolor" );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
CONSOLE_COMMAND( textcolor, "args: string color, 0-255 rgba color, 0xrrggbbaa color | desc: changes the text color in the console" )
{
    Rgba textColor = Rgba::WHITE;

    if (args->GetColorFromInts( textColor ) || args->GetColorFromString( textColor ) || args->GetColorFromHex( textColor ))
    {
        DeveloperConsole::SetTextColor( textColor );
        DeveloperConsole::WriteLine( "Text Color changed", DeveloperConsole::GetTextColor() );
    }
    else
    {
        DeveloperConsole::WriteLine( "textcolor: Invalid Argument", ERROR_TEXT_COLOR );
        RunConsoleCommandFromLine( "help textcolor" );
    }
}