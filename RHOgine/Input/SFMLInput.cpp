#include "SFMLInput.h"

#include "../defines.h"

#include "../RHOStrings.h"

using namespace rho;

FIRST_COME_SINGLETON_INSTANCE(SFMLInput);

///////////////////////////////////////////////////////////////////////////////

SFMLInput::SFMLInput()
:	m_keyMap( sf::Keyboard::KeyCount, false )
{
    auto mouse_pos = sf::Mouse::getPosition();
    m_prevMouseX = mouse_pos.x;
    m_prevMouseY = mouse_pos.y;
}

///////////////////////////////////////////////////////////////////////////////

SFMLInput::~SFMLInput()
{
}

///////////////////////////////////////////////////////////////////////////////

Event SFMLInput::makeKeyPressedEvent( int key_code, bool alt, bool control, bool shift )
{
    Event event( hs::KeyPressedString );

	event.setArgInt( hs::key_code, key_code );
	event.setArgBool( hs::alt, alt );
	event.setArgBool( hs::control, control );
	event.setArgBool( hs::shift, shift );

    return event;
}

///////////////////////////////////////////////////////////////////////////////

Event SFMLInput::makeKeyDownEvent( int key_code )
{
	Event event( hs::KeyDownString );

	event.setArgInt( hs::key_code, key_code );

	return event;
}

///////////////////////////////////////////////////////////////////////////////

Event SFMLInput::makeKeyReleasedEvent( int key_code, bool alt, bool control, bool shift )
{
    Event event( hs::KeyReleasedString );

	event.setArgInt( hs::key_code, key_code );
	event.setArgBool( hs::alt, alt );
	event.setArgBool( hs::control, control );
	event.setArgBool( hs::shift, shift );

    return event;
}

///////////////////////////////////////////////////////////////////////////////

Event SFMLInput::makemouseWheelMoveEvent( int delta, int x, int y )
{
    Event event( hs::MouseWheelMoveString );

	event.setArgInt( hs::delta, delta );
	event.setArgInt( hs::x, x );
	event.setArgInt( hs::y, y );

    return event;
}

///////////////////////////////////////////////////////////////////////////////

Event SFMLInput::makeMouseButtonPressedEvent( int button_code, int x, int y )
{
    Event event( hs::MouseButtonPressedString );

	event.setArgInt( hs::button_code, button_code );
	event.setArgInt( hs::x, x );
	event.setArgInt( hs::y, y );

    return event;
}

///////////////////////////////////////////////////////////////////////////////

Event SFMLInput::makeMouseButtonReleasedEvent( int button_code, int x, int y )
{
    Event event( hs::MouseButtonReleasedString );

	event.setArgInt( hs::button_code, button_code );
	event.setArgInt( hs::x, x );
	event.setArgInt( hs::y, y );

    return event;
}

///////////////////////////////////////////////////////////////////////////////

Event SFMLInput::makeMouseMoveEvent( int x, int y, int rel_x, int rel_y )
{
    Event event( hs::MouseMovedString );

	event.setArgInt( hs::x, x );
	event.setArgInt( hs::y, y );
	event.setArgInt( hs::RelativeX, rel_x );
	event.setArgInt( hs::RelativeY, rel_y );

    return event;
}

///////////////////////////////////////////////////////////////////////////////

Event SFMLInput::makeJoystickButtonPressedEvent ( int joystick_code, int joystick_button_code )
{
    Event event( hs::JoystickPressedString );

	event.setArgInt( hs::joystick_code, joystick_code );
	event.setArgInt( hs::joystick_button_code, joystick_button_code );

    return event;
}

///////////////////////////////////////////////////////////////////////////////

Event SFMLInput::makeJoystickButtonReleasedEvent ( int joystick_code, int joystick_button_code )
{
    Event event( hs::JoystickReleasedString );

	event.setArgInt( hs::joystick_code, joystick_code );
	event.setArgInt( hs::joystick_button_code, joystick_button_code );

    return event;
}

///////////////////////////////////////////////////////////////////////////////

Event SFMLInput::makeJoystickMoveEvent ( int joystick_code, int joystick_axis_code, float joystick_axis_position )
{
    Event event( hs::JoystickMovedString );

	event.setArgInt( hs::joystick_code, joystick_code );
	event.setArgInt( hs::joystick_axis_code, joystick_axis_code );
	event.setArgFloat( hs::joystick_axis_position, joystick_axis_position );

    return event;
}

///////////////////////////////////////////////////////////////////////////////

bool SFMLInput::isEventInput( sf::Event const & sfml_event )
{
	switch ( sfml_event.type )
	{
    case sf::Event::KeyPressed:
    case sf::Event::KeyReleased:
    case sf::Event::MouseWheelMoved:
    case sf::Event::MouseButtonPressed:
    case sf::Event::MouseButtonReleased:
    case sf::Event::MouseMoved:
    case sf::Event::JoystickButtonPressed:
    case sf::Event::JoystickButtonReleased:
    case sf::Event::JoystickMoved:
		return true;
        break;
	default:
		return false;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////

void SFMLInput::HandleSFMLInputEvent( sf::Event const & sfml_event )
{
	Event game_event;
	// Check event type
	switch ( sfml_event.type ) // If input, what type of input
	{// Make New Event object
    case sf::Event::KeyPressed:
		if ( sfml_event.key.code == sf::Keyboard::Unknown )	// Ignore this key
			break;

		game_event = makeKeyPressedEvent(
			sfml_event.key.code,
			sfml_event.key.alt,
			sfml_event.key.control,
			sfml_event.key.shift );

		m_keyMap[ sfml_event.key.code ] = true;
        break;
    case sf::Event::KeyReleased:
		if ( sfml_event.key.code == sf::Keyboard::Unknown )	// Ignore this key
			break;

		game_event = makeKeyReleasedEvent(
			sfml_event.key.code,
			sfml_event.key.alt,
			sfml_event.key.control,
			sfml_event.key.shift );

		m_keyMap[ sfml_event.key.code ] = false;
        break;
    case sf::Event::MouseWheelMoved:
		game_event = makemouseWheelMoveEvent(
			sfml_event.mouseWheel.delta,
			sfml_event.mouseWheel.x,
			sfml_event.mouseWheel.y );
        break;
    case sf::Event::MouseButtonPressed:
		game_event = makeMouseButtonPressedEvent(
			sfml_event.mouseButton.button,
			sfml_event.mouseButton.x,
			sfml_event.mouseButton.y );
        break;
    case sf::Event::MouseButtonReleased:
		game_event = makeMouseButtonReleasedEvent(
			sfml_event.mouseButton.button,
			sfml_event.mouseButton.x,
			sfml_event.mouseButton.y );
        break;
    case sf::Event::MouseMoved:
        {
            game_event = makeMouseMoveEvent(
                sfml_event.mouseMove.x,
                sfml_event.mouseMove.y,
                sfml_event.mouseMove.x - m_prevMouseX,
                sfml_event.mouseMove.y - m_prevMouseY );

            m_prevMouseX = sfml_event.mouseMove.x;
            m_prevMouseY = sfml_event.mouseMove.y;
        }
        break;
    case sf::Event::JoystickButtonPressed:
        game_event = makeJoystickButtonPressedEvent(
            sfml_event.joystickButton.joystickId,
            sfml_event.joystickButton.button );
        break;
    case sf::Event::JoystickButtonReleased:
        game_event = makeJoystickButtonReleasedEvent(
            sfml_event.joystickButton.joystickId,
            sfml_event.joystickButton.button );
        break;
    case sf::Event::JoystickMoved:
        game_event = makeJoystickMoveEvent(
            sfml_event.joystickMove.joystickId,
            sfml_event.joystickMove.axis,
            sfml_event.joystickMove.position );
        break;
    default:
        // Throw error, we shouldn't have been given this event
        assert( 0 && "Unkown Input Event" );
        break;
	}

	// Queue it into event manager
	broadcastEvent( game_event );
}

///////////////////////////////////////////////////////////////////////////////

InputCOR & SFMLInput::getCOR()
{
    return m_chainOfResp;
}

///////////////////////////////////////////////////////////////////////////////

InputCOR const & SFMLInput::getCOR() const
{
    return m_chainOfResp;
}

///////////////////////////////////////////////////////////////////////////////

void SFMLInput::broadcastEvent( rho::Event const & input_event )
{
	// First, give to to COR, if that doesn't handles it, then broadcast to the world!
	if ( ! m_chainOfResp.sendDownChain( input_event ) )
	{
		// Queue it into event manager
		EventManager::GetInstance().QueueEvent( input_event );
	}
}

///////////////////////////////////////////////////////////////////////////////

void SFMLInput::update( sf::Time const & elapsed_time )
{
	for ( int key = sf::Keyboard::A ;
          key < sf::Keyboard::KeyCount ;
          ++key )
	{
		if ( m_keyMap[ key ] )	// If the key is down
		{
			// queue Key Pressed Event
			broadcastEvent(  makeKeyDownEvent( (sf::Keyboard::Key)key ) );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
