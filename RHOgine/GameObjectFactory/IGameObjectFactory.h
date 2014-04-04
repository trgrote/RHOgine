#ifndef IGAMEOBJECT_FACTORY_H
#define IGAMEOBJECT_FACTORY_H

#include "../GameObjects/GameObject.h"

namespace rho
{

/// \brief Interface For all GameObject Factories
class IGameObjectFactory
{
public:
	virtual ~IGameObjectFactory(){}

	/// \brief Returns the type of Object this Factory creates
	virtual HashString const getType() const = 0;

	/// \brief Makes a new GameObject of the type that this Factory is responsible for
	virtual GameObject * createInstance() const = 0;
	virtual GameObject * createInstance(GameObjectInfo const & ) const = 0;
};


}

#endif
