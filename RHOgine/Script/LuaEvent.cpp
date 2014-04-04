#include "LuaEvent.h"

#include "../EventManager/EventManager.h"

#include "lua_helpers.h"

#include <iostream>
#include <cassert>

#include "LuaHashString.h"

using namespace std;

using namespace rho;
using namespace rho::lua;

// typedef sLuaWrapper< rho::Event > LuaEventWrapper;

#define LUA_EVENT_NAME "EventType"

#define empty_stack(lua) lua_pop(lua,lua_gettop(lua))

// Will push the data the variant has, or if Variant doesn't have anything, will push nil
static void pushVariant( lua_State * lua, Variant const & variant )
{
	// Based off type, push different thing, check please
	switch ( variant.type )
	{
	case Variant::TYPE_INT:
		lua_pushinteger( lua, variant.getAsInt() );
		break;
	case Variant::TYPE_FLOAT:
		lua_pushnumber( lua, variant.getAsFloat() );
		break;
	case Variant::TYPE_BOOL:
		lua_pushboolean( lua, variant.getAsBool() );
		break;
	case Variant::TYPE_HASHEDSTRING_ID:
		lua_pushinteger( lua, variant.getAsStringID() );
		break;
	case Variant::TYPE_VOID_PTR:
		lua_pushlightuserdata( lua, variant.getAsVoidPtr() );
		break;
	case Variant::TYPE_NONE:
	default:
		lua_pushnil( lua );
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////

void rho::lua::pushNewEvent( lua_State * lua, rho::Event const & event )
{
	// Make New Lua UserData
	// Event ** lua_event = (Event**)lua_newuserdata( lua, sizeof( Event** ) );
	// // Create new Event inside of the wrapper
	// *lua_event = new Event( event );

	// // Set the metatable
	// // Apply meta table
	// luaL_getmetatable(lua, LUA_EVENT_NAME);
	// lua_setmetatable(lua, -2);

	rho::Event * p_event = const_cast<rho::Event *>( &event );

	lua_pushlightuserdata( lua, p_event );
}

///////////////////////////////////////////////////////////////////////////////

Event * rho::lua::toEvent( lua_State * lua, int const & pos )
{
	return static_cast<Event*>( checklightuserdata( lua, pos ) );
}

///////////////////////////////////////////////////////////////////////////////

// Arguments given for this function?
// HashString?
// Sure
// or just normal string
static int l_newEvent( lua_State * lua )
{
	// What is the type given?
	HashString event_type = chechHashString( lua, 1 );

	lua_pop( lua, 1 );	// pop the argument off the stack, THAT'S OUR STACK!

	// Make New Lua UserData
	Event * p_event = new Event( event_type );

	lua_pushlightuserdata( lua, p_event );

	//rho::lua::stackdump( lua );

	return 1;	// we are returning the newly created event
}

///////////////////////////////////////////////////////////////////////////////

static int l_deleteEvent( lua_State * lua )
{
	Event * p_event = toEvent( lua, 1 );
	delete p_event;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getEventType( lua_State * lua )
{
    // Grab userdata from stack
	rho::Event * p_event = static_cast< rho::Event * >( checklightuserdata( lua, 1 ) );

	lua_pushstring( lua, p_event->EventType().getString().c_str() );

    return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getSender( lua_State * lua )
{
	rho::Event * p_event = static_cast< rho::Event * >( lua::checklightuserdata( lua, 1 ) );

	lua_pop( lua, 1 );

	// Push sender, or NIL if not defined
	if ( p_event->m_pSender )
		lua_pushlightuserdata( lua, p_event->m_pSender );
	else
		lua_pushnil( lua );

	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setSender( lua_State * lua )
{
	rho::Event * p_event = static_cast< rho::Event * >( checklightuserdata( lua, 1 ) );

	void * p_sender = lua::checklightuserdata( lua, 2 );

	lua_pop( lua, 2 );

    p_event->m_pSender = p_sender;

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setArgInt( lua_State * lua )
{
	rho::Event * p_event = static_cast< rho::Event * >( checklightuserdata( lua, 1 ) );
    rho::HashString arg_name = chechHashString( lua, 2 );

	// Grab int from stack
	int val = luaL_checkint( lua, 3 );

	// Perform set arg on event
	p_event->setArgInt( arg_name, val );

	// Pop the args we were given
	lua::pop_all(lua);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setArgFloat( lua_State * lua )
{
	rho::Event * p_event = static_cast< rho::Event * >( checklightuserdata( lua, 1 ) );
    rho::HashString arg_name = chechHashString( lua, 2 );

	// Grab int from stack
	float val = luaL_checknumber( lua, 3 );

	// Perform set arg on event
	p_event->setArgFloat( arg_name, val );

	// Pop the args we were given
	lua::pop_all(lua);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setArgBool( lua_State * lua )
{
	rho::Event * p_event = static_cast< rho::Event * >( checklightuserdata( lua, 1 ) );
    rho::HashString arg_name = chechHashString( lua, 2 );

	// Grab int from stack
	luaL_checktype( lua, 3, LUA_TBOOLEAN );
	bool val = lua_toboolean( lua, 3 );

	// Perform set arg on event
	p_event->setArgInt( arg_name, val );

	// Pop the args we were given
	lua::pop_all(lua);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setArgHashString( lua_State * lua )
{
	// Grab userdata from stack
	rho::Event * p_event = static_cast< rho::Event * >( checklightuserdata( lua, 1 ) );
    rho::HashString arg_name = chechHashString( lua, 2 );

	rho::HashString val = chechHashString( lua, 3 );

	// Perform set arg on event
	p_event->setArgHashedStringID( arg_name, val.getHashValue() );

	// Pop the args we were given
	lua::pop_all(lua);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setArgVoidPtr( lua_State * lua )
{
	// Grab userdata from stack
	rho::Event * p_event = static_cast< rho::Event * >( checklightuserdata( lua, 1 ) );
    rho::HashString arg_name = chechHashString( lua, 2 );

	// Grab int from stack
	luaL_checktype( lua, 3, LUA_TLIGHTUSERDATA );

    void * val = lua_touserdata( lua, 3 );

	// Perform set arg on event
	p_event->setArgVoidPtr( arg_name, val );

	// Pop the args we were given
	lua::pop_all(lua);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getArg( lua_State * lua )
{
    rho::Event * p_event = static_cast< rho::Event * >( checklightuserdata( lua, 1 ) );
    HashString arg_name = chechHashString( lua, 2 );

    lua::pop_all(lua);

	// If the Arg doesn't exist, then gtfo
	if ( ! p_event->argExists( arg_name ) )
	{
		lua_pushnil( lua );
		return 1;
	}

    // If any argument exists, then return it, else return NIL
    rho::Variant arg = p_event->getArg( arg_name );

	pushVariant( lua, arg );		// push to lua

    return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_toTable( lua_State * lua )
{
	// Grab userdata from stack
	rho::Event * lua_event = lua::toEvent( lua, 1 );

	lua_newtable( lua );

	// Push Event name
	lua_pushstring( lua, lua_event->EventType().getString().c_str() );

	lua_setfield( lua, -2, "EventType" );

	for ( auto map_pair : lua_event->m_argMap )
	{
		switch ( map_pair.second.type )
		{
		case Variant::TYPE_INT:
			lua_pushinteger( lua, map_pair.second.getAsInt() );
			break;
		case Variant::TYPE_FLOAT:
			lua_pushnumber( lua, map_pair.second.getAsFloat() );
			break;
		case Variant::TYPE_BOOL:
			lua_pushboolean( lua, map_pair.second.getAsBool() );
			break;
		case Variant::TYPE_HASHEDSTRING_ID:
			lua_pushinteger( lua, map_pair.second.getAsStringID() );
			break;
		case Variant::TYPE_VOID_PTR:
			lua_pushlightuserdata( lua, map_pair.second.getAsVoidPtr() );
			break;
        default:
            break;
		}

		lua_setfield( lua, -2, map_pair.first.getString().c_str() );
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static const struct luaL_Reg event_methods [] =
{
	{ "newEvent",       	l_newEvent },
	{ "deleteEvent",		l_deleteEvent },
	{ "getEventType",       l_getEventType },
	{ "getSender",			l_getSender },
	{ "setSender",			l_setSender },
	{ "setArgInt", 			l_setArgInt },
	{ "setArgFloat", 		l_setArgFloat },
	{ "setArgBool", 		l_setArgBool },
	{ "setArgHashString", 	l_setArgHashString },
	{ "setArgVoidPtr", 		l_setArgVoidPtr },
	{ "getArg",				l_getArg },
	{ "toTable",			l_toTable },
	{ NULL, 		        NULL }		// Null termination
};

/////////////////// EVENT MANAGER /////////////////////////////////////////////

static int l_fireEvent( lua_State * lua )
{
	rho::Event * lua_event = lua::toEvent( lua, 1 );

	rho::EventManager::GetInstance().FireEvent( *lua_event );	// Shouldn't Copy Event ( safe to delete after )

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_queueEvent( lua_State * lua )
{
	rho::Event * lua_event = lua::toEvent( lua, 1 );

	rho::EventManager::GetInstance().FireEvent( *lua_event );	// Should Copy the Event ( safe to delete Afetr )

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static const struct luaL_Reg event_manager_methods [] =
{
	{ "fireEvent",			l_fireEvent },
	{ "queueEvent",			l_queueEvent },
	{ NULL, 		        NULL }		// Null termination
};

///////////////////////////////////////////////////////////////////////////////

void rho::lua::registerLuaEvent( lua_State * lua )
{
	// luaL_newmetatable( lua, LUA_EVENT_NAME );	// create new metatable

	// lua_pushvalue(lua, -1);
	// lua_setfield(lua, -2, "__index" );

	// Register( lua, NULL, event_methods );

	// // Register Event() function
	// lua_register( lua, "Event", newEvent );

	// // rho::lua::stackdump( lua );

	lua::register_funcs( lua, "Event", event_methods );
	lua::register_funcs( lua, "EventManager", event_manager_methods );

	// lua_pop( lua, 1 );
}
