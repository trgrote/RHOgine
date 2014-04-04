#include "LuaCamera.h"

#include "lua_helpers.h"

#include "../Graphics/Camera.h"

using namespace rho;
using namespace lua;

///////////////////////////////////////////////////////////////////////////////

static int l_newCamera( lua_State * lua  )
{
    Camera * p_camera = NULL;

    if ( lua_gettop( lua ) == 4 )
    {
        sf::FloatRect view_bounds;

        view_bounds.left    = luaL_checknumber(lua, 1);
        view_bounds.top     = luaL_checknumber(lua, 2);
        view_bounds.width   = luaL_checknumber(lua, 3);
        view_bounds.height  = luaL_checknumber(lua, 4);

        p_camera = new Camera(view_bounds);
    }
    else
    {
        p_camera = new Camera;
    }

    lua_pushlightuserdata( lua, p_camera );

	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_deleteCamera( lua_State * lua  )
{
	Camera * p_camera = static_cast<Camera*>( lua::checklightuserdata( lua, 1 ) );

	delete p_camera;

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_move( lua_State * lua )
{
	Camera * p_camera = static_cast<Camera*>( lua::checklightuserdata( lua, 1 ) );

	float x_offset = luaL_checkint( lua, 2 );
	float y_offset = luaL_checkint( lua, 3 );

	lua::pop_all( lua );

	p_camera->move( x_offset, y_offset );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getCenter( lua_State * lua )
{
	Camera * p_camera = static_cast<Camera*>( lua::checklightuserdata( lua, 1 ) );

	lua::pop_all( lua );

	lua_pushnumber( lua, p_camera->getCenter().x );
	lua_pushnumber( lua, p_camera->getCenter().y );

	return 2;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setCenter( lua_State * lua )
{
	Camera * p_camera = static_cast<Camera*>( lua::checklightuserdata( lua, 1 ) );

	float x = luaL_checkint( lua, 2 );
	float y = luaL_checkint( lua, 3 );

	lua::pop_all( lua );

	p_camera->setCenter( x, y );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static luaL_Reg const camerea_funcs [] =
{
	{ "newCamera", 			l_newCamera },
	{ "deleteCamera", 		l_deleteCamera },
	{ "move", 				l_move },
	{ "getCenter", 			l_getCenter },
	{ "setCenter", 			l_setCenter },
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////////

int lua::registerLuaCamera( lua_State * lua )
{
	lua::register_funcs( lua, "Camera", camerea_funcs );

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
