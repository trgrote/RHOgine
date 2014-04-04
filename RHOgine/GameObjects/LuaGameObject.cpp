#include "LuaGameObject.h"
#include "../RHOgine.h"
#include "../Scripting.h"

using namespace rho;

#define CHECK_GAME_OBJ( l, i ) static_cast<GameObject*>( rho::lua::checklightuserdata( l, i ) )

char const * LuaGameObject::s_kInitName     	= "init";
char const * LuaGameObject::s_kUpdateName   	= "update";
char const * LuaGameObject::s_kDrawName     	= "draw";
char const * LuaGameObject::s_kHandleEvent  	= "handleEvent";
char const * LuaGameObject::s_kShutdown     	= "shutdown";
char const * LuaGameObject::s_kGameObjectData 	= "GameObjectData";
int LuaGameObject::s_iGameObjectDataRegistryKey = LUA_REFNIL;

///////////////////////////////////////////////////////////////////////////////

LuaGameObject::LuaGameObject( HashString const & obj_name )
:   lua( cRHOgine::GetInstance().getLua() )
{
    ErrorContext ec( "LuaGameObject::Ctor", obj_name.getString().c_str() );

	// Create Table fro self in the registry
	lua_pushlightuserdata( lua, (void*)this );			// stack: this
	lua_newtable( lua );						// stack: this (table)
	lua_rawset( lua, LUA_REGISTRYINDEX );		// registry[ this ] = {}
	// stack: empty

	// Make sure this bitch works
	lua_pushlightuserdata( lua, (void*)this );			// stack: this
	lua_gettable( lua, LUA_REGISTRYINDEX);			// stack: registry[ this ]
	assert_msg( lua_istable( lua, -1 ), "Lua Failed to load GameData table in Registry" );

	lua_pop( lua, 1 );

    obj_table_name = obj_name.getString().c_str();  // Get the string

    // Find the Global Table with the name of the object given
    lua_getglobal( lua, obj_table_name );   // Push

    // Make sure the value is a table
    assert_msg ( lua_istable( lua, -1 ), "Table Object not found" );

    // If init exists, call function
    lua_getfield ( lua, -1, s_kInitName );

    // assert_msg ( lua_isfunction( lua, -1 ), "init exists but not as function" );
    if ( lua_isfunction( lua, -1 ) )
    {
        lua_pushlightuserdata( lua, this );     // the only argument is this

        if ( lua_pcall( lua, 1, 0, 0 ) != 0 )
        {
            // Run Dem functions
            xassert_msg(
                false,
                "Error running function without table argument, %s: %s\n",
                s_kInitName,
                lua_tostring(lua, -1)
            );
        }
    }
    else
    {
        lua_pop( lua, 1 );  // pop value off stack if function wasn't found
    }

	// lua::stackdump( lua );

    lua_pop( lua, 1);
}

///////////////////////////////////////////////////////////////////////////////

LuaGameObject::LuaGameObject( HashString const & obj_name, GameObjectInfo const & info )
:   GameObject( info ),
	lua( cRHOgine::GetInstance().getLua() )
{
    ErrorContext ec( "LuaGameObject::Ctor", obj_name.getString().c_str() );

	// Create Table fro self in the registry
	lua_pushlightuserdata( lua, (void*)this );			// stack: this
	lua_newtable( lua );						// stack: this (table)
	lua_rawset( lua, LUA_REGISTRYINDEX );		// registry[ this ] = {}
	// stack: empty

	// Make sure this bitch works
	lua_pushlightuserdata( lua, (void*)this );			// stack: this
	lua_gettable( lua, LUA_REGISTRYINDEX);			// stack: registry[ this ]
	assert_msg( lua_istable( lua, -1 ), "Lua Failed to load GameData table in Registry" );

	lua_pop( lua, 1 );

    obj_table_name = obj_name.getString().c_str();  // Get the string

    // Find the Global Table with the name of the object given
    lua_getglobal( lua, obj_table_name );   // Push

    // Make sure the value is a table
    assert_msg ( lua_istable( lua, -1 ), "Table Object not found" );

    // If init exists, call function
    lua_getfield ( lua, -1, s_kInitName );

    // assert_msg ( lua_isfunction( lua, -1 ), "init exists but not as function" );
    if ( lua_isfunction( lua, -1 ) )
    {
        lua_pushlightuserdata( lua, this );     // the only argument is this

		// Check to see if we were given a table argument, if we so, we need to do some shit
		if ( info.tableArgIndex > 0 && lua_istable( lua, info.tableArgIndex ) )
		{
			// std::cout << "WE HAVE A TABLE AS A CONSTRUCTOR ARGUMENT!\n";
			// Copy the table at that index and do a pcall with 2 arguments instead of 1
			lua_pushnil( lua );
			lua_copy( lua, info.tableArgIndex, lua_gettop( lua ) );
            if ( lua_pcall( lua, 2, 0, 0 ) != 0 )
            {
                xassert_msg(
                    false,
                    "Error running function with table argument, %s: %s\n",
                    s_kInitName,
                    lua_tostring(lua, -1)
                );
            }
		}
		else
		{
            if ( lua_pcall( lua, 1, 0, 0 ) != 0 )
            {
                // Run Dem functions
                xassert_msg(
                    false,
                    "Error running function without table argument, %s: %s\n",
                    s_kInitName,
                    lua_tostring(lua, -1)
                );
            }
		}
    }
    else
    {
        lua_pop( lua, 1 );  // pop value off stack if function wasn't found
    }

	// lua::stackdump( lua );

    lua_pop( lua, 1);
}

///////////////////////////////////////////////////////////////////////////////

LuaGameObject::~LuaGameObject()
{
    ErrorContext ec( "LuaGameObject::Dtor", obj_table_name );

    // Find the Global Table with the name of the object given
    lua_getglobal( lua, obj_table_name );   // Push

    // Make sure the value is a table
    assert_msg ( lua_istable( lua, -1 ), "Table Object not found" );

    // If init exists, call function
    lua_getfield ( lua, -1, s_kShutdown );

    // assert_msg ( lua_isfunction( lua, -1 ), "init exists but not as function" );
    if ( lua_isfunction( lua, -1 ) )
    {
        lua_pushlightuserdata( lua, this );     // the only argument is this

        if ( lua_pcall( lua, 1, 0, 0 ) != 0 )  // do the call
        {
            std::cerr << "error running function " << s_kShutdown << ":" << lua_tostring(lua, -1) << std::endl;
            // rho::lua::runtime_error( lua, std::cerr );
            assert_msg( 0, "shutdown call failed");
        }
    }
    else
    {
        lua_pop( lua, 1 );  // pop value off stack if function wasn't found
    }

	// lua::stackdump( lua );

    lua_pop( lua, 1);   // pop table off stack

    /// \todo Set GameObjectData table index[ this ] to nil so lua gc will clear its contents
	// Remove Table from registry
	lua_pushlightuserdata( lua, this );			// stack: this
	lua_pushnil( lua );							// stack: this (table)
	lua_rawset( lua, LUA_REGISTRYINDEX );		// registry[ this ] = nil
	lua::pop_all( lua );
}

///////////////////////////////////////////////////////////////////////////////

void LuaGameObject::update( sf::Time const & elpased_time )
{
    ErrorContext ec( "LuaGameObject::update", obj_table_name );

    // Find the Global Table with the name of the object given
    lua_getglobal( lua, obj_table_name );   // Stack: ObjectTable

    // Make sure the value is a table
    //assert_msg ( lua_istable( lua, -1 ), "Table Object not found" );
    if ( ! lua_istable( lua, -1 ) )
    {
        lua_pop( lua, 1 );
        return;
    }

    // If update exists, call function
    lua_getfield ( lua, -1, s_kUpdateName );    // Stack: ObjecTable InitFunction

    // assert_msg ( lua_isfunction( lua, -1 ), "update exists but not as function" );
    if ( ! lua_isfunction( lua, -1 ) )
    {
        lua_pop( lua, 2 );
        return;
    }

    lua_pushlightuserdata( lua, this );     //  Stack: ObjecTable InitFunction this

    lua_pushnumber( lua, elpased_time.asSeconds() );    //  Stack: ObjecTable UpdateFunction this dt

    if (lua_pcall(lua, 2, 0, 0) != 0)  // The top two fields are arguments ( this, dt ). Run Function UpdateFunction
	{
		std::cerr << "error running function " << s_kUpdateName << ":" << lua_tostring(lua, -1) << std::endl;
		// rho::lua::runtime_error( lua, std::cerr );
		assert_msg( 0, "update call failed");
	}

    // Stack: ObjectTable
    lua_pop( lua, 1 );      // Stack: (EMPTY)
}

///////////////////////////////////////////////////////////////////////////////

void LuaGameObject::draw( sf::RenderTarget &target, sf::RenderStates states )
{
    ErrorContext ec( "LuaGameObject::draw", obj_table_name );

    // Find the Global Table with the name of the object given
    lua_getglobal( lua, obj_table_name );   // Stack: ObjectTable

    // Make sure the value is a table
    //assert_msg ( lua_istable( lua, -1 ), "Table Object not found" );
    if ( ! lua_istable( lua, -1 ) )
    {
        lua_pop( lua, 1 );
        return;
    }

    // If update exists, call function
    lua_getfield ( lua, -1, s_kDrawName );    // Stack: ObjecTable InitFunction

    // assert_msg ( lua_isfunction( lua, -1 ), "update exists but not as function" );
    if ( ! lua_isfunction( lua, -1 ) )
    {
        lua_pop( lua, 2 );
        return;
    }

    lua_pushlightuserdata( lua, this );     //  Stack: ObjecTable DrawFunction this

    lua_pushlightuserdata( lua, &target );    //  Stack: ObjecTable DrawFunction this render_target

    if (lua_pcall(lua, 2, 0, 0) != 0)  // The top two fields are arguments ( this, render_target ). Run Function DrawFunction
	{
		std::cerr << "error running function " << s_kDrawName << ":" << lua_tostring(lua, -1) << std::endl;
		// rho::lua::runtime_error( lua, std::cerr );
		assert_msg( 0, "draw call failed");
	}

    // Stack: ObjectTable
    lua_pop( lua, 1 );      // Stack: (EMPTY)
}

///////////////////////////////////////////////////////////////////////////////

void LuaGameObject::HandleEvent( rho::Event const & event )
{
    ErrorContext ec( "LuaGameObject::HandleEvent", obj_table_name );

    // Find the Global Table with the name of the object given
    lua_getglobal( lua, obj_table_name );   // Stack: ObjectTable

    // Make sure the value is a table
    //assert_msg ( lua_istable( lua, -1 ), "Table Object not found" );
    if ( ! lua_istable( lua, -1 ) )
    {
        lua_pop( lua, 1 );
        return;
    }

    // If update exists, call function
    lua_getfield( lua, -1, s_kHandleEvent );    // Stack: ObjecTable InitFunction

    // assert_msg ( lua_isfunction( lua, -1 ), "update exists but not as function" );
    if ( ! lua_isfunction( lua, -1 ) )
    {
        lua_pop( lua, 2 );
        return;
    }

    lua_pushlightuserdata( lua, this );     //  Stack: ObjecTable InitFunction this

    lua::pushNewEvent( lua, event );

    if ( lua_pcall( lua, 2, 0, 0 ) != 0 )  // The top two fields are arguments ( this, dt ). Run Function UpdateFunction
	{
		std::cerr << "error running function " << s_kHandleEvent << ":" << lua_tostring( lua, -1 ) << std::endl;
		// rho::lua::runtime_error( lua, std::cerr );
		assert_msg( 0, "handle event call failed");
	}

    // Stack: ObjectTable
    lua_pop( lua, 1 );      // Stack: (EMPTY)
}


///////////////////////////////////////////////////////////////////////////////
//// Lua Registeration Section ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

using namespace lua;

static int l_setAlive( lua_State * lua )
{
	// Make sure we were given an actual GameObject
	GameObject * game_obj = NULL;

	if ( lua_islightuserdata( lua, 1 ) )
	{
		game_obj = static_cast<GameObject*>( lua_touserdata( lua, 1 ) );
	}
	else
	{
		// error
		return luaL_argerror( lua, 1, "GameObject not given" );
	}

	// Do a lookup in the GameObjectManager singleton

	// Grab boolean argument
	if ( ! lua_isboolean( lua, 2 ) )
    {
        return luaL_argerror( lua, 2, "Boolean not given" );
    }
	bool arg = lua_toboolean( lua, 2 );

	game_obj->setAlive( arg );

	lua::pop_all( lua );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getPositionX( lua_State * lua )
{
	// Make sure we were given an actual GameObject
	GameObject * game_obj = NULL;

	if ( lua_islightuserdata( lua, 1 ) )
	{
		game_obj = static_cast<GameObject*>( lua_touserdata( lua, 1 ) );

		lua_pop( lua, 1 );

		lua_pushnumber( lua, game_obj->getPositionX() );	// push the x position
	}
	else
	{
		// error
		return luaL_argerror( lua, 1, "GameObject not given" );
	}

	/// \todo Do a lookup in the GameObjectManager singleton

	// push the result
	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getPositionY( lua_State * lua )
{
	// Make sure we were given an actual GameObject
	GameObject * game_obj = NULL;

	if ( lua_islightuserdata( lua, 1 ) )
	{
		game_obj = static_cast<GameObject*>( lua_touserdata( lua, 1 ) );

		lua_pop( lua, 1 );

		lua_pushnumber( lua, game_obj->getPositionY() );	// push the x position
	}
	else
	{
		// error
		return luaL_argerror( lua, 1, "GameObject not given" );
	}

	/// \todo Do a lookup in the GameObjectManager singleton

	// push the result
	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getPosition( lua_State * lua )
{
	// Make sure we were given an actual GameObject
	GameObject * game_obj = NULL;

	if ( lua_islightuserdata( lua, 1 ) )
	{
		game_obj = static_cast<GameObject*>( lua_touserdata( lua, 1 ) );
		lua_pop( lua, 1 );

		lua_pushnumber( lua, game_obj->getPositionX() );	// push the X position
		lua_pushnumber( lua, game_obj->getPositionY() );	// push the Y position
	}
	else
	{
		// error
		return luaL_argerror( lua, 1, "GameObject not given" );
	}

	/// \todo Do a lookup in the GameObjectManager singleton

	// push the result
	return 2;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setPosition( lua_State * lua )
{
	// Make sure we were given an actual GameObject
	GameObject * game_obj = NULL;

	if ( lua_islightuserdata( lua, 1 ) )
	{
		game_obj = static_cast<GameObject*>( lua_touserdata( lua, 1 ) );

		// Grab the two other arguments
		int x = luaL_checkint( lua, 2 );	// grab second argument
		int y = luaL_checkint( lua, 3 );	// grab third argument

		lua_pop( lua, 3 );	// pop all the arguments

		// perform action
		game_obj->setPosition( x, y );
	}
	else
	{
		// error
		return luaL_argerror( lua, 1, "GameObject not given" );
	}

	/// \todo Do a lookup in the GameObjectManager singleton

	lua::pop_all( lua );

	return 0;	// return nothing
}

///////////////////////////////////////////////////////////////////////////////

static int l_addEventToRegister( lua_State * lua )
{
    // First Argument: lightuserdata: GameObject Pointer
    GameObject * game_obj = NULL;

	if ( lua_islightuserdata( lua, 1 ) )
	{
		game_obj = static_cast<GameObject*>( lua_touserdata( lua, 1 ) );
	}
	else
	{
		// error
		return luaL_argerror( lua, 1, "GameObject not given" );
	}

    // Second Argument: String for Event Name to register
    HashString event_name = chechHashString( lua, 2 );

	// pop args off
	lua_pop( lua, 2 );

    // Register to event
    game_obj->addEventToRegister( event_name );

    // Returned: Nothing
    return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_setDepth( lua_State * lua )
{
	// Make sure we were given an actual GameObject
	GameObject * game_obj = NULL;

	if ( lua_islightuserdata( lua, 1 ) )
	{
		game_obj = static_cast<GameObject*>( lua_touserdata( lua, 1 ) );
	}
	else
	{
		// error
		return luaL_argerror( lua, 1, "GameObject not given" );
	}

	// Do a lookup in the GameObjectManager singleton

	// Grab depth argument
	int arg = luaL_checkinteger( lua, 2 );

	game_obj->setDepth( arg );

	lua::pop_all( lua );

	return 0;
}

/// \todo GameObject.getData( object ) to return table reference for a data table, specific to that object

static int l_getData( lua_State * lua )
{
	// Make sure we were given an actual GameObject
	GameObject * game_obj = NULL;

	if ( lua_islightuserdata( lua, 1 ) )
	{
		game_obj = static_cast<GameObject*>( lua_touserdata( lua, 1 ) );
	}
	else
	{
		// error
		return luaL_argerror( lua, 1, "GameObject not given" );
	}

	lua::pop_all( lua );  // pop the gameobject off the stack there bub

	// get the table at index == this
	lua_pushlightuserdata( lua, (void*)game_obj );			// stack: gameobject
	lua_rawget( lua, LUA_REGISTRYINDEX);			// stack: registry[ gameobject ]

	assert_msg( lua_istable( lua, -1 ), "WTF Get Data returned not a table!" );

	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_getScene( lua_State * lua )
{
    GameObject * game_obj = NULL;
    if ( lua_islightuserdata( lua, 1 ) )
	{
		game_obj = static_cast<GameObject*>( lua_touserdata( lua, 1 ) );
	}
	else
	{
		// error
		return luaL_argerror( lua, 1, "GameObject not given" );
	}

	lua::pop_all(lua);

    if ( game_obj->getScene() == NULL )
    {
        lua_pushnil( lua );
    }
    else
    {
        lua_pushlightuserdata( lua, game_obj->getScene() );
    }


    return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_handleEvent( lua_State * lua )
{
	GameObject * p_game_object = CHECK_GAME_OBJ( lua, 1 );
	Event * p_event = lua::toEvent( lua, 2 );

	p_game_object->HandleEvent( *p_event );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static luaL_Reg const gameObject_functions [] =
{
	{ "setAlive", 			l_setAlive },
	{ "getPositionX", 		l_getPositionX },
	{ "getPositionY", 		l_getPositionY },
	{ "getPosition", 		l_getPosition },
	{ "setPosition", 		l_setPosition },
	{ "addEventToRegister", l_addEventToRegister },
	{ "setDepth",           l_setDepth },
	{ "getData",			l_getData },
	{ "getScene",           l_getScene },
	{ "handleEvent",		l_handleEvent },
	{ NULL, 		        NULL }		// Null termination
};

///////////////////////////////////////////////////////////////////////////////

void LuaGameObject::registerLuaGameObject( lua_State * lua )
{
	lua::register_funcs( lua, "GameObject", gameObject_functions );

	// Make Global Table named GameObjectData
	lua_newtable( lua );	// Stack: (table)
	lua_setglobal( lua, s_kGameObjectData );	// Stack: empty

	// now, _G[ "GameObjectData" ] should be equal to an empty table
	// Get Reference Key to it and store as Static varriable
	lua_getglobal( lua, s_kGameObjectData );	// stack: (table)

	s_iGameObjectDataRegistryKey = luaL_ref( lua, LUA_REGISTRYINDEX );	// Store our table under a special index in the registry table, for fast lookups
	// stack: empty

	// Test, assert, thingy
	assert_msg( s_iGameObjectDataRegistryKey != LUA_REFNIL, "Registry Adding Failed\n" );

	lua_rawgeti( lua, LUA_REGISTRYINDEX, s_iGameObjectDataRegistryKey );	// stack: (table)

	assert_msg( lua_istable( lua, -1 ), "Registry failed to pull up our table we made :(" );

	lua_pop( lua, 1 );	// stack: empty
}
