#include "GameObjectManager.h"

#include <cassert>

#include "../EventManager/EventManager.h"

#include "../RHOStrings.h"

using namespace rho;

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
	clearAll();
}

void GameObjectManager::clear()
{
	for ( pGameObject & p_game_obj : m_gameObjects )
	{
		GameObject * dead_obj = p_game_obj.get();	// grab the object

		Event destroy_event( rho::hs::GameObjDestroy );
		destroy_event.m_pSender = this;
		destroy_event.setArgVoidPtr( hs::GameObject, dead_obj );

		EventManager::GetInstance().FireEvent( destroy_event );	// shoot off event

		// If it's a persistent Object, then save it in memory, don't kill it
		// A destory event should still be thrown, just like a creation event, because
		// game systems need to know when to add and remove Objects from
		// their specific collections
		if ( ! p_game_obj->isPersistent() )
		{
			p_game_obj.release();	// just let go
		}
	}

	m_gameObjects.clear();
}

void GameObjectManager::clearAll()
{
	for ( pGameObject & p_game_obj : m_gameObjects )
	{
		GameObject * dead_obj = p_game_obj.get();	// grab the object

		Event destroy_event( rho::hs::GameObjDestroy );
		destroy_event.m_pSender = this;
		destroy_event.setArgVoidPtr( hs::GameObject, dead_obj );

		EventManager::GetInstance().FireEvent( destroy_event );	// shoot off event
	}

	m_gameObjects.clear();
}

/// Add GameObject to Object manager
/// \note Memory management is controlled by the Manager, don't delete the GameObject given.
GameObjectID GameObjectManager::addGameObject( GameObject * obj )
{
	// First, Check
	ErrorContext ec( "GameObjectManager::addGameObject" );
	assert_msg( obj != NULL, "NULL GameObject Given to Object Manager" );

	// Third, add to list
	m_gameObjects.push_back( pGameObject( obj ) );

	// fourth, send out event that GameObject has been made
	Event create_event( rho::hs::GameObjCreate );

	create_event.m_pSender = this;

	create_event.setArgVoidPtr( hs::GameObject, obj );

	EventManager::GetInstance().FireEvent( create_event );

    // Register this GameObject as an Event Listener
    rho::EventManager::GetInstance().AddListener(obj);

	return obj->getID();
}

bool GameObjectManager::removeGameObject( GameObjectID const & obj_id )
{
	for ( std::list< pGameObject >::iterator iter = m_gameObjects.begin();
		  iter != m_gameObjects.end();
		  ++iter )
	{
		if ( ! (*iter)->getID() == obj_id )	// if this object is not alive
		{
			GameObject * dead_obj = (*iter).get();	// grab the object

			Event destroy_event( rho::hs::GameObjDestroy );
			destroy_event.m_pSender = this;
			destroy_event.setArgVoidPtr( hs::GameObject, dead_obj );

			EventManager::GetInstance().FireEvent( destroy_event );	// shoot off event

			iter = m_gameObjects.erase( iter );	// erase GameObject at that position and return the next position

			return true;
		}
	}

	return false;
}

bool GameObjectManager::isRegistered( GameObject const * game_obj ) const
{
	for ( pGameObject const & p_game_obj : m_gameObjects )
	{
		if ( p_game_obj.get() == game_obj )
		{
			return true;
		}
	}

	return false;
}

/// Return GameObject with the given ID
GameObject * GameObjectManager::getGameObject( GameObjectID id )
{
	for ( pGameObject & game_obj : m_gameObjects )
	{
		if ( game_obj->getID() == id )
		{
			return game_obj.get();
		}
	}

	return NULL;
}

std::list< GameObject * > GameObjectManager::getAllPersistent() const
{
	std::list< GameObject *> rval;

	for ( pGameObject const & p_game_obj : m_gameObjects )
	{
		if ( p_game_obj->isPersistent() )
		{
			rval.push_back( p_game_obj.get() );
		}
	}

	return rval;
}

void GameObjectManager::cleanUpDead()
{
	// foreach gameObject, check if dead, if so, remove from list
	for ( std::list< pGameObject >::iterator iter = m_gameObjects.begin();
		  iter != m_gameObjects.end();
		  ++iter )
	{
		if ( ! (*iter)->getAlive() )// if this object is not alive
		{
			GameObject * dead_obj = (*iter).get();	// grab the object

			Event destroy_event( rho::hs::GameObjDestroy );
			destroy_event.m_pSender = this;
			destroy_event.setArgVoidPtr( hs::GameObject, dead_obj );

			EventManager::GetInstance().FireEvent( destroy_event );	// shoot off event

			iter = m_gameObjects.erase( iter );	// erase GameObject at that position and return the next position
		}
	}
}
