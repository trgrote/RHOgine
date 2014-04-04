#ifndef SFML_INPUT_H
#define SFML_INPUT_H

#include <vector>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include "InputCOR.h"
#include "../EventManager/EventManager.h"

#include "../FirstComeSingleton.h"

namespace rho
{

/// Input System
class SFMLInput : public FirstComeSingleton< SFMLInput >
{
// STATIC
public:
	static bool isEventInput( sf::Event const & event );
private:
	static Event makeKeyPressedEvent( int key_code, bool alt, bool control, bool shift );
	static Event makeKeyDownEvent( int key_code );
	static Event makeKeyReleasedEvent( int key_code, bool alt, bool control, bool shift );
	static Event makemouseWheelMoveEvent( int delta, int x, int y );
	static Event makeMouseButtonPressedEvent( int button_code, int x, int y );
	static Event makeMouseButtonReleasedEvent( int button_code, int x, int y );
	static Event makeMouseMoveEvent( int x, int y, int rel_x, int rel_y );
	static Event makeJoystickButtonPressedEvent ( int joystick_code, int joystick_button_code );
	static Event makeJoystickButtonReleasedEvent( int joystick_code, int joystick_button_code );
	static Event makeJoystickMoveEvent( int joystick_code, int joystick_axis_code, float joystick_axis_position );
// END OF STATIC
private:
	typedef std::vector<bool> BoolVector;
	BoolVector m_keyMap;

	/// Chain of Responsibility
	InputCOR m_chainOfResp;

	void broadcastEvent( rho::Event const & input_event );

	int m_prevMouseX, m_prevMouseY;

public:

	/// \brief Mouse Input Event Types
	enum eInputEvent
	{
		MOUSE		= 0,
		KEYBOARD,
		JOYSTICK
	};

	SFMLInput();
	virtual ~SFMLInput();

	InputCOR 		& getCOR();
	InputCOR const 	& getCOR() const;

	void HandleSFMLInputEvent( sf::Event const & event );

	/**
	  *	Updates the Input System.
	  * This sends out events for downed keys
	  *	\arg elapsed_time Elapsed Time since previous frame
	  */
	void update( sf::Time const & elapsed_time );
};

}

#endif
