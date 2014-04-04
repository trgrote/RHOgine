#include "LuaHashString.h"
#include "lua_helpers.h"

using namespace rho;
using namespace lua;

///////////////////////////////////////////////////////////////////////////////
bool rho::lua::isHashString( lua_State * lua, int const & index )
{
    return lua_isnumber( lua, index ) || lua_isstring ( lua, index );
}

///////////////////////////////////////////////////////////////////////////////

HashString rho::lua::chechHashString ( lua_State * lua, int const & pos )
{
	HashString rval;

	if ( lua_isnumber( lua, pos ) )
	{
		// Make sure HashString with this value is interned
		StringID id = lua_tointeger( lua, pos );

		if ( ! HashString::isStringInterned( id ) )
		{
			luaL_argerror ( lua, pos, "Invalid or uninternned StringID given" );
		}

		rval = HashString( id );	// else, it's ok
	}
	else if ( lua_isstring ( lua, pos ) )
	{
		rval = HashString( luaL_checkstring( lua, pos ) );
	}
	else
	{
		luaL_argerror( lua, pos, "Neither StringID or String given for HashString type" );

		return HashString( HashString::s_kEmptyString );
	}

	return rval;
}

///////////////////////////////////////////////////////////////////////////////

StringID checkhashStringID( lua_State * lua, int const & pos )
{
    if ( lua_isnumber( lua, pos ) )
	{
		// Make sure HashString with this value is interned
		StringID id = lua_tointeger( lua, pos );

		if ( ! HashString::isStringInterned( id ) )
		{
			luaL_argerror ( lua, pos, "Invalid or uninternned StringID given" );
		}

		return id;
	}
	else
    {
        luaL_argerror ( lua, pos, "Invalid or uninternned StringID given" );
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_internString( lua_State * lua )
{
    StringID rval = 0;

    // Get String
	char const * str = luaL_checkstring( lua, 1 );	// get First lua
	lua_pop( lua, 1 );	// Remove Arguments

    // Hash IT!
	rval = HashString::internString( str );

	lua_pushnumber( lua, rval );	// Push the string back

	return 1;
}

static int l_getStringFromHash( lua_State * lua )
{
    StringID id = (StringID) luaL_checkint( lua, 1 );
    std::string rval = HashString::getStringFromHash(id);

    lua_pushstring( lua, rval.c_str() );

    return 1;
}

///////////////////////////////////////////////////////////////////////////////

static luaL_Reg const hashString_functions [] =
{
	{ "internString", 		l_internString },
	{ "getStringFromHash", 	l_getStringFromHash },
	{ NULL, 		        NULL }		// Null termination
};

///////////////////////////////////////////////////////////////////////////////

void rho::lua::registerLuaHashString( lua_State * lua )
{
	// Make new table called "HashString"
	// push functions "internString" and "isStringInterned" into table
	// pop table off stack
	lua::register_funcs( lua, "HashString", hashString_functions );
}

