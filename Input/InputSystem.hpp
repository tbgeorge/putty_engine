//=================================================================================
// InputSystem.hpp
// Author: Tyler George
// Date  : February 3, 2015
//=================================================================================

#pragma once
#ifndef __included_InputSystem__
#define __included_InputSystem__


#define NUMBER_OF_VIRTUAL_KEYS 256
#define MAX_NUMBER_OF_XBOX_CONTROLLERS 4
#define MAX_NUMBER_OF_MOUSE_BUTTONS 2

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"

///---------------------------------------------------------------------------------
/// Enumerations
///---------------------------------------------------------------------------------
enum XboxControllerButtons
{
	XBOX_BUTTON_A,
	XBOX_BUTTON_B,
	XBOX_BUTTON_Y,
	XBOX_BUTTON_X,
	XBOX_BUTTON_BUMPER_LEFT,
	XBOX_BUTTON_BUMPER_RIGHT,
	XBOX_BUTTON_STICK_LEFT,
	XBOX_BUTTON_STICK_RIGHT,
	XBOX_BUTTON_TRIGGER_LEFT,
	XBOX_BUTTON_TRIGGER_RIGHT,
	XBOX_BUTTON_BACK,
	XBOX_BUTTON_START,
	XBOX_BUTTON_DPAD_NORTH,
	XBOX_BUTTON_DPAD_SOUTH,
	XBOX_BUTTON_DPAD_EAST,
	XBOX_BUTTON_DPAD_WEST,
	XBOX_BUTTON_DPAD_NORTHEAST,
	XBOX_BUTTON_DPAD_NORTHWEST,
	XBOX_BUTTON_DPAD_SOUTHEAST,
	XBOX_BUTTON_DPAD_SOUTHWEST,
	XBOX_NUMBER_OF_BUTTONS
};

enum XboxControllerSticks
{
	XBOX_STICK_LEFT,
	XBOX_STICK_RIGHT,
	XBOX_NUMBER_OF_STICKS
};

enum XboxControllerTriggers
{
	XBOX_TRIGGER_LEFT,
	XBOX_TRIGGER_RIGHT,
	XBOX_NUMBER_OF_TRIGGERS
};

enum MouseButtons
{
    MOUSE_LEFT,
    MOUSE_RIGHT
};

enum PointerType
{
    PT_ARROW,
    PT_HAND,
    PT_TEXT,
    PT_WAIT
};

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
struct MouseButtonState
{
    bool m_isDown;
    bool m_wasJustReleased;
};

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
struct KeyState
{
	bool	m_isDown;
	bool	m_wasJustReleased;
};

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
struct XboxControllerState
{
	bool	m_isPluggedIn;
	int		m_controllerID;
	KeyState	m_buttonDownStates[ XBOX_NUMBER_OF_BUTTONS ];
	Vector2 m_normalizedStickStates[ XBOX_NUMBER_OF_STICKS ];
	float	m_normalizedTriggerStates[ XBOX_NUMBER_OF_TRIGGERS ];
};

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class InputSystem
{
public:
	///---------------------------------------------------------------------------------
	/// Constructors/Destructor
	///---------------------------------------------------------------------------------
	InputSystem();
	~InputSystem() {};

	///---------------------------------------------------------------------------------
	/// Initialization Methods
	///---------------------------------------------------------------------------------
    void Startup( void* windowHandle );
	void Shutdown();

	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------
	bool			IsReadyToQuit() const;

	// Keyboard
	bool			IsKeyDown( int key ) const;
    bool            IsCapsLockEnabled() const;
	bool			WasKeyJustReleased( int key ) const;

	// Xbox Controller
	unsigned int	GetNumberOfXboxControllers() const;
	bool			HasController( unsigned int controllerID ) const;
	bool			IsXboxControllerButtonDown( XboxControllerButtons buttonID, unsigned int controllerID = 0 ) const;
	bool			WasXboxControllerButtonJustReleased( XboxControllerButtons buttonID, unsigned int controllerID = 0 ) const;
	Vector2			GetXboxControllerStickMagnitude( XboxControllerSticks stickID, unsigned int controllerID = 0 ) const;
	float			GetXboxControllerTriggerMagnitude( XboxControllerTriggers triggerID, unsigned int controllerID = 0 ) const;

	// Mouse
    void            ResetMouseCursor();
	void			ShowMousePointer( bool showCursor );
	const Vector2	GetMousePosition();
	void			SetMousePosition( const Vector2& cursorPosition );
	int				GetWheelClicks();

    Vector3         GetWNormalizedMouseNearPosition( OpenGLRenderer* renderer );
    Vector3         GetWNormalizedMouseFarPosition( OpenGLRenderer* renderer );

	bool			IsMouseButtonDown( int button );
    bool			WasMouseButtonJustReleased( int button );

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    void SetPointerType( PointerType type );

	///---------------------------------------------------------------------------------
	/// Update Methods
	///---------------------------------------------------------------------------------
	void	SetSystemQuit();
	void	Update();
	
	// Keyboard
	void	SetKeyState( int key, bool isDown );
	
	// Mouse
    void    SetMouseButtonState( int button, bool isDown );
	void	SetWheelClicks( int numClicks );

	// Xbox Controller
	void	SetButtonState( XboxControllerButtons buttonID, bool isDown, unsigned int controllerID = 0 );
	void	SetXboxStickDeadzoneThreshold( float deadzoneThreshold );


protected:
	///---------------------------------------------------------------------------------
	/// Update Methods
	///---------------------------------------------------------------------------------
	void	ProcessKeyboardInput();
	void	ProcessXboxControllersInput();
	void	ProcessXboxControllerInput( XboxControllerState& controllerState );
	float	NormalizeXboxControllerStickValue( short stickMagnitude );
	float	NormalizeXboxControllerTriggerValue(unsigned char triggerMagnitude);

private:
	///---------------------------------------------------------------------------------
	/// Private member variables
	///---------------------------------------------------------------------------------
	void*				m_windowHandle;
    void*			    m_previousAddress;
	XboxControllerState m_xboxContollerStates[ MAX_NUMBER_OF_XBOX_CONTROLLERS ];
	KeyState			m_keyDownStates[ NUMBER_OF_VIRTUAL_KEYS ];
    MouseButtonState    m_mouseButtonStates[MAX_NUMBER_OF_MOUSE_BUTTONS];
	float				m_stickDeadzoneThreshold;
	bool				m_isReadyToQuit;
	int					m_numWheelClicks;

    PointerType         m_currentPointer;

};

///---------------------------------------------------------------------------------
/// Externals
///---------------------------------------------------------------------------------
extern InputSystem* s_theInputSystem;

#endif