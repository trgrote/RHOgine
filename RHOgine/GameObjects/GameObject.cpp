#include "GameObject.h"

#include <iostream>

#include "../EventManager/EventManager.h"

#include "../RHOStrings.h"

using namespace rho;

GameObjectID GameObject::s_iNextID = 0;

///////////////////////////////////////////////////////////////////////////////

GameObject::GameObject()
:	m_id( ++s_iNextID ),
    m_isAlive( true ),
	m_bPersistent( false ),
	m_bEnabled( true ),
    m_bVisible( true ),
    m_iDepth( 0 ),
    m_fRotation( 0.0f ),
	m_bounds( 0, 0, 20, 20 ),
	m_pScene( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

GameObject::GameObject( GameObjectInfo const & info )
:	m_id( ++s_iNextID ),
    m_isAlive( true ),
	m_bPersistent( false ),
	m_bEnabled( info.enabled ),
    m_bVisible( info.visible ),
    m_iDepth( 0 ),
    m_fRotation( 0.0f ),
	m_bounds( info.x, info.y, 20, 20 ),
	m_pScene( info.scene )
{
}

///////////////////////////////////////////////////////////////////////////////

GameObject::~GameObject()
{
}

///////////////////////////////////////////////////////////////////////////////

GameObjectID GameObject::getID() const
{
	return m_id;
}

///////////////////////////////////////////////////////////////////////////////

void GameObject::setID(GameObjectID const & id)
{
    m_id = id;
}

///////////////////////////////////////////////////////////////////////////////

bool const & GameObject::getAlive() const
{
    return m_isAlive;
}

///////////////////////////////////////////////////////////////////////////////

void GameObject::setAlive(bool const & alive)
{
    m_isAlive = alive;
}

///////////////////////////////////////////////////////////////////////////////

bool const & GameObject::isPersistent() const
{
    return m_bPersistent;
}

///////////////////////////////////////////////////////////////////////////////

void GameObject::setPersistent(bool const & persistent)
{
    m_bPersistent = persistent;
}

///////////////////////////////////////////////////////////////////////////////

//// ITransformable

void GameObject::setPosition( int const & x, int const & y )
{
    // Relative Position values
    static int rel_x;
    static int rel_y;

    // Get the relative position
    rel_x = x - m_bounds.left;
    rel_y = y - m_bounds.top;

    // Set Change
	m_bounds.left = x;
	m_bounds.top = y;

    // Queue GameObject Moved Event
	static Event event( hs::GameObjMoved );
    event.setArgVoidPtr(hs::GameObject, this);
    event.setArgInt( hs::x, m_bounds.left );
    event.setArgInt( hs::y, m_bounds.top );
    event.setArgInt( hs::RelativeX, rel_x );
    event.setArgInt( hs::RelativeY, rel_y );

    EventManager::GetInstance().QueueEvent(event);
}

///////////////////////////////////////////////////////////////////////////////

int GameObject::getPositionX() const
{
	return m_bounds.left;
}

///////////////////////////////////////////////////////////////////////////////

int GameObject::getPositionY() const
{
	return m_bounds.top;
}

///////////////////////////////////////////////////////////////////////////////

int GameObject::getCenterX() const
{
    return ( m_bounds.width / 2 ) + m_bounds.left;
}

///////////////////////////////////////////////////////////////////////////////

int GameObject::getCenterY() const
{
    return ( m_bounds.height / 2 ) + m_bounds.top;
}

///////////////////////////////////////////////////////////////////////////////

int GameObject::getWidth() const
{
    return m_bounds.width;
}

///////////////////////////////////////////////////////////////////////////////

int GameObject::getHeight() const
{
    return m_bounds.height;
}

///////////////////////////////////////////////////////////////////////////////

float GameObject::getRotation() const
{
    return m_fRotation;
}

//// IUpdateable

bool const GameObject::enabled() const
{
	return m_bEnabled;
}

///////////////////////////////////////////////////////////////////////////////

void GameObject::update( sf::Time const & elpased_time )
{
}

//// ISFMLDrawable

bool const GameObject::visible() const
{
	return m_bVisible;
}

///////////////////////////////////////////////////////////////////////////////

int const GameObject::getDepth() const
{
	return m_iDepth;
}

///////////////////////////////////////////////////////////////////////////////

void GameObject::setDepth(int const & depth)
{
    m_iDepth = depth;

    static Event event( hs::GameObjDepthChanged );
    event.setArgVoidPtr(hs::GameObject, this);

    EventManager::GetInstance().QueueEvent(event);
}

///////////////////////////////////////////////////////////////////////////////

void GameObject::draw( sf::RenderTarget &target, sf::RenderStates states )
{
}

//// BaseEventListener

void GameObject::HandleEvent( rho::Event const & event )
{
}

