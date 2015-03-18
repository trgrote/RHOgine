MainMenu = MainMenu or {}

-- Global Constants
local spinSpeed = 50

local startButtonBounds =
{
	left 	= 300,
	top 	= 500,
	width 	= 450,
	height 	= 50
}

local exitButtonBounds =
{
	left 	= 300,
	top 	= 550,
	width 	= 450,
	height 	= 50
}

local function inBounds( xpos, ypos, buttonBounds )

	if 	xpos > buttonBounds.left and
		xpos < buttonBounds.left + buttonBounds.width then
		if 	ypos > buttonBounds.top and
			ypos < buttonBounds.top + buttonBounds.height then
			
			return true
		end
	end

	return false
	
end

function MainMenu.init( obj )
	-- Register to bullshit occuring
	GameObject.addEventToRegister( obj, "MouseButtonReleased" )
	GameObject.addEventToRegister( obj, "MouseMoved" )
	
	local obj_data = GameObject.getData( obj )
	
	obj_data.overExit = false
	obj_data.overStart = false
	
	-- Load Overlap
	obj_data.overlaySprite = Sprite.newSprite ( "textures/50px 50% grid overlay 1280x1024.png" )
	
	-- Exit On Over Thing
	obj_data.exitSprite = Sprite.newSprite ( "textures/ExitSelect.png" )
	Sprite.setPosition( obj_data.exitSprite, 400, 575 )
	
	-- Start On Over thingy
	obj_data.startSprite = Sprite.newSprite ( "textures/StartSelect.png" )
	Sprite.setPosition( obj_data.startSprite, 250, 500 )
	
	-- Load Background
	obj_data.backgroundSprite = Sprite.newSprite( "textures/RainbowlandBIt.png" )
	Sprite.setPosition( obj_data.backgroundSprite, 0, 0 )
	
	-- Load Spin background
	obj_data.spinSprite = Sprite.newSprite( "textures/StartScreen StarSpin.png" )
	Sprite.setPosition( obj_data.spinSprite, 1024 / 2, 768 / 2 )
	Sprite.setOrigin( obj_data.spinSprite, 1024 / 2 + 128, 1024 / 2 + 128 )
	Sprite.setRotation( obj_data.spinSprite, 0 )
	
	-- All the Tiles, ya butthead
	obj_data.menuSprite = Sprite.newSprite( "textures/TitleText Blocked.png" )
	Sprite.setPosition( obj_data.menuSprite, 0, 0 )
	
	local sound = Sound.createSound( "LoadScreen" )
	obj_data.sound = sound
	Sound.playSound( obj_data.sound )
	
end

function MainMenu.update( obj, dt )
	local obj_data = GameObject.getData( obj )
	
	Sprite.setRotation( 
		obj_data.spinSprite,  
		( Sprite.getRotation( obj_data.spinSprite ) + spinSpeed * dt ) % 360
	)
	
end

function MainMenu.draw( obj, render_target )
	local obj_data = GameObject.getData( obj )
	RenderTarget.draw( render_target, obj_data.backgroundSprite )
	RenderTarget.draw( render_target, obj_data.spinSprite )
	RenderTarget.draw( render_target, obj_data.menuSprite )
	
	
	if obj_data.overExit then
		RenderTarget.draw( render_target, obj_data.exitSprite )
	end
	
	if obj_data.overStart then
		RenderTarget.draw( render_target, obj_data.startSprite )
	end
	
	-- RenderTarget.draw( render_target, obj_data.overlaySprite )
end

function MainMenu.handleEvent( obj, event )
	local obj_data = GameObject.getData( obj )

	if Event.getEventType(event) == "MouseButtonReleased" then
		local x = Event.getArg( event, "x" )
		local y = Event.getArg( event, "y" )
		
		if inBounds( x, y, startButtonBounds ) then
			GameObject.setAlive( obj, false )
			
			setUpPongScene()
		elseif inBounds( x, y, exitButtonBounds ) then
			Game.endGame()
		end
		
	elseif Event.getEventType(event) == "MouseMoved" then		
		local x = Event.getArg( event, "x" )
		local y = Event.getArg( event, "y" )
		
		if inBounds( x, y, startButtonBounds ) then
			obj_data.overStart = true
			obj_data.overExit = false
		elseif inBounds( x, y, exitButtonBounds ) then
			obj_data.overExit = true
			obj_data.overStart = false
		else
			obj_data.overExit = false
			obj_data.overStart = false
		end		
	end
end

function MainMenu.shutdown( obj )
	local obj_data = GameObject.getData( obj )
		
	Sprite.deleteSprite( obj_data.backgroundSprite )
	Sprite.deleteSprite( obj_data.spinSprite )
	Sprite.deleteSprite( obj_data.menuSprite )
	Sprite.deleteSprite( obj_data.overlaySprite )
	Sprite.deleteSprite( obj_data.exitSprite )
	Sprite.deleteSprite( obj_data.startSprite )
	Sound.stopSound( obj_data.sound )
	Sound.deleteSound( obj_data.sound )
end
