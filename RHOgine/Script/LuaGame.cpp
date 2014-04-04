#include "LuaGame.h"

#include "../RHOgine.h"

using namespace rho;

///////////////////////////////////////////////////////////////////////////////

static int l_getElapsedTime( lua_State * lua )
{
	cRHOgine & p_game = cRHOgine::GetInstance();

	sf::Time const & elapsed_time = p_game.getElapsedTime();

	lua::pop_all( lua );

	lua_pushnumber( lua, elapsed_time.asSeconds() );	// Push Time

	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_drawScene( lua_State * lua )
{
	Camera * p_camera = static_cast< Camera * >( lua_touserdata( lua, 1 ) );
	Scene * p_scene = static_cast< Scene * >( lua_touserdata( lua, 2 ) );

	cRHOgine::GetInstance().drawScene( *p_camera, *p_scene );

	lua::pop_all( lua );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_endGame( lua_State * lua )
{
	cRHOgine::GetInstance().EndGame();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static luaL_Reg const game_functions [] =
{
	{ "getElapsedTime", l_getElapsedTime },
	{ "drawScene",      l_drawScene },
	{ "endGame", 		l_endGame },
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////////

int lua::registerLuaGame( lua_State * lua )
{
	lua::register_funcs( lua, "Game", game_functions );

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
