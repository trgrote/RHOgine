#ifndef LUA_DOFILE_COMMAND_H
#define LUA_DOFILE_COMMAND_H

#include "../Console/IConsoleCommand.h"

#include <lua/lua.hpp>
#include "../Script/lua_helpers.h"

namespace rho
{

namespace commands
{

class LuaDofileCommand : public IConsoleCommand
{
public:
	lua_State * m_pLua;
	
	LuaDofileCommand( lua_State * lua );
	
	virtual std::string getName() const;
    virtual std::string getDescription() const;
    virtual std::string execute( std::list< std::string > const & arg_list );
};

}

}

#endif
