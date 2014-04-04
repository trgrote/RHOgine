#ifndef DRAW_MANAGER_H
#define DRAW_MANAGER_H

#include "../GameObjects/InterfaceManager.h"

#include "ISFMLDrawable.h"

namespace rho
{

/**
  *	\brief LogicManager inherits InterfaceManager which automatically stores any GameObject that is created and also implements IUpdateable.
  */
class DrawManager : public InterfaceManager< ISFMLDrawable >
{
protected:
    virtual void OnInterfaceAdded( InterfaceType * interface );
	virtual void OnInterfaceRemoved( InterfaceType * interface );

    Event m_drawGUIEvent;

public:
	DrawManager();
	virtual ~DrawManager();

	/// Draws all Drawable Objects
    virtual void draw( sf::RenderTarget &target, sf::RenderStates states = sf::RenderStates::Default );

    virtual void drawGUI( sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default );
};

}

#endif
