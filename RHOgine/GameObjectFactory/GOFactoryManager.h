#ifndef GOFACTORY_MANAGER_H
#define GOFACTORY_MANAGER_H

#include <map>

#include "../GameObjects/GameObject.h"
#include "IGameObjectFactory.h"

#include "../FirstComeSingleton.h"

namespace rho
{

/// \brief Manager of all GameObject factories
class GOFactoryManager : public FirstComeSingleton< GOFactoryManager >
{
protected:
	typedef std::unique_ptr< IGameObjectFactory > pIFactory;
	typedef std::map< HashString, pIFactory > FactoryMap;

	/// \brief Map containing all types => factories
	FactoryMap m_factoryMap;	

public:
	GameObjectInfo * newGameObjInfo();
	void deleteGameObjInfo(GameObjectInfo * info);

	/// \brief Make New Instance of the given type
	/// \arg Type Name
	/// \return New Instance of the Given Type
	/// \note May Make it crash if trying to make a new Object of an non-existent type, cause I'm a dick like that
	GameObject * createInstance( HashString const & type ) const;
	GameObject * createInstance( HashString const & type, GameObjectInfo const & ) const;

	bool loadObjectFactoriesFromJSON( char const * const json_file_name );
	
	bool loadLuaObjectFactory( HashString const & object_name, HashString const & script_name );
	
	/// \brief Adds a Registers a new Factory for making gameobjects
	/// If factory of the same type already exists, the old factory
	/// if overwritten and deleted, and replaced with new factory
	/// \note Takes ownership of factory instance, don't delete, plz
	void addFactory( IGameObjectFactory * p_factory );
};

}

#endif
