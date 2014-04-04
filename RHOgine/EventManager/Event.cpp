#include "Event.h"

#include "../HashString.h"

using namespace rho;

///////////////////////////////////////////////////////////////////////////////

void Event::setArgInt( HashString const & arg_name, int const & val )
{
	m_argMap[ arg_name ] = val;
}

///////////////////////////////////////////////////////////////////////////////

void Event::setArgFloat( HashString const & arg_name, float const & val )
{
	m_argMap[ arg_name ] = val;
}

///////////////////////////////////////////////////////////////////////////////

void Event::setArgBool( HashString const & arg_name, bool const & val )
{
	m_argMap[ arg_name ] = val;
}

///////////////////////////////////////////////////////////////////////////////

void Event::setArgHashedStringID( HashString const & arg_name, StringID const & val )
{
	m_argMap[ arg_name ] = val;
}

///////////////////////////////////////////////////////////////////////////////

void Event::setArgVoidPtr( HashString const & arg_name, void * void_ptr )
{
	m_argMap[ arg_name ] = void_ptr;
}

///////////////////////////////////////////////////////////////////////////////

bool Event::argExists( HashString const & arg_name ) const
{
	return m_argMap.find( arg_name ) != m_argMap.cend();
}

///////////////////////////////////////////////////////////////////////////////

int Event::getArgInt( HashString const & arg_name ) const
{
    xassert_msg( argExists( arg_name ), "Arg %s doesn't exist", arg_name.getString().c_str() );
    return m_argMap.at( arg_name ).getAsInt();
}

///////////////////////////////////////////////////////////////////////////////

float Event::getArgFloat( HashString const & arg_name ) const
{
    xassert_msg( argExists( arg_name ), "Arg %s doesn't exist", arg_name.getString().c_str() );
	return m_argMap.at( arg_name ).getAsFloat();
}

///////////////////////////////////////////////////////////////////////////////

bool Event::getArgBool( HashString const & arg_name ) const
{
	// check if exists
	xassert_msg( argExists( arg_name ), "Arg %s doesn't exist", arg_name.getString().c_str() );
	// Check if argument is bool
    return m_argMap.at( arg_name ).getAsBool();
}

///////////////////////////////////////////////////////////////////////////////

StringID Event::getArgHashedStringID( HashString const & arg_name ) const
{
	// check if exists
	xassert_msg( argExists( arg_name ), "Arg %s doesn't exist", arg_name.getString().c_str() );
	// Check if argument is String ID
	return m_argMap.at( arg_name ).getAsStringID();
}

///////////////////////////////////////////////////////////////////////////////

Variant Event::getArg( HashString const & arg_name ) const
{
    xassert_msg( argExists( arg_name ), "Arg %s doesn't exist", arg_name.getString().c_str() );
    return m_argMap.at( arg_name );
}

///////////////////////////////////////////////////////////////////////////////

void * Event::getArgVoidPtr( HashString const & arg_name ) const
{
    xassert_msg( argExists( arg_name ), "Arg %s doesn't exist", arg_name.getString().c_str() );
    return m_argMap.at( arg_name ).getAsVoidPtr();
}

///////////////////////////////////////////////////////////////////////////////

Variant::VARIANT_TYPE Event::getArgType( HashString const & arg_name ) const
{
    if ( ! argExists( arg_name ) )
        return Variant::TYPE_NONE;
    return m_argMap.at( arg_name ).type;
}

///////////////////////////////////////////////////////////////////////////////

bool Event::isArgInt( HashString const & arg_name ) const
{
	return getArgType( arg_name ) == Variant::TYPE_INT;
}

///////////////////////////////////////////////////////////////////////////////

bool Event::isArgFloat( HashString const & arg_name ) const
{
	return getArgType( arg_name ) == Variant::TYPE_FLOAT;
}

///////////////////////////////////////////////////////////////////////////////

bool Event::isArgBool( HashString const & arg_name ) const
{
	return getArgType( arg_name ) == Variant::TYPE_BOOL;
}

///////////////////////////////////////////////////////////////////////////////

bool Event::isArgHashStringID( HashString const & arg_name ) const
{
	return getArgType( arg_name ) == Variant::TYPE_HASHEDSTRING_ID;
}

///////////////////////////////////////////////////////////////////////////////

bool Event::isArgVoidPtr( HashString const & arg_name ) const
{
	return getArgType( arg_name ) == Variant::TYPE_VOID_PTR;
}

///////////////////////////////////////////////////////////////////////////////

