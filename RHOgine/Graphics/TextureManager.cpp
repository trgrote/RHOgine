#include "TextureManager.h"

#include <cassert>

using namespace rho;

FIRST_COME_SINGLETON_INSTANCE(TextureManager);

///////////////////////////////////////////////////////////////////////////////

TextureManager::TextureManager()
{
}

///////////////////////////////////////////////////////////////////////////////

TextureManager::~TextureManager()
{
}

///////////////////////////////////////////////////////////////////////////////

pTexture TextureManager::getTexture( HashString const & texture_name )
{
	TextureMap::iterator texture_pos = m_textureMap.find( texture_name );
    pTexture p_texture;

	if ( texture_pos != m_textureMap.end() )	// If we did find it
	{
		p_texture = texture_pos->second;
	}
	else
	{
		p_texture = pTexture( new sf::Texture );
		if ( ! p_texture->loadFromFile( texture_name.getString() ) ) 	// If load fails, then use default texture
		{
			// Right now, fuck shit up
			return pTexture();
		}

		m_textureMap[ texture_name ] = p_texture;   // Add to map
	}

	return p_texture;
}

///////////////////////////////////////////////////////////////////////////////

void TextureManager::checkForUnused()
{
	for ( TextureMap::iterator iter = m_textureMap.begin();
		  iter != m_textureMap.end();
		  ++iter )
	{
		if ( (*iter).second.use_count() == 1 )	// If we are the only holder of this bullshit
		{
			iter = m_textureMap.erase( iter );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
