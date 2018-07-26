
Entities = {
    DynamicObject = {
        TransformComponent = {
            x = 50, 
            y = 18
        }, 
        ExtentComponent = {
            extentX = 6, 
            extentY = 8, 
            offsetX = 0, 
            offsetY = 0
        },
        CollisionComponent = {
            extentX = 3, 
            extentY = 4, 
            offsetX = 0, 
            offsetY = 0,
            type = ColliderType.MOVING_OBJECT
        },
        PlatformerPhysicsComponent = {
            maxFootSpeed = 64.0,
			timeToMaxFootSpeed = 0.1,
			timeToStop = 0.05,
            distanceToPeak = 25.0,
            jumpHeight = 40.0,
            downwardsGravityScale = 2.5
        },
        PhraseComponent = {
            phrase = "I'm saying hi from this lua Object!! :D"
        }, 
        SpriteComponent = {
            filename = "./res/assets/test.png"
        }, 
        BehaviourComponent = {
            onUpdate = function (this, deltaTime)
                movement = this:getPlatformerPhysicsComponent()
                if(movement) then
                    if (isKeyPressed(Key.A)) then
                        movement:move(MovementDirection.Left)
					end
                    if (isKeyPressed(Key.D)) then
                        movement:move(MovementDirection.Right)
                    end

                    if (isKeyPressed(Key.Space)) then
                        movement:jump()
                    end

                else 
                    print("We got no Platformer Physics Component")
                end
            end
        }, 
        InputComponent = {
            onKeyUp = function (this, key)
                --print("Down: ", button)
                if(button == Key.A) then
                    --print("Key was an A")
                else
                    --print("Key was NOT an A")
                end
            end, 
            onKeyDown = function (this, key)
                --[[
                if(key == Key.Space) then
                    movement = this:getPlatformerPhysicsComponent()
                    if(movement) then
                        movement:jump()
                    end
                end
                --]]
            end, 
            onButtonUp = function (this, button)
                --print("Button Up: ", button)
            end, 
            onButtonDown = function (this, button)
                transform = this:getTransformComponent() 
                if(transform) then
                    --print("Accessing our transform: ", transform.position.y) 
                end
                --print("Button Down: ", button)
            end, 
            forLeftJoystick = function (this, x, y)
                local deltaTime = getDeltaTime()
                transform = this:getTransformComponent()
                if(transform) then
                    transform.position.x = transform.position.x + (x * 3 * deltaTime) 
                    transform.position.y = transform.position.y + (y * 3 * deltaTime) 
                end
            end, 
            forRightJoystick = function (this, x, y)
                local deltaTime = getDeltaTime()
                transform = this:getTransformComponent()
                if(transform) then
                    transform.position.x = transform.position.x + (x * 3 * deltaTime) 
                    transform.position.y = transform.position.y + (y * 3 * deltaTime) 
                end
            end
        }
    },

    DummyDynamicObject = {
        TransformComponent = {
            x = 71, 
            y = 18
        }, 
        ExtentComponent = {
            extentX = 6, 
            extentY = 8, 
            offsetX = 0, 
            offsetY = 0
        },
        CollisionComponent = {
            extentX = 3, 
            extentY = 4, 
            offsetX = 0, 
            offsetY = 0,
            type = ColliderType.MOVING_OBJECT
        },  
        PhraseComponent = {
            phrase = "I'm saying hi from this lua Object!! :D"
        }, 
        SpriteComponent = {
            filename = "./res/assets/test.png"
        }
    },

    StaticObject = {
        TransformComponent = {
            x = 0, 
            y = 10
        },
        ExtentComponent = {
            extentX = 800, 
            extentY = 800, 
            offsetX = 0, 
            offsetY = 0
        },
        CollisionComponent = {
            extentX = 40, 
            extentY = 43, 
            offsetX = 0, 
            offsetY = 0,
            type = ColliderType.TERRAIN
        }
    }, 

	
	--I believe in miracles.
	--Where you from?
    --[[You]]TestingThing = {
	--You Testing Think yeah
        PhraseComponent = {
            phrase = "I'm saying hi from this lua Object!! :D"
        }
    },

    DefaultMap = {
        TiledMapComponent = {
            filename = "res/assets/maps/CandyTest.lua", 
            mainLayer = 1
        }
    },

    DefaultCamera = {
        TransformComponent = {
            x = 70, 
            y = 4
        }, 
        CameraComponent = {
            mainCamera = true,
            zoom = 1.0
        }
    }

}

print("Loaded Custom Entity Types")
                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                       