//=================================================================================
// XboxController.hpp
// Author: Tyler George
// Date  : January 29, 2015
//=================================================================================

#pragma once

#ifndef __included_XboxController__
#define __included_XboxController__

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#include "Engine/Math/Vector2.hpp"

///---------------------------------------------------------------------------------
/// Xbox Controller Constants
///---------------------------------------------------------------------------------
const unsigned short	BIT_A_BUTTON				= 0x1000;
const unsigned short	BIT_B_BUTTON				= 0x2000;
const unsigned short	BIT_X_BUTTON				= 0x4000;
const unsigned short	BIT_Y_BUTTON				= 0x8000;
const unsigned short	BIT_RB_BUTTON				= 0x0200;
const unsigned short	BIT_LB_BUTTON				= 0x0100;
const unsigned short	BIT_RS_BUTTON				= 0x0080;
const unsigned short	BIT_LS_BUTTON				= 0x0040;
const unsigned short	BIT_START_BUTTON			= 0x0010;
const unsigned short	BIT_BACK_BUTTON				= 0x0020;
const unsigned short	BIT_DPAD_NORTH_BUTTON		= 0x0001;
const unsigned short	BIT_DPAD_SOUTH_BUTTON		= 0x0002;
const unsigned short	BIT_DPAD_WEST_BUTTON		= 0x0004;
const unsigned short	BIT_DPAD_EAST_BUTTON		= 0x0008;
const unsigned short	BIT_DPAD_NORTHEAST_BUTTON	= 0x0009;
const unsigned short	BIT_DPAD_NORTHWEST_BUTTON	= 0x0005;
const unsigned short	BIT_DPAD_SOUTHEAST_BUTTON	= 0x000a;
const unsigned short	BIT_DPAD_SOUTHWEST_BUTTON	= 0x0006;

const float				STICK_MIN_VALUE				= -32768.0f;
const float				STICK_MAX_VALUE				= 32767.0f;

const float				STICK_DEADZONE_THRESHOLD	= 0.25f;

const float				TRIGGER_MIN_VALUE			= 0.0f;
const float				TRIGGER_MAX_VALUE			= 255.0f;




///---------------------------------------------------------------------------------
/// Connection
///---------------------------------------------------------------------------------
bool IsControllerConnected( const int controllerNumber );
DWORD GetControllerState( const int controllerNumber, XINPUT_STATE& xboxControllerState );

///---------------------------------------------------------------------------------
/// Buttons
///---------------------------------------------------------------------------------
bool IsAButtonDown( const int controllerNumber );
bool IsBButtonDown( const int controllerNumber );
bool IsXButtonDown( const int controllerNumber );
bool IsYButtonDown( const int controllerNumber );
bool IsRBButtonDown( const int controllerNumber );
bool IsLBButtonDown( const int controllerNumber );
bool IsRSButtonDown( const int controllerNumber );
bool IsLSButtonDown( const int controllerNumber );
bool IsStartButtonDown( const int controllerNumber );
bool IsBackButtonDown( const int controllerNumber );
bool IsDpadNorthButtonDown( const int controllerNumber );
bool IsDpadSouthButtonDown( const int controllerNumber );
bool IsDpadWestButtonDown( const int controllerNumber );
bool IsDpadEastButtonDown( const int controllerNumber );
bool IsDpadNorthEastButtonDown( const int controllerNumber );
bool IsDpadNorthWestButtonDown( const int controllerNumber );
bool IsDpadSouthEastButtonDown( const int controllerNumber );
bool IsDpadSouthWestButtonDown( const int controllerNumber );

///---------------------------------------------------------------------------------
/// Sticks and Triggers
///---------------------------------------------------------------------------------
const Vector2	GetNormalizedLeftStickInput( const int controllerNumber );
const Vector2	GetNormalizedRightStickInput( const int controllerNumber );
float			GetNormalizedLeftTriggerInput( const int controllerNumber );
float			GetNormalizedRightTriggerInput( const int controllerNumber );


///---------------------------------------------------------------------------------
/// Vibration
///---------------------------------------------------------------------------------
void Vibrate( const int controllerNumber, int leftVal, int rightVal );


//	/*printf( "  wButtons=%5d (0x%08x),\n", xboxControllerState.Gamepad.wButtons, xboxControllerState.Gamepad.wButtons );
	//	printf( "  isXButtonDown=%s\n", isXButtonDown ? "Yes it is" : "No it is not" );
	//	printf( "  bLeftTrigger=%3d, bRightTrigger=%3d\n", xboxControllerState.Gamepad.bLeftTrigger, xboxControllerState.Gamepad.bRightTrigger );
	//	printf( "  sThumbLX=%d\n", xboxControllerState.Gamepad.sThumbLX );
	//	printf( "  sThumbLY=%d\n", xboxControllerState.Gamepad.sThumbLY );
	//	printf( "  sThumbRX=%d\n", xboxControllerState.Gamepad.sThumbRX );
	//	printf( "  sThumbRY=%d\n", xboxControllerState.Gamepad.sThumbRY );
	//	printf( "\n" );*/
	//}
#endif