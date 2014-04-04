#include "LuaDrawableRect.h"

#include <SFML/Graphics.hpp>

#include <unordered_set>

using namespace rho;

/// Map containing all RectngableShapes that we've given to lua bo bua.
/// boolean value detemrines is object has been deleted yet
static std::unordered_set< sf::RectangleShape * > s_luaRectangles;

#define CHECK_DRAWABLERECT( p_rect_shape ) {\
	if ( s_luaRectangles.find( p_rect_shape ) == s_luaRectangles.cend() )\
		return luaL_argerror( lua, 1, "Expected undeleted DrawableRectangle" ); }


///////////////////////////////////////////////////////////////////////////////

static int l_newDrawableRect( lua_State * lua )
{
	int num_args = lua_gettop( lua );
	sf::RectangleShape * p_rval = NULL;

	if ( num_args == 4 )
	{
		float x 		= luaL_checknumber( lua, 1 );
		float y 		= luaL_checknumber( lua, 2 );
		float width 	= luaL_checknumber( lua, 3 );
		float height 	= luaL_checknumber( lua, 4 );

		lua_pop( lua, 4 );

		p_rval = new sf::RectangleShape( sf::Vector2f ( width, height ) );
		p_rval->setPosition( x, y );
	}
	else if ( num_args == 2 )
	{
		float width 	= luaL_checknumber( lua, 1 );
		float height 	= luaL_checknumber( lua, 2 );

		lua_pop( lua, 2 );

		p_rval = new sf::RectangleShape( sf::Vector2f ( width, height ) );
	}
	else
	{
		return luaL_error( lua, "New Drawable Rect Expecting 2 or 4 arguments, got %d", num_args );
	}

	s_luaRectangles.insert( p_rval );	// add new value in the set

	lua_pushlightuserdata( lua, p_rval );

	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_deleteDrawableRect( lua_State * lua )
{
	sf::RectangleShape * p_rect_shape = static_cast< sf::RectangleShape * >(
		lua::checklightuserdata( lua, 1 ) );

	std::unordered_set< sf::RectangleShape * >::iterator ptr_iter =
		s_luaRectangles.find( p_rect_shape );

	if ( ptr_iter == s_luaRectangles.end() )
		return luaL_argerror( lua, 1, "Expected undeleted DrawableRectangle" );

	delete p_rect_shape;

	lua::pop_all( lua );

	s_luaRectangles.erase( ptr_iter );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setFillColor( lua_State * lua )
{
	sf::RectangleShape * p_rect_shape = static_cast< sf::RectangleShape * >(
		lua::checklightuserdata( lua, 1 ) );

	CHECK_DRAWABLERECT( p_rect_shape );

	int red 	= luaL_checkint( lua, 2 );
	int green 	= luaL_checkint( lua, 3 );
	int blue 	= luaL_checkint( lua, 4 );

	if ( lua_gettop( lua ) == 5 )
	{
		// there is another argument ...
		int alpha = luaL_checkint( lua, 5 );

		p_rect_shape->setFillColor( sf::Color( red, green, blue, alpha ) );
	}
	else
	{
		p_rect_shape->setFillColor( sf::Color( red, green, blue ) );
	}

	lua::pop_all( lua );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setPosition( lua_State * lua )
{
	sf::RectangleShape * p_rect_shape = static_cast< sf::RectangleShape * >(
		lua::checklightuserdata( lua, 1 ) );

	CHECK_DRAWABLERECT( p_rect_shape );

	float x = luaL_checknumber( lua, 2 );
	float y = luaL_checknumber( lua, 3 );

	lua::pop_all( lua );

	p_rect_shape->setPosition( sf::Vector2f( x, y ) );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getPosition( lua_State * lua )
{
    sf::RectangleShape * p_rect_shape = static_cast< sf::RectangleShape * >(
		lua::checklightuserdata( lua, 1 ) );

	CHECK_DRAWABLERECT( p_rect_shape );

	lua::pop_all( lua );

    auto pos = p_rect_shape->getPosition();

    lua_pushnumber(lua, pos.x);
    lua_pushnumber(lua, pos.y);

    return 2;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getGlobalBounds( lua_State * lua )
{
	sf::RectangleShape * p_rect_shape = static_cast< sf::RectangleShape * >(
		lua::checklightuserdata( lua, 1 ) );

	CHECK_DRAWABLERECT( p_rect_shape );

	lua::pop_all( lua );

    auto bounds = p_rect_shape->getGlobalBounds();

    lua_pushnumber(lua, bounds.left);
    lua_pushnumber(lua, bounds.top);
    lua_pushnumber(lua, bounds.width);
    lua_pushnumber(lua, bounds.height);

	return 4;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setOutlineThickness( lua_State * lua )
{
    sf::RectangleShape * p_rect_shape = static_cast< sf::RectangleShape * >(
		lua::checklightuserdata( lua, 1 ) );

	CHECK_DRAWABLERECT( p_rect_shape );



    p_rect_shape->setOutlineThickness(
        luaL_checkint( lua, 2 )
        );
lua::pop_all( lua );
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setOutlineColor( lua_State * lua )
{
    sf::RectangleShape * p_rect_shape = static_cast< sf::RectangleShape * >(
		lua::checklightuserdata( lua, 1 ) );

	CHECK_DRAWABLERECT( p_rect_shape );

	int red 	= luaL_checkint( lua, 2 );
	int green 	= luaL_checkint( lua, 3 );
	int blue 	= luaL_checkint( lua, 4 );

	if ( lua_gettop( lua ) == 5 )
	{
		// there is another argument ...
		int alpha = luaL_checkint( lua, 5 );

		p_rect_shape->setOutlineColor( sf::Color( red, green, blue, alpha ) );
	}
	else
	{
		p_rect_shape->setOutlineColor( sf::Color( red, green, blue ) );
	}

	lua::pop_all( lua );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setBounds( lua_State * lua )
{
    sf::RectangleShape * p_rect_shape = static_cast< sf::RectangleShape * >(
		lua::checklightuserdata( lua, 1 ) );

	CHECK_DRAWABLERECT( p_rect_shape );

    float x 		= luaL_checknumber( lua, 2 );
    float y 		= luaL_checknumber( lua, 3 );
    float width 	= luaL_checknumber( lua, 4 );
    float height 	= luaL_checknumber( lua, 5 );

    p_rect_shape->setPosition(x,y);
    p_rect_shape->setSize(sf::Vector2f(width,height));

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

static luaL_Reg const rect_funcs [] =
{
	{ "newDrawableRect", 	 l_newDrawableRect 		},
	{ "deleteDrawableRect",  l_deleteDrawableRect 	},
	{ "setFillColor",	     l_setFillColor 			},
	{ "setPosition", 		 l_setPosition 			},
	{ "getPosition", 		 l_getPosition 			},
	{ "getGlobalBounds", 	 l_getGlobalBounds 		},
	{ "setOutlineThickness", l_setOutlineThickness 		},
	{ "setOutlineColor", 	 l_setOutlineColor 		},
	{ "setBounds", 	        l_setBounds 		},
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////////

void rho::lua::registerDrawableRect( lua_State * lua )
{
	s_luaRectangles.clear();
	lua::register_funcs( lua, "DrawableRect", rect_funcs );
}

///////////////////////////////////////////////////////////////////////////////
