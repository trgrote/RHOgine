#ifndef LUA_HASH_STRING_H
#define LUA_HASH_STRING_H

#include <lua/lua.hpp>

#include "../HashString.h"

namespace rho
{

namespace lua
{

void registerLuaHashString( lua_State * lua );

bool isHashString( lua_State * lua, int const & index );
HashString chechHashString ( lua_State * lua, int const & pos );
StringID checkhashStringID( lua_State * lua, int const & pos );

}

}

#endif
