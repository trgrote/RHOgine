#include "LuaGOFactoryManager.h"

#include "../GameObjectFactory/GOFactoryManager.h"

using namespace rho;

///////////////////////////////////////////////////////////////////////////////

static int l_loadObjectFactoriesFromJSON( lua_State * lua )
{
	char const * const json_file_name = luaL_checkstring( lua, 1 );

	lua_pop( lua, 1 );

	if ( FirstComeSingleton< GOFactoryManager >::GetInstance().loadObjectFactoriesFromJSON(
		json_file_name ) )
	{
		lua_pushboolean( lua, 1 );	// return true
	}
	else
	{
		lua_pushboolean( lua, 0 );	// return false
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_loadLuaObjectFactory( lua_State * lua )
{
	HashString object_name, script_name;
	
	object_name = lua::chechHashString( lua, 1 );
	script_name = lua::chechHashString( lua, 2 );
	
	lua::pop_all( lua );
	
	lua_pushboolean( 
		lua, 
		GOFactoryManager::GetInstance().loadLuaObjectFactory( object_name, script_name ) 
	);
	
	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static luaL_Reg const gof_man_functions [] =
{
    { "loadObjectFactoriesFromJSON",        l_loadObjectFactoriesFromJSON },
    { "loadLuaObjectFactory",        		l_loadLuaObjectFactory },
    { NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////////

void rho::lua::registerLuaGOFactoryManager( lua_State * lua )
{
	lua::register_funcs( lua, "GOFactoryManager", gof_man_functions );
}

///////////////////////////////////////////////////////////////////////////////
