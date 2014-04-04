#ifndef IUPDATEABLE_H
#define IUPDATEABLE_H

#include <SFML/System/Clock.hpp>

#include "../HashString.h"

namespace rho
{

/**
  *	\brief Interface that describes an object that updates.
  *	\sa UpdateableManager, UpdateableGameObject, kIUpdateableInterfaceName
  *	\todo Add time argument to update method
  */
class IUpdateable
{
public:
	/// Flag to tell the LogicManager if we should update this object
	virtual bool const enabled() const = 0;
	/// Method called to update this object
    virtual void update( sf::Time const & elpased_time ) = 0;

    virtual ~IUpdateable(){}
};

}

#endif
