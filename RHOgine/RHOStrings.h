/**
  *	\brief This file is intended to hold all the HashStrings that are used around the RHO Engine.
  *	It creates the namespace rho::hs to hold all these mofos for easy lookup in CodeBlocks
  *	and clearing out the rho namespace.
  */

#ifndef RHO_STRINGS_H
#define RHO_STRINGS_H

#include "HashString.h"

namespace rho
{

/// Holds Static const HashStrings
namespace hs
{
	// Interface Names
	HashString const IUpdateable				( "IUpdateable" );
	HashString const ISFMLDrawable				( "ISFMLDrawable" );
	HashString const ITransformable             ( "ITransformable" );
	HashString const ICollideable               ( "ICollideable" );

	// Holds Event Names
	HashString const GameObjCreate				( "GameObjectCreated" );
	HashString const GameObjDestroy				( "GameObjectDestroyed" );
	HashString const GameObjMoved               ( "GameObjectMoved" );
	HashString const GameObjDepthChanged        ( "GameObjectDepthChanged" );

	HashString const KeyPressedString			( "KeyPressed" );
	HashString const KeyDownString			    ( "KeyDown" );
	HashString const KeyReleasedString			( "KeyReleased" );
	HashString const MouseWheelMoveString		( "MouseWheelMoved" );
	HashString const MouseButtonPressedString	( "MouseButtonPressed" );
	HashString const MouseButtonReleasedString	( "MouseButtonReleased" );
	HashString const MouseMovedString			( "MouseMoved" );
	HashString const JoystickPressedString      ( "JoystickButtonPressed" );
	HashString const JoystickReleasedString     ( "JoystickButtonReleased" );
	HashString const JoystickMovedString        ( "JoystickMoved" );

	HashString const CameraMoved				( "CameraMoved" );
	HashString const CameraResized				( "CameraResized" );
	HashString const CameraRotated				( "CameraRotated" );

    HashString const DrawGUI                    ( "DrawGUI" );

	// Event Argument Names
	HashString const x							( "x" );
	HashString const y							( "y" );

	HashString const key_code					("key_code");
	HashString const button_code				("button_code");
	HashString const alt						("alt");
	HashString const control					("control");
	HashString const shift						("shift");
	HashString const delta						("delta");
	HashString const joystick_code              ("joystick_code");
	HashString const joystick_button_code       ("joystick_button_code");
	HashString const joystick_axis_code         ("joystick_axis_code");
	HashString const joystick_axis_position     ("joystick_axis_position");

	HashString const GameObject					("GameObject");

	/// Relative Movement on X Axis
	HashString const RelativeX					( "RelativeX" );
	/// Relative Movement on Y Axis
	HashString const RelativeY					( "RelativeY" );

	HashString const angle						( "angle" );

	HashString const RenderTarget               ( "RenderTarget" );
	HashString const DrawStates                 ( "DrawStates" );

}

}

#endif
