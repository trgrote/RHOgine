#include "BaseConsole.h"

#include <SFML/Window/Event.hpp>

#include "HelpCommand.h"

#include <iostream>

#include "../RHOStrings.h"

using namespace rho;

///////////////////////////////////////////////////////////////////////////////

BaseConsole::BaseConsole()
:	m_consoleDown( false ),
	m_bufferPrefix( "" ),
	m_historyIndex( -1 )
{
	addCommand( new HelpCommand( this ) );
}

///////////////////////////////////////////////////////////////////////////////

BaseConsole::~BaseConsole()
{
}

///////////////////////////////////////////////////////////////////////////////

pIConsoleCommand BaseConsole::getCommand( std::string const & command_name )
{
	pIConsoleCommand command = NULL;

	try
	{
		command = m_commands.at( command_name ).get();
	}
	catch (...)
	{
		command = NULL;
	}

	return command;
}

///////////////////////////////////////////////////////////////////////////////

std::list< std::string > BaseConsole::splitNewLines( std::string const & str )
{
	std::list< std::string > rval;

	std::string current;

	for ( std::string::const_iterator iter = str.cbegin() ; iter != str.cend() ; ++iter )
	{
		char const current_char = (*iter);
		// if this character is a new line, then insert
		if ( current_char == '\n' )
		{
			rval.push_back( current );
			current.clear();
		}
		else
		{
			current.push_back( current_char );
		}
	}

	// If there is anything else, push into return list
	if ( current.length() > 0 )
	{
		rval.push_back( current );
	}

	return rval;
}

///////////////////////////////////////////////////////////////////////////////

std::list< std::string > BaseConsole::splitSpaces( std::string const & str )
{
	std::list< std::string > rval;

	std::string current;

	for ( std::string::const_iterator iter = str.cbegin() ; iter != str.cend() ; ++iter )
	{
		char const current_char = (*iter);
		// if this character is a new line, then insert
		if ( current_char == ' ' )
		{
			rval.push_back( current );
			current.clear();
		}
		else
		{
			current.push_back( current_char );
		}
	}

	// If there is anything else, push into return list
	if ( current.length() > 0 )
	{
		rval.push_back( current );
	}

	return rval;
}

///////////////////////////////////////////////////////////////////////////////

std::string BaseConsole::getBufferHistory( int const & index ) const
{
	std::string rval = "";

	if ( index < 0 )	// if you gave us dumb data, return empty string
	{
		return rval;
	}

	if ( m_bufferHistory.size() == 0 )	// if we have no history, return empty string
	{
		return rval;
	}

	int current_index = 0;

	for ( std::list< std::string >::const_reverse_iterator iter = m_bufferHistory.crbegin();
		  iter != m_bufferHistory.crend();
		  ++iter )
	{
		if ( current_index == index )
		{
			rval = (*iter);
			break;
		}

		++current_index;
	}

	return rval;
}

///////////////////////////////////////////////////////////////////////////////

bool BaseConsole::isPrintable( int const & key )
{
	switch ( key )
	{
		case sf::Keyboard::A:
		case sf::Keyboard::B:
		case sf::Keyboard::C:
		case sf::Keyboard::D:
		case sf::Keyboard::E:
		case sf::Keyboard::F:
		case sf::Keyboard::G:
		case sf::Keyboard::H:
		case sf::Keyboard::I:
		case sf::Keyboard::J:
		case sf::Keyboard::K:
		case sf::Keyboard::L:
		case sf::Keyboard::M:
		case sf::Keyboard::N:
		case sf::Keyboard::O:
		case sf::Keyboard::P:
		case sf::Keyboard::Q:
		case sf::Keyboard::R:
		case sf::Keyboard::S:
		case sf::Keyboard::T:
		case sf::Keyboard::U:
		case sf::Keyboard::V:
		case sf::Keyboard::W:
		case sf::Keyboard::X:
		case sf::Keyboard::Y:
		case sf::Keyboard::Z:
		case sf::Keyboard::Num1:
		case sf::Keyboard::Num2:
		case sf::Keyboard::Num3:
		case sf::Keyboard::Num4:
		case sf::Keyboard::Num5:
		case sf::Keyboard::Num6:
		case sf::Keyboard::Num7:
		case sf::Keyboard::Num8:
		case sf::Keyboard::Num9:
		case sf::Keyboard::Num0:
		case sf::Keyboard::LBracket:
		case sf::Keyboard::RBracket:
		case sf::Keyboard::SemiColon:
		case sf::Keyboard::Comma:
		case sf::Keyboard::Period:
		case sf::Keyboard::Quote:
		case sf::Keyboard::Slash:
		case sf::Keyboard::BackSlash:
		case sf::Keyboard::Equal:
		case sf::Keyboard::Dash:
		case sf::Keyboard::Space:
		case sf::Keyboard::Add:
		case sf::Keyboard::Subtract:
		case sf::Keyboard::Multiply:
		case sf::Keyboard::Divide:
			return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////

char BaseConsole::toPrintable( int const & sfml_key, bool const & shift_down )
{
    switch ( sfml_key )
	{
		case sf::Keyboard::A:
		    return ( shift_down ? 'A' : 'a' );
		case sf::Keyboard::B:
			return ( shift_down ? 'B' : 'b' );
		case sf::Keyboard::C:
			return ( shift_down ? 'C' : 'c' );
		case sf::Keyboard::D:
			return ( shift_down ? 'D' : 'd' );
		case sf::Keyboard::E:
			return ( shift_down ? 'E' : 'e' );
		case sf::Keyboard::F:
			return ( shift_down ? 'F' : 'f' );
		case sf::Keyboard::G:
			return ( shift_down ? 'G' : 'g' );
		case sf::Keyboard::H:
			return ( shift_down ? 'H' : 'h' );
		case sf::Keyboard::I:
			return ( shift_down ? 'I' : 'i' );
		case sf::Keyboard::J:
			return ( shift_down ? 'J' : 'j' );
		case sf::Keyboard::K:
			return ( shift_down ? 'K' : 'k' );
		case sf::Keyboard::L:
			return ( shift_down ? 'L' : 'l' );
		case sf::Keyboard::M:
			return ( shift_down ? 'M' : 'm' );
		case sf::Keyboard::N:
			return ( shift_down ? 'N' : 'n' );
		case sf::Keyboard::O:
			return ( shift_down ? 'O' : 'o' );
		case sf::Keyboard::P:
			return ( shift_down ? 'P' : 'p' );
		case sf::Keyboard::Q:
			return ( shift_down ? 'Q' : 'q' );
		case sf::Keyboard::R:
			return ( shift_down ? 'R' : 'r' );
		case sf::Keyboard::S:
			return ( shift_down ? 'S' : 's' );
		case sf::Keyboard::T:
			return ( shift_down ? 'T' : 't' );
		case sf::Keyboard::U:
			return ( shift_down ? 'U' : 'u' );
		case sf::Keyboard::V:
			return ( shift_down ? 'V' : 'v' );
		case sf::Keyboard::W:
			return ( shift_down ? 'W' : 'w' );
		case sf::Keyboard::X:
			return ( shift_down ? 'X' : 'x' );
		case sf::Keyboard::Y:
			return ( shift_down ? 'Y' : 'y' );
		case sf::Keyboard::Z:
			return ( shift_down ? 'Z' : 'z' );
		case sf::Keyboard::Num1:
			return ( shift_down ? '!' : '1' );
		case sf::Keyboard::Num2:
			return ( shift_down ? '@' : '2' );
		case sf::Keyboard::Num3:
			return ( shift_down ? '#' : '3' );
		case sf::Keyboard::Num4:
			return ( shift_down ? '$' : '4' );
		case sf::Keyboard::Num5:
			return ( shift_down ? '%' : '5' );
		case sf::Keyboard::Num6:
			return ( shift_down ? '^' : '6' );
		case sf::Keyboard::Num7:
			return ( shift_down ? '&' : '7' );
		case sf::Keyboard::Num8:
			return ( shift_down ? '*' : '8' );
		case sf::Keyboard::Num9:
			return ( shift_down ? '(' : '9' );
		case sf::Keyboard::Num0:
			return ( shift_down ? ')' : '0' );
		case sf::Keyboard::LBracket:
			return ( !shift_down ? '[' : '{' );
		case sf::Keyboard::RBracket:
			return ( !shift_down ? ']' : '}' );
		case sf::Keyboard::SemiColon:
			return ( !shift_down ? ';' : ':' );
		case sf::Keyboard::Comma:
			return ( !shift_down ? ',' : '<' );
		case sf::Keyboard::Period:
			return ( !shift_down ? '.' : '>' );
		case sf::Keyboard::Quote:
			return ( !shift_down ? '\'' : '\"' );
		case sf::Keyboard::Slash:
			return ( !shift_down ? '/' : '?' );
		case sf::Keyboard::BackSlash:
			return ( !shift_down ? '\\' : '|' );
		case sf::Keyboard::Equal:
			return ( !shift_down ? '=' : '+' );
		case sf::Keyboard::Dash:
			return ( !shift_down ? '-' : '_' );
		case sf::Keyboard::Space:
			return ' ';
		case sf::Keyboard::Add:
			return '+';
		case sf::Keyboard::Subtract:
			return '-';
		case sf::Keyboard::Multiply:
			return '*';
		case sf::Keyboard::Divide:
			return '/';
	}

	return ' ';
}

///////////////////////////////////////////////////////////////////////////////

void BaseConsole::processBuffer( std::string const & buffer )
{

	// reset history index
	m_historyIndex = -1;

	// Reset saved buffer
	m_tempSavedBuffer = "";

	// if it's empty, just log it and go along, don't try to parse it
	if ( buffer == "" )
	{
		logMessage( m_bufferPrefix );
		return;
	}

	// record buffer in buffer history
	m_bufferHistory.push_back( buffer );

	std::list< std::string > tokens = splitSpaces( buffer );

	// the fist token should be the command
	std::string command_name = tokens.front();

	// get command by this name
	auto iter = m_commands.find( command_name );

	std::string output = m_bufferPrefix;	// prepend the command prefix before
	output += buffer;
	output += "\n";

	if ( iter != m_commands.end() )
	{
		// Sweet, we found it, shoot off the commands
		pIConsoleCommand command = iter->second.get();

		std::list< std::string > args = tokens;

		args.pop_front();

		output += command->execute( args );
	}
	else
	{
		output += "\"";
		output += command_name;
		output += "\"";
		output += " ";
		output += "What the fuck are you talking about?\n";
	}

    std::cout << buffer << std::endl;
	logMessage( output );
}

///////////////////////////////////////////////////////////////////////////////

std::list< std::string > BaseConsole::getCommandNamesLike( std::string const & command_so_far ) const
{
	std::list< std::string > rval;

	for ( auto iter = m_commands.cbegin();
		  iter != m_commands.cend();
		  ++iter )
	{
		std::string command = iter->first;

		// If this command starts with the command_so_far, then add to the rval list
		size_t find_index = command.find( command_so_far );

		if ( find_index != std::string::npos && find_index == 0 )
		{
			rval.push_back( command );
		}
	}

	return rval;
}

///////////////////////////////////////////////////////////////////////////////

std::list< pIConsoleCommand > BaseConsole::getCommandsLike( std::string const & command_so_far ) const
{
	std::list< pIConsoleCommand > rval;

	for ( auto iter = m_commands.cbegin();
		  iter != m_commands.cend();
		  ++iter )
	{
		std::string command = iter->first;

		// If this command starts with the command_so_far, then add to the rval list
		size_t find_index = command.find( command_so_far );

		if ( find_index != std::string::npos && find_index == 0 )
		{
			rval.push_back( iter->second.get() );
		}
	}

	return rval;
}

///////////////////////////////////////////////////////////////////////////////

/// Add a Console Command to the console
void BaseConsole::addCommand( pIConsoleCommand command )
{
	// Make Sure command isn't already added
	// add command
	m_commands[ command->getName() ] = std::unique_ptr< IConsoleCommand> ( command );
}

///////////////////////////////////////////////////////////////////////////////

/// Log a message to be displayed in the console
void BaseConsole::logMessage( std::string const & message )
{
	if ( message == "")
	{
		return;
	}

	// Write Message to Command/Log History
	std::list< std::string > all_messages = splitNewLines( message );

	if ( all_messages.size() == 0 )
	{
		return;
	}

	for ( std::list< std::string >::const_iterator iter = all_messages.cbegin();
		  iter != all_messages.cend();
		  ++iter )
	{
		// add each line to the log messages
		m_consoleLog.push_back( (*iter) );
	}

	OnLogAdded( all_messages );
}

///////////////////////////////////////////////////////////////////////////////

bool BaseConsole::HandleInput( rho::Event const & input_event )
{
	// If this isn't a key press, then don't give a fuck
	if ( input_event.EventType() != hs::KeyPressedString &&
         input_event.EventType() != hs::KeyDownString &&
         input_event.EventType() != hs::KeyReleasedString )
	{
		return false;
	}

	// Respond to input
	if ( isConsoleDown() )	// if the console is down, respond to typing
	{
        // Even though we don't handle these, still say we do
        if ( input_event.EventType() == hs::KeyDownString ||
             input_event.EventType() == hs::KeyReleasedString )
        {
            return true;
        }

		int key = input_event.getArgInt( hs::key_code );
		// Log key presses into buffer

		if ( key == sf::Keyboard::Up )
		{
			// previous history entry
			if ( m_historyIndex < (int)m_bufferHistory.size() - 1 ) // if we have a history to go to
			{
				// if the current index is -1, then save the buffer into the temp buffer thingy
				if ( m_historyIndex == -1 )
				{
					m_tempSavedBuffer = m_buffer;
				}

				++m_historyIndex;	// increment history index
				// set buffer to buffer at that history index
				m_buffer = getBufferHistory( m_historyIndex );

				OnBufferChanged( m_buffer );
			}
		}
		if ( key == sf::Keyboard::Down )
		{
			// we can go down, if the history index is currently greater than -1
			if ( m_historyIndex > -1 )
			{
				--m_historyIndex;	// decrement history index

				// if history index is -1, then return to saved buffer
				if ( m_historyIndex == -1 )
				{
					m_buffer = m_tempSavedBuffer;
					OnBufferChanged( m_buffer );

					m_tempSavedBuffer = "";		// set saved buffer as empty string
				}
				else			// else, return the history index at the neq history index
				{
					m_buffer = getBufferHistory( m_historyIndex );

					OnBufferChanged( m_buffer );
				}
			}
		}

		// if tab is pressed, attempt to autocomplete, if ambigious print out possibilities below
		if ( key == sf::Keyboard::Tab )
		{
			// make sure arguments haven't been started to be entered
			if ( m_buffer.find( ' ' ) != std::string::npos )
			{
				return true;	// gtfo
			}

			// Get all commands that start the same way
			std::list< std::string > similar_commands = getCommandNamesLike( m_buffer );

			if ( similar_commands.size() > 1 )
			{
				// print out all possible commands in a easy to read manner
				// I would say place four spaces between each command and after three commands, add a newline
				std::string output;

				int current_commands_on_line = 0;
				int const max_commands_per_line = 3;

				for ( std::list< std::string >::const_iterator iter = similar_commands.cbegin();
					  iter != similar_commands.cend();
					  ++iter )
				{
					output += (*iter);

					++current_commands_on_line;

					// if we have enough commands
					if ( current_commands_on_line % max_commands_per_line == 0 )
					{
						// append newline
						output += "\n";
					}
					else
					{
						output += "    ";
					}
				}

				// Log entry
				logMessage( output );
			}
			else if ( similar_commands.size() == 1 )
			{
				// else replace the buffer with the only possible command it could be
				m_buffer = similar_commands.front();
				m_buffer += " ";
				OnBufferChanged( m_buffer );
			}
			// else, you have a bad command I don't give a shit
		}

		// If the user presses backspace, and there is something to delete, then remove the last letter
		if ( key == sf::Keyboard::BackSpace && m_buffer.length() > 0 )
		{
			// take off last letter of buffer string
			m_buffer.pop_back();

			OnBufferChanged( m_buffer );
		}

		// if enter key is pressed, process buffer
		if ( key == sf::Keyboard::Return )
		{
			// Proc
			processBuffer( m_buffer );

			// clear buffer
			m_buffer.clear();

			OnBufferChanged( m_buffer );
		}

		if ( key == sf::Keyboard::Tilde )
        {
            m_consoleDown = false;
        }

		// If all else fails
		// make sure it is a typable character, then put into buffer
		if ( isPrintable( key ) )
		{
			m_buffer += toPrintable( key, input_event.getArgBool( hs::shift ) );	// append new key
			OnBufferChanged( m_buffer );
		}

		return true;
	}
	else if ( input_event.EventType() == hs::KeyPressedString &&
            input_event.getArgInt( hs::key_code ) == sf::Keyboard::Tilde )	// Only respond to '~' key to bring down the console
	{
		// toggle console
        m_consoleDown = true;
		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////
