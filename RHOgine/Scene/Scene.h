/**
  *	Base Scene File
  */

#ifndef SCENE_H
#define SCENE_H

#include <SFML/Graphics.hpp>

// include GameObject Manager
#include "../GameObjects/GameObjectManager.h"
#include "../GameObjects/InterfaceManager.h"

#include "../GameObjectFactory/GOFactoryManager.h"

#include "../GameObjects.h"

#include "../Logic/LogicManager.h"
#include "../Graphics/DrawManager.h"

namespace rho
{

/**
  *	\brief Base Scene Class.
  * A Scene compartmentalizes the logic, data, and view of an entire level ( or scene of a game ).
  *	A Scene could be the main menu, a level, or a loading screen. Anything the game requires.
  * A Scene contains it's own GameObjectManager, LogicManager, DrawManager, and lots of other junk.
  * A Scene is both updated and drawn every frame.
  * Scenes can be switched based off the name of the scene, through the Game class,
  * which will hold the current scene and logic on how to load other scenes
  * in the game.
  */
class Scene
{
protected:
    /// Manages all gameObjects in this scene
	GameObjectManager m_gameObjectManager;

    /// Logic manager, updates all Updateable objects in the scene
	LogicManager m_logicManager;

	/// Draw manager, draws all drawable objects in the scene
	DrawManager m_drawManager;

public:
	Scene();
	virtual ~Scene();

    virtual inline GameObjectManager & getGameObjectManager() { return m_gameObjectManager; }

    virtual GameObject * createGameObject( HashString const & obj_type_name );
    virtual GameObject * createGameObject( HashString const & obj_type_name, GameObjectInfo & info );

	virtual void update( sf::Time const & elapsed_time );
	virtual void draw( sf::RenderWindow & window );
};

}

#endif

