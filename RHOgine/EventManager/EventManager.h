#ifndef EVENTMANAGER_H_INCLUDED
#define EVENTMANAGER_H_INCLUDED

#include <list>
#include <vector>
#include <map>

#include "IEventListener.h"

#include "../FirstComeSingleton.h"

namespace rho
{

/// The Real Event Manager
class EventManager : public FirstComeSingleton< EventManager >
{
private:
	// Classes with HandleEvent() methods
	typedef std::list<IEventListenerPtr>        			EventListenerList;
	typedef std::map<StringID, EventListenerList>   		EventListenerMap;
	typedef std::pair<StringID, EventListenerList>  		EventListenerMapPair;
	typedef std::pair<EventListenerMap::iterator, bool>		EventListenerMapResult;
	typedef std::list<Event>                				EventQueue;

	enum eConstants { NumQueues = 2 };

	/// Listener Map
	EventListenerMap m_ListenerMap;

	/// Event Queues, ALL TWO OF THEM!
	EventQueue m_queues[NumQueues];

	/// Keeps track of current queue
	StringID m_CurrentQueue;

public:
	EventManager();
	virtual ~EventManager();

	virtual void AddListener(
		IEventListenerPtr listener,
		HashString const & event_type );

	virtual void AddListener(
		IEventListenerPtr listener);

	virtual void AddListenerAll(
		IEventListenerPtr listener );

	virtual void DelListener(
		IEventListenerPtr listener,
		HashString const & event_type );

	virtual void DelListener(
		IEventListenerPtr listener );

	// Fuck the queue, fire this shit NOW!
	virtual void FireEvent( Event const & event );

	// Be Nice and put event into the queue to wait it's turn
	virtual void QueueEvent( Event const & event );

	// Like an update method, it handles all events in the queue
	virtual void HandleQueuedEvents();
};

}

#endif // EVENTMANAGER_H_INCLUDED
