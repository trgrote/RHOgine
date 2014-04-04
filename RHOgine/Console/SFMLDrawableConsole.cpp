#include "SFMLDrawableConsole.h"

#define HORIZONTAL_PADDING  	10
#define UPPER_PADDING  			10
#define FONT_SIZE  				20
#define FONT_NAME				"sansation.ttf"
#define MAX_LINES_ON_SCREEN		11


#include <iostream>
#include <cassert>


using namespace rho;

SFMLDrawableConsole::SFMLDrawableConsole()
:	m_initialized( false )
{
}

SFMLDrawableConsole::SFMLDrawableConsole( int const & window_width, int const & window_height )
:	m_initialized( true )
{

	m_bufferPrefix = ">> ";

	m_consoleBox.setPosition( 0 + HORIZONTAL_PADDING, 0 + UPPER_PADDING );
	m_consoleBox.setSize(
		sf::Vector2f( window_width - HORIZONTAL_PADDING * 2,
					  window_height - HORIZONTAL_PADDING - ( window_height / 2 ) ) );

	// Set the Color BLUE!
	m_consoleBox.setFillColor( sf::Color::Blue );

	m_consoleBox.setOutlineThickness( 1.0f );

	m_consoleBox.setOutlineColor( sf::Color::Red );

	m_bufferXPos = m_consoleBox.getPosition().x + 5;
	m_bufferYPos = m_consoleBox.getSize().y + m_consoleBox.getPosition().y - FONT_SIZE - 5;

	// Load Font
	m_font.loadFromFile( FONT_NAME );

	// Set Text
	m_text.setFont( m_font );

	m_text.setCharacterSize( FONT_SIZE );

	m_text.setColor( sf::Color::White );

	m_text.setPosition( m_bufferXPos, m_bufferYPos );

	m_text.setString(m_bufferPrefix);

	// Set History Text
	m_historyText.setFont( m_font );

	m_historyText.setCharacterSize( FONT_SIZE );

	m_historyText.setColor( sf::Color::Black );

	m_historyText.setPosition( m_bufferXPos, UPPER_PADDING + 5 );

	buildHistoryText();
}

SFMLDrawableConsole::~SFMLDrawableConsole()
{
}

void SFMLDrawableConsole::buildHistoryText()
{
	// Find total number of messages
	int num_logs = m_consoleLog.size();

	int start_index = num_logs - MAX_LINES_ON_SCREEN;

	int current_index = 0;

	// If we have less than the max number of logs, then place newlines to fill the space in between
	std::string output;

	// if ( num_logs < MAX_LINES_ON_SCREEN )
	for ( int i = 0 ; i < ( MAX_LINES_ON_SCREEN - num_logs ) ; ++i )
	{
		output += "\n";
	}

	for ( std::list< std::string >::const_iterator iter = m_consoleLog.cbegin() ;
		  iter != m_consoleLog.cend();
		  ++iter )
	{
		if ( current_index >= start_index )
		{
			output += (*iter);
			output += "\n";
		}
		++current_index;
	}

	m_historyText.setString( output );
}

void SFMLDrawableConsole::OnLogAdded( std::list< std::string > const & messages )
{
	assert ( m_initialized && "Using Console::OnLogAdded while uninitilized" );
	buildHistoryText();
}

void SFMLDrawableConsole::OnBufferChanged(std::string const & new_buffer )
{
	assert ( m_initialized && "Using Console::OnBufferChanged while uninitilized" );

	std::string new_buffer_with_prefix = m_bufferPrefix;
	new_buffer_with_prefix += new_buffer;

	m_text.setString( new_buffer_with_prefix );
}

void SFMLDrawableConsole::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	assert ( m_initialized && "Using Console::draw while uninitilized" );

	if ( isConsoleDown() )
	{
		// Draw Box
		target.draw( m_consoleBox );

		// Draw Buffer
		target.draw(m_text);

		// Draw History
		target.draw( m_historyText );
	}
}
