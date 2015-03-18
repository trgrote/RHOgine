#include "GOFactoryManager.h"

#include "LuaGameObjectFactory.h"

#include <json/json.h>
#include <fstream>
#include <cstring>

using namespace rho;

FIRST_COME_SINGLETON_INSTANCE( GOFactoryManager );

///////////////////////////////////////////////////////////////////////////////

GameObjectInfo * GOFactoryManager::newGameObjInfo()
{
	GameObjectInfo * rval = (GameObjectInfo*) malloc( sizeof( GameObjectInfo ) );
	
	memset( rval, 0, sizeof( GameObjectInfo ) );	// Everything is zeros

	rval->visible = true;
	rval->enabled = true;
	
	return rval;
}

///////////////////////////////////////////////////////////////////////////////

void GOFactoryManager::deleteGameObjInfo( GameObjectInfo * info )
{
	free( info );
}

///////////////////////////////////////////////////////////////////////////////

GameObject * GOFactoryManager::createInstance( HashString const & type ) const
{
	static auto iter = m_factoryMap.cbegin();
	iter = m_factoryMap.find( type );

	assert_msg ( iter != m_factoryMap.cend(), "Couldn't Find factory of given type" );

	return (*iter).second->createInstance();
}

///////////////////////////////////////////////////////////////////////////////

GameObject * GOFactoryManager::createInstance( HashString const & type, GameObjectInfo const & info ) const
{
	static auto iter = m_factoryMap.cbegin();
	iter = m_factoryMap.find( type );

	assert_msg ( iter != m_factoryMap.cend(), "Couldn't Find factory of given type" );

	return (*iter).second->createInstance(info);
}

///////////////////////////////////////////////////////////////////////////////

void GOFactoryManager::addFactory( IGameObjectFactory * p_factory )
{
	m_factoryMap[ p_factory->getType() ] = pIFactory( p_factory );	// Add Factory at Type Key
}

///////////////////////////////////////////////////////////////////////////////

bool GOFactoryManager::loadObjectFactoriesFromJSON( char const * const json_file_name )
{
	std::list< std::string > object_files;

    Json::Value root;
	Json::Reader reader;
	std::fstream file_in( json_file_name, std::fstream::in );

	if ( !reader.parse( file_in, root ) )
	{
		std::cerr << "Failed to to load " << json_file_name << std::endl;
		std::cerr << reader.getFormatedErrorMessages() << std::endl;
		return false;
	}

    if ( root["objects"].isArray() )
    {
        Json::Value object_file_list = root["objects"];

        for ( Json::Value::ArrayIndex i = 0 ;
			  i < object_file_list.size() ;
			  ++i )
		{
            if ( object_file_list[ i ].isString() )	// Grab the current script
            {
                object_files.push_back( object_file_list[i].asString() );
            }
            else
            {
                std::cerr << "Not Given String for Object JSON File Name, WTF am I supposed to do with that?!\n";
                return false;
            }
		}
    }
    else
    {
        std::cerr << "No Object Array Given" << std::endl;
		return false;
    }

    for ( auto const & json_file : object_files )
    {
        LuaGameObjectInfo obj_info;
        if ( loadObjectInfoFromJSON( json_file, obj_info ) )
        {
            addFactory( new LuaGameObjectFactory(obj_info) );
        }
        else
        {
            std::cerr << "Failed to Load " << json_file << std::endl;
			return false;
        }
    }

	return true;
}

///////////////////////////////////////////////////////////////////////////////

bool GOFactoryManager::loadLuaObjectFactory( HashString const & object_name, HashString const & script_name )
{
	LuaGameObjectInfo info;
	info.name = object_name;
	info.scripts.push_back( script_name );
	
	addFactory( new LuaGameObjectFactory( info ) );
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////
