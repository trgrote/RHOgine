#ifndef LOGIC_MANAGER_H
#define LOGIC_MANAGER_H

#include "../GameObjects/InterfaceManager.h"

#include "IUpdateable.h"

namespace rho
{

/**
  *	\brief LogicManager inherits InterfaceManager which automatically stores any GameObject that is created and also implements IUpdateable.
  */
class LogicManager : public InterfaceManager< IUpdateable >
{
protected:
    virtual void OnInterfaceAdded( InterfaceType * interface );
	virtual void OnInterfaceRemoved( InterfaceType * interface );

public:
	LogicManager();
	virtual ~LogicManager();

	/// Updates all Objects that implement the IUpdateable Interface
    virtual void UpdateAll(sf::Time const & elpased_time);
};

}

#endif
