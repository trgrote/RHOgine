#ifndef LUA_EVENT_H
#define LUA_EVENT_H

#include <lua/lua.hpp>
#include "../EventManager/Event.h"

namespace rho
{

namespace lua
{

// typedef sLuaWrapper< rho::Event > LuaEventWrapper;
/// \todo Lua Events: Add EventType() method to return HashString ID
/// \todo Lua Events: Allow HashString StringID instead of strings for get/setArg methods
/// \todo General Lua: Add more argument type checking for Lua C Functions.
/// \todo General Lua: Make Sure every call to lua has a runtime error output ( runtime error sucks )
/// \todo General Lua/Console: Have Lua errors print to console
/// \todo General Lua/Console: create console commands: dofile, and execute function to allow console to run lua commands.
void registerLuaEvent( lua_State * lua );
void pushNewEvent( lua_State * lua, rho::Event const & event );
Event * toEvent( lua_State * lua, int const & pos );

}

}

#endif
