#ifndef IINPUT_HANDLER_H
#define IINPUT_HANDLER_H

#include "../EventManager/Event.h"

namespace rho
{

/**
  *	\brief Interface for handling input events in the rho::IInputCOR
  */
class IInputHandler
{
public:
	/**
	  *	Method to handle or not handle a given input event.
	  *	\arg input_event Input Event to handle
	  *	\return True if this event was handled here, false to pass down event to next handler
	  */
	virtual bool HandleInput( rho::Event const & input_event ) = 0;

    virtual ~IInputHandler(){}
};

typedef IInputHandler * pIInputHandler;

}

#endif
