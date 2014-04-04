#ifndef INTERFACE_MANAGER_H
#define INTERFACE_MANAGER_H

#include "../RHOStrings.h"
#include <cassert>
#include <list>
#include "../EventManager/BaseEventListener.h"
#include "GameObjectManager.h"

namespace rho
{

/**
  *	\brief Interface Manager.
  *	This Object grabs any Created Objects
  *	through the Event Manager that match the given interface name
  *	and then converts and stores them in an internal list so derived classes can use.
  *	\note I didn't make a source file for this because it's too fucking confusing
  *		and implementing template classes is the by far the worst thing about C++. IMHO
  */
template <class T_InterfaceType >
class InterfaceManager : public rho::BaseEventListener
{
protected:
	/// Ok, so for some reason, C++ doesn't allow derived classes of
	/// template types know about the passed in Template Argument.
	/// So, the only way around it is to have a typedef in the class
	/// that is basically useless.
	/// This is irredeemable stupid. Greate Job, Bjarne!
	typedef T_InterfaceType InterfaceType;

	/// List of Pointers to Objects that implement this interface
    std::list< InterfaceType * > m_interfaceList;

    HashString const & InterfaceTypeName;

	// Internal Event Handlers
	/** \brief Internal Method called when an new interface is added to the list of interfaces.
	  *	Protected classes have override and respond accordingly to added interfaces.
	  */
	virtual void OnInterfaceAdded( InterfaceType * interface ){}

	/** \brief Internal Method called when an new interface is removed to the list of interfaces.
	  *	Protected classes have override and respond accordingly to removed interfaces.
	  */
	virtual void OnInterfaceRemoved( InterfaceType * interface ){}

public:

	/** \brief Only Constructor, We NEED The InterfaceTypeName to be set for any of this
	  * to work, seriously, give it!
	  * \arg interface_type_name The HashString of the Interface type.
	  *		This is compared to the GameObject::implements method so make sure it's legit.
	  */
    InterfaceManager( HashString const & interface_type_name )
    : InterfaceTypeName( interface_type_name )
	{
	    addEventToRegister( rho::hs::GameObjCreate );
	    addEventToRegister( rho::hs::GameObjDestroy );
	}

	/** \brief This Method Handles the Object Creation and Destruction event
	  * \note If your derived manager class uses handle event, make sure to call this
	  *		in your implementation.
	  */
    virtual void HandleEvent( Event const & event )
	{
		if ( event.EventType() == rho::hs::GameObjCreate )
		{
			GameObject * game_obj = static_cast<GameObject*> ( event.getArgVoidPtr( "GameObject" ) );

			//if ( game_obj->implements( InterfaceTypeName ) )
			{
				InterfaceType * p_interface = dynamic_cast< InterfaceType * >( game_obj );

				assert ( p_interface != NULL && "Conversion From GameObject to Interface Type Failed" );

				m_interfaceList.push_back( p_interface );

				OnInterfaceAdded( p_interface );
			}
		}
		else if ( event.EventType() == rho::hs::GameObjDestroy )
		{
			GameObject * game_obj = static_cast<GameObject*> ( event.getArgVoidPtr( "GameObject" ) );

			//if ( game_obj->implements( InterfaceTypeName ) )
			{
				InterfaceType * p_interface = dynamic_cast< InterfaceType * >( game_obj );

				assert ( p_interface != NULL && "Conversion From GameObject to Interface Type Failed" );

				m_interfaceList.remove( p_interface );	// remove from list

				OnInterfaceRemoved( p_interface );
			}
		}
	}
};

}

#endif
