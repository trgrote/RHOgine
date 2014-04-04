#ifndef ISFML_DRAWABLE_H
#define ISFML_DRAWABLE_H

#include <SFML/Graphics.hpp>

#include "../HashString.h"

namespace rho
{

class ISFMLDrawable
{
public:
	/// If true, this object is drawn
	virtual bool const visible() const = 0;
	/**
	  * Returns the Depth of the Drawable Object
	  *	Depth determines when an object is drawn.
	  * The higher the value, the farther away from the screen and the earlier it's drawn
	  *	The lower the value, the close to the screen and later it's drawn.
	  *	-1 would be drawn after 1, so -1 would appear on top of 1
	  *	\return Object Depth
	  */
	virtual int const getDepth() const = 0;
	/// Draw the object
	virtual void draw( sf::RenderTarget &target, sf::RenderStates states ) = 0;

    virtual ~ISFMLDrawable(){}
};

}

#endif
