#include "LuaText.h"

#include "../Graphics/FontManager.h"

#include <SFML/Graphics.hpp>

#include <unordered_set>

///////////////////////////////////////////////////////////////////////////////

using namespace rho;

namespace
{

class RefCountText : public sf::Text
{
public:
	pFont m_pFont;
};

}

///////////////////////////////////////////////////////////////////////////////

static std::unordered_set< RefCountText * > s_luaTexts;

#define CHECK_TEXT( lua, index )\
	static_cast< RefCountText* >( lua::checklightuserdata( lua, index ) )

#define VERIFY_TEXT( p_text )\
	if ( s_luaTexts.find( p_text ) == s_luaTexts.cend() )\
		return luaL_argerror( lua, 1, "Expected undeleted Text" )

///////////////////////////////////////////////////////////////////////////////

static int l_newText( lua_State * lua )
{
	char const * font_name = luaL_checkstring( lua, 1 );

	pFont font = FontManager::GetInstance().getFont( font_name );

	RefCountText * p_text = new RefCountText;

	p_text->m_pFont = font;

	p_text->setFont( *(p_text->m_pFont) );

	lua::pop_all( lua );

	s_luaTexts.insert( p_text );

	lua_pushlightuserdata( lua, p_text );

	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_deleteText( lua_State * lua )
{
	RefCountText * p_text = CHECK_TEXT( lua, 1 );
	VERIFY_TEXT( p_text );

	delete p_text;

	s_luaTexts.erase( p_text );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setPosition( lua_State * lua )
{
	RefCountText * p_text = CHECK_TEXT( lua, 1 );
	VERIFY_TEXT( p_text );

	float x = luaL_checknumber( lua, 2 );
	float y = luaL_checknumber( lua, 3 );

	lua::pop_all( lua );

	p_text->setPosition( x, y );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getPosition( lua_State * lua )
{
	RefCountText * p_text = CHECK_TEXT( lua, 1 );
	VERIFY_TEXT( p_text );

	lua::pop_all( lua );

	auto pos = p_text->getPosition();

	lua_pushnumber( lua, pos.x );
	lua_pushnumber( lua, pos.y );

	return 2;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setString( lua_State * lua )
{
	RefCountText * p_text = CHECK_TEXT( lua, 1 );
	VERIFY_TEXT( p_text );
	char const * string = luaL_checkstring( lua, 2 );
	
	lua::pop_all( lua );

	p_text->setString( string );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getString( lua_State * lua )
{
	RefCountText * p_text = CHECK_TEXT( lua, 1 );
	VERIFY_TEXT( p_text );
	
	lua::pop_all( lua );

	lua_pushstring( lua, p_text->getString().toAnsiString().c_str() );

	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setColor( lua_State * lua )
{
	RefCountText * p_text = CHECK_TEXT( lua, 1 );
	VERIFY_TEXT( p_text );
	
	// Make Da color
	sf::Color color;
	
	color.r 	= luaL_checkint( lua, 2 );
	color.g 	= luaL_checkint( lua, 3 );
	color.b 	= luaL_checkint( lua, 4 );
	
	if ( lua_gettop( lua ) == 5 )
	{
		// there is another argument ...
		color.a = luaL_checkint( lua, 5 );
	}
	
	lua::pop_all( lua );
	
	p_text->setColor( color );
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getColor( lua_State * lua )
{
	RefCountText * p_text = CHECK_TEXT( lua, 1 );
	VERIFY_TEXT( p_text );
	
	lua::pop_all( lua );
	
	// Make Da color
	sf::Color color = p_text->getColor();
	
	lua_pushinteger( lua, color.r );
	lua_pushinteger( lua, color.g );
	lua_pushinteger( lua, color.b );
	lua_pushinteger( lua, color.a );
	
	return 4;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setCharacterSize( lua_State * lua )
{
	RefCountText * p_text = CHECK_TEXT( lua, 1 );
	VERIFY_TEXT( p_text );

	p_text->setCharacterSize( luaL_checkint( lua, 2 ) );

	lua::pop_all( lua );
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getCharacterSize( lua_State * lua )
{
	RefCountText * p_text = CHECK_TEXT( lua, 1 );
	VERIFY_TEXT( p_text );
	
	lua::pop_all( lua );

	lua_pushinteger( lua, p_text->getCharacterSize() );
	
	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static luaL_Reg const text_funcs [] =
{
	{ "newText", 			l_newText },
	{ "deleteText", 		l_deleteText },
	{ "setPosition", 		l_setPosition },
	{ "getPosition", 		l_getPosition },
	{ "setString", 			l_setString },
	{ "getString", 			l_getString },
	{ "setColor", 			l_setColor },
	{ "getColor", 			l_getColor },
	{ "setCharacterSize", 	l_setCharacterSize },
	{ "getCharacterSize", 	l_getCharacterSize },
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////////

void rho::lua::registerText( lua_State * lua )
{
	lua::register_funcs( lua, "Text", text_funcs );
}

///////////////////////////////////////////////////////////////////////////////
