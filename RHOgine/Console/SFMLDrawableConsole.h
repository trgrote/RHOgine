#ifndef SFML_DRAWABLE_CONSOLE_H
#define SFML_DRAWABLE_CONSOLE_H

#include <SFML/Graphics.hpp>

#include "BaseConsole.h"

namespace rho
{

/// Drawble, instance-able console for sfml
class SFMLDrawableConsole : public BaseConsole, public sf::Drawable
{
private:
	bool m_initialized;
    sf::RectangleShape m_consoleBox;
    sf::Font m_font;
    sf::Text m_text;
	sf::Text m_historyText;
		
	int m_bufferXPos;
	int m_bufferYPos;
	
	/// Builds the History Text displayed above the current console
	void buildHistoryText();
	
    virtual void OnBufferChanged(std::string const & new_buffer );
	virtual void OnLogAdded( std::list< std::string > const & messages );

public:
	SFMLDrawableConsole();
    SFMLDrawableConsole( int const & window_width, int const & window_height );
    virtual ~SFMLDrawableConsole();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

}

#endif
