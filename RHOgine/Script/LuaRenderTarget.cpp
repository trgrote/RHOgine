#include "LuaRenderTarget.h"

#include <SFML/Graphics.hpp>

using namespace rho;

///////////////////////////////////////////////////////////////////////////////

static int l_draw( lua_State * lua )
{
	sf::RenderTarget * p_render_target =
		static_cast< sf::RenderTarget * > ( lua::checklightuserdata( lua, 1 ) );

	sf::Drawable * p_drawable =
		static_cast< sf::Drawable * > ( lua::checklightuserdata( lua, 2 ) );

    auto render_state = sf::RenderStates::Default;

    if ( lua_gettop(lua) == 3 )
    {
        render_state.blendMode = static_cast<sf::BlendMode>(luaL_checkinteger(lua, 3));
        p_render_target->draw( *p_drawable, render_state );
    }

	lua::pop_all( lua );

	p_render_target->draw( *p_drawable, render_state );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static luaL_Reg const render_target_funcs [] =
{
	{ "draw", 			l_draw },
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////////

void lua::registerRenderTarget( lua_State * lua )
{
	lua::register_funcs( lua, "RenderTarget", render_target_funcs );

	// Register BlendModes
	// get the table,
	lua_getglobal( lua, "BlendMode" );

	// It it doesn't exist, make a new one
	if ( lua_isnil( lua, 1 ) || !lua_istable( lua, 1 ) )
	{
		lua_pop( lua, 1 );	// pop that shit off the stack
		lua_newtable( lua );
		lua_setglobal( lua, "BlendMode" );
		lua_getglobal( lua, "BlendMode" );
	}

	lua_pushstring( lua, "None" );
    lua_pushinteger( lua, sf::BlendMode::BlendNone );
    lua_settable ( lua, -3 );

    lua_pushstring( lua, "Add" );
    lua_pushinteger( lua, sf::BlendMode::BlendAdd );
    lua_settable ( lua, -3 );

    lua_pushstring( lua, "Alpha" );
    lua_pushinteger( lua, sf::BlendMode::BlendAlpha );
    lua_settable ( lua, -3 );

    lua_pushstring( lua, "Multiply" );
    lua_pushinteger( lua, sf::BlendMode::BlendMultiply );
    lua_settable ( lua, -3 );

    lua_pop(lua, 1);
}

///////////////////////////////////////////////////////////////////////////////
