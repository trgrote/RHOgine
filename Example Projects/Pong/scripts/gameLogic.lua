dofile "scripts/class.lua"

ScreenShaker = class()

local shakeTime = 1
local shakePower = 20
local maxShake = 10

function ScreenShaker:__init( camera_arg )
	self.camera = camera_arg
	self.start_pos_x, self.start_pos_y = Camera.getCenter( self.camera )
	self.totalTime = 0
	self.done = false
	
	self.xShake = math.random( 5, maxShake ) 
	self.yShake = math.random( 5, maxShake ) 
	
	if math.random( 1, 100 ) % 2 == 0 then
		self.xShake = -self.xShake
	end
	
	if math.random( 1, 100 ) % 2 == 0 then
		self.yShake = -self.yShake
	end
	
	print ( self.xShake )
	print ( self.yShake )
	
end

function ScreenShaker:isDone()
	return self.done
end

function ScreenShaker:update( dt )
	self.totalTime = self.totalTime + dt
	
	if self.totalTime > shakeTime then
		self.done = true
		-- print "DOEN WITH SHAKE"
		return
	end
	
	-- Function is sin( 20 * totalTime ) * ( 1 - self.totalTime/1 )
	local new_x = self.xShake * math.sin( shakePower * self.totalTime ) * ( 1 - self.totalTime / shakeTime )
	local new_y = self.yShake * math.sin( shakePower * self.totalTime ) * ( 1 - self.totalTime / shakeTime )
	
	-- print ( new_x )
	
	Camera.setCenter( self.camera, self.start_pos_x + new_x, self.start_pos_y + new_y )
	-- Camera.move( self.camera, new_x, 0 )
end

GameLogic = GameLogic or {}


function GameLogic.init( obj )
	-- Register to bullshit occuring
	GameObject.addEventToRegister( obj, ballDeathEventName )
	GameObject.addEventToRegister( obj, scoreChangedEventName )
	-- GameObject.addEventToRegister( pad_obj, "KeyDown" )
end

function GameLogic.shutdown( obj, dt )
	print "GameLogic Destroyed"
end

function GameLogic.update( obj, dt )
	local obj_data = GameObject.getData( obj )
	
	if obj_data.screenShaker ~= nil then
		-- Shake the Camera
		-- print "Calling Screen Shaker"
		obj_data.screenShaker:update( dt )
		
		-- camera
		
		if obj_data.screenShaker:isDone() then
			-- print "Deleting Screen Shaker"
			obj_data.screenShaker = nil
		end
	end
end

function GameLogic.beginCameraShake( obj )
	local obj_data = GameObject.getData( obj )
	
	obj_data.screenShaker = ScreenShaker( camera )
	
end

function GameLogic.handleEvent( obj, event )
	if Event.getEventType( event ) == ballDeathEventName then
		local player_side = HashString.getStringFromHash( Event.getArg( event, "player_side" ) )

		GameLogic.beginCameraShake( obj )
		
		-- Depend in the side the ball died on, change who won
		if player_side == Player1String then
			-- Points Go to player 2
			changeScore( Player2String, 1 )
			resetRound( Player2String )
		elseif player_side == Player2String then
			-- Points Go to player 1
			changeScore( Player1String, 1 )
			resetRound( Player1String )
		end		
	elseif Event.getEventType( event ) == scoreChangedEventName then
		-- Call other function there bub
		GameLogic.onScoreChanged(
			obj,
			HashString.getStringFromHash( Event.getArg( event, "player" ) ),
			HashString.getStringFromHash( Event.getArg( event, "amount" ) )
		)
	end
end

function GameLogic.onScoreChanged( obj, player, amount )
	-- Check Scores, if one player has higher score, then reset game
	-- resetRound( player )
end
