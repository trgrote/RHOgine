#include "LuaWindow.h"

#include "../RHOgine.h"

#include <cstring>

using namespace rho;

static int l_createWindow( lua_State * lua )
{
    sf::VideoMode video_mode( 640, 480 );   // Default
    sf::String title = "RHOgine Window";
    sf::Uint32 style = sf::Style::Default;
    // Context Settings coming ... if we ever need them

    // Complete Table in Lua, as argument ( if not given, uses default )
    //{
//        width 			= 800,
//        height 			= 600,
//        bitsPerPixel 	= 32,
//        title = "RHOGINE-dingus Test CASE ALPHA BRAVO",
//        style = "Fullscreen"
//    }

    if ( lua_istable( lua, 1 ) )    // Top of the stack should be a FUCKING TABLE, HELLS YEAH!
    {
        // width
        lua_getfield( lua, 1, "width" );
        if ( lua_isnumber( lua, -1 ) )
        {
            video_mode.width = luaL_checkinteger( lua, -1 );
            if ( video_mode.width < 1 )
                    luaL_argerror(lua, 1, "Argument: width was < 1 " );

        }
        lua_pop( lua, 1 );

        // height
        lua_getfield( lua, 1, "height" );
        if ( lua_isnumber( lua, -1 ) )
        {
            video_mode.height = luaL_checkinteger( lua, -1 );
            if ( video_mode.height < 1 )
                    luaL_argerror(lua, 1, "Argument: height was < 1 " );

        }
        lua_pop( lua, 1 );

        // bitsPerPixel
        lua_getfield( lua, 1, "bitsPerPixel" );
        if ( lua_isnumber( lua, -1 ) )
        {
            video_mode.bitsPerPixel = luaL_checkinteger( lua, -1 );
            if ( video_mode.bitsPerPixel < 1 )
                    luaL_argerror(lua, 1, "Argument: bitsPerPixel was < 1 " );

        }
        lua_pop( lua, 1 );

        // title
        lua_getfield( lua, 1, "title" );
        if ( lua_isstring( lua, -1 ) )
        {
            title = sf::String( luaL_checkstring( lua, -1 ) );
        }
        lua_pop( lua, 1 );

        // style
        lua_getfield( lua, 1, "style" );
        if ( lua_isstring( lua, -1 ) )
        {
            char const * str_style = luaL_checkstring( lua, -1 );

            if ( strcmp(str_style, "Fullscreen" ) == 0 )
            {
                style = sf::Style::Fullscreen;
            }
			else if ( strcmp(str_style, "SplashScreen" ) == 0 )
			{
				style = sf::Style::None;
			}
            else
            {
                // Include Warnings
            }
        }
        lua_pop( lua, 1 );
    }
    else
    {
        return luaL_argerror(lua, 1, "Table expected");
    }

    sf::RenderWindow & window = cRHOgine::GetInstance().getRenderWindow();

    window.create(
        video_mode,
        title,
        style
    );

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getSize( lua_State * lua )
{
    sf::RenderWindow & window = cRHOgine::GetInstance().getRenderWindow();

    auto size = window.getSize();

    lua_pushinteger(lua, size.x );
    lua_pushinteger(lua, size.y );

    return 2;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getFullScreen( lua_State * lua )
{
    return 1;
}

///////////////////////////////////////////////////////////////////////////////

static luaL_Reg const window_functions [] =
{
	{ "createWindow",   l_createWindow },
	{ "getSize",        l_getSize },
	{ "getFullScreen", 	l_getFullScreen },
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////////

int lua::registerLuaWindow( lua_State * lua )
{
	lua::register_funcs( lua, "Window", window_functions );

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
