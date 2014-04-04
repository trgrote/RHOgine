#include "FontManager.h"

#include <cassert>

using namespace rho;

FIRST_COME_SINGLETON_INSTANCE(FontManager);

///////////////////////////////////////////////////////////////////////////////

FontManager::FontManager()
{
}

///////////////////////////////////////////////////////////////////////////////

FontManager::~FontManager()
{
}

///////////////////////////////////////////////////////////////////////////////

pFont FontManager::getFont( HashString const & font_name )
{
	FontMap::iterator font_pos = m_fontMap.find( font_name );
    pFont p_font;

	if ( font_pos != m_fontMap.end() )	// If we did find it
	{
		p_font = font_pos->second;
	}
	else
	{
		p_font = pFont( new sf::Font );
		if ( ! p_font->loadFromFile( font_name.getString() ) ) 	// If load fails, then use default font
		{
			// Right now, fuck shit up
			return pFont();
		}

		m_fontMap[ font_name ] = p_font;   // Add to map
	}

	return p_font;
}

///////////////////////////////////////////////////////////////////////////////

void FontManager::checkForUnused()
{
	for ( FontMap::iterator iter = m_fontMap.begin();
		  iter != m_fontMap.end();
		  ++iter )
	{
		if ( (*iter).second.use_count() == 1 )	// If we are the only holder of this bullshit
		{
			iter = m_fontMap.erase( iter );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
