//=================================================================================
// InputSystem.cpp
// Author: Tyler George
// Date  : February 3, 2015
//=================================================================================

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Utilities/Console.hpp"


///---------------------------------------------------------------------------------
/// Externals
///---------------------------------------------------------------------------------
InputSystem* s_theInputSystem = NULL;

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
LRESULT __stdcall WindowMessageProcessingFunction( HWND windowHandle, UINT messageID, WPARAM wParam, LPARAM lParam ) 
{
	if( s_theInputSystem )
	{
		int mouseDegrees = 0;
		int mouseClicks = 0;

		switch (messageID) 
		{
		case WM_KEYDOWN:
			s_theInputSystem->SetKeyState( (int) wParam, true );
			return 0;

		case WM_KEYUP:
			s_theInputSystem->SetKeyState( (int) wParam, false );
			return 0;

        case WM_LBUTTONDOWN:
            s_theInputSystem->SetMouseButtonState( 0, true );
            return 0;

        case WM_LBUTTONUP:
            s_theInputSystem->SetMouseButtonState( 0, false );
            return 0;

        case WM_RBUTTONDOWN:
            s_theInputSystem->SetMouseButtonState( 1, true );
            return 0;

        case WM_RBUTTONUP:
            s_theInputSystem->SetMouseButtonState( 1, false );
            return 0;



		case WM_MOUSEWHEEL:
			mouseDegrees = wParam & 0xFFFF0000;
			mouseDegrees >>= 16;
			mouseClicks = mouseDegrees / WHEEL_DELTA;
			s_theInputSystem->SetWheelClicks( mouseClicks );
			return 0;

		case WM_CLOSE:
			s_theInputSystem->SetSystemQuit();
			return 0;

		default:
			break;
		}
	}
	return DefWindowProc( windowHandle, messageID, wParam, lParam );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
InputSystem::InputSystem()
	: m_windowHandle( NULL )
	, m_previousAddress( 0 )
	, m_stickDeadzoneThreshold( 0.0f )
	, m_isReadyToQuit( false )
    , m_currentPointer( PT_ARROW )
{
	//ASSERT
	if( !s_theInputSystem )
		s_theInputSystem = this;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void InputSystem::Startup( void* windowHandle )
{
	m_windowHandle = windowHandle;

    m_previousAddress = (void*) SetWindowLongPtr( (HWND)m_windowHandle, GWLP_WNDPROC, (LONG_PTR)&WindowMessageProcessingFunction );

	// Initialize keyboard states
	for( unsigned int keyIndex = 0; keyIndex < NUMBER_OF_VIRTUAL_KEYS; ++keyIndex )
	{
		m_keyDownStates[ keyIndex ].m_isDown = false;
		m_keyDownStates[ keyIndex ].m_wasJustReleased = false;
	}

	// Initialize controller button, trigger, and stick states
	for( unsigned int controllerIndex = 0; controllerIndex < MAX_NUMBER_OF_XBOX_CONTROLLERS; ++controllerIndex )
	{
		XboxControllerState& controllerState = m_xboxContollerStates[ controllerIndex ];
		controllerState.m_isPluggedIn = false;
		controllerState.m_controllerID = controllerIndex;

		for( unsigned int buttonIndex = 0; buttonIndex < XBOX_NUMBER_OF_BUTTONS; ++buttonIndex )
		{
			controllerState.m_buttonDownStates[ buttonIndex ].m_isDown = false;
			controllerState.m_buttonDownStates[ buttonIndex ].m_wasJustReleased = false;
		}

		for( unsigned int triggerIndex = 0; triggerIndex < XBOX_NUMBER_OF_TRIGGERS; ++triggerIndex )
			controllerState.m_normalizedTriggerStates[ triggerIndex ] = 0.0f;

		for( unsigned int stickIndex = 0; stickIndex < XBOX_NUMBER_OF_STICKS; ++stickIndex )
			controllerState.m_normalizedStickStates[ stickIndex ] = Vector2( 0.0f, 0.0f );
	}

    // Initialize Mouse States
    for (unsigned int buttonIndex = 0; buttonIndex < MAX_NUMBER_OF_MOUSE_BUTTONS; ++buttonIndex)
    {
        m_mouseButtonStates[buttonIndex].m_isDown = false;
        m_mouseButtonStates[buttonIndex].m_wasJustReleased = false;
    }

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void InputSystem::Shutdown()
{
    SetWindowLongPtr( (HWND)m_windowHandle, GWLP_WNDPROC, (LONG_PTR)m_previousAddress );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool InputSystem::IsReadyToQuit() const
{
	return m_isReadyToQuit;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool InputSystem::IsKeyDown( int key ) const
{
	if( key >= 0 && key < NUMBER_OF_VIRTUAL_KEYS )
		return m_keyDownStates[ key ].m_isDown;
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool InputSystem::IsCapsLockEnabled() const
{
    return ( GetKeyState( VK_CAPITAL ) == 1 );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool InputSystem::WasKeyJustReleased( int key ) const
{
	if( key >= 0 && key < NUMBER_OF_VIRTUAL_KEYS )
		return m_keyDownStates[ key ].m_wasJustReleased;
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned int InputSystem::GetNumberOfXboxControllers() const
{
	unsigned int numberOfConnectedControllers = 0;
	for( unsigned int controllerIndex = 0; controllerIndex < MAX_NUMBER_OF_XBOX_CONTROLLERS; ++controllerIndex )
	{
		if( m_xboxContollerStates[ controllerIndex ].m_isPluggedIn )
			numberOfConnectedControllers++;
	}
	return numberOfConnectedControllers;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool InputSystem::HasController( unsigned int controllerID ) const 
{
	if( controllerID > MAX_NUMBER_OF_XBOX_CONTROLLERS )
		return false;

	else 
		return m_xboxContollerStates[ controllerID ].m_isPluggedIn;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool InputSystem::IsXboxControllerButtonDown( XboxControllerButtons buttonID, unsigned int controllerID ) const
{
	if( controllerID > MAX_NUMBER_OF_XBOX_CONTROLLERS || controllerID < 0 )
		return false;
	if( buttonID > XBOX_NUMBER_OF_BUTTONS || buttonID < 0 )
		return false;
	if( !HasController( controllerID ) )
		return false;

	return m_xboxContollerStates[ controllerID ].m_buttonDownStates[ buttonID ].m_isDown;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool InputSystem::WasXboxControllerButtonJustReleased( XboxControllerButtons buttonID, unsigned int controllerID /* = 0 */ ) const
{
	if( controllerID > MAX_NUMBER_OF_XBOX_CONTROLLERS || controllerID < 0 )
		return false;
	if( buttonID > XBOX_NUMBER_OF_BUTTONS || buttonID < 0 )
		return false;
	if( !HasController( controllerID ) )
		return false;

	return m_xboxContollerStates[ controllerID ].m_buttonDownStates[ buttonID ].m_wasJustReleased;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Vector2 InputSystem::GetXboxControllerStickMagnitude( XboxControllerSticks stickID, unsigned int controllerID ) const
{
	if( controllerID > MAX_NUMBER_OF_XBOX_CONTROLLERS || controllerID < 0 )
		return Vector2( 0.0f, 0.0f );
	if( stickID > XBOX_NUMBER_OF_STICKS || stickID < 0 )
		return Vector2( 0.0f, 0.0f );
	if( !HasController( controllerID ) )
		return Vector2( 0.0f, 0.0f );

	return m_xboxContollerStates[ controllerID ].m_normalizedStickStates[ stickID ];
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
float InputSystem::GetXboxControllerTriggerMagnitude( XboxControllerTriggers triggerID, unsigned int controllerID ) const
{
	if( controllerID > MAX_NUMBER_OF_XBOX_CONTROLLERS || controllerID < 0 )
		return 0.0f;
	if( triggerID > XBOX_NUMBER_OF_STICKS || triggerID < 0 )
		return 0.0f;
	if( !HasController( controllerID ) )
		return 0.0f;

	return m_xboxContollerStates[ controllerID ].m_normalizedTriggerStates[ triggerID ];
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void InputSystem::ResetMouseCursor()
{
    HCURSOR cursor;

    switch (m_currentPointer)
    {
    case PT_ARROW:
        cursor = LoadCursor( NULL, IDC_ARROW );
        break;
    case PT_HAND:
        cursor = LoadCursor( NULL, IDC_HAND );
        break;
    case PT_TEXT:
        cursor = LoadCursor( NULL, IDC_IBEAM );
        break;
    case PT_WAIT:
        cursor = LoadCursor( NULL, IDC_WAIT );
        break;
    default:
        break;
    }
    SetCursor( cursor );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void InputSystem::ShowMousePointer( bool showCursor )
{
	ShowCursor( showCursor );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
const Vector2 InputSystem::GetMousePosition() 
{
	POINT cursorPosition;
	GetCursorPos( &cursorPosition );
    ScreenToClient( (HWND) m_windowHandle, &cursorPosition );
    RECT clientSize;
    GetClientRect( (HWND)m_windowHandle, &clientSize );

	return Vector2( (float)cursorPosition.x, clientSize.bottom - (float)cursorPosition.y );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void InputSystem::SetMousePosition( const Vector2& cursorPosition )
{
    POINT clientCursorPos;
    clientCursorPos.x = (int) cursorPosition.x;
    clientCursorPos.y = (int)cursorPosition.y;

    ClientToScreen( (HWND)m_windowHandle, &clientCursorPos );
	SetCursorPos( (int) clientCursorPos.x, (int) clientCursorPos.y );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
int InputSystem::GetWheelClicks()
{
	int wheelClicks = m_numWheelClicks;
	m_numWheelClicks = 0;
	return wheelClicks;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Vector3 InputSystem::GetWNormalizedMouseNearPosition( OpenGLRenderer* renderer )
{
    if (!renderer)
        return Vector3::ZERO;

    Vector2 mousePos = GetMousePosition();
    Vector2 halfDisplaySize( renderer->GetDisplayWidth() * 0.5f, renderer->GetDisplayHeight() * 0.5f );
    Vector2 offset = mousePos - halfDisplaySize;
    Vector2 ndcCoords( RangeMap( offset.x, -halfDisplaySize.x, halfDisplaySize.x, -1.0f, 1.0f ), RangeMap( offset.y, -halfDisplaySize.y, halfDisplaySize.y, -1.0f, 1.0f ) );

    Matrix4f perspectiveToWorld = MatrixMultiply( renderer->GetViewMatrix(), renderer->GetPerspectiveMatrix() );
    perspectiveToWorld.Invert();

    Vector4f nearWorldPosVec = perspectiveToWorld.TransformVector( Vector4f( ndcCoords.x, ndcCoords.y, 0.0f, 1.0f ) );

    Vector3 nearWorldPos( nearWorldPosVec.x / nearWorldPosVec.w, nearWorldPosVec.y / nearWorldPosVec.w, nearWorldPosVec.z / nearWorldPosVec.w );

    return nearWorldPos;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Vector3 InputSystem::GetWNormalizedMouseFarPosition( OpenGLRenderer* renderer )
{
    if (!renderer)
        return Vector3::ZERO;

    Vector2 mousePos = GetMousePosition();
    Vector2 halfDisplaySize( renderer->GetDisplayWidth() * 0.5f, renderer->GetDisplayHeight() * 0.5f );
    Vector2 offset = mousePos - halfDisplaySize;
    Vector2 ndcCoords( RangeMap( offset.x, -halfDisplaySize.x, halfDisplaySize.x, -1.0f, 1.0f ), RangeMap( offset.y, -halfDisplaySize.y, halfDisplaySize.y, -1.0f, 1.0f ) );

    Matrix4f perspectiveToWorld = MatrixMultiply( renderer->GetViewMatrix(), renderer->GetPerspectiveMatrix() );
    perspectiveToWorld.Invert();

    Vector4f farWorldPosVec = perspectiveToWorld.TransformVector( Vector4f( ndcCoords.x, ndcCoords.y, 1.0f, 1.0f ) );

    Vector3 farWorldPos( farWorldPosVec.x / farWorldPosVec.w, farWorldPosVec.y / farWorldPosVec.w, farWorldPosVec.z / farWorldPosVec.w );

    return farWorldPos;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool InputSystem::IsMouseButtonDown( int button )
{
    return m_mouseButtonStates[button].m_isDown;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool InputSystem::WasMouseButtonJustReleased( int button )
{
    return m_mouseButtonStates[button].m_wasJustReleased;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void InputSystem::SetPointerType( PointerType type )
{
    m_currentPointer = type;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void InputSystem::SetSystemQuit() 
{
	m_isReadyToQuit = true;
}
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void InputSystem::SetKeyState( int key, bool isDown )
{
	if( key < 0 || key >= NUMBER_OF_VIRTUAL_KEYS )
		return;

	if( m_keyDownStates[ key ].m_isDown && !isDown )
		m_keyDownStates[ key ].m_wasJustReleased = true;
	m_keyDownStates[ key ].m_isDown = isDown;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void InputSystem::SetMouseButtonState( int button, bool isDown )
{
    if (button < 0 || button >= MAX_NUMBER_OF_MOUSE_BUTTONS)
        return;

    if (m_mouseButtonStates[button].m_isDown && !isDown)
        m_mouseButtonStates[button].m_wasJustReleased = true;
    m_mouseButtonStates[button].m_isDown = isDown;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void InputSystem::SetWheelClicks( int numClicks )
{
	m_numWheelClicks += numClicks;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void InputSystem::SetButtonState( XboxControllerButtons buttonID, bool isDown, unsigned int controllerID )
{
	if( controllerID > MAX_NUMBER_OF_XBOX_CONTROLLERS || controllerID < 0 )
		return;
	if( buttonID > XBOX_NUMBER_OF_BUTTONS || buttonID < 0 )
		return;
	if( !HasController( controllerID ) )
		return;

	if( m_xboxContollerStates->m_buttonDownStates[ buttonID ].m_isDown && !isDown )
		m_xboxContollerStates->m_buttonDownStates[ buttonID ].m_wasJustReleased = true;
	m_xboxContollerStates->m_buttonDownStates[ buttonID ].m_isDown = isDown;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void InputSystem::ProcessKeyboardInput()
{
	HWND myWindowHandle = (HWND)( m_windowHandle );
	MSG	queuedMessage;

	for( ; ; )
	{
		BOOL wasMessageFound = PeekMessage( &queuedMessage, myWindowHandle, 0, 0, PM_REMOVE );
		if( !wasMessageFound )
			break;

		TranslateMessage( &queuedMessage );
		DispatchMessage( &queuedMessage );
	}
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void InputSystem::ProcessXboxControllersInput() 
{
	for( unsigned int controllerID = 0; controllerID < MAX_NUMBER_OF_XBOX_CONTROLLERS; ++controllerID )
		ProcessXboxControllerInput( m_xboxContollerStates[ controllerID ] );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void InputSystem::ProcessXboxControllerInput( XboxControllerState& controllerState )
{
	static const unsigned short	BIT_A_BUTTON				= 0x1000;
	static const unsigned short	BIT_B_BUTTON				= 0x2000;
	static const unsigned short	BIT_X_BUTTON				= 0x4000;
	static const unsigned short	BIT_Y_BUTTON				= 0x8000;
	static const unsigned short	BIT_RIGHT_BUMPER_BUTTON		= 0x0200;
	static const unsigned short	BIT_LEFT_BUMPER_BUTTON		= 0x0100;
	static const unsigned short	BIT_RIGHT_STICK_BUTTON		= 0x0080;
	static const unsigned short	BIT_LEFT_STICK_BUTTON		= 0x0040;
	static const unsigned short	BIT_START_BUTTON			= 0x0010;
	static const unsigned short	BIT_BACK_BUTTON				= 0x0020;
	static const unsigned short	BIT_DPAD_NORTH_BUTTON		= 0x0001;
	static const unsigned short	BIT_DPAD_SOUTH_BUTTON		= 0x0002;
	static const unsigned short	BIT_DPAD_WEST_BUTTON		= 0x0004;
	static const unsigned short	BIT_DPAD_EAST_BUTTON		= 0x0008;
	static const unsigned short	BIT_DPAD_NORTHEAST_BUTTON	= 0x0009;
	static const unsigned short	BIT_DPAD_NORTHWEST_BUTTON	= 0x0005;
	static const unsigned short	BIT_DPAD_SOUTHEAST_BUTTON	= 0x000a;
	static const unsigned short	BIT_DPAD_SOUTHWEST_BUTTON	= 0x0006;

	XINPUT_STATE xboxInputState;
	memset( &xboxInputState, 0, sizeof( xboxInputState ) );

	DWORD errorStatus = XInputGetState( controllerState.m_controllerID, &xboxInputState );
	if( errorStatus == ERROR_SUCCESS )
	{
		controllerState.m_isPluggedIn = true;
		SetButtonState( XBOX_BUTTON_A, ( BIT_A_BUTTON & xboxInputState.Gamepad.wButtons ) ? true : false, controllerState.m_controllerID );
		SetButtonState( XBOX_BUTTON_B, ( BIT_B_BUTTON & xboxInputState.Gamepad.wButtons ) ? true : false, controllerState.m_controllerID );
		SetButtonState( XBOX_BUTTON_X, ( BIT_X_BUTTON & xboxInputState.Gamepad.wButtons ) ? true : false, controllerState.m_controllerID );
		SetButtonState( XBOX_BUTTON_Y, ( BIT_Y_BUTTON & xboxInputState.Gamepad.wButtons ) ? true : false, controllerState.m_controllerID );
		SetButtonState( XBOX_BUTTON_BUMPER_LEFT, ( BIT_LEFT_BUMPER_BUTTON & xboxInputState.Gamepad.wButtons ) ? true : false, controllerState.m_controllerID );
		SetButtonState( XBOX_BUTTON_BUMPER_RIGHT, ( BIT_RIGHT_BUMPER_BUTTON & xboxInputState.Gamepad.wButtons ) ? true : false, controllerState.m_controllerID );
		SetButtonState( XBOX_BUTTON_STICK_LEFT, ( BIT_LEFT_STICK_BUTTON & xboxInputState.Gamepad.wButtons ) ? true : false, controllerState.m_controllerID );
		SetButtonState( XBOX_BUTTON_STICK_RIGHT, ( BIT_RIGHT_STICK_BUTTON & xboxInputState.Gamepad.wButtons ) ? true : false, controllerState.m_controllerID );
		SetButtonState( XBOX_BUTTON_BACK, ( BIT_BACK_BUTTON & xboxInputState.Gamepad.wButtons ) ? true : false, controllerState.m_controllerID );
		SetButtonState( XBOX_BUTTON_START, ( BIT_START_BUTTON & xboxInputState.Gamepad.wButtons ) ? true : false, controllerState.m_controllerID );
		SetButtonState( XBOX_BUTTON_DPAD_NORTH, ( BIT_DPAD_NORTH_BUTTON & xboxInputState.Gamepad.wButtons ) ? true : false, controllerState.m_controllerID );
		SetButtonState( XBOX_BUTTON_DPAD_SOUTH, ( BIT_DPAD_SOUTH_BUTTON & xboxInputState.Gamepad.wButtons ) ? true : false, controllerState.m_controllerID );
		SetButtonState( XBOX_BUTTON_DPAD_EAST, ( BIT_DPAD_EAST_BUTTON & xboxInputState.Gamepad.wButtons ) ? true : false, controllerState.m_controllerID );
		SetButtonState( XBOX_BUTTON_DPAD_WEST, ( BIT_DPAD_WEST_BUTTON & xboxInputState.Gamepad.wButtons ) ? true : false, controllerState.m_controllerID );
		SetButtonState( XBOX_BUTTON_DPAD_NORTHEAST, ( BIT_DPAD_NORTHEAST_BUTTON & xboxInputState.Gamepad.wButtons ) ? true : false, controllerState.m_controllerID );
		SetButtonState( XBOX_BUTTON_DPAD_NORTHWEST, ( BIT_DPAD_NORTHWEST_BUTTON & xboxInputState.Gamepad.wButtons ) ? true : false, controllerState.m_controllerID );
		SetButtonState( XBOX_BUTTON_DPAD_SOUTHEAST, ( BIT_DPAD_SOUTHEAST_BUTTON & xboxInputState.Gamepad.wButtons ) ? true : false, controllerState.m_controllerID );
		SetButtonState( XBOX_BUTTON_DPAD_SOUTHWEST, ( BIT_DPAD_SOUTHWEST_BUTTON & xboxInputState.Gamepad.wButtons ) ? true : false, controllerState.m_controllerID );

		SetButtonState( XBOX_BUTTON_TRIGGER_LEFT, xboxInputState.Gamepad.bLeftTrigger > 127 ? true : false, controllerState.m_controllerID );
		SetButtonState( XBOX_BUTTON_TRIGGER_RIGHT, xboxInputState.Gamepad.bRightTrigger > 127 ? true : false, controllerState.m_controllerID );

		float leftStickX = NormalizeXboxControllerStickValue( xboxInputState.Gamepad.sThumbLX );
		float leftStickY = NormalizeXboxControllerStickValue( xboxInputState.Gamepad.sThumbLY );
		float rightStickX = NormalizeXboxControllerStickValue( xboxInputState.Gamepad.sThumbRX );
		float rightStickY = NormalizeXboxControllerStickValue( xboxInputState.Gamepad.sThumbRY );

		controllerState.m_normalizedStickStates[ XBOX_STICK_LEFT ] = Vector2( leftStickX, leftStickY );
		controllerState.m_normalizedStickStates[ XBOX_STICK_RIGHT ] = Vector2( rightStickX, rightStickY );

		float normalizedLeftTrigger = NormalizeXboxControllerTriggerValue( xboxInputState.Gamepad.bLeftTrigger );
		float normalizedRightTrigger = NormalizeXboxControllerTriggerValue( xboxInputState.Gamepad.bRightTrigger );

		controllerState.m_normalizedTriggerStates[ XBOX_TRIGGER_LEFT ] = normalizedLeftTrigger;
		controllerState.m_normalizedTriggerStates[ XBOX_TRIGGER_RIGHT ] = normalizedRightTrigger;
	}
	else if( errorStatus == ERROR_DEVICE_NOT_CONNECTED )
		controllerState.m_isPluggedIn = false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void InputSystem::SetXboxStickDeadzoneThreshold( float deadzoneThreshold )
{
	m_stickDeadzoneThreshold = deadzoneThreshold;
}
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
float InputSystem::NormalizeXboxControllerStickValue( short stickMagnitude )
{
	float normalizedMagnitude = RangeMap( (float) stickMagnitude, -32767.0f, 32768.0f, -1.0f, 1.0f );

	if( fabs( normalizedMagnitude ) < m_stickDeadzoneThreshold )
		normalizedMagnitude = 0.0f;

	return normalizedMagnitude;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
float InputSystem::NormalizeXboxControllerTriggerValue(unsigned char triggerMagnitude)
{
	float normalizedMagnitude = RangeMap( (float) triggerMagnitude, 0.0f, 255.0f, 0.0f, 1.0f );

	return normalizedMagnitude;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void InputSystem::Update()
{
    ResetMouseCursor();

	for( unsigned int keyIndex = 0; keyIndex < NUMBER_OF_VIRTUAL_KEYS; ++keyIndex )
		m_keyDownStates[ keyIndex ].m_wasJustReleased = false;

	for( int buttonIndex = 0; buttonIndex < XBOX_NUMBER_OF_BUTTONS; ++buttonIndex )
	{
		for( int controllerIndex = 0; controllerIndex < MAX_NUMBER_OF_XBOX_CONTROLLERS; ++controllerIndex )
			m_xboxContollerStates[ controllerIndex ].m_buttonDownStates[ buttonIndex ].m_wasJustReleased = false;
	}

    for (unsigned int buttonIndex = 0; buttonIndex < MAX_NUMBER_OF_MOUSE_BUTTONS; ++buttonIndex)
        m_mouseButtonStates[buttonIndex].m_wasJustReleased = false;

	ProcessKeyboardInput();
	ProcessXboxControllersInput();
}


