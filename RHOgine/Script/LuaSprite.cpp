#include "LuaSprite.h"
#include <SFML/Graphics.hpp>
#include "../Graphics.h"

#include "LuaHashString.h"

using namespace rho;
using namespace sf;
using namespace lua;

///////////////////////////////////////////////////////////////////////////////

#define CHECK_SPRITE( lua, index )\
	static_cast< RefCountSprite* >( lua::checklightuserdata( lua, index ) )

///////////////////////////////////////////////////////////////////////////////

namespace
{

class RefCountSprite : public sf::Sprite
{
private:
	/// Reference Counted Texture Resource
	pTexture m_pTexture;

public:
	void setTexturePtr ( pTexture p_texture )
	{
		m_pTexture = p_texture;
	}
};

}

///////////////////////////////////////////////////////////////////////////////

static int l_newSprite( lua_State * lua )
{
	RefCountSprite * p_sprite = new RefCountSprite;

	if ( lua_gettop(lua) >= 1 && isHashString( lua, 1 ) )
	{
		HashString texture_name = chechHashString( lua, 1 );

		pTexture p_texture = TextureManager::GetInstance().getTexture( texture_name );

		if ( p_texture.get() == NULL )
		{
			return luaL_error( lua, "Could not load texture %s\n", texture_name.getString().c_str() );
		}

		p_sprite->setTexturePtr( p_texture );		// Set the Ref Counted Texture Resource
		p_sprite->setTexture( *p_texture, true );	// Set the Texure and Resize the Sprite to the new Texture Dimensions
	}

	lua::pop_all( lua );

	lua_pushlightuserdata( lua, p_sprite );

	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_deleteSprite( lua_State * lua )
{
	RefCountSprite * p_sprite = CHECK_SPRITE( lua, 1 );
	delete p_sprite;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setTexture( lua_State * lua )
{
	RefCountSprite * p_sprite = CHECK_SPRITE( lua, 1 );
	HashString texture_name = chechHashString( lua, 2 );

	pTexture p_texture = TextureManager::GetInstance().getTexture( texture_name );

	if ( p_texture.get() == NULL )
	{
		return luaL_error( lua, "Could not load texture %s\n", texture_name.getString().c_str() );
	}

	p_sprite->setTexturePtr( p_texture );		// Set the Ref Counted Texture Resource
	p_sprite->setTexture( *p_texture, true );	// Set the Texure and Resize the Sprite to the new Texture Dimensions

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setPosition( lua_State * lua )
{
	RefCountSprite * p_sprite = CHECK_SPRITE( lua, 1 );

	int x = luaL_checkint( lua, 2 );
	int y = luaL_checkint( lua, 3 );

	lua::pop_all( lua );

	p_sprite->setPosition( x, y );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getPosition( lua_State * lua )
{
	RefCountSprite * p_sprite = CHECK_SPRITE( lua, 1 );

	lua::pop_all( lua );

	auto pos = p_sprite->getPosition();

	lua_pushnumber( lua, pos.x );
	lua_pushnumber( lua, pos.y );

	return 2;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setScale( lua_State * lua )
{
	RefCountSprite * p_sprite = CHECK_SPRITE( lua, 1 );

	float x = luaL_checknumber( lua, 2 );
	float y = luaL_checknumber( lua, 3 );

	lua::pop_all( lua );

	p_sprite->setScale( x, y );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getScale( lua_State * lua )
{
	RefCountSprite * p_sprite = CHECK_SPRITE( lua, 1 );

	lua::pop_all( lua );

	auto pos = p_sprite->getScale();

	lua_pushnumber( lua, pos.x );
	lua_pushnumber( lua, pos.y );

	return 2;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getBounds( lua_State * lua )
{
	RefCountSprite * p_sprite = CHECK_SPRITE( lua, 1 );

	lua::pop_all( lua );

	auto pos = p_sprite->getGlobalBounds();

	lua_pushnumber( lua, pos.left );
	lua_pushnumber( lua, pos.top );
	lua_pushnumber( lua, pos.width );
	lua_pushnumber( lua, pos.height );

	return 4;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getOrigin( lua_State * lua )
{
    RefCountSprite * p_sprite = CHECK_SPRITE( lua, 1 );

	lua::pop_all( lua );

    auto origin = p_sprite->getOrigin();

    lua_pushnumber( lua, origin.x );
	lua_pushnumber( lua, origin.y );

    return 2;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setOrigin( lua_State * lua )
{
    RefCountSprite * p_sprite = CHECK_SPRITE( lua, 1 );

    float x = luaL_checknumber( lua, 2 );
	float y = luaL_checknumber( lua, 3 );

    lua::pop_all( lua );

    p_sprite->setOrigin(x,y);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getRotation( lua_State * lua )
{
    RefCountSprite * p_sprite = CHECK_SPRITE( lua, 1 );

	lua::pop_all( lua );

    lua_pushnumber( lua, p_sprite->getRotation() );

    return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setRotation( lua_State * lua )
{
    RefCountSprite * p_sprite = CHECK_SPRITE( lua, 1 );

    float rotation = luaL_checknumber( lua, 2 );

    lua::pop_all( lua );

    p_sprite->setRotation(rotation);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setTextureRect( lua_State * lua )
{
	RefCountSprite * p_sprite = CHECK_SPRITE( lua, 1 );

	sf::IntRect texture_rect;
	texture_rect.left 	= luaL_checkint( lua, 2 );
	texture_rect.top 	= luaL_checkint( lua, 3 );
	texture_rect.width 	= luaL_checkint( lua, 4 );
	texture_rect.height = luaL_checkint( lua, 5 );

	p_sprite->setTextureRect( texture_rect );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getTextureRect( lua_State * lua )
{
	RefCountSprite * p_sprite = CHECK_SPRITE( lua, 1 );

	lua::pop_all( lua );

	auto pos = p_sprite->getTextureRect();

	lua_pushnumber( lua, pos.left );
	lua_pushnumber( lua, pos.top );
	lua_pushnumber( lua, pos.width );
	lua_pushnumber( lua, pos.height );

	return 4;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getTextureSize( lua_State * lua )
{
    RefCountSprite * p_sprite = CHECK_SPRITE( lua, 1 );

	lua::pop_all( lua );

    auto size = p_sprite->getTexture()->getSize();

	lua_pushnumber( lua, size.x );
	lua_pushnumber( lua, size.y );

	return 2;
}

///////////////////////////////////////////////////////////////////////////////

static luaL_Reg const sprite_funcs [] =
{
	{ "newSprite", 			l_newSprite },
	{ "deleteSprite", 		l_deleteSprite },
	{ "setTexture",	        l_setTexture },
	{ "setPosition", 		l_setPosition },
	{ "getPosition", 		l_getPosition },
	{ "getScale", 			l_getScale },
	{ "setScale", 			l_setScale },
	{ "getBounds", 			l_getBounds },
	{ "getOrigin", 			l_getOrigin     },
	{ "setOrigin", 			l_setOrigin     },
	{ "getRotation", 		l_getRotation   },
	{ "setRotation", 		l_setRotation   },
	{ "setTextureRect", 	l_setTextureRect   },
	{ "getTextureRect", 	l_getTextureRect   },
	{ "getTextureSize",     l_getTextureSize    },
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////////

void lua::registerSprite( lua_State * lua )
{
	lua::register_funcs( lua, "Sprite", sprite_funcs );
}

///////////////////////////////////////////////////////////////////////////////
