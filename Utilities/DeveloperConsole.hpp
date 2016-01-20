//=================================================================================
// DeveloperConsole.hpp
// Author: Tyler George
// Date  : June 10, 2015
//=================================================================================

#pragma once

#ifndef __included_DeveloperConsole__
#define __included_DeveloperConsole__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include <list>
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/GraphicsDebugger.hpp"
#include "Engine/Renderer/FontRenderer.hpp"
#include "Engine/Common/EngineCommon.hpp"

///---------------------------------------------------------------------------------
/// Constants
///---------------------------------------------------------------------------------
const int MAX_NUM_VISIBLE_LOG_LINES = 8;

///---------------------------------------------------------------------------------
/// Line
///---------------------------------------------------------------------------------
struct Line
{
    Rgba color;
    std::string text;
};

typedef std::vector<Line> Lines;

///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class DeveloperConsole
{
public:
	///---------------------------------------------------------------------------------
	/// Public Constructors/Destructors
	///---------------------------------------------------------------------------------
    ~DeveloperConsole();

    ///---------------------------------------------------------------------------------
    /// Public Initialization
    ///---------------------------------------------------------------------------------
    static void Shutdown();

	///---------------------------------------------------------------------------------
	/// Public Static Accessors/Queries
	///---------------------------------------------------------------------------------
    static DeveloperConsole* GetInstance( OpenGLRenderer* renderer, GraphicsDebugger* graphicsDebugger );
    static Rgba GetTextColor();

    ///---------------------------------------------------------------------------------
    /// Static Mutators
    ///---------------------------------------------------------------------------------
    static void WriteLine( const std::string& text, const Rgba& color );
    static void SetEnabled();
    static void SetDisabled();
    static bool IsEnabled();
    static bool IsReadyToDisable();
    static void TextShadowEnabled( bool enabled );
    static void SetTextShadowColor( const Rgba& color );
    static void SetTextColor( const Rgba& color );
    static void ClearLog();

	///---------------------------------------------------------------------------------
	/// Static Process Input
	///---------------------------------------------------------------------------------
    static void ProcessInput( InputSystem* inputSystem );

	///---------------------------------------------------------------------------------
	/// Static Render
	///---------------------------------------------------------------------------------
    static void Render( const bool& debugModeEnabled );

	///---------------------------------------------------------------------------------
	/// Public Member Variables
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Static Variables
	///---------------------------------------------------------------------------------


private:
	///---------------------------------------------------------------------------------
	/// Private Constructors/Destructors
	///---------------------------------------------------------------------------------
    DeveloperConsole( OpenGLRenderer* renderer, GraphicsDebugger* graphicsDebugger );

    ///---------------------------------------------------------------------------------
    /// Private Initialization
    ///---------------------------------------------------------------------------------
    void StartUp();

    ///---------------------------------------------------------------------------------
    /// Private Static Accessors/Queries
    ///---------------------------------------------------------------------------------
    static Vector3 GetCursorPos( const unsigned int& fontHeight );

	///---------------------------------------------------------------------------------
	/// Private Member Variables
	///---------------------------------------------------------------------------------
    OpenGLRenderer* m_renderer;
    FontRenderer* m_fontRenderer;
    GraphicsDebugger* m_graphicsDebugger;

    unsigned int m_shaderID;
    unsigned int m_fontShaderID;
    Font* m_font;

    Vector3 m_cursorPos;
    Vector3 m_defaultCursorPos;

    std::string m_currentLine;
    int m_currentLineCursorIndex;

    bool m_textShadowEnabled;
    Rgba m_textShadowColor;
    Rgba m_textColor;

    std::vector<Line> m_log;
    std::vector<std::string> m_commands;

    int m_bottomLogLineIndex;
    int m_historyIndex;

    bool m_enabled;
    bool m_isReadyToQuit;

    ///---------------------------------------------------------------------------------
    /// Private Static Variables
    ///---------------------------------------------------------------------------------
    static DeveloperConsole* s_theDeveloperConsole;
};

///---------------------------------------------------------------------------------
/// Inline function implementations
///---------------------------------------------------------------------------------

#endif