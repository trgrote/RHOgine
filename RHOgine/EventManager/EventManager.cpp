#include "EventManager.h"
#include <cassert>

using namespace rho;

/// Wild Card Type.  Used for ALL. Internal implementation ONLY. Don't use this, ya morons!
HashString const s_kWildCardType( "WILDCARD BITCHES! YEEE HAAAA!" );

FIRST_COME_SINGLETON_INSTANCE( EventManager );

/************EventManager***********************/
EventManager::EventManager()
:		m_CurrentQueue(0)
{
}

EventManager::~EventManager()
{
}

void EventManager::AddListener( IEventListenerPtr listener, HashString const & event_type )
{
    // Attempt to Find map entry that already has Event Type has key
    EventListenerMap::iterator emIt = m_ListenerMap.find( event_type.getHashValue() );

    // If these is no entry for this Event Type, create a new entry
    if ( emIt == m_ListenerMap.end() )
    {
        // insert pair into map
        EventListenerMapResult res = m_ListenerMap.insert( EventListenerMapPair(
            event_type.getHashValue(), EventListenerList() ) );

        // Check to see if insertion was succesful
        // if ( res.second == false )
            // return false;

        // if ( res.first == m_ListenerMap.end() )
            // return false;

        // store iterator position so to iterate later and check for duplicates
        emIt = res.first;
    }

    EventListenerList & ListenerList = (*emIt).second;

    // Check if the Listner has already been put into the event list
    for ( EventListenerList::iterator it = ListenerList.begin();
            it != ListenerList.end(); it++ )
    {
        if ( *it == listener )
            return;
    }

    // Finally, insert the listner into the map
    ListenerList.push_back( listener );
}

void EventManager::AddListener( IEventListenerPtr listener )
{
	std::list< HashString > events = listener->getEventsToRegister();

	for ( HashString const & event_type : events )
	{
		AddListener( listener, event_type );
	}
}

void EventManager::AddListenerAll( IEventListenerPtr listener )
{
	AddListener( listener, s_kWildCardType );
}

void EventManager::DelListener( IEventListenerPtr listener, HashString const & event_type )
{
    // Find List for event type
    EventListenerMap::iterator it = m_ListenerMap.find( event_type.getHashValue() );

    // If we found the event type location
    if ( it != m_ListenerMap.end() )
    {
        EventListenerList & list = it->second;

        for ( EventListenerList::iterator it2 = list.begin();
                it2 != list.end(); ++it2 )
        {
            // Check to see if Listener is there
            if ( (*it2) == listener )
            {
                list.erase( it2 );  // remove from list
                break;  // Break out of loop
            }
        }
    }
}

// Brute Force Delete listener, this finds ANNY occurence of listener in Map and erases it
void EventManager::DelListener( IEventListenerPtr listener )
{
    // Go through entire map and find occurence of handler
    for ( EventListenerMap::iterator it = m_ListenerMap.begin(),
			  itEnd = m_ListenerMap.end(); it != itEnd; ++it )
	{
	    // Get List
		EventListenerList & list = it->second;

		// Go through list to find occurence of Handler
		for ( EventListenerList::iterator it2 = list.begin(),
				  it2End = list.end(); it2 != it2End; ++it2 )
		{
			if ( *it2 == listener )
			{
				// found match, remove from table,
				list.erase( it2 );

				// and early-quit the inner loop as addListener()
				// code ensures that each listener can only
				// appear in one event's processing list once.
				break;
			}
		}
	}
}

void EventManager::FireEvent( Event const & event )
{
	// Wild card checks
	EventListenerMap::iterator it = m_ListenerMap.find( s_kWildCardType.getHashValue() );

	// Check for wild card listeners
	if ( it != m_ListenerMap.end() )
    {
        EventListenerList const & table = it->second;

        for ( EventListenerList::const_iterator it2 = table.begin();
            it2 != table.end(); it2++ )
        {
            IEventListenerPtr listener = *it2;
            listener->HandleEvent( event );
        }
    }

    // Find a matching event type in the map
    it = m_ListenerMap.find( event.EventType().getHashValue() );

    // Make Sure there are listners
    if ( it != m_ListenerMap.end() )
    {
        EventListenerList const & table = it->second;

        for ( EventListenerList::const_iterator it2 = table.begin();
            it2 != table.end(); it2++ )
        {
            IEventListenerPtr listener = *it2;
            listener->HandleEvent( event );
        }
    }
}

void EventManager::QueueEvent( Event const & event )
{
    // Find Events in shared map
    EventListenerMap::iterator it =
        m_ListenerMap.find( event.EventType().getHashValue() );

	// Find if we have any wild card listeners
	EventListenerMap::iterator wild_card_it =
        m_ListenerMap.find( s_kWildCardType.getHashValue() );

    // Queue it if found in any maps
    if ( it != m_ListenerMap.end() || wild_card_it != m_ListenerMap.end() )
        m_queues[m_CurrentQueue].push_back( event );
}

void EventManager::HandleQueuedEvents()
{
    // Change current Queue
    StringID queueToProcess = m_CurrentQueue;
    m_CurrentQueue = (m_CurrentQueue + 1) % NumQueues;

    // Go through all events in the 'queue'
    while ( m_queues[ queueToProcess ].size() > 0 )
    {
        // Get event
        Event event = m_queues[queueToProcess].front();
        m_queues[queueToProcess].pop_front();

		// fire event
		FireEvent( event );
    }
}
