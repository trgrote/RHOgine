#ifndef MISC_COMMANDS_H
#define MISC_COMMANDS_H

#include "IConsoleCommand.h"
#include <SFML/Graphics.hpp>

namespace rho
{

namespace commands
{

/// Command that Exits the Game 
class ExitCommand : public rho::IConsoleCommand
{
public:
    sf::RenderWindow & m_window;

    ExitCommand( sf::RenderWindow & window );
    virtual ~ExitCommand();

    virtual std::string getName() const;
    virtual std::string getDescription() const;
    virtual std::string execute( std::list< std::string > const & arg_list );
};

/// Command that just echos all the arguments given to it
class EchoCommand : public rho::IConsoleCommand
{
public:
    virtual std::string getName() const;

    virtual std::string getDescription() const;
    virtual std::string execute( std::list< std::string > const & arg_list );
};

}

}

#endif
