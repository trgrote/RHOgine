#ifndef IEVENTLISTENER_H
#define IEVENTLISTENER_H

#include "Event.h"

#include <list>

namespace rho
{

/** Event Listener Interface to be inherited by any Listener Classes.
  * Example How to use:
  * \code
  * class UpdateListener : public IEventListener
  * {
  * public:
  *		virtual void HandleEvent( Event const & event )
  *		{
  *			// Check the type of event
  *			if ( event.EventType() == Evt_Update::s_kType ) // check against the constant HashedString type for constant time!
  *			{
  *				// Grab Arguments from Event
  *				int elaped_time = event.getArgInt( "dt" );
  *
  *				cout << "Update Event: Elapsed time = " << elaped_time << endl;
  *			}
  *			else if ( event.EventType() == Evt_Render::s_kType )
  *			{
  *				cout << "Render Event" << endl;
  *			}
  *		}
  *	};
  *	EventManager.Get().AddListener( updateListener, Evt_Update::s_ktype );
  * \endcode
  */
class IEventListener
{
public:
	virtual ~IEventListener() {}

	virtual void HandleEvent( Event const & event ) = 0;

	virtual std::list< HashString > const & getEventsToRegister() const = 0;
};

/// Event Listener Pointer
typedef IEventListener * IEventListenerPtr;

}

#endif
