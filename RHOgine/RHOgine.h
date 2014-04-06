#ifndef RHOGINE_H_INCLUDED
#define RHOGINE_H_INCLUDED

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

#include "defines.h"

#include "EventManager/EventManager.h"

#include "GameObjects.h"

#include "Input.h"

#include "Scripting.h"

#include "Scene/Scene.h"

#include "Graphics.h"

#include "Console/SFMLDrawableConsole.h"
#include "Console/MiscCommands.h"

#include "Audio/Audio.h"

#include "FirstComeSingleton.h"

// Unique pointers
namespace rho
{

/**
  *	\brief Configuration Setting for the Game,
  *	Currently, only used during cRHOgine::Inititalize() method
  *	to set up window.
  *	\todo Make setConfig() method to allow rebuilding of window and
  *	settings on the fly.
  */
struct cRHOgineConfig
{
	/// Enabled VSYNC
	/// Default is disabled
	bool vsyncEnabled;

	/// Brief Enable Full screen
	/// Default is off
	bool fullScreen;

	/// Default is 800
	unsigned int windowWidth;
	/// Default is 600
	unsigned int windowHeight;

	/// Title of the Window
	std::string windowTitle;

	/// Default is 60
	unsigned int frameLimit;
};

class cRHOgine : public FirstComeSingleton<cRHOgine>
{
private:
	//////////////////////////////////////////////////////////////////////////
	//	Private Trilogy of Evil, Ctor and Dtor
	//////////////////////////////////////////////////////////////////////////
	cRHOgine(const cRHOgine&);
	cRHOgine& operator=(const cRHOgine&);

protected:
	/// SFML Render Window
	sf::RenderWindow m_window;

    EventManager m_eventManager;            // Event Manager
    SFMLInput m_input;                      // Input System
    LuaState m_pLua;                        // Lua State
    std::unique_ptr< SFMLDrawableConsole > m_pConsole;
    TextureManager m_textureManager;
    FontManager m_fontManager;
    SpineManager m_spineManager;

    /// Game Object Factory manager
    GOFactoryManager m_GOFactoryManager;

	/// Holds Elapsed Time since last update
	sf::Time m_elapsedTime;

	/// Config Settings, used during Initialize.
	/// can be modified during ctor to change window settings
	cRHOgineConfig m_windowConfig;
	
	// Average Framerate Counting
	// Find average elapsed time based off previous 10 frame increment
	float m_cycleTotal;		// Sum of all elapsed times in the current framerate calculating cycle
	int m_framesInCycle;
	float m_frameRate;		// Average Framerate
	bool m_bFrameRateDisplayON;
	sf::Text m_fpsText;
	sf::Font m_fpsFont;
	// static float const s_kFramesPerCycle;
	//

	// Protec Default cstor for inherited games
	cRHOgine();

	/**
	  *	\brief Initialize all external modules necessary for the game.
	  *
	  *	\author		Tarrence De Bruin
	  *	\date		June 8th, 2013
	  */
	virtual int Initialize();

	/**
	  *	Function:			Shutdown
	  *	Description:		Shutdown any external modules that have been initialized.
	  *
	  *	Author:				Tarrence De Bruin
	  *	Last Modified by:	Tarrence De Bruin
	  *	Date Modified:		June 8th, 2013
	  */
	virtual void Shutdown();

	/**
	  *	\brief Load up the Lua virtual machine with all the libraries this game needs.
	  *	\note Inherited Games, if they want to add more m_pLua libraries, should call this
	  *		base method before adding the new m_pLua libraries.
	  *	\author Taylor Grote
	  */
	virtual void LoadLua();

	/**
	  *	\brief Do the updates on all modules needing it.
	  *
	  *	\author Tarrence De Bruin
	  *	\date June 8th, 2013
	  */
	virtual void update(sf::Time const & elapsed_time);

	/**
	  *	\brief Clears buffer to allow for drawing to new frame
	  *
	  *	\author		Taylor Grote
	  */
	virtual void beginDraw();

	/**
	  *	\brief Draws the Game
	  *
	  *	\author		Tarrence De Bruin
	  *	\date		June 8th, 2013
	  */
	virtual void draw();

	/**
	  *	\brief Draw Console and Swap Buffers
	  *
	  *	\author		Taylor Grote
	  */
	virtual void endDraw();

public:

	/// People should be allowed to destroy this monster
	virtual ~cRHOgine();

	//////////////////////////////////////////////////////////////////////////
	//	Public Functions
	//////////////////////////////////////////////////////////////////////////

    inline sf::RenderWindow & getRenderWindow() { return m_window; }

    /// Returns the Scripting State
    inline lua_State * getLua() { return m_pLua; }

	/// Return Texture Manager
    inline TextureManager & getTextureManager() { return m_textureManager; }

	/// Return the GameObject Factory Manager
	inline GOFactoryManager & getFactoryManager() { return m_GOFactoryManager; }

    inline SpineManager & getSpine() { return m_spineManager; }

	/**
	  *	\brief 		Ends the Current Game.
	  *	\note (SPOILERS) It just tells the window the close.
	  */
    virtual void EndGame();

	/**
	  *	\brief 		Draw a scene with the given camera
	  */
	virtual void drawScene( Camera const & camera, Scene & scene );

	/**
	  *	\brief 		The Main Game Loop.
	  *
	  *	\author 	Tarrence De Bruin
	  *	\date		June 8th, 2013
	  */
	virtual void run();

	/**
	  *	Return the Elapsed Time since the last frame
	  *	\author Taylor Grote
	  */
	inline sf::Time const & getElapsedTime() const { return m_elapsedTime; }
	
	inline float const & getAverageFramerate() const { return m_frameRate; }
	inline bool const & toggleFPSDisplay() { return m_bFrameRateDisplayON = !m_bFrameRateDisplayON; }
};

} 	// namepace rho

#endif // RHOGINE_H_INCLUDED
