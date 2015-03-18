#include "LuaEvent.h"

#include "../EventManager/EventManager.h"

#include "lua_helpers.h"

#include <iostream>
#include <cassert>
#include <SFML/Window/Event.hpp>

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
    HashString arg_name = chechHashString( lua, 2 );

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
    HashString arg_name = chechHashString( lua, 2 );

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
    HashString arg_name = chechHashString( lua, 2 );

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
    HashString arg_name = chechHashString( lua, 2 );

	HashString val = chechHashString( lua, 3 );

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
    HashString arg_name = chechHashString( lua, 2 );

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
	lua_getglobal( lua, "Keys" );

	// It it doesn't exist, make a new one
	if ( lua_isnil( lua, 1 ) || !lua_istable( lua, 1 ) )
	{
		lua_pop( lua, 1 );	// pop that shit off the stack
		lua_newtable( lua );
		lua_setglobal( lua, "Keys" );
		lua_getglobal( lua, "Keys" );
	}

	lua_pushstring( lua, "A" ); lua_pushinteger( lua, sf::Keyboard::A ); lua_settable ( lua, -3 );
	lua_pushstring( lua, "B" ); lua_pushinteger( lua, sf::Keyboard::B ); lua_settable( lua, -3 );
	lua_pushstring( lua, "C" ); lua_pushinteger( lua, sf::Keyboard::C ); lua_settable( lua, -3 );
	lua_pushstring( lua, "D" ); lua_pushinteger( lua, sf::Keyboard::D ); lua_settable( lua, -3 );
	lua_pushstring( lua, "E" ); lua_pushinteger( lua, sf::Keyboard::E ); lua_settable( lua, -3 );
	lua_pushstring( lua, "F" ); lua_pushinteger( lua, sf::Keyboard::F ); lua_settable( lua, -3 );
	lua_pushstring( lua, "G" ); lua_pushinteger( lua, sf::Keyboard::G ); lua_settable( lua, -3 );
	lua_pushstring( lua, "H" ); lua_pushinteger( lua, sf::Keyboard::H ); lua_settable( lua, -3 );
	lua_pushstring( lua, "I" ); lua_pushinteger( lua, sf::Keyboard::I ); lua_settable( lua, -3 );
	lua_pushstring( lua, "J" ); lua_pushinteger( lua, sf::Keyboard::J ); lua_settable( lua, -3 );
	lua_pushstring( lua, "K" ); lua_pushinteger( lua, sf::Keyboard::K ); lua_settable( lua, -3 );
	lua_pushstring( lua, "L" ); lua_pushinteger( lua, sf::Keyboard::L ); lua_settable( lua, -3 );
	lua_pushstring( lua, "M" ); lua_pushinteger( lua, sf::Keyboard::M ); lua_settable( lua, -3 );
	lua_pushstring( lua, "N" ); lua_pushinteger( lua, sf::Keyboard::N ); lua_settable( lua, -3 );
	lua_pushstring( lua, "O" ); lua_pushinteger( lua, sf::Keyboard::O ); lua_settable( lua, -3 );
	lua_pushstring( lua, "P" ); lua_pushinteger( lua, sf::Keyboard::P ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Q" ); lua_pushinteger( lua, sf::Keyboard::Q ); lua_settable( lua, -3 );
	lua_pushstring( lua, "R" ); lua_pushinteger( lua, sf::Keyboard::R ); lua_settable( lua, -3 );
	lua_pushstring( lua, "S" ); lua_pushinteger( lua, sf::Keyboard::S ); lua_settable( lua, -3 );
	lua_pushstring( lua, "T" ); lua_pushinteger( lua, sf::Keyboard::T ); lua_settable( lua, -3 );
	lua_pushstring( lua, "U" ); lua_pushinteger( lua, sf::Keyboard::U ); lua_settable( lua, -3 );
	lua_pushstring( lua, "V" ); lua_pushinteger( lua, sf::Keyboard::V ); lua_settable( lua, -3 );
	lua_pushstring( lua, "W" ); lua_pushinteger( lua, sf::Keyboard::W ); lua_settable( lua, -3 );
	lua_pushstring( lua, "X" ); lua_pushinteger( lua, sf::Keyboard::X ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Y" ); lua_pushinteger( lua, sf::Keyboard::Y ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Z" ); lua_pushinteger( lua, sf::Keyboard::Z ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Num0" ); lua_pushinteger( lua, sf::Keyboard::Num0 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Num1" ); lua_pushinteger( lua, sf::Keyboard::Num1 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Num2" ); lua_pushinteger( lua, sf::Keyboard::Num2 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Num3" ); lua_pushinteger( lua, sf::Keyboard::Num3 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Num4" ); lua_pushinteger( lua, sf::Keyboard::Num4 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Num5" ); lua_pushinteger( lua, sf::Keyboard::Num5 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Num6" ); lua_pushinteger( lua, sf::Keyboard::Num6 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Num7" ); lua_pushinteger( lua, sf::Keyboard::Num7 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Num8" ); lua_pushinteger( lua, sf::Keyboard::Num8 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Num9" ); lua_pushinteger( lua, sf::Keyboard::Num9 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Escape" ); lua_pushinteger( lua, sf::Keyboard::Escape ); lua_settable( lua, -3 );
	lua_pushstring( lua, "LControl" ); lua_pushinteger( lua, sf::Keyboard::LControl ); lua_settable( lua, -3 );
	lua_pushstring( lua, "LShift" ); lua_pushinteger( lua, sf::Keyboard::LShift ); lua_settable( lua, -3 );
	lua_pushstring( lua, "LAlt" ); lua_pushinteger( lua, sf::Keyboard::LAlt ); lua_settable( lua, -3 );
	lua_pushstring( lua, "LSystem" ); lua_pushinteger( lua, sf::Keyboard::LSystem ); lua_settable( lua, -3 );
	lua_pushstring( lua, "RControl" ); lua_pushinteger( lua, sf::Keyboard::RControl ); lua_settable( lua, -3 );
	lua_pushstring( lua, "RShift" ); lua_pushinteger( lua, sf::Keyboard::RShift ); lua_settable( lua, -3 );
	lua_pushstring( lua, "RAlt" ); lua_pushinteger( lua, sf::Keyboard::RAlt ); lua_settable( lua, -3 );
	lua_pushstring( lua, "RSystem" ); lua_pushinteger( lua, sf::Keyboard::RSystem ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Menu" ); lua_pushinteger( lua, sf::Keyboard::Menu ); lua_settable( lua, -3 );
	lua_pushstring( lua, "LBracket" ); lua_pushinteger( lua, sf::Keyboard::LBracket ); lua_settable( lua, -3 );
	lua_pushstring( lua, "RBracket" ); lua_pushinteger( lua, sf::Keyboard::RBracket ); lua_settable( lua, -3 );
	lua_pushstring( lua, "SemiColon" ); lua_pushinteger( lua, sf::Keyboard::SemiColon ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Comma" ); lua_pushinteger( lua, sf::Keyboard::Comma ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Period" ); lua_pushinteger( lua, sf::Keyboard::Period ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Quote" ); lua_pushinteger( lua, sf::Keyboard::Quote ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Slash" ); lua_pushinteger( lua, sf::Keyboard::Slash ); lua_settable( lua, -3 );
	lua_pushstring( lua, "BackSlash" ); lua_pushinteger( lua, sf::Keyboard::BackSlash ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Tilde" ); lua_pushinteger( lua, sf::Keyboard::Tilde ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Equal" ); lua_pushinteger( lua, sf::Keyboard::Equal ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Dash" ); lua_pushinteger( lua, sf::Keyboard::Dash ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Space" ); lua_pushinteger( lua, sf::Keyboard::Space ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Return" ); lua_pushinteger( lua, sf::Keyboard::Return ); lua_settable( lua, -3 );
	lua_pushstring( lua, "BackSpace" ); lua_pushinteger( lua, sf::Keyboard::BackSpace ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Tab" ); lua_pushinteger( lua, sf::Keyboard::Tab ); lua_settable( lua, -3 );
	lua_pushstring( lua, "PageUp" ); lua_pushinteger( lua, sf::Keyboard::PageUp ); lua_settable( lua, -3 );
	lua_pushstring( lua, "PageDown" ); lua_pushinteger( lua, sf::Keyboard::PageDown ); lua_settable( lua, -3 );
	lua_pushstring( lua, "End" ); lua_pushinteger( lua, sf::Keyboard::End ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Home" ); lua_pushinteger( lua, sf::Keyboard::Home ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Insert" ); lua_pushinteger( lua, sf::Keyboard::Insert ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Delete" ); lua_pushinteger( lua, sf::Keyboard::Delete ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Add" ); lua_pushinteger( lua, sf::Keyboard::Add ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Subtract" ); lua_pushinteger( lua, sf::Keyboard::Subtract ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Multiply" ); lua_pushinteger( lua, sf::Keyboard::Multiply ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Divide" ); lua_pushinteger( lua, sf::Keyboard::Divide ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Left" ); lua_pushinteger( lua, sf::Keyboard::Left ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Right" ); lua_pushinteger( lua, sf::Keyboard::Right ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Up" ); lua_pushinteger( lua, sf::Keyboard::Up ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Down" ); lua_pushinteger( lua, sf::Keyboard::Down ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Numpad0" ); lua_pushinteger( lua, sf::Keyboard::Numpad0 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Numpad1" ); lua_pushinteger( lua, sf::Keyboard::Numpad1 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Numpad2" ); lua_pushinteger( lua, sf::Keyboard::Numpad2 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Numpad3" ); lua_pushinteger( lua, sf::Keyboard::Numpad3 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Numpad4" ); lua_pushinteger( lua, sf::Keyboard::Numpad4 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Numpad5" ); lua_pushinteger( lua, sf::Keyboard::Numpad5 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Numpad6" ); lua_pushinteger( lua, sf::Keyboard::Numpad6 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Numpad7" ); lua_pushinteger( lua, sf::Keyboard::Numpad7 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Numpad8" ); lua_pushinteger( lua, sf::Keyboard::Numpad8 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Numpad9" ); lua_pushinteger( lua, sf::Keyboard::Numpad9 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "F1" ); lua_pushinteger( lua, sf::Keyboard::F1 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "F2" ); lua_pushinteger( lua, sf::Keyboard::F2 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "F3" ); lua_pushinteger( lua, sf::Keyboard::F3 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "F4" ); lua_pushinteger( lua, sf::Keyboard::F4 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "F5" ); lua_pushinteger( lua, sf::Keyboard::F5 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "F6" ); lua_pushinteger( lua, sf::Keyboard::F6 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "F7" ); lua_pushinteger( lua, sf::Keyboard::F7 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "F8" ); lua_pushinteger( lua, sf::Keyboard::F8 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "F9" ); lua_pushinteger( lua, sf::Keyboard::F9 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "F10" ); lua_pushinteger( lua, sf::Keyboard::F10 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "F11" ); lua_pushinteger( lua, sf::Keyboard::F11 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "F12" ); lua_pushinteger( lua, sf::Keyboard::F12 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "F13" ); lua_pushinteger( lua, sf::Keyboard::F13 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "F14" ); lua_pushinteger( lua, sf::Keyboard::F14 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "F15" ); lua_pushinteger( lua, sf::Keyboard::F15 ); lua_settable( lua, -3 );
	lua_pushstring( lua, "Pause" ); lua_pushinteger( lua, sf::Keyboard::Pause ); lua_settable( lua, -3 );

    lua_pop(lua, 1);
	
}
