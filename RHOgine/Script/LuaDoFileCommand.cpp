#include "LuaDoFileCommand.h"
#include <iostream>
#include <sstream>

using namespace rho;
using namespace commands;
using namespace lua;

LuaDofileCommand::LuaDofileCommand( lua_State * lua )
:	m_pLua( lua )
{
}

std::string LuaDofileCommand::getName() const
{
	return "dofile";
}

std::string LuaDofileCommand::getDescription() const
{
	return "run lua script";
}

std::string LuaDofileCommand::execute( std::list< std::string > const & arg_list )
{
	// First Argument is script name, that's fucking it
	std::string script_name = arg_list.front();

	std::stringstream rval_stream;

    bool rval = compile_error( m_pLua, luaL_loadfile( m_pLua, script_name.c_str() ), rval_stream );
    if ( lua_pcall( m_pLua, 0, LUA_MULTRET, 0) != LUA_OK )
	{
		// print out the error that SHOULD be on the top of the stack
		rval_stream << lua_tostring( m_pLua, -1 ) << std::endl;

		lua_pop( m_pLua, 1 );	// pop off error

		rval = false;
	}
	else if ( rval != false )
	{
		rval_stream << "Successfully Ran " << script_name << std::endl;
	}

    // stackdump( m_pLua );
    rho::lua::pop_all(m_pLua);

	return rval_stream.str();
}
