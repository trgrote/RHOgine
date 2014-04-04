#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <memory>

#include "../HashString.h"
#include "../FirstComeSingleton.h"

namespace rho
{

typedef std::shared_ptr< sf::Texture > pTexture;

class TextureManager : public FirstComeSingleton< TextureManager >
{
private:
	typedef std::map< HashString, pTexture > TextureMap;

	TextureMap m_textureMap;

public:

	TextureManager();
	virtual ~TextureManager();

	/**
	  *	 Gets the Texture, if it exists, else loads the texture.
	  * If Texture Fails to load, returns NULL pointer
	  *	\arg texutre_name Texutre name
	  *	\return texture shared_ptr.
	  */
	pTexture getTexture( HashString const & texture_name );

	/**
	  *	\brief Removes any currently unused textures.
	  */
	void checkForUnused();

};

}

#endif
