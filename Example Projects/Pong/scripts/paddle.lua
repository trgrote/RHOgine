Paddle = Paddle or {}			-- Set up class table

local xpos = screenBoundary.left + 50
local ypos = screenBoundary.bottom / 2
local flag = 1
local paddleSpeed = 800

function Paddle.init ( pad_obj, args )
	GameObject.addEventToRegister( pad_obj, "KeyDown" )
	
	local obj_data = GameObject.getData( pad_obj )	
	
	-- Change image to star.png
	local sprite = Sprite.newSprite()
	Sprite.setTexture( sprite, "textures/paddle.png" )
	Sprite.setScale( sprite, 0.10, 0.10 )
	
	obj_data.sprite = sprite
	obj_data.x = xpos
	obj_data.y = ypos
	obj_data.flag = flag
	-- Update the Position and Update the Sprite
	GameObject.setPosition( pad_obj, xpos, ypos )
	-- StaticSpriteComponent.updatePosition( GameObject.getComponent( jas_obj, "Sprite" ) )
	Sprite.setPosition( sprite, xpos, ypos )
	
	if args and args.player == Player2String then
		Paddle.setPlayer( pad_obj, 2 )
	end
	
end

function Paddle.update ( pad_obj, dt )
	local obj_data = GameObject.getData( pad_obj )
	GameObject.setPosition( pad_obj, obj_data.x, obj_data.y )
	Sprite.setPosition( obj_data.sprite, obj_data.x, obj_data.y )
	local tmpCollide, tmpSection = Paddle.isColliding ( pad_obj )
	if tmpCollide == true then
		if tmpSection == 1 then
		-- Change Ball trajectory from top paddle hit
		elseif tmpSection == 2 then
		-- Change Ball trajectory from middle paddle hit
		elseif tmpSection == 3 then
		-- Change Ball tranjectory from bottom paddle hit
		end
	end
end

function Paddle.draw ( pad_obj, render_target )
	local obj_data = GameObject.getData( pad_obj )
	RenderTarget.draw( render_target, obj_data.sprite )
end

function Paddle.handleEvent ( pad_obj, event )
	local obj_data = GameObject.getData( pad_obj )
	local tmpX, tmpY, tmpWidth, tmpHeight = Sprite.getBounds ( obj_data.sprite )
	
	if Event.getEventType(event) == "KeyDown" then
		if obj_data.flag == 1 then
			if Event.getArg( event, "key_code" ) == 22 and tmpY > screenBoundary.top then
				-- print ( type( scene ) )
				obj_data.y = obj_data.y - ( paddleSpeed * Game.getElapsedTime() )
			end
			if Event.getArg ( event, "key_code" ) == 18 and tmpY + tmpHeight < screenBoundary.bottom then
				obj_data.y = obj_data.y + ( paddleSpeed * Game.getElapsedTime() )
			end
		end
		if obj_data.flag == 2 then
			if Event.getArg( event, "key_code" ) == 73 and tmpY > screenBoundary.top then
				-- print ( type( scene ) )
				obj_data.y = obj_data.y - ( paddleSpeed * Game.getElapsedTime() )
			end
			if Event.getArg ( event, "key_code" ) == 74 and tmpY + tmpHeight < screenBoundary.bottom then
				obj_data.y = obj_data.y + ( paddleSpeed * Game.getElapsedTime() )
			end
		end
	end
end

function Paddle.setPlayer ( pad_obj, newFlag )
	local obj_data = GameObject.getData( pad_obj )
	obj_data.flag = newFlag
	if obj_data.flag == 2 then
		obj_data.x = screenBoundary.right - 60
		Sprite.setScale( obj_data.sprite, -0.10, 0.10 )
	end
end

function Paddle.isColliding ( pad_obj )
	if ballObj == nil then
		return false, 0
	end
	local obj_data = GameObject.getData ( pad_obj )
	local collideFlag = false
	local padSection = 0
	local tmpX, tmpY, tmpWidth, tmpHeight = Sprite.getBounds ( obj_data.sprite )
	local ball_data = GameObject.getData ( ballObj )
	local blX, blY, blWidth, blHeight = Sprite.getBounds ( ball_data.sprite )
	if (tmpX <= blX and tmpX + tmpWidth >= blX) or (tmpX <= blX + blWidth and tmpX + tmpWidth >= blX + blWidth) then
		print "KA"
		if (blY > tmpY and blY < tmpY + tmpHeight) or (blY + blHeight > tmpY and blY + blHeight < tmpY + tmpHeight) then
			print "BOING"
			local tmpMid = blY + (blHeight / 2)
			local tmpPadUp = tmpY + (tmpHeight * 0.33)
			local tmpPadMid = tmpY + (tmpHeight * 0.66)
			ball_data.ball_speed = ball_data.ball_speed + 25
			ball_data.xspeed = ball_data.xspeed * -1
			if (tmpMid < tmpPadUp) then
				ball_data.yspeed = ball_data.yspeed * 0.50
			elseif (tmpMid > tmpPadUp and tmpMid < tmpPadMid) then
				ball_data.yspeed = ball_data.yspeed * 1
			elseif (tmpMid > tmpPadMid) then
				ball_data.yspeed = ball_data.yspeed * 1.50
			else
				
			end
		end
	end
	return collideFlag, padSection
end

function Paddle.shutdown ( pad_obj )
	print "Paddle Destroyed"
	local obj_data = GameObject.getData( pad_obj )	
	Sprite.deleteSprite( obj_data.sprite )
end

print "Paddle Object Loaded"
