#ifndef BASE_CONSOLE_H
#define BASE_CONSOLE_H

#include "IConsole.h"

#include "../Input/IInputHandler.h"

#include <list>
#include <map>
#include <memory>

namespace rho
{

class BaseConsole : public IConsole, public IInputHandler
{
friend class HelpCommand;

protected:
	/// All the strings being displayed in the history
	std::list< std::string > m_consoleLog;

	/// Current Buffer Line
	std::string m_buffer;

	/// Internal flag signalling if the console is down
	bool m_consoleDown;

	/// All the console commands.
	std::map< std::string, std::unique_ptr< IConsoleCommand > > m_commands;

	/// History of all the buffers submitted.
	/// \note Not commmands, this records the buffer regardless if it's a real command, but it ignores empty buffers
	std::list< std::string > m_bufferHistory;

	/// This buffer saves our current buffer when we go through the history, so we can return to it
	/// it's basically, history entry -1
	std::string m_tempSavedBuffer;

	/// String Prefix to display before the typed buffer
	std::string m_bufferPrefix;

	/// Current History Index we are at
	/// no history is -1.
	/// 0 is previous, 1 is two commands ago, etc
	int m_historyIndex;

	/// Helper function to return the command with the given name
	pIConsoleCommand getCommand( std::string const & command_name );

	/// Get the buffer from X commands ago
	/// if no buffer exists, returns empty string
	std::string getBufferHistory( int const & index ) const;

	/// Split the string along its newlines
	static std::list< std::string > splitNewLines( std::string const & str );
	static std::list< std::string > splitSpaces( std::string const & str );

	/// Take the buffer and try to process it as a command with arguments
	void processBuffer( std::string const & buffer );

	/// return true if the key given can be printed and is typeable in a command
	static bool isPrintable( int const & key );

    static char toPrintable( int const & sfml_key, bool const & shift_down );

	/// Return commands that have the same begining as these do
	std::list< std::string > getCommandNamesLike( std::string const & command_so_far ) const;
	std::list< pIConsoleCommand > getCommandsLike( std::string const & command_so_far ) const;

	virtual void OnBufferChanged( std::string const & new_buffer ){}
	virtual void OnLogAdded( std::list< std::string > const & messages ) {}

	BaseConsole();
	virtual ~BaseConsole();

public:

	/// Add a Console Command to the console
	virtual void addCommand( pIConsoleCommand command );

	/// Log a message to be displayed in the console
	virtual void logMessage( std::string const & message );

	/// Is the console currently down and typeable
	inline virtual bool isConsoleDown() const
	{
		return m_consoleDown;
	}

	virtual bool HandleInput( rho::Event const & input_event );
};

}

#endif
