#include "LuaShader.h"
#include <SFML/Graphics.hpp>
#include <unordered_set>

#include <iostream>

using namespace rho;


static std::unordered_set< sf::Shader * > s_luaShaders;

#define CHECK_SHADER( p_shader, arg ) {\
	if ( s_luaShaders.find( p_shader ) == s_luaShaders.cend() )\
		return luaL_argerror( lua, arg, "Expected undeleted Shader" ); }

///////////////////////////////////////////////////////////////////////////////

static int l_newShader( lua_State * lua )
{
	// Takes Two Arguments: file name, shader type
	sf::Shader * p_shader = new sf::Shader;

	char const * file_name 			= luaL_checkstring( lua, 1 );

	// Else second argument is shader type
	if ( lua_isnumber( lua, 2 ) )
	{
		sf::Shader::Type shader_type = static_cast<sf::Shader::Type>(luaL_checkinteger(lua, 2));

		if ( ! p_shader->loadFromFile( file_name, shader_type ) )
		{
			lua_pushnil( lua );
			return 1;
		}
	}
	// IF we are given another string, then that's the fragment shader file to load
	else if ( lua_isstring( lua, 2 ) )
	{
		char const * fragment_file_name = luaL_checkstring( lua, 2 );

		if ( ! p_shader->loadFromFile( file_name, fragment_file_name ) )
		{
			lua_pushnil( lua );
			return 1;
		}
	}
	else
	{
		// No fucking idea
		return luaL_argerror( lua, 2, "Unexpected type, neither string nor integer" );
	}

	s_luaShaders.insert( p_shader );	// add new value in the set

	lua_pushlightuserdata( lua, p_shader );

	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_deleteShader( lua_State * lua )
{
	sf::Shader * p_shader = static_cast< sf::Shader * >(
		lua::checklightuserdata( lua, 1 ) );

	std::unordered_set< sf::Shader * >::iterator ptr_iter =
		s_luaShaders.find( p_shader );

	if ( ptr_iter == s_luaShaders.end() )
		return luaL_argerror( lua, 1, "Expected undeleted Shader" );

	delete p_shader;

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

// Right now, onyl support for float values and vectors2,3
static int l_setParameter( lua_State * lua )
{
	sf::Shader * p_shader = static_cast< sf::Shader * >(
		lua::checklightuserdata( lua, 1 ) );

	CHECK_SHADER( p_shader, 1 );

	char const * param_name = luaL_checkstring( lua, 2 );

	if ( lua_isnumber( lua, 3 ) )
	{
		float x = luaL_checknumber( lua, 3 );

		if ( lua_isnumber( lua, 4 ) )
		{
			float y = luaL_checknumber( lua, 4 );

			if ( lua_isnumber( lua, 5 ) )
			{
				float z = luaL_checknumber( lua, 5 );

				p_shader->setParameter( param_name, x, y, z );
			}
			else
			{
				p_shader->setParameter( param_name, x, y );
			}
		}
		else
		{
			p_shader->setParameter( param_name, x );
		}
	}
	else
	{
		return luaL_argerror( lua, 2, "Unsupported Shader Parameter Type" );
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setParameterCurrentTexture( lua_State * lua )
{
	sf::Shader * p_shader = static_cast< sf::Shader * >(
		lua::checklightuserdata( lua, 1 ) );

	CHECK_SHADER( p_shader, 1 );

	char const * param_name = luaL_checkstring( lua, 2 );

	p_shader->setParameter( param_name, sf::Shader::CurrentTexture );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static luaL_Reg const shader_funcs [] =
{
	{ "newShader", 			l_newShader 	},
	{ "deleteShader", 		l_deleteShader 	},
	{ "setParameter", 		l_setParameter 	},
	{ "setParameterCurrentTexture", 		l_setParameterCurrentTexture 	},
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////////

void lua::registerLuaShader( lua_State * lua )
{
	lua::register_funcs( lua, "Shader", shader_funcs );

	lua_getglobal( lua, "ShaderType" );

	// It it doesn't exist, make a new one
	if ( lua_isnil( lua, 1 ) || !lua_istable( lua, 1 ) )
	{
		lua_pop( lua, 1 );	// pop that shit off the stack
		lua_newtable( lua );
		lua_setglobal( lua, "ShaderType" );
		lua_getglobal( lua, "ShaderType" );
	}

	lua_pushstring( lua, "Vertex" );
    lua_pushinteger( lua, sf::Shader::Vertex );
    lua_settable ( lua, -3 );

    lua_pushstring( lua, "Fragment" );
    lua_pushinteger( lua, sf::Shader::Fragment );
    lua_settable ( lua, -3 );

	lua_pop(lua, 1);
}
