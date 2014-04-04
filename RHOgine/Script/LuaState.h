#ifndef RHO_LUA_STATE_H
#define RHO_LUA_STATE_H

#include <lua/lua.hpp>
#include "../FirstComeSingleton.h"

namespace rho
{

/**
  * RAII Lua State
  */
class LuaState : public FirstComeSingleton< LuaState >
{
public:
	lua_State * m_pLua;

	LuaState()
	:	m_pLua( luaL_newstate() )
	{
		luaL_openlibs( m_pLua );
	}

	virtual ~LuaState()
	{
		lua_close( m_pLua );
	}

	inline operator lua_State*()
	{
		return m_pLua;
	}
};

}

#endif
