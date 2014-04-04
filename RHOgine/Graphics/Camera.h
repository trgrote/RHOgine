#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

#include "../EventManager/EventManager.h"

namespace rho
{

/**
  * \brief This mother fucker right here is basically sf::View, but we give it the event manager
  * so it can throw events when stuff happens.
  * \note These Methods are exactly the sames as sf::View, so if you want
  * info on what they do, just look at SFML's documentation.
  */
class Camera : public sf::View
{
protected:
	EventManager & m_eventManager;

	/// Helper Method to create camera moved event
	Event makeCameraMovedEvent( float const & x, float const & y, float const & xrel, float const & yrel );
	Event makeCameraResizedEvent( float const & x, float const & y, float const & xrel, float const & yrel );
	Event makeCameraRotatedEvent( float const & current_angle, float const & delta );

	void queueEvent( Event const & event );

public:
    /// If set to true, Events will not be thrown for this Camera
	bool SuppressEvents;

	Camera();
	Camera( const sf::FloatRect & rectangle );
	Camera( sf::Vector2f const & center, sf::Vector2f const & size );

	virtual ~Camera(){}

	virtual void setCenter(float const & x, float const & y);
	virtual void setCenter( sf::Vector2f const & center );

	virtual void move(float const & offsetX, float const & offsetY);
	virtual void move(sf::Vector2f const & offset);

	virtual void setSize(float const & width, float const & height);
	virtual void setSize(sf::Vector2f const & size);

	virtual void reset(sf::FloatRect const & rectangle);

	virtual void zoom(float const & factor);

	virtual void rotate(float const & angle);
	virtual void setRotation(float const & angle);
	
	virtual sf::FloatRect getRect() const;

	Camera & operator=( sf::View const & other )
	{
		sf::View::operator=( other );
		return *this;
	}

	Camera & operator=( Camera const & other )
	{
	    *this = other;
		return *this;
	}

};

}

#endif
