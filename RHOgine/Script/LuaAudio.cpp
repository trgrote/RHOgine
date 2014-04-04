#include "LuaAudio.h"

#include "../RHOgine.h"
#include "../Scripting.h"

using namespace rho;

///////////////////////////////////////////////////////////////////////////////

static int l_loadMusic( lua_State * lua )
{
    // First Argument: String for Music File to Load
    HashString music_file = luaL_checkstring( lua, 1 );

	AudioLib::GetInstance()->load_music(music_file);

	lua::pop_all( lua );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_playMusic( lua_State * lua )
{
	AudioLib::GetInstance()->play_music();

	lua::pop_all( lua );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_stopMusic( lua_State * lua )
{
	AudioLib::GetInstance()->stop_music();

	lua::pop_all( lua );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_pauseMusic( lua_State * lua )
{
	AudioLib::GetInstance()->pause_music();

	lua::pop_all( lua );

	return 0;
}

static luaL_Reg const music_functions [] =
{
    { "loadMusic",          l_loadMusic },
    { "playMusic",          l_playMusic },
    { "stopMusic",          l_stopMusic },
    { "pauseMusic",         l_pauseMusic },
    { NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////////

static int l_clearSoundCache( lua_State * lua )
{
	AudioLib::GetInstance()->clear_cache();

	lua::pop_all( lua );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_addSoundCache( lua_State * lua )
{
    HashString sound_filename = luaL_checkstring( lua, 1 );
    HashString sound_alias = luaL_checkstring( lua, 2 );
	AudioLib::GetInstance()->add_to_cache(sound_filename,sound_alias);

	lua::pop_all( lua );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_playSound( lua_State * lua )
{
    // Make sure we were given an actual GameObject

    HashString sound_alias = luaL_checkstring( lua, 1 );
    AudioLib::GetInstance()->playSound(sound_alias);

	lua::pop_all( lua );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_stopAllSound( lua_State * lua )
{
    AudioLib::GetInstance()->stopAll();

	lua::pop_all( lua );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_pauseAllSound( lua_State * lua )
{
    AudioLib::GetInstance()->pauseAll();

	lua::pop_all( lua );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_playAllSound( lua_State * lua )
{
    AudioLib::GetInstance()->playAll();

	lua::pop_all( lua );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_soundUpdate( lua_State * lua )
{
	AudioLib::GetInstance()->update();

	lua::pop_all( lua );

	return 0;
}

static luaL_Reg const audio_functions [] =
{
	{ "clearSoundCache",    l_clearSoundCache },
	{ "addSoundCache",      l_addSoundCache },
	{ "playSound",          l_playSound },
	{ "stopAllSound",       l_stopAllSound },
	{ "pauseAllSound",      l_pauseAllSound },
	{ "playAllSound",       l_playAllSound },
	{ "update",             l_soundUpdate },
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////////

static int l_createSoundObject( lua_State * lua )
{
    HashString sound_alias = luaL_checkstring( lua, 1 );

	lua::pop_all( lua );

    lua_pushlightuserdata ( lua, AudioLib::GetInstance()->createSound(sound_alias));

	return 1;
}

static int l_deleteSoundObject( lua_State * lua )
{
    sf::Sound * sound_obj = NULL;

    if ( lua_islightuserdata( lua, 1 ) )
	{
		// game_obj = static_cast<GameObject*>( lua_touserdata( lua, 1 ) );
		sound_obj = static_cast<sf::Sound*> (lua_touserdata( lua, 1 ));
	}
	else
	{
		// error
		return luaL_argerror( lua, 1, "Sound Object not given" );
	}

    sound_obj->stop();

	AudioLib::GetInstance()->deleteSound(sound_obj);

	lua::pop_all( lua );

	return 0;
}

static int l_playSoundObject (lua_State * lua )
{
    sf::Sound * sound_obj = NULL;

    if ( lua_islightuserdata( lua, 1 ) )
	{
		// game_obj = static_cast<GameObject*>( lua_touserdata( lua, 1 ) );
		sound_obj = static_cast<sf::Sound*> (lua_touserdata( lua, 1 ));
	}
	else
	{
		// error
		return luaL_argerror( lua, 1, "Sound Object not given" );
	}

	sound_obj->play();

	lua::pop_all( lua );

	return 0;
}

static int l_setPitch (lua_State * lua)
{
    sf::Sound * sound_obj = NULL;

    if ( lua_islightuserdata( lua, 1 ) )
	{
		// game_obj = static_cast<GameObject*>( lua_touserdata( lua, 1 ) );
		sound_obj = static_cast<sf::Sound*> (lua_touserdata( lua, 1 ));
	}
	else
	{
		// error
		return luaL_argerror( lua, 1, "Sound Object not given" );
	}

    float tmpValue = lua_tonumber( lua, 2 );

	sound_obj->setPitch(tmpValue);

	lua::pop_all( lua );

	return 0;
}

static int l_setSoundPosition (lua_State * lua)
{
    sf::Sound * sound_obj = NULL;

    if ( lua_islightuserdata( lua, 1 ) )
	{
		// game_obj = static_cast<GameObject*>( lua_touserdata( lua, 1 ) );
		sound_obj = static_cast<sf::Sound*> (lua_touserdata( lua, 1 ));
	}
	else
	{
		// error
		return luaL_argerror( lua, 1, "Sound Object not given" );
	}

    float x = lua_tonumber( lua, 2 );
    float y = lua_tonumber( lua, 3 );
    float z = lua_tonumber( lua, 4 );

	sound_obj->setPosition(x,y,z);

	lua::pop_all( lua );

	return 0;
}

static int l_setSoundVolume (lua_State * lua)
{
    sf::Sound * sound_obj = NULL;

    if ( lua_islightuserdata( lua, 1 ) )
	{
		// game_obj = static_cast<GameObject*>( lua_touserdata( lua, 1 ) );
		sound_obj = static_cast<sf::Sound*> (lua_touserdata( lua, 1 ));
	}
	else
	{
		// error
		return luaL_argerror( lua, 1, "Sound Object not given" );
	}

    float volume = lua_tonumber( lua, 2 );

	sound_obj->setVolume(volume);

	lua::pop_all( lua );

	return 0;
}

static int l_pauseSoundObject (lua_State * lua)
{
    sf::Sound * sound_obj = NULL;

    if ( lua_islightuserdata( lua, 1 ) )
	{
		// game_obj = static_cast<GameObject*>( lua_touserdata( lua, 1 ) );
		sound_obj = static_cast<sf::Sound*> (lua_touserdata( lua, 1 ));
	}
	else
	{
		// error
		return luaL_argerror( lua, 1, "Sound Object not given" );
	}

	sound_obj->pause();

	lua::pop_all( lua );

	return 0;
}

static int l_stopSoundObject (lua_State * lua)
{
    sf::Sound * sound_obj = NULL;

    if ( lua_islightuserdata( lua, 1 ) )
	{
		// game_obj = static_cast<GameObject*>( lua_touserdata( lua, 1 ) );
		sound_obj = static_cast<sf::Sound*> (lua_touserdata( lua, 1 ));
	}
	else
	{
		// error
		return luaL_argerror( lua, 1, "Sound Object not given" );
	}

	sound_obj->stop();

	lua::pop_all( lua );

	return 0;
}

static int l_setBufferSoundObject (lua_State * lua)
{
    sf::Sound * sound_obj = NULL;

    if ( lua_islightuserdata( lua, 1 ) )
	{
		// game_obj = static_cast<GameObject*>( lua_touserdata( lua, 1 ) );
		sound_obj = static_cast<sf::Sound*> (lua_touserdata( lua, 1 ));
	}
	else
	{
		// error
		return luaL_argerror( lua, 1, "Sound Object not given" );
	}

    HashString sound_alias = luaL_checkstring( lua, 2 );

    AudioLib::GetInstance()->setBuffer(sound_obj, sound_alias);

	lua::pop_all( lua );

	return 0;
}

static luaL_Reg const sound_functions [] =
{
    { "createSound",        l_createSoundObject },
    { "deleteSound",        l_deleteSoundObject },
    { "playSound",          l_playSoundObject },
    { "pauseSound",         l_pauseSoundObject },
    { "stopSound",          l_stopSoundObject },
    { "setPitch",           l_setPitch },
    { "setPosition",        l_setSoundPosition },
    { "setVolume",          l_setSoundVolume },
    { "setBuffer",          l_setBufferSoundObject },
    { NULL, NULL }
};

int lua::registerLuaAudio( lua_State * lua )
{
    lua::register_funcs( lua, "Music", music_functions );
	lua::register_funcs( lua, "Audio", audio_functions );
	lua::register_funcs( lua, "Sound", sound_functions );

	return 1;
}
