#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <memory>

#include "../HashString.h"
#include "../FirstComeSingleton.h"

namespace rho
{

typedef std::shared_ptr< sf::Font > pFont;

class FontManager : public FirstComeSingleton< FontManager >
{
private:
	typedef std::map< HashString, pFont > FontMap;

	FontMap m_fontMap;

public:

	FontManager();
	virtual ~FontManager();

	/**
	  *	 Gets the Font, if it exists, else loads the Font.
	  * If Font Fails to load, returns NULL pointer
	  *	\arg font_name Texutre name
	  *	\return Font shared_ptr.
	  */
	pFont getFont( HashString const & font_name );

	/**
	  *	\brief Removes any currently unused Fonts.
	  */
	void checkForUnused();

};

}

#endif
