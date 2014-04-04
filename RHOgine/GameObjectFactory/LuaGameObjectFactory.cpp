#include "LuaGameObjectFactory.h"

#include "../GameObjects/LuaGameObject.h"

#include <json/json.h>

#include <fstream>

using namespace rho;

///////////////////////////////////////////////////////////////////////////////

LuaGameObjectFactory::LuaGameObjectFactory( HashString const & type )
:	m_type( type )
{
}

///////////////////////////////////////////////////////////////////////////////

LuaGameObjectFactory::LuaGameObjectFactory( LuaGameObjectInfo const & type_info )
:   m_type( type_info.name ),
    m_scripts( type_info.scripts )
{
    lua_State * lua = LuaState::GetInstance();

    // Foreach Script, Lua Do file
    for ( HashString const & script : m_scripts )
    {
        lua::dofile( lua, script.getString().c_str() );
    }
}

///////////////////////////////////////////////////////////////////////////////

HashString const LuaGameObjectFactory::getType() const
{
	return m_type;
}

///////////////////////////////////////////////////////////////////////////////

GameObject * LuaGameObjectFactory::createInstance() const
{
	return new LuaGameObject( m_type );
}

///////////////////////////////////////////////////////////////////////////////

GameObject * LuaGameObjectFactory::createInstance(GameObjectInfo const & info) const
{
	return new LuaGameObject( m_type, info );
}

///////////////////////////////////////////////////////////////////////////////

bool rho::loadObjectInfoFromJSON( std::string json_file, rho::LuaGameObjectInfo & obj_info )
{
    Json::Value root;
	Json::Reader reader;
	std::fstream file_in( json_file, std::fstream::in );

	if ( !reader.parse( file_in, root ) )
	{
		std::cerr << "Failed to to load " << json_file << std::endl;
		std::cerr << reader.getFormatedErrorMessages() << std::endl;
		return false;
	}

    if ( root["name"].isString() )
    {
        obj_info.name = HashString( root["name"].asString() );
    }
    else
    {
        std::cerr << "Could not find name\n";
        return false;
    }

    // try to find the scripts section
    if ( root["scripts"].isArray() )
    {
        // Foreach entry
        Json::Value scripts = root[ "scripts" ];

        if ( scripts < 1 )
		{
			std::cerr << "No Scripts Given for Map\n";
			return false;
		}

		for ( Json::Value::ArrayIndex i = 0 ;
			  i < scripts.size() ;
			  ++i )
		{
            if ( scripts[ i ].isString() )	// Grab the current script
            {
                obj_info.scripts.push_back( HashString( scripts[i].asString() ) );
            }
            else
            {
                std::cerr << "Not Given String for Script Name, WTF am I supposed to do with that?!\n";
                return false;
            }
		}
    }
    else
    {
        std::cerr << "Could not find scripts\n";
        return false;
    }

	return true;
}

///////////////////////////////////////////////////////////////////////////////
