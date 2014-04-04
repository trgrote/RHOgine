#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <map>
#include "../Variant.h"

#include "../HashString.h"

namespace rho
{

/// \brief Generic Event object
/// \todo Add Size Contraint to Argument Map ( because Pools )
class Event final
{
public:
	// Can I constrain the size of a map object?
	std::map< HashString, Variant > m_argMap;
// public:
	HashString eventType;

	/// Pointer to the Sender or Envoker of this Event
	void * m_pSender;

	/// Returns the Event type of this Event
	HashString const & EventType() const { return eventType; }

	/// Constructor with an Empty String as the Event Name
	Event() : eventType(HashString::s_kEmptyString), m_pSender( NULL ) {}
	
	/// Constructor with the Event Type set
	Event( HashString const & event_type ) 
		: eventType( event_type ), m_pSender(NULL){}
		
	/// Constructor with Event Type and Sender Given
	Event( HashString const & event_type, void * sender ) 
		: eventType( event_type ), m_pSender(sender){}

	/// Sets the argument arg_name as the given integer value
	void setArgInt( HashString const & arg_name, int const & val );
	
	/// Sets the argument arg_name as the given float value
	void setArgFloat( HashString const & arg_name, float const & val );
	
	/// Sets the argument arg_name as the given boolean value
	void setArgBool( HashString const & arg_name, bool const & val );
	
	/// Sets the argument arg_name as the given HashString ID value
	void setArgHashedStringID( HashString const & arg_name, 	
		StringID const & val );
	
	/// Sets the argument arg_name as the given void ptr value
	void setArgVoidPtr( HashString const & arg_name, void * void_ptr );

	/// Returns true if arg_name exists in this events argument map
	bool argExists( HashString const & arg_name ) const;
	
	/** 
	  *	\brief Returns the argument, arg_name, as an integer, if arg doesn't exist or 
	  * is not an integer, asserts.
	  */
	int getArgInt( HashString const & arg_name ) const;
	
	/** 
	  *	\brief Returns the argument, arg_name, as an float, if arg doesn't exist or 
	  * is not an float, asserts.
	  */
	float getArgFloat( HashString const & arg_name ) const;
	
	/** 
	  *	\brief Returns the argument, arg_name, as an bool, if arg doesn't exist or 
	  * is not an bool, asserts.
	  */
	bool getArgBool( HashString const & arg_name ) const;
	
	/** 
	  *	\brief Returns the argument, arg_name, as an StringID, if arg doesn't exist or 
	  * is not an StringID, asserts
	  */
	StringID getArgHashedStringID( HashString const & arg_name ) const;
	
	/** 
	  *	\brief Returns the argument, arg_name, as an void *, if arg doesn't exist or 
	  * is not an void *, asserts
	  */
	void * getArgVoidPtr( HashString const & arg_name ) const;

	/** 
	  *	\brief Returns the argument, arg_name, as variant. if arg doesn't exist,
	  * asserts
	  */
	Variant getArg( HashString const & arg_name ) const;
	
	/** 
	  *	\brief Returns the argument type, arg_name.
	  *	\return Arg type, if exists, else TYPE_NONE
	  */
	Variant::VARIANT_TYPE getArgType( HashString const & arg_name ) const;
	
	/** 
	  *	\brief Returns true, if argument, arg_name, is an int.
	  */
	bool isArgInt( HashString const & arg_name ) const;
	
	/** 
	  *	\brief Returns true, if argument, arg_name, is a float.
	  */
	bool isArgFloat( HashString const & arg_name ) const;
	
	/** 
	  *	\brief Returns true, if argument, arg_name, is a bool.
	  */
	bool isArgBool( HashString const & arg_name ) const;
	
	/** 
	  *	\brief Returns true, if argument, arg_name, is a StringID.
	  */
	bool isArgHashStringID( HashString const & arg_name ) const;
	
	/** 
	  *	\brief Returns true, if argument, arg_name, is a Void Ptr.
	  */
	bool isArgVoidPtr( HashString const & arg_name ) const;
};

}

#endif
