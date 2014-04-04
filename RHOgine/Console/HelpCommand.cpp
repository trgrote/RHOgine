#include "HelpCommand.h"
#include <string>

using namespace rho;

std::string HelpCommand::getName() const
{
	return "help";
}

std::string HelpCommand::getDescription() const
{
	return "Lists all available commands";
}

std::string HelpCommand::execute( std::list< std::string > const & arg_list )
{
	// if we have an argument, then search the availble arguments that start with the argument
	std::string command_begining = ( arg_list.size() > 0 ? arg_list.front() : "" );

	std::list< pIConsoleCommand > commands = m_pConsole->getCommandsLike( command_begining );

	std::string rval = "Listing All Commands:\n";

	// then foreach of those, list it's name and it's description, ya butthead.
	for ( std::list< pIConsoleCommand >::const_iterator iter = commands.cbegin();
		  iter != commands.cend();
		  ++iter )
	{
		pIConsoleCommand const p_command = (*iter);

		// Start with tab
		std::string line = "";

		line += p_command->getName();

        for ( int i = line.size() ; i < m_kNumCharsForName ; ++i  )
        {
            line += " ";
        }
		line += p_command->getDescription();

		line += "\n";

		rval += line;
	}

	return rval;
}
