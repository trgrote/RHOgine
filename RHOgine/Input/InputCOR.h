#ifndef INPUT_COR_H
#define INPUT_COR_H

#include <list>
#include "IInputHandler.h"
#include "../EventManager/BaseEventListener.h"

namespace rho
{

/**
  *	\brief Input Chain of Command. 
  *	Allows rho::IInputHandler s to register and respond to input events.
  */
class InputCOR
{
private:
	/// Handler Chain
	std::list< pIInputHandler > m_handlerChain;

	typedef std::list< pIInputHandler >::iterator ChainIter;

public:
    InputCOR();
    virtual ~InputCOR();

	/**
	  *	Adds a weak reference to an input handler to the chain of responsibility.  
	  *	If top_prioirity is set to true, it puts this handler at the top of the chain,
	  *	else it pushes it to the bottom.
	  *	\arg input_handler Input Handler to be added.
	  *	\arg top_priority Flag to set this input handler at the top of the chain.
	  */
	virtual void addInputHandler( pIInputHandler input_handler, bool const & top_priority = false );
	

	virtual void delInputHandler( pIInputHandler input_handler );
	
	/**	\brief Sends the input event down the chain of command, until a handler uses it
	  *	\arg event Input Event that can be handled by any handler on the chain
	  *	\return true if any handler on the chain handled the input event
	  */
	bool sendDownChain( rho::Event const & event );
};

}

#endif
