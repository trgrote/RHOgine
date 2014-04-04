#include "LuaScene.h"

#include "../RHOgine.h"
#include "lua_helpers.h"

#include <iostream>

using namespace rho;
using namespace lua;

///////////////////////////////////////////////////////////////////////////////

static int l_newScene( lua_State * lua  )
{
	Scene * p_scene = new Scene;

	lua_pushlightuserdata( lua, p_scene );

	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_deleteScene( lua_State * lua  )
{
	Scene * p_scene = static_cast<Scene*>( lua_touserdata( lua, 1 ) );

	delete p_scene;

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_createGameObject( lua_State * lua )
{
	ErrorContext ec( "Creating GameObject inside of a scene", NULL );
	
	Scene * p_scene = static_cast<Scene*>( lua_touserdata( lua, 1 ) );

	HashString obj_name = chechHashString( lua, 2 );	// get object name

    // lua::pop_all(lua);

	GameObjectInfo * info = GOFactoryManager::GetInstance().newGameObjInfo();
	
	info->scene = p_scene;

	// if we have another argument
	if ( lua_gettop( lua ) > 2 && lua_istable( lua, 3 ) )	// if there is another argument, and it's a table, then we got another ctor argument from lua
	{
		// grab any info you find from the table that is generic GameObject info, like x, y, visible, enabled, etc
		// std::cout << "We got more info\n";
		
		info->tableArgIndex = 3;	// set table index
		
		// Scan the goddang table for generic info to put into the info structure
		lua_getfield( lua, 3, "x" );
		if ( ! lua_isnil( lua, -1 ) && lua_isnumber( lua, -1 ) )	// if we found a value that wasn't nil
		{
			info->x = lua_tointeger( lua, -1 );
		}
		lua_pop( lua, 1 );
		
		lua_getfield( lua, 3, "y" );
		if ( ! lua_isnil( lua, -1 ) && lua_isnumber( lua, -1 ) )	// if we found a value that wasn't nil
		{
			info->y = lua_tointeger( lua, -1 );
		}
		lua_pop( lua, 1 );
		
		lua_getfield( lua, 3, "enabled" );
		if ( ! lua_isnil( lua, -1 ) && lua_isboolean( lua, -1 ) )	// if we found a value that wasn't nil
		{
			info->enabled = lua_toboolean( lua, -1 );
		}
		lua_pop( lua, 1 );
		
		lua_getfield( lua, 3, "visible" );
		if ( ! lua_isnil( lua, -1 ) && lua_isboolean( lua, -1 ) )	// if we found a value that wasn't nil
		{
			info->visible = lua_toboolean( lua, -1 );
		}
		lua_pop( lua, 1 );
	}
	
	lua_pushlightuserdata( lua, p_scene->createGameObject(obj_name, *info) );

	GOFactoryManager::GetInstance().deleteGameObjInfo( info );
	
	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_update( lua_State * lua )
{
    Scene * p_scene = static_cast< Scene*> ( lua::checklightuserdata( lua, 1 ) );

    float dt = luaL_checknumber( lua, 2 );

    lua::pop_all(lua);

    p_scene->update(sf::seconds( dt ) );

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

static luaL_Reg const scene_functions [] =
{
	{ "newScene", 			l_newScene },
	{ "deleteScene", 		l_deleteScene },
	{ "createGameObject", 	l_createGameObject },
	{ "update",             l_update },
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////////

int lua::registerLuaScene( lua_State * lua )
{
	lua::register_funcs( lua, "Scene", scene_functions );

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
