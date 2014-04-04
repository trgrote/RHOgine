#include "lua_helpers.h"

#include <cstring>
#include <stdarg.h>

// Compile Error
bool rho::lua::compile_error(lua_State * L, int status, std::ostream & stream )
{
    if(status == 0)
        return true; // No errors.

    switch (status)
    {
    case LUA_ERRMEM:
        stream << "Memory allocation error: " << lua_tostring(L, -1) << std::endl;
        break;
    case LUA_ERRSYNTAX:
        stream << "Syntax error: " << lua_tostring(L, -1) << std::endl;
        break;
    }

    // Remove error message.
    lua_pop(L, 1);

    return false;
}

// Run Time Error
int rho::lua::runtime_error(lua_State * L, std::ostream & stream)
{
    lua_getglobal (L, "debug");
    if ( !lua_istable (L, -1) )
    {
        lua_pop (L, 1);
        return 1;
    }

	// std::cerr << "Debug is Defined\n";

    lua_getfield (L, -1, "traceback");
    if ( !lua_isfunction (L, -1) )
    {
        lua_pop (L, 2);
        return 1;
    }

	// std::cerr << "Traceback is Defined\n";

    lua_pushvalue (L, 1);  // pass error message
    lua_pushinteger (L, 2);  // skip this function and traceback

	// stackdump( L );

    lua_call (L, 2, 1);  // call debug.traceback

    // Extract message.
    if ( lua_isstring ( L , -1 ) )
    {
        stream << lua_tostring(L, -1) << std::endl;
    }
	// else
	// {
		// stream << "Ok, so this piece of shit had an error, but doesn't want to tell me about it\n";
	// }

	// stackdump( L );

    return 1;
}

bool rho::lua::dofile(lua_State * L, const char * file )
{
    bool rval;
    rval = compile_error( L, luaL_loadfile(L,file), std::cerr );
    if ( lua_pcall( L, 0, LUA_MULTRET, 0) != LUA_OK )
	{
		// print out the error that SHOULD be on the top of the stack
		std::cerr << lua_tostring( L, -1 ) << std::endl;

		lua_pop( L, 1 );	// pop off error

		rval = false;
	}
    // runtime_error(L,std::cerr);
    return rval;
}

void rho::lua::stackdump(lua_State* l)
{
    int i;
    int top = lua_gettop(l);

    printf("total in stack %d\n",top);

    for (i = 1; i <= top; i++)
    {  /* repeat for each level */
        int t = lua_type(l, i);
        printf( "Position ( %d, %d ) = ", i, i - top - 1);

        switch (t) {
            case LUA_TSTRING:  /* strings */
                printf("string: '%s'\n", lua_tostring(l, i));
                break;
            case LUA_TBOOLEAN:  /* booleans */
                printf("boolean %s\n",lua_toboolean(l, i) ? "true" : "false");
                break;
            case LUA_TNUMBER:  /* numbers */
                printf("number: %g\n", lua_tonumber(l, i));
                break;
            default:  /* other values */
                printf("%s\n", lua_typename(l, t));
                break;
        }
    }
    printf("\n");  /* end the listing */
}

void rho::lua::Register( lua_State * L, const char * name, const luaL_Reg * funcs )
{
	if ( name != NULL )
	{
		lua_newtable(L);
		lua_setglobal(L, name );
		lua_getglobal(L, name);
	}
	for ( int i = 0 ; funcs[i].name != NULL && funcs[i].func != NULL ; ++i )
	{
		lua_pushstring(L, funcs[i].name );
		lua_pushcfunction(L, funcs[i].func);
		lua_settable ( L, -3 );
	}

	if ( name != NULL )
		lua_pop(L,1);   // Pop Table off Stack
}

void rho::lua::register_funcs( lua_State * lua, const char * table_name, const luaL_Reg * funcs )
{
	// get the table,
	lua_getglobal( lua, table_name );

	// It it doesn't exist, make a new one
	if ( lua_isnil( lua, 1 ) || !lua_istable( lua, 1 ) )
	{
		lua_pop( lua, 1 );	// pop that shit off the stack
		lua_newtable( lua );
		lua_setglobal( lua, table_name );
		lua_getglobal( lua, table_name );
	}

	for ( int i = 0 ; funcs[i].name != NULL && funcs[i].func != NULL ; ++i )
	{
		lua_pushstring( lua, funcs[i].name );
		lua_pushcfunction( lua, funcs[i].func);
		lua_settable ( lua, -3 );
	}

	// Set the table as a global, if it wasn't already created as one
	lua_pop( lua, 1 );
}

// example call:
// call_va( "Player_OnUpdate", "pi", player_object_ptr, elapsed_time );
void rho::lua::call_va ( lua_State * L, const char *func, const char *sig, ...)
{
	va_list vl;
	int narg, nres;  /* number of arguments and results */

	va_start(vl, sig);
	lua_getglobal(L, func);  /* get function */

	/* push arguments */
	narg = 0;
	while (*sig) /* push arguments */
	{
		char arg = (*sig++);

		if ( arg == 'f' )
		{
			lua_pushnumber(L, va_arg(vl, double));
		}
		else if ( arg == 'i' )
		{
			lua_pushnumber(L, va_arg(vl, int));
		}
		else if ( arg == 's' )
		{
			lua_pushstring(L, va_arg(vl, char *));
		}
		else if ( arg == 'p' )
		{
			lua_pushlightuserdata( L, va_arg( vl, void * ) );
		}
		else if ( arg == '>' )
		{
			break;
		}
		else
		{
			std::cerr << "invalid option " << *(sig - 1) << std::endl;
			return;
		}

		narg++;
		luaL_checkstack(L, 1, "too many arguments");
	}

	/* do the call */
	nres = strlen(sig);  /* number of expected results */

	if (lua_pcall(L, narg, nres, 0) != 0)  /* do the call */
	{
		std::cerr << "error running function " << func << ":" << lua_tostring(L, -1) << std::endl;
	}

	/* retrieve results */
	nres = -nres;  /* stack index of first result */

	while (*sig)  /* get results */
	{
		char result = (*sig++) ;

		if ( result == 'f' )  /* double result */
		{
			if (!lua_isnumber(L, nres))
				std::cerr <<  "wrong result type" << std::endl;
			*va_arg(vl, float *) = lua_tonumber(L, nres);
		}
		else if ( result ==  'i' )  /* int result */
		{
			if (!lua_isnumber(L, nres))
				std::cerr <<  "wrong result type" << std::endl;
			*va_arg(vl, int *) = (int)lua_tonumber(L, nres);
		}
		else if ( result ==  's' )	/* string result */
		{
			if (!lua_isstring(L, nres))
				std::cerr <<  "wrong result type" << std::endl;
			*va_arg(vl, const char **) = lua_tostring(L, nres);
		}
		else
		{
			std::cerr << "invalid option " << *(sig - 1) << std::endl;
		}

		nres++;
	}
	va_end(vl);
}

bool rho::lua::global_func_exists( lua_State * lua, char const * func_name )
{
	bool rval = false;
	lua_getglobal( lua, func_name );

	if ( (!lua_isnil( lua, 1 ) ) && lua_isfunction( lua, 1 ) )
	{
		rval = true;
	}

	lua_pop( lua, 1 );

	return rval;
}

bool rho::lua::call_func_if_exists( lua_State * lua, const char *func, const char *sig, ... )
{
	if ( !global_func_exists( lua, func ) )
	{
		// std::cerr << "Function Doesn't Exist\n";
		return false;
	}

	va_list vl;
	int narg, nres;  /* number of arguments and results */

	va_start(vl, sig);
	lua_getglobal( lua, func);  /* get function */

	/* push arguments */
	narg = 0;
	while (*sig) /* push arguments */
	{
		char arg = (*sig++);

		if ( arg == 'f' )
		{
			lua_pushnumber(lua, va_arg(vl, double));
		}
		else if ( arg == 'i' )
		{
			lua_pushnumber(lua, va_arg(vl, int));
		}
		else if ( arg == 's' )
		{
			lua_pushstring(lua, va_arg(vl, char *));
		}
		else if ( arg == 'p' )
		{
			lua_pushlightuserdata( lua, va_arg( vl, void * ) );
		}
		else if ( arg == '>' )
		{
			break;
		}
		else
		{
			std::cerr << "invalid option " << *(sig - 1) << std::endl;
			return false;
		}

		narg++;
		luaL_checkstack(lua, 1, "too many arguments");
	}

	/* do the call */
	nres = strlen(sig);  /* number of expected results */

	if (lua_pcall(lua, narg, nres, 0) != 0)  /* do the call */
	{
		std::cerr << "error running function " << func << ":" << lua_tostring(lua, -1) << std::endl;
		// rho::lua::runtime_error( lua, std::cerr );
		return false;
	}

	/* retrieve results */
	nres = -nres;  /* stack index of first result */

	while (*sig)  /* get results */
	{
		char result = (*sig++) ;

		if ( result == 'f' )  /* double result */
		{
			if (!lua_isnumber(lua, nres))
				std::cerr <<  "wrong result type" << std::endl;
			*va_arg(vl, float *) = lua_tonumber(lua, nres);
		}
		else if ( result ==  'i' )  /* int result */
		{
			if (!lua_isnumber(lua, nres))
				std::cerr <<  "wrong result type" << std::endl;
			*va_arg(vl, int *) = (int)lua_tonumber(lua, nres);
		}
		else if ( result ==  's' )	/* string result */
		{
			if (!lua_isstring(lua, nres))
				std::cerr <<  "wrong result type" << std::endl;
			*va_arg(vl, const char **) = lua_tostring(lua, nres);
		}
		else
		{
			std::cerr << "invalid option " << *(sig - 1) << std::endl;
		}

		nres++;
	}
	va_end(vl);

	return true;
}

bool rho::lua::checkbool( lua_State * lua, int const & index )
{
	if ( ! lua_isboolean( lua, index ) )
	{
		return luaL_argerror( lua, index, "Boolean expected" );
	}
	
	return static_cast<bool>( lua_toboolean( lua, index ) );
}

void rho::lua::pop_all( lua_State * lua )
{
	lua_pop( lua, lua_gettop( lua ) );
}

void * rho::lua::checklightuserdata( lua_State * lua, int const & index )
{
	luaL_argcheck(
		lua,
		lua_islightuserdata( lua, index ),
		index,
		"Light Userdata expected" );

	return lua_touserdata( lua, index );
}
