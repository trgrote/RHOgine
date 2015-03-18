-- PONG GAME

-- Load up this FUCKING GAME
function init()
	print "Init Called"

	-- Load Pong Game Global Functions
	dofile "scripts/globals.lua"

	-- print( type( changeScore ) )

	-- Create Window
	Window.createWindow(
		{
			width 			= 1024,
			height 			= 768,
			-- style 			= "Fullscreen",
			-- bitsPerPixel 	= 32,
			title 			= "Saga Panga"
		}
	)
	
	scene = Scene.newScene()
	camera = Camera.newCamera()

	-- Load All Sound Objects
	Audio.clearSoundCache()
	Audio.addSoundCache( "audio/BallHit1.wav", "BallHit1" )
	Audio.addSoundCache( "audio/BallHit2.wav", "BallHit2" )
	Audio.addSoundCache( "audio/BallHit3.wav", "BallHit3" )
	Audio.addSoundCache( "audio/Exit.wav", "Exit" )
	Audio.addSoundCache( "audio/LoadScreen.wav", "LoadScreen" )
	Audio.addSoundCache( "audio/LoopInGame1.wav", "LoopInGame1" )
	Audio.addSoundCache( "audio/LoopInGame2.wav", "LoopInGame2" )
	Audio.addSoundCache( "audio/LoopInGame3.wav", "LoopInGame3" )
	Audio.addSoundCache( "audio/Start.wav", "Start" )
	
	-- Load Object Info
	if GOFactoryManager.loadObjectFactoriesFromJSON("objects.json") then
		print "Objects Loaded"
	else
		print "Objects failed to load"
		Game.endGame()
		return
	end
	
	scene = Scene.newScene()	-- Make New Scene
	
	-- setUpPongScene()	-- Set up PANG SCENE
	setUpMenuScreen()
end

-- Update this FUCKING GAME
function update( dt )
	-- print "Update Called"
	Scene.update( scene, dt )
end

-- Draw this FUCKING GAME
function draw( render_target )
	-- print "Draw Called"
	Game.drawScene( camera, scene )
end

-- Shutdown this FUCKING GAME
function shutdown()
	print "Shutdown Called"
	Camera.deleteCamera( camera )
	Scene.deleteScene( scene )
end

-- End of game.lua