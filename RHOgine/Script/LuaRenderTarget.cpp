#include "LuaRenderTarget.h"

#include <SFML/Graphics.hpp>

using namespace rho;

static sf::BlendMode getBlendMode( int const & blend_mode )
{
	if ( blend_mode == 0 )
		return sf::BlendNone;
	if ( blend_mode == 1 )
		return sf::BlendAdd;
	if ( blend_mode == 2 )
		return sf::BlendAlpha;
	if ( blend_mode == 3 )
		return sf::BlendMultiply;
	return sf::BlendNone;
}

///////////////////////////////////////////////////////////////////////////////

static int l_draw( lua_State * lua )
{
	sf::RenderTarget * p_render_target =
		static_cast< sf::RenderTarget * > ( lua::checklightuserdata( lua, 1 ) );

	sf::Drawable * p_drawable =
		static_cast< sf::Drawable * > ( lua::checklightuserdata( lua, 2 ) );

    auto render_state = sf::RenderStates::Default;

	// Third Argument is blend mode
    if ( lua_gettop(lua) >= 3 )
    {
        render_state.blendMode = getBlendMode(luaL_checkinteger(lua, 3));
    }
	
	// 4th Argument is a shader pointer
	if ( lua_gettop(lua) >= 4 )
    {
        render_state.shader = static_cast<sf::Shader*>(lua::checklightuserdata(lua, 4));
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
    lua_pushinteger( lua, 0 );		// BlendNone = 0
    lua_settable ( lua, -3 );

    lua_pushstring( lua, "Add" );
    lua_pushinteger( lua, 1 );		// BlendAdd = 1
    lua_settable ( lua, -3 );

    lua_pushstring( lua, "Alpha" );
    lua_pushinteger( lua, 2 );		// BlendAlpha = 2
    lua_settable ( lua, -3 );

    lua_pushstring( lua, "Multiply" );
    lua_pushinteger( lua, 3 );	// BlendMultiply = 3
    lua_settable ( lua, -3 );

    lua_pop(lua, 1);
}

///////////////////////////////////////////////////////////////////////////////
