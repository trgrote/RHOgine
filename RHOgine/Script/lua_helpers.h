#ifndef RHO_LUA_HELPERS_H
#define RHO_LUA_HELPERS_H

#include <lua/lua.hpp>

#include <iostream>

#include "LuaHashString.h"

namespace rho
{

namespace lua
{

// Compile Error
bool compile_error(lua_State * L, int status, std::ostream & stream );

// Run Time Error
int runtime_error(lua_State * L, std::ostream & stream);

bool dofile(lua_State * L, const char * file );

void stackdump(lua_State* l);

void Register( lua_State * L, const char * name, const luaL_Reg * funcs );

void register_funcs( lua_State * lua, const char * table_name, const luaL_Reg * funcs );

void call_va (lua_State * L, const char *func, const char *sig, ...);

bool global_func_exists( lua_State * lua, char const * func_name );

bool call_func_if_exists( lua_State * lua, const char *func, const char *sig, ... );

void * checklightuserdata( lua_State * lua, int const & index );

bool checkbool( lua_State * lua, int const & index );

// Clear the stack
void pop_all( lua_State * lua );
}

}

#endif
