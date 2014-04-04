#ifndef ICONSOLE_H
#define ICONSOLE_H

#include "IConsoleCommand.h"

namespace rho
{

class IConsole
{
public:
	
	/// Add a Console Command to the console
	virtual void addCommand( pIConsoleCommand command ) = 0;
	
	/// Log a message to be displayed in the console
	virtual void logMessage( std::string const & message ) = 0;
	
	virtual bool isConsoleDown() const = 0;
};

}

#endif
