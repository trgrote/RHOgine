#ifndef LUA_GAME_OBJECT_H
#define LUA_GAME_OBJECT_H

#include "../GameObjects.h"

#include "../Scripting.h"

namespace rho
{

class LuaGameObject : public GameObject
{
protected:
    /// The Object's Table Name
    char const * obj_table_name;

    lua_State * lua;

public:
	LuaGameObject( HashString const & obj_name );
	LuaGameObject( HashString const & obj_name, GameObjectInfo const & info );
	virtual ~LuaGameObject();

	virtual void update( sf::Time const & elpased_time );
	virtual void draw( sf::RenderTarget &target, sf::RenderStates states );
	virtual void HandleEvent( rho::Event const & event );

/// STATIC ///
protected:
	static char const * s_kInitName;
    static char const * s_kUpdateName;
    static char const * s_kDrawName;
    static char const * s_kHandleEvent;
    static char const * s_kShutdown;
    static char const * s_kGameObjectData;

	static int s_iGameObjectDataRegistryKey;

public:
	static void registerLuaGameObject( lua_State * lua );

};

}

#endif
