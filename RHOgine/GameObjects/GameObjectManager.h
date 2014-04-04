#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include <list>
#include <memory>

#include "../FirstComeSingleton.h"

#include "../EventManager/EventManager.h"
#include "GameObject.h"

namespace rho
{

/**
  * \brief Manages all GameObjects within the Game.
  */
class GameObjectManager
{
private:
	typedef std::unique_ptr< GameObject > pGameObject;

	// List of all GameObjects
	std::list< pGameObject > m_gameObjects;

public:

	GameObjectManager();

	virtual ~GameObjectManager();

	// Delete all Objects except persistent ones
	void clear();

	/// Clear all GameObjects, including persistent ones
	/// Even the immortal should die
	/// This will be used if we are going to scene like the main menu, where
	/// GameObjects from the actual game shouldn't be
	/// Or, if the game ends and we are cleaning up
	void clearAll();

	/// Add GameObject to Object manager
	/// \note Memory management is controlled by the Manager, don't delete the GameObject given.
	GameObjectID addGameObject( GameObject * obj );

	bool removeGameObject( GameObjectID const & obj_id );

	/// Returns true if the GameObject pointer given is registered in the collection
	bool isRegistered( GameObject const * game_obj ) const;

	/// Return GameObject with the given ID
	GameObject * getGameObject( GameObjectID id );
	
	// Returns list of all Persistent Objects
	std::list< GameObject * > getAllPersistent() const;

	// Update method, cleans out dead objects
	void cleanUpDead();
};

}

#endif
