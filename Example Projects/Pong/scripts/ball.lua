-- These Varriables should will be set before the Game Has Loaded this script
-- These will be globals

Ball = Ball or {}		-- Set up class table

local screen_width = 1024
local screen_height = 768
local sprite_size = 20

math.randomseed( os.time() ) 	-- Seed Random

-- !! TODO !! --
-- Serve location is the middle of the paddle, coming from whoever won the previous match.
-- (!!pitch the ball from the center of last round winner's paddle instead of a random spot!!)
local p1PaddleServ = screenBoundary.bottom / 2
local p2PaddleServ = screenBoundary.bottom / 2

ball_life = 1000000000000

-- Construct Dak's balls
function Ball.init( ball_obj )
	
	local obj_data = GameObject.getData( ball_obj )		-- Return a table of data specific to this instance of the object	
	
	-- Default Values for this object
	obj_data.ball_speed = 750
	obj_data.elapsedTime 	= ball_life
	obj_data.xpos 		= 0
	obj_data.ypos 		= 0
	obj_data.xspeed 	= 0
	obj_data.yspeed 	= 0
	
	-- !! TODO !! --
	-- Change image to new candy ball graphic
	local sprite = Sprite.newSprite()
	Sprite.setTexture( sprite, "textures/GumBall.png" )
	obj_data.sprite = sprite
	
	-- Play the ball serve sound fx
	-- local sound = Sound.createSound( "ball_serve" )
	-- obj_data.sound = sound
	-- Sound.playSound( obj_data.sound )
	
	-- Ball is served from either P1 or P2, depending on last score
	-- Replace random function with flag received from game's ball respawn logic
	local starting_border = math.random( 1, 2 )
		
	if starting_border == 1 then					-- Serve from P1
		obj_data.xpos 	= 78						-- How far from the wall the ball starts (far edge of paddle)
		obj_data.xspeed = 1 * 0.45					-- Speed must be going right, pitch at 45 degrees down
		
		obj_data.ypos = p1PaddleServ
		obj_data.yspeed = 1 - math.abs(obj_data.xspeed)
		
	else											-- Serve from P2
		obj_data.xpos 	= (1024 - 128)      		-- How far from the wall the ball starts (far edge of paddle)
		obj_data.xspeed = -1 * 0.45					-- Speed must be going left, pitch at 45 degrees down
		
		obj_data.ypos = p2PaddleServ
		obj_data.yspeed = 1 - math.abs(obj_data.xspeed)
	end
	
	
	-- Update the Position and Update the Sprite
	GameObject.setPosition( ball_obj, obj_data.xpos, obj_data.ypos )
	Sprite.setPosition( sprite, obj_data.xpos, obj_data.ypos )

end

function Ball.update( ball_obj, dt )
	
	local obj_data = GameObject.getData( ball_obj )		-- Return a table of data specific to this instance of the object	
	sprite = obj_data.sprite
	
	-- Move the ball
	obj_data.xpos = obj_data.xpos + obj_data.ball_speed * dt * obj_data.xspeed
	obj_data.ypos = obj_data.ypos + obj_data.ball_speed * dt * obj_data.yspeed
	
	local x = (obj_data.xpos / 512 ) - 1
	local y = (obj_data.ypos / 384 ) - 1
	-- Sound.setPosition ( obj_data.sound, x, 0, y ) 
	
	-- If the ball somehow manages to get out of bounds, kill it with no score
	obj_data.elapsedTime = obj_data.elapsedTime - dt
	if obj_data.elapsedTime <= 0 then
		GameObject.setAlive( ball_obj, false )
	end	
	
	-- If the ball reaches the y-borders, it changes the movement on the y-plane
	if obj_data.ypos <= 105 then
	obj_data.yspeed = obj_data.yspeed * -1
	--obj_data.ball_speed = obj_data.ball_speed + 25
	end
	
	if obj_data.ypos >= 705 then
	obj_data.yspeed = obj_data.yspeed * -1
	--obj_data.ball_speed = obj_data.ball_speed + 25
	end
	
	-- !! TODO !! --
	-- Implement the "who scored and serves next" logic --
	-- If these borders are collided, kill the ball and tell the game logic that somebody
	-- scored a point based on whichever wall the ball hit. The ball takes an argument from
	-- some other dude to know who serves next round.
	if obj_data.xpos <= 20 then
	--obj_data.xspeed = 1 - (obj_data.xspeed * -1)
	GameObject.setAlive( ball_obj, false )
	fireBallDeathEvent( ball_obj, Player1String )
	end
	
	if obj_data.xpos >= (screen_width - 75) then
	--obj_data.xspeed = obj_data.xspeed * -1
	GameObject.setAlive( ball_obj, false )
	fireBallDeathEvent( ball_obj, Player2String )
	end
	
	-- Update the position of the ball's spirte
	GameObject.setPosition( ball_obj, obj_data.xpos, obj_data.ypos )
	Sprite.setPosition( sprite, obj_data.xpos, obj_data.ypos )
end

function Ball.draw( ball_obj, render_target )
	local obj_data = GameObject.getData( ball_obj )
	RenderTarget.draw( render_target, obj_data.sprite )
end

function Ball.shutdown( ball_obj )
	print "HE DEAD"
	local obj_data = GameObject.getData( ball_obj )	
	Sprite.deleteSprite( obj_data.sprite )
	-- Sound.deleteSound ( obj_data.sound )
end

print "Ball served"