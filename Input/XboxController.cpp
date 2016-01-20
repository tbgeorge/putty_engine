//=================================================================================
// XboxController.cpp
// Author: Tyler George
// Date  : January 29, 2015
//=================================================================================

#include "Engine/Input/XboxController.hpp"
#include "Engine/Math/MathUtils.hpp"
#pragma comment( lib, "xinput" ) 


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsControllerConnected( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_DEVICE_NOT_CONNECTED )
		return false;
	return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
DWORD GetControllerState( const int controllerNumber, XINPUT_STATE& xboxControllerState )
{
	memset( &xboxControllerState, 0, sizeof( xboxControllerState ) );
	DWORD errorStatus = XInputGetState( controllerNumber, &xboxControllerState );
	return errorStatus;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsAButtonDown( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_SUCCESS )
	{
		return BIT_A_BUTTON & xboxControllerState.Gamepad.wButtons;
	}
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsBButtonDown( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_SUCCESS )
	{
		return BIT_B_BUTTON & xboxControllerState.Gamepad.wButtons;
	}
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsXButtonDown( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_SUCCESS )
	{
		return BIT_X_BUTTON & xboxControllerState.Gamepad.wButtons;
	}
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsYButtonDown( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_SUCCESS )
	{
		return BIT_Y_BUTTON & xboxControllerState.Gamepad.wButtons;
	}
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsRBButtonDown( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_SUCCESS )
	{
		return BIT_RB_BUTTON & xboxControllerState.Gamepad.wButtons;
	}
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsLBButtonDown( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_SUCCESS )
	{
		return BIT_LB_BUTTON & xboxControllerState.Gamepad.wButtons;
	}
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsRSButtonDown( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_SUCCESS )
	{
		return BIT_RS_BUTTON & xboxControllerState.Gamepad.wButtons;
	}
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsLSButtonDown( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_SUCCESS )
	{
		return BIT_LS_BUTTON & xboxControllerState.Gamepad.wButtons;
	}
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsStartButtonDown( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_SUCCESS )
	{
		return BIT_START_BUTTON & xboxControllerState.Gamepad.wButtons;
	}
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsBackButtonDown( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_SUCCESS )
	{
		return BIT_BACK_BUTTON & xboxControllerState.Gamepad.wButtons;
	}
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsDpadNorthButtonDown( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_SUCCESS )
	{
		return BIT_DPAD_NORTH_BUTTON & xboxControllerState.Gamepad.wButtons;
	}
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsDpadSouthButtonDown( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_SUCCESS )
	{
		return BIT_DPAD_SOUTH_BUTTON & xboxControllerState.Gamepad.wButtons;
	}
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsDpadWestButtonDown( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_SUCCESS )
	{
		return BIT_DPAD_WEST_BUTTON & xboxControllerState.Gamepad.wButtons;
	}
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsEastButtonDown( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_SUCCESS )
	{
		return BIT_DPAD_EAST_BUTTON & xboxControllerState.Gamepad.wButtons;
	}
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsDpadNorthEastButtonDown( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_SUCCESS )
	{
		return BIT_DPAD_NORTHEAST_BUTTON & xboxControllerState.Gamepad.wButtons;
	}
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsDpadNorthWestButtonDown( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_SUCCESS )
	{
		return BIT_DPAD_NORTHWEST_BUTTON & xboxControllerState.Gamepad.wButtons;
	}
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsDpadSouthEastButtonDown( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_SUCCESS )
	{
		return BIT_DPAD_SOUTHEAST_BUTTON & xboxControllerState.Gamepad.wButtons;
	}
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool IsDpadSouthWestButtonDown( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	if( errorStatus == ERROR_SUCCESS )
	{
		return BIT_DPAD_SOUTHWEST_BUTTON & xboxControllerState.Gamepad.wButtons;
	}
	return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
const Vector2 GetNormalizedLeftStickInput( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	Vector2 stickInput( 0.0f, 0.0f );
	if( errorStatus == ERROR_SUCCESS )
	{
		float stickInputX = (float) xboxControllerState.Gamepad.sThumbLX;
		float stickInputY = (float) xboxControllerState.Gamepad.sThumbLY;

		stickInputX = RangeMap( stickInputX, STICK_MIN_VALUE, STICK_MAX_VALUE, -1.0f, 1.0f );
		stickInputY = RangeMap( stickInputY, STICK_MIN_VALUE, STICK_MAX_VALUE, -1.0f, 1.0f );

		stickInput.SetXY( stickInputX, stickInputY );

		if( stickInput.CalcLength() < STICK_DEADZONE_THRESHOLD )
			stickInput.SetXY( 0.0f, 0.0f );

	}

	return stickInput;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
const Vector2 GetNormalizedRightStickInput( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	Vector2 stickInput( 0.0f, 0.0f );
	if( errorStatus == ERROR_SUCCESS )
	{
		float stickInputX = (float) xboxControllerState.Gamepad.sThumbRX;
		float stickInputY = (float) xboxControllerState.Gamepad.sThumbRY;

		stickInputX = RangeMap( stickInputX, STICK_MIN_VALUE, STICK_MAX_VALUE, -1.0f, 1.0f );
		stickInputY = RangeMap( stickInputY, STICK_MIN_VALUE, STICK_MAX_VALUE, -1.0f, 1.0f );

		stickInput.SetXY( stickInputX, stickInputY );

		if( stickInput.CalcLength() < STICK_DEADZONE_THRESHOLD )
			stickInput.SetXY( 0.0f, 0.0f );

	}

	return stickInput;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
float GetNormalizedRightTriggerInput( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	float triggerInput = 0.0f;
	if( errorStatus == ERROR_SUCCESS )
	{
		triggerInput = (float) xboxControllerState.Gamepad.bRightTrigger;

		triggerInput = RangeMap( triggerInput, TRIGGER_MIN_VALUE, TRIGGER_MAX_VALUE, 0.0f, 1.0f );
	}

	return triggerInput;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
float GetNormalizedLeftTriggerInput( const int controllerNumber )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = GetControllerState( controllerNumber, xboxControllerState );

	float triggerInput = 0.0f;
	if( errorStatus == ERROR_SUCCESS )
	{
		triggerInput = (float) xboxControllerState.Gamepad.bLeftTrigger;

		triggerInput = RangeMap( triggerInput, TRIGGER_MIN_VALUE, TRIGGER_MAX_VALUE, 0.0f, 1.0f );
	}

	return triggerInput;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Vibrate( const int controllerNumber, int leftVal, int rightVal )
{
	XINPUT_VIBRATION Vibration;

	memset( &Vibration, 0, sizeof(XINPUT_VIBRATION) );

	Vibration.wLeftMotorSpeed = leftVal;
	Vibration.wRightMotorSpeed = rightVal;

	XInputSetState(controllerNumber, &Vibration);
}