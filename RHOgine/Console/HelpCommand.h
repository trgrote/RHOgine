#ifndef HELP_COMMAND_H
#define HELP_COMMAND_H

#include "IConsoleCommand.h"
#include "BaseConsole.h"
#include <cassert>

namespace rho
{

class HelpCommand : public IConsoleCommand
{
protected:
	BaseConsole * m_pConsole;

	/// Number of Characters for the command name
	int const m_kNumCharsForName = 10;

public:
	HelpCommand( BaseConsole * const p_console )
	:	m_pConsole( p_console )
	{
	}
	
	virtual std::string getName() const;
	virtual std::string getDescription() const;
	virtual std::string execute( std::list< std::string > const & arg_list );
};

}

#endif
