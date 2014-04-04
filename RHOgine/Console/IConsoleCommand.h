#ifndef ICONSOLE_COMMAND_H
#define ICONSOLE_COMMAND_H

#include <string>
#include <list>

namespace rho
{

/**
  *	\brief Interface for console commands to inherit.
  */
class IConsoleCommand
{
public:
	/**
	  *	\brief Return the name of this command
	  *	\return name of this command
	  */
	virtual std::string getName() const = 0;
	
	/**
	  *	\brief Return a description of this command.
	  *	This will be used in commands like help and showallcommands to 
	  *	display information about all or one console command.
	  *	\return command description
	  */
	virtual std::string getDescription() const = 0;
	
	/**
	  *	\brief Execute this command and return the output to
	  *	be displayed in the console.
	  *	\arg arg_list The Arguments given to this command.
	  *	\return Output to display to console.
	  *	\note arg_list[0] is not the name of the command. It is literally the
	  *		first argument.
	  */
	virtual std::string execute( std::list< std::string > const & arg_list ) = 0;
	
	virtual ~IConsoleCommand(){}
};

typedef IConsoleCommand * pIConsoleCommand;

}

#endif
