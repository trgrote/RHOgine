#include "DrawManager.h"

#include "../RHOStrings.h"

using namespace rho;

///////////////////////////////////////////////////////////////////////////////

DrawManager::DrawManager()
:   InterfaceManager(hs::ISFMLDrawable),
    m_drawGUIEvent( hs::DrawGUI, this )
{

}

///////////////////////////////////////////////////////////////////////////////

DrawManager::~DrawManager()
{
}

///////////////////////////////////////////////////////////////////////////////

void DrawManager::OnInterfaceAdded( InterfaceType * interface )
{
	//cout << "I'm now adding a game object by the name of " << (void*)interface << " to my list of objects\n";
	// Sort the Interface List with reverse order ( first object should have highest depth )
	m_interfaceList.sort(
		[]( ISFMLDrawable const * first, ISFMLDrawable const * second )
		{
			return ( first->getDepth() >= second->getDepth() );
		}
	);
}

///////////////////////////////////////////////////////////////////////////////

void DrawManager::OnInterfaceRemoved( InterfaceType * interface )
{
	//cout << "I'm now removing a game object by the name of " << (void*)interface << " from my list of game objects\n";
}

///////////////////////////////////////////////////////////////////////////////

void DrawManager::draw( sf::RenderTarget &target, sf::RenderStates states )
{
	for ( ISFMLDrawable * p_drawable : m_interfaceList )
	{
		assert ( p_drawable != NULL );
		if ( p_drawable->visible() )
		{
			p_drawable->draw( target, states );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

void DrawManager::drawGUI( sf::RenderTarget &target, sf::RenderStates states )
{
    m_drawGUIEvent.setArgVoidPtr( hs::RenderTarget, &target );
    m_drawGUIEvent.setArgVoidPtr( hs::DrawStates, &states );

    EventManager::GetInstance().FireEvent( m_drawGUIEvent );      // Fire Draw GUI Event
}

///////////////////////////////////////////////////////////////////////////////
