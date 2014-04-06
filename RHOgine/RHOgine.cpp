#include "RHOgine.h"
#include <cassert>
#include <cstdio>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>

#include "defines.h"

// 10 Frames per cycle
#define FRAMES_PER_CYCLE 		20
#define FONT_NAME				"sansation.ttf"
#define FONT_SIZE  				20

using namespace rho;

FIRST_COME_SINGLETON_INSTANCE(cRHOgine);

///////////////////////////////////////////////////////////////////////////////

cRHOgine::cRHOgine()
{
    ErrorContext::init();

    ErrorContext ec( "cRHOgine::Constructor" );

	m_windowConfig.vsyncEnabled = false;
	m_windowConfig.fullScreen = false;
	m_windowConfig.windowWidth = 800;
	m_windowConfig.windowHeight = 600;
	m_windowConfig.windowTitle = "RHOgine Game";
	m_windowConfig.frameLimit = 120;

	m_cycleTotal = 0;
	m_framesInCycle = 0;
	m_frameRate = 0;
	m_bFrameRateDisplayON = false;
}

///////////////////////////////////////////////////////////////////////////////

cRHOgine::~cRHOgine()
{
	m_input.getCOR().delInputHandler(m_pConsole.get());

	ErrorContext::shutdown();
}

///////////////////////////////////////////////////////////////////////////////

int cRHOgine::Initialize()
{
    ErrorContext ec( "cRHOgine::Initilize" );

	// Create Window
	m_window.create(
		sf::VideoMode(m_windowConfig.windowWidth, m_windowConfig.windowHeight),
		m_windowConfig.windowTitle );

	m_window.setVerticalSyncEnabled( m_windowConfig.vsyncEnabled );	// vsync enabled ( The Tarrence Fix )
	m_window.setFramerateLimit( m_windowConfig.frameLimit );		// Set Frame Limit, default is 60

	m_pConsole = std::unique_ptr< SFMLDrawableConsole >( new SFMLDrawableConsole( m_window.getSize().x, m_window.getSize().y ) );

	// Load Console
	m_pConsole->addCommand( new commands::EchoCommand() );
	m_pConsole->addCommand( new commands::ExitCommand(m_window) );
	m_pConsole->addCommand( new commands::LuaDofileCommand(m_pLua) );
	m_pConsole->addCommand( new commands::ToggleFPSCommand() );

	m_input.getCOR().addInputHandler(m_pConsole.get(), true);     // Place Console on top of the chain of responsibility

	LoadLua();

	// Load Font
	m_fpsFont.loadFromFile( FONT_NAME );
	m_fpsText.setFont( m_fpsFont );
	m_fpsText.setCharacterSize( FONT_SIZE );
	m_fpsText.setColor( sf::Color::White );
	m_fpsText.setPosition( 5, 5 );

	return true;
}

///////////////////////////////////////////////////////////////////////////////

void cRHOgine::Shutdown()
{
}

///////////////////////////////////////////////////////////////////////////////

void cRHOgine::LoadLua()
{
    ErrorContext ec( "cRHOgine::LoadLua" );
	rho::lua::registerLuaGame(m_pLua);					// Register Global Game Functions
	rho::lua::registerLuaWindow( m_pLua );              // Register Window
	rho::lua::registerLuaHashString(m_pLua);			// Register HashStrings
	rho::lua::registerLuaEvent(m_pLua);                	// Register Event Data Type
	LuaGameObject::registerLuaGameObject(m_pLua);       // Register GameObject functions
	rho::lua::registerLuaAudio(m_pLua);                 // Register Audio functions
	rho::lua::registerLuaScene(m_pLua);
	rho::lua::registerLuaCamera( m_pLua );
	rho::lua::registerRenderTarget( m_pLua );
	rho::lua::registerSprite(m_pLua );
	rho::lua::registerLuaGOFactoryManager(m_pLua);
	rho::lua::registerDrawableRect( m_pLua );
	rho::lua::registerText( m_pLua );
	rho::lua::registerSpine( m_pLua );
}

///////////////////////////////////////////////////////////////////////////////

void cRHOgine::EndGame()
{
    m_window.close();
}

///////////////////////////////////////////////////////////////////////////////

void cRHOgine::run()
{
    ErrorContext ec( "cRHOgine::run" );
	// Call init Methods
	Initialize();

	// Call Load methods

	// Call First Run method or whatever

	// Game loop, ya doofus!
	sf::Clock clock;		// walk into the club, like what I got a big clock

	// Simulated Game Loop
	while ( m_window.isOpen() )
	{
		// Get elapsed time since tast restart
		m_elapsedTime = clock.restart();		// is it really that time, again?

		// Calculate Average Framerate
		m_cycleTotal += m_elapsedTime.asSeconds();
		++m_framesInCycle;

		if ( m_framesInCycle >= FRAMES_PER_CYCLE )
		{
			m_frameRate = 1 / ( m_cycleTotal / (float) m_framesInCycle );
			m_framesInCycle = 0;
			m_cycleTotal = 0;
			// std::cout << "Frame Rate: " << m_frameRate << std::endl;
		}

		update( m_elapsedTime );             	// Update Game and Scene

		beginDraw();                            // Clear the Buffer
		draw();                             	// Draw the Game
		endDraw();                             	// Swap Buffer
	}

	Shutdown();
}

///////////////////////////////////////////////////////////////////////////////

void cRHOgine::update( sf::Time const & elapsed_time )
{
    ErrorContext ec( "cRHOgine::update" );
	static sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_window.close();

		if ( SFMLInput::isEventInput( event ) )
		{
			m_input.HandleSFMLInputEvent(event);
		}
	}

	m_input.update( elapsed_time );

	m_eventManager.HandleQueuedEvents();   // Handle Events

    lua_gc( m_pLua, LUA_GCSTEP, 1 );
}

///////////////////////////////////////////////////////////////////////////////

void cRHOgine::beginDraw()
{
	m_window.clear();	// Bye bye old frame
}

///////////////////////////////////////////////////////////////////////////////

void cRHOgine::draw()
{
}

///////////////////////////////////////////////////////////////////////////////

void cRHOgine::endDraw()
{
	// Draw Console
	m_window.setView(m_window.getDefaultView());
	m_window.draw(*m_pConsole);

	if ( m_bFrameRateDisplayON )
	{
		std::stringstream fps_string;
		fps_string << "FPS: " << m_frameRate;
		m_fpsText.setString(fps_string.str());
		m_window.draw( m_fpsText );
	}

	m_window.display();	// Swappin' Buffers
}

///////////////////////////////////////////////////////////////////////////////

void cRHOgine::drawScene( Camera const & camera, Scene & scene )
{
    ErrorContext ec( "cRHOgine::drawScene" );
	m_window.setView( camera );		// Set Camerea
	scene.draw( m_window );			// Draw the Scene
}

///////////////////////////////////////////////////////////////////////////////
