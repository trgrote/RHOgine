#include <iostream>
#include <memory>

#include <RHOgine/RHOgine.h>

#define SCRIPT_NAME "game.lua"

using namespace std;
using namespace rho;

class TestGame : public rho::cRHOgine
{
private:
protected:

	bool m_bScriptFound;

public:
    TestGame()
	{
		m_windowConfig.windowTitle = "Test Game";
		m_windowConfig.vsyncEnabled = false;
		m_windowConfig.windowWidth = 800;
		m_windowConfig.windowHeight = 600;
		m_bScriptFound = false;
	}

    virtual ~TestGame()
	{
		if ( m_bScriptFound )
			lua::call_func_if_exists( m_pLua, "shutdown", "" );
	}

    virtual int Initialize()
	{
	    cRHOgine::Initialize();

		// Load Some Dumass GameObjects up into this Bitch
		// assert_msg( m_GOFactoryManager.loadObjectFactoriesFromJSON("objects.json"), "Objects Failed to load" );

		// If lua file exists, then call it, then call global init
		if ( FILE * lua_script = fopen( SCRIPT_NAME, "r" ) )
		{
			fclose( lua_script );
			lua::dofile(m_pLua, SCRIPT_NAME );

			lua::call_func_if_exists( m_pLua, "init", "" );

			m_bScriptFound = true;
		}

		return true;
	}

    virtual void update(sf::Time const & elapsed_time)
	{
		cRHOgine::update( elapsed_time );
		if ( m_bScriptFound )
			lua::call_func_if_exists( m_pLua, "update", "f", elapsed_time.asSeconds() );
	}

    virtual void draw()
	{
		cRHOgine::draw();
		if ( m_bScriptFound )
			lua::call_func_if_exists( m_pLua, "draw", "p", &m_window );
	}
};

int main( int argc, char * argv[] )
{
    try
    {
        unique_ptr< cRHOgine > game ( new TestGame() );

        game->run();
    }
    catch( std::exception & excep )
    {
        std::cerr << excep.what() << std::endl;
    }

    system("PAUSE");

    return 0;
}
