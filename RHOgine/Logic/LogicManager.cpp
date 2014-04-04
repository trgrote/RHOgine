#include "LogicManager.h"

#include "../RHOStrings.h"

using namespace rho;

LogicManager::LogicManager()
:	InterfaceManager(hs::IUpdateable)
{
}

LogicManager::~LogicManager()
{

}

void LogicManager::OnInterfaceAdded( InterfaceType * interface )
{
	//cout << "I'm now adding a game object by the name of " << (void*)interface << " to my list of objects\n";
}

void LogicManager::OnInterfaceRemoved( InterfaceType * interface )
{
	//cout << "I'm now removing a game object by the name of " << (void*)interface << " from my list of game objects\n";
}

void LogicManager::UpdateAll(sf::Time const & elpased_time)
{
	for ( IUpdateable * p_updateable : m_interfaceList )
	{
		assert ( p_updateable != NULL );
		if ( p_updateable->enabled() )
		{
			p_updateable->update(elpased_time);
		}
	}
}
