#include "InputCOR.h"

#include "../RHOStrings.h"

using namespace rho;

InputCOR::InputCOR()
{
}

InputCOR::~InputCOR()
{
}

bool InputCOR::sendDownChain( Event const & event )
{
	for ( ChainIter iter = m_handlerChain.begin() ; iter != m_handlerChain.end(); ++iter )
	{
		pIInputHandler handler = (*iter);

		// If this handler handles the input, then stop
		if ( handler->HandleInput( event ) )
		{
			return true;
		}
		// else, go down the list to the next one
	}
	
	return false;
}

void InputCOR::addInputHandler( pIInputHandler input_handler, bool const & top_priority )
{
	// If we request top priority, then put at the top
	if ( top_priority )
	{
		m_handlerChain.push_front( input_handler );
	}
	else
	{
		m_handlerChain.push_back( input_handler );
	}
}

void InputCOR::delInputHandler( pIInputHandler input_handler )
{
	m_handlerChain.remove( input_handler );
}