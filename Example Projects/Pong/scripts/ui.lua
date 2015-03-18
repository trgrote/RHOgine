UI = UI or {}			-- Set up class table

function UI.init( ui_obj )
	GameObject.addEventToRegister( ui_obj, scoreChangedEventName )

	-- Set Depth to Closest to Screen
	GameObject.setDepth( ui_obj, -10 )

	-- Get Object Data
	local obj_data = GameObject.getData( ui_obj )

	-- Load Text and Bullshit
	-- Load Debug Overlay
	obj_data.overlay = Sprite.newSprite( "textures/50px 50% grid overlay 1280x1024.png" )

	-- Load Scoreboard
	obj_data.scoreBoard = Sprite.newSprite( "textures/Scoreboard2.png" )
	Sprite.setPosition( obj_data.scoreBoard, 255, 0 )
	Sprite.setScale( obj_data.scoreBoard, 0.5, 0.5 )

	-- Load Text
	obj_data.playerOneScore = Text.newText("textures/electrox.ttf")
	Text.setPosition( obj_data.playerOneScore, 150, -5 )
	Text.setString( obj_data.playerOneScore, string.format( "%d", playerOneScore ) )
	Text.setColor( obj_data.playerOneScore, 255, 204, 51 )
	Text.setCharacterSize( obj_data.playerOneScore, 48 )

	obj_data.playerTwoScore = Text.newText("textures/electrox.ttf")
	Text.setPosition( obj_data.playerTwoScore, 850, -5 )
	Text.setString( obj_data.playerTwoScore, string.format( "%d", playerTwoScore ) )
	Text.setColor( obj_data.playerTwoScore, 255, 204, 51 )
	Text.setCharacterSize( obj_data.playerTwoScore, 48 )
end

function UI.update( ui_obj, dt )
	local obj_data = GameObject.getData( ui_obj )
end

function UI.draw( ui_obj, render_target )
	local obj_data = GameObject.getData( ui_obj )
	-- RenderTarget.draw( render_target, obj_data.overlay )
	RenderTarget.draw( render_target, obj_data.scoreBoard )
	RenderTarget.draw( render_target, obj_data.playerOneScore )
	RenderTarget.draw( render_target, obj_data.playerTwoScore )
end

function UI.shutdown( ui_obj )
	local obj_data = GameObject.getData( ui_obj )

	Text.deleteText( obj_data.playerOneScore )
	Text.deleteText( obj_data.playerTwoScore )

	Sprite.deleteSprite( obj_data.scoreBoard )
	Sprite.deleteSprite( obj_data.overlay )
end

function UI.handleEvent( ui_obj, event )
	-- Handle Events
	local obj_data = GameObject.getData( ui_obj )
	if Event.getEventType(event) == scoreChangedEventName then
		-- print "Score Changed!"
		-- Update Text
		local player = HashString.getStringFromHash( Event.getArg( event, "player" ) )

		if player == Player1String then
			Text.setString( obj_data.playerOneScore, string.format( "%d", playerOneScore ) )
		elseif player == Player2String then
			Text.setString( obj_data.playerTwoScore, string.format( "%d", playerTwoScore ) )
		else
			print "I hate everything"
		end
	end
end

print "UI Loaded"
