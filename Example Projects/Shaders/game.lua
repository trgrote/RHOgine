
local scene = nil
local camera = nil

local screen_width 	= 1024
local screen_height = 768

-- local screen_width 	= 800
-- local screen_height = 600

function init()
	print "Init Called"
	
	-- dofile( "scripts/DrawableRectTest.lua" )
	
	-- print ( type ( Window ) )
	-- print ( type ( Window.getSize ) )
	-- print ( type ( Window.createWindow ) )
	
	-- Create Window
	Window.createWindow(
		{
			width 			= screen_width,
			height 			= screen_height,
			-- style 			= "Fullscreen",
			-- bitsPerPixel 	= 32,
			title 			= "Charles in Charge"
		}
	)
	
	scene = Scene.newScene()
	camera = Camera.newCamera()
	
	if GOFactoryManager.loadLuaObjectFactory("Jasmine", "scripts/jasmine.lua" ) then
		-- print "Everything Loaded, a-ok"
	else
		print "EVERYTHING FUCKING BROKE ABANDON SHIP!"
		Game.endGame()
		return
	end
	
	-- Scene.createGameObject( scene, "Jasmine", 
		-- {
			-- -- x = 5,
			-- -- y = 5
			-- -- visible = false
		-- } 
	-- )
	Scene.createGameObject( scene, "Jasmine" )
end

function update( dt )
	-- print "Update Called"
	Scene.update( scene, dt )
end

function draw( render_target )
	-- print "Draw Called"
	Game.drawScene( camera, scene )
end

function shutdown()
	print "Shutdown Called"
	Camera.deleteCamera( camera )
	Scene.deleteScene( scene )
end
