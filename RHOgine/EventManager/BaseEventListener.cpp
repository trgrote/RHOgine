#include "BaseEventListener.h"

#include "EventManager.h"

using namespace rho;

///////////////////////////////////////////////////////////////////////////////

void BaseEventListener::addEventToRegister( HashString const & event_name )
{
	m_eventsToRegister.push_back( event_name );
}

///////////////////////////////////////////////////////////////////////////////

std::list< HashString > const & BaseEventListener::getEventsToRegister() const
{
	return m_eventsToRegister;
}

///////////////////////////////////////////////////////////////////////////////

BaseEventListener::~BaseEventListener()
{
	EventManager::GetInstance().DelListener( this );
}
