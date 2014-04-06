#include "MiscCommands.h"

#include "../RHOgine.h"

using namespace rho;
using namespace commands;

// Exit Command

ExitCommand::ExitCommand( sf::RenderWindow & window )
:   m_window( window )
{

}

ExitCommand::~ExitCommand()
{

}

std::string ExitCommand::getName() const
{
	return "exit";
}

std::string ExitCommand::getDescription() const
{
	return "Exit the program";
}

std::string ExitCommand::execute( std::list< std::string > const & arg_list )
{
	m_window.close();
	return "Exiting ...\n";
}

// Echo Command

std::string EchoCommand::getName() const
{
	return "echo";
}

std::string EchoCommand::getDescription() const
{
	return "Echo the arguments given";
}

std::string EchoCommand::execute( std::list< std::string > const & arg_list )
{
	std::string output;

	for ( std::list< std::string >::const_iterator iter = arg_list.cbegin();
		  iter != arg_list.cend();
		  ++iter )
	{
		output += (*iter);
		output += " ";
	}

	return output;
}

// toggle Frame rate command
// Echo Command

std::string ToggleFPSCommand::getName() const
{
	return "toggleFPS";
}

std::string ToggleFPSCommand::getDescription() const
{
	return "Toggle On/Off the Framerate counter";
}

std::string ToggleFPSCommand::execute( std::list< std::string > const & arg_list )
{
	std::string output;

	if ( rho::cRHOgine::GetInstance().toggleFPSDisplay() )
	{
		output = "FPS Counter ON";
	}
	else
	{
		output = "FPS Counter OFF";
	}

	return output;
}
