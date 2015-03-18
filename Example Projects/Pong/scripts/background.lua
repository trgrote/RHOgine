Background = Background or {}			-- Set up class table

function Background.init( obj )	
	-- Set Depth to farthest to Screen
	GameObject.setDepth( obj, 10 )
	
	-- Get Object Data
	local obj_data = GameObject.getData( obj )
	
	-- Load Background
	obj_data.backgroundSprite = Sprite.newSprite( "textures/RainbowlandBIt.png" )
	Sprite.setPosition( obj_data.backgroundSprite, 0, 0 )
	
	-- Load Court
	obj_data.courtSprite = Sprite.newSprite( "textures/BoundingCourt.png" )
	Sprite.setPosition( obj_data.courtSprite, 6, 95 )
end

function Background.update( obj, dt )
	local obj_data = GameObject.getData( obj )
end

function Background.draw( obj, render_target )
	local obj_data = GameObject.getData( obj )
	RenderTarget.draw( render_target, obj_data.backgroundSprite )
	RenderTarget.draw( render_target, obj_data.courtSprite )
end

function Background.shutdown( obj )
	local obj_data = GameObject.getData( obj )
		
	Sprite.deleteSprite( obj_data.backgroundSprite )
	Sprite.deleteSprite( obj_data.courtSprite )
end

print "Background Loaded"
