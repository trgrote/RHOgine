#ifndef LUA_GAMEOBJECT_FACTORY_H
#define LUA_GAMEOBJECT_FACTORY_H

#include "IGameObjectFactory.h"

namespace rho
{

struct LuaGameObjectInfo
{
    /// Name of the GameObject
    HashString name;
    /// What scripts need to be loaded for this GameObject to work
    std::list< HashString > scripts;
};

bool loadObjectInfoFromJSON( std::string json_file, rho::LuaGameObjectInfo & obj_info );

/// \brief Generic Factory for loading Lua GameObjects
class LuaGameObjectFactory : public IGameObjectFactory
{
private:
	HashString const m_type;

    std::list< HashString > m_scripts;
	
public:
	LuaGameObjectFactory( HashString const & type );
	LuaGameObjectFactory( LuaGameObjectInfo const & type_info );
	virtual ~LuaGameObjectFactory(){}

	/// \brief Returns the type of Object this Factory creates
	virtual HashString const getType() const;

	/// \brief Makes a new GameObject of the type that this Factory is responsible for
	virtual GameObject * createInstance() const;
	virtual GameObject * createInstance( GameObjectInfo const & ) const;
};

}

#endif
