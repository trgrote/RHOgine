#include "Camera.h"

#include "../RHOStrings.h"
#include "../RHOgine.h"

using namespace rho;

///////////////////////////////////////////////////////////////////////////////

Event Camera::makeCameraMovedEvent( float const & x, float const & y, float const & xrel, float const & yrel )
{
	Event move_event( hs::CameraMoved, this );

	move_event.setArgFloat( hs::x, x );
	move_event.setArgFloat( hs::y, y );
	move_event.setArgFloat( hs::RelativeX, xrel );
	move_event.setArgFloat( hs::RelativeY, yrel );

	return move_event;
}

///////////////////////////////////////////////////////////////////////////////

Event Camera::makeCameraResizedEvent( float const & x, float const & y, float const & xrel, float const & yrel )
{
	Event resize_event( hs::CameraResized, this );

	resize_event.setArgFloat( hs::x, x );
	resize_event.setArgFloat( hs::y, y );
	resize_event.setArgFloat( hs::RelativeX, xrel );
	resize_event.setArgFloat( hs::RelativeY, yrel );

	return resize_event;
}

///////////////////////////////////////////////////////////////////////////////

Event Camera::makeCameraRotatedEvent( float const & current_angle, float const & delta )
{
	Event rotate_event( hs::CameraResized, this );

	rotate_event.setArgFloat( hs::angle, current_angle );
	rotate_event.setArgFloat( hs::delta, delta );

	return rotate_event;
}

///////////////////////////////////////////////////////////////////////////////

void Camera::queueEvent( Event const & event )
{
	if ( ! SuppressEvents )
	{
		m_eventManager.QueueEvent( event );
	}
}

///////////////////////////////////////////////////////////////////////////////

Camera::Camera()
:	sf::View( cRHOgine::GetInstance().getRenderWindow().getDefaultView() ),
	m_eventManager( EventManager::GetInstance() ),
	SuppressEvents( false )
{
}

///////////////////////////////////////////////////////////////////////////////

Camera::Camera( const sf::FloatRect & rectangle )
:	sf::View( rectangle ),
	m_eventManager( EventManager::GetInstance() ),
	SuppressEvents( false )
{
}

///////////////////////////////////////////////////////////////////////////////

Camera::Camera( sf::Vector2f const & center, sf::Vector2f const & size )
:	sf::View( center, size ),
	m_eventManager( EventManager::GetInstance() ),
	SuppressEvents( false )
{
}

///////////////////////////////////////////////////////////////////////////////

void Camera::setCenter( float const & x, float const & y )
{
	float xrel = x - getCenter().x;
	float yrel = y - getCenter().y;

	sf::View::setCenter( x, y );

	// Queue up event
	queueEvent( makeCameraMovedEvent( getCenter().x, getCenter().y, xrel, yrel ) );
}

///////////////////////////////////////////////////////////////////////////////

void Camera::setCenter( sf::Vector2f const & center )
{
	float xrel = center.x - getCenter().x;
	float yrel = center.y - getCenter().y;

	sf::View::setCenter( center );

	// Queue up event
	queueEvent( makeCameraMovedEvent( getCenter().x, getCenter().y, xrel, yrel ) );
}

///////////////////////////////////////////////////////////////////////////////

void Camera::move(float const & offsetX, float const & offsetY)
{
	sf::View::move( offsetX, offsetY );		// Call base method ( I don't know how to do that shit )

	// Queue up event
	queueEvent( makeCameraMovedEvent( getCenter().x, getCenter().y, offsetX, offsetY ) );
}

///////////////////////////////////////////////////////////////////////////////

void Camera::move(sf::Vector2f const & offset)
{
	sf::View::move( offset );		// Call base method ( I don't know how to do that shit )

	// Queue up event
	queueEvent( makeCameraMovedEvent( getCenter().x, getCenter().y, offset.x, offset.y ) );
}

///////////////////////////////////////////////////////////////////////////////

void Camera::setSize(float const & width, float const & height)
{
	sf::Vector2f old_size = getSize();

	sf::View::setSize( width, height );

	queueEvent(
		makeCameraResizedEvent( getSize().x,
								getSize().y,
								getSize().x - old_size.x,
								getSize().y - old_size.y ) );
}

///////////////////////////////////////////////////////////////////////////////

void Camera::setSize(sf::Vector2f const & size)
{
	sf::Vector2f old_size = getSize();

	sf::View::setSize( size );

	queueEvent(
		makeCameraResizedEvent( getSize().x,
								getSize().y,
								getSize().x - old_size.x,
								getSize().y - old_size.y ) );
}

///////////////////////////////////////////////////////////////////////////////

void Camera::zoom(float const & factor)
{
	sf::Vector2f old_size = getSize();

	sf::View::zoom( factor );

	queueEvent(
		makeCameraResizedEvent( getSize().x,
								getSize().y,
								getSize().x - old_size.x,
								getSize().y - old_size.y ) );
}

///////////////////////////////////////////////////////////////////////////////

void Camera::rotate(float const & angle)
{
	sf::View::rotate( angle );

	queueEvent(
		makeCameraRotatedEvent( getRotation(), angle ) );
}

///////////////////////////////////////////////////////////////////////////////

void Camera::setRotation(float const & angle)
{
	float old_angle = getRotation();

	sf::View::setRotation( angle );

	queueEvent(
		makeCameraRotatedEvent( getRotation(), getRotation() - old_angle ) );
}

///////////////////////////////////////////////////////////////////////////////

void Camera::reset(sf::FloatRect const & rectangle)
{
	sf::Vector2f old_size = getSize();
	sf::Vector2f old_pos = getCenter();
	float old_angle = getRotation();

	sf::View::reset( rectangle );

	queueEvent(
		makeCameraRotatedEvent( getRotation(), getRotation() - old_angle ) );

	queueEvent(
		makeCameraResizedEvent( getSize().x,
								getSize().y,
								getSize().x - old_size.x,
								getSize().y - old_size.y ) );

	queueEvent( makeCameraMovedEvent( getCenter().x,
									  getCenter().y,
									  getCenter().x - old_pos.x,
									  getCenter().y - old_pos.y ) );
}

///////////////////////////////////////////////////////////////////////////////

sf::FloatRect Camera::getRect() const
{
	sf::FloatRect rect;

	sf::Vector2f center = getCenter();
	sf::Vector2f size = getSize();

	rect.left 	= center.x - size.x / 2;
	rect.top 	= center.y - size.y / 2;
	rect.width 	= this->getSize().x;
	rect.height = this->getSize().y;

	return rect;
}

///////////////////////////////////////////////////////////////////////////////
