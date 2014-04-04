#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <list>
#include <initializer_list>
#include <memory>

#include "../EventManager/BaseEventListener.h"
#include "../Logic/IUpdateable.h"
#include "../Graphics/ISFMLDrawable.h"
#include "../HashString.h"
#include "../GameObjects/ITransformable.h"
#include "../Script/lua_helpers.h"

namespace rho
{

class Scene;

typedef unsigned int GameObjectID;

/// Object Info
struct GameObjectInfo
{
	/// \brief Scene this object is in
	Scene * scene;
	/// \brief X Position
	int x;
	/// \brief Y Position
	int y;
	/// \brief Should the GameObject Start Enabled
	bool enabled;
	/// \brief Should the GameObject Start Visible
	bool visible;

	//////// LUA ARG INFO /////
	// IF a GameObject is created from a lua context
	// it may come with a lua table as an argument list.
	// In this case, we are given the lua state, and
	// the stack index where the table is located

	/// \brief Constructor Argument Table
	int tableArgIndex;
};

/**
  *	\brief GameObject/
  *	All Objects in the Game Should inherit this class. and Registered with the GameObjectManager.
  *	\sa rho::GameObjectManager.
  */
class GameObject    :		public rho::IUpdateable,
							public rho::ISFMLDrawable,
							public rho::ITransformable,
							public rho::BaseEventListener
							// public rho::ICollideable
{
protected:

	/// GameObject's Unique ID
	GameObjectID m_id;

	/// Bool Flag indicating if this Object is Alive or Dead
	bool m_isAlive;

	/// Flag Indicating this Object persists between scenes
	bool m_bPersistent;

    /// Will this object update?
	bool m_bEnabled;

	/// Will this object be drawn
    bool m_bVisible;

	/// At What depth is the object drawn?
    int m_iDepth;

    /// Rotation of the Object
    float m_fRotation;

	/// Boundary and Position of the Object
	sf::Rect<int> m_bounds;

    /// \brief Scene this GameObject Belongs to
    Scene * m_pScene;

public:

	GameObject();

	GameObject(GameObjectInfo const & info);

	/**
	  *	\brief Dstor
	  */
	virtual ~GameObject();

	/**
	  *	\brief Return the GameObject's ID
	  */
	GameObjectID getID() const;

	/**
	  *	\brief Set this Object's ID
	  *	\note Should only be used by GameObjectManager
	  */
	void setID(GameObjectID const & id);

	/**
	  *	Return true if this GameObject is alive.
	  */
	bool const & getAlive() const;

	/**
	  *	Set the alive status of this GameObject
	  */
	void setAlive(bool const & alive);

	/**
	  *	Return true if this GameObject is Persistent.
	  */
	bool const & isPersistent() const;

	/**
	  *	Set the Persistent status of this GameObject
	  */
	void setPersistent(bool const & persistent);

    inline Scene * getScene() { return m_pScene; }
    inline void setScene( Scene * p_scene ) { m_pScene = p_scene; }

	//// ITransformable
	virtual void setPosition( int const & x, int const & y );
	virtual int getPositionX() const;
	virtual int getPositionY() const;
    virtual int getCenterX() const;
	virtual int getCenterY() const;
	virtual int getWidth() const;
	virtual int getHeight() const;
	virtual float getRotation() const;

	//// IUpdateable
	virtual bool const enabled() const;
    virtual void update( sf::Time const & elpased_time );

	//// ISFMLDrawable
	virtual bool const visible() const;
	virtual int const getDepth() const;
	virtual void setDepth(int const & depth);
	virtual void draw( sf::RenderTarget &target, sf::RenderStates states );

	//// BaseEventListener
	virtual void HandleEvent( rho::Event const & event );

// Static Members //
    static GameObjectID s_iNextID;
// End of Static //

};

}

#endif
