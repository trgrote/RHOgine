#ifndef BASE_EVENT_LISTENER_H
#define BASE_EVENT_LISTENER_H

#include "IEventListener.h"

namespace rho
{

/**
  *	\brief Abstract Class that gives access to a usuable m_eventsToRegister list that
  *	derived Event Listeners can use and populate for the IEventListener::getEventsToRegister
  *	interface method.
  *	Also implements the getEventsToRegister method and returns this list
  */
class BaseEventListener : public IEventListener
{
protected:
	std::list< HashString > m_eventsToRegister;
public:

    virtual void addEventToRegister( HashString const & event_name );
	virtual std::list< HashString > const & getEventsToRegister() const;

	virtual ~BaseEventListener();

};

}

#endif
