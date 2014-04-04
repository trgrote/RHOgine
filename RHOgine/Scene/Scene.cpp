#include "Scene.h"

#include "../RHOgine.h"

using namespace rho;

///////////////////////////////////////////////////////////////////////////////

Scene::Scene()
{
	// Register Event Listeners
	EventManager & event_manager = EventManager::GetInstance();

	event_manager.AddListener( &m_logicManager );
	event_manager.AddListener( &m_drawManager );
}

///////////////////////////////////////////////////////////////////////////////

Scene::~Scene()
{
	EventManager & event_manager = EventManager::GetInstance();

	event_manager.DelListener( &m_logicManager );
	event_manager.DelListener( &m_drawManager );
}

///////////////////////////////////////////////////////////////////////////////

GameObject * Scene::createGameObject(
    HashString const & obj_type_name )
{
    GameObjectInfo info =
    {
        this,
        0,
        0
    };

	return createGameObject( obj_type_name, info);
}

///////////////////////////////////////////////////////////////////////////////

GameObject * Scene::createGameObject(
	HashString const & obj_type_name,
	GameObjectInfo & info )
{
	GOFactoryManager & factory = cRHOgine::GetInstance().getFactoryManager();

    info.scene = this;
	GameObject * game_obj = factory.createInstance( obj_type_name, info );

	getGameObjectManager().addGameObject( game_obj );

	return game_obj;
}

///////////////////////////////////////////////////////////////////////////////

void Scene::update( sf::Time const & elapsed_time )
{
    m_logicManager.UpdateAll(elapsed_time);  	// Update the Logic Engine
    m_gameObjectManager.cleanUpDead();  		// Clear Up any Dead GameObjects
}

///////////////////////////////////////////////////////////////////////////////

void Scene::draw( sf::RenderWindow & window )
{
    m_drawManager.draw( window );
}

///////////////////////////////////////////////////////////////////////////////
