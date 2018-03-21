
Entities = {
    DynamicObject = {
        TransformComponent = {
            x = 0, 
            y = 10
        }, 
        ExtentComponent = {
            extentX = 40, 
            extentY = 43, 
            offsetX = 0, 
            offsetY = 0
        },
        CollisionComponent = {
            extentX = 40, 
            extentY = 43, 
            offsetX = 0, 
            offsetY = 0,
            dynamic = true
        },  
        PhraseComponent = {
            phrase = "I'm saying hi from this lua Object!! :D"
        }, 
        SpriteComponent = {
            filename = "C:/tmp/test.png"
        }, 
        BehaviourComponent = {
            onUpdate = function (this, deltaTime)
                transform = this:getTransformComponent()
                local x = getAxisPosition(Axis.X)
                local y = getAxisPosition(Axis.Y)
                if(transform) then
                    --[[
                    transform.position.x = transform.position.x + (10.0 * deltaTime) 
                    transform.position.y = transform.position.y + (10.0 * deltaTime) 
                    --]]

                    if (isKeyPressed(Key.A)) then
                        transform.position.x = transform.position.x - (200.0 * deltaTime) 
                    elseif (isKeyPressed(Key.D)) then
                        transform.position.x = transform.position.x + (200.0 * deltaTime)
                    end

                    if (isKeyPressed(Key.W)) then
                        transform.position.y = transform.position.y - (200.0 * deltaTime)
                    elseif (isKeyPressed(Key.S)) then
                        transform.position.y = transform.position.y + (200.0 * deltaTime)
                    end

                     if (isKeyPressed(Key.J)) then
                        transform.position.x = transform.position.x - (20000.0 * deltaTime) 
                    elseif (isKeyPressed(Key.L)) then
                        transform.position.x = transform.position.x + (20000.0 * deltaTime)
                    end

                    if (isKeyPressed(Key.I)) then
                        transform.position.y = transform.position.y - (20000.0 * deltaTime)
                    elseif (isKeyPressed(Key.K)) then
                        transform.position.y = transform.position.y + (20000.0 * deltaTime)
                    end

                else 
                    print("We got no transform")
                end
                --[[
                            print("X:", getAxisPosition(Axis.X))
                            print("Y:", getAxisPosition(Axis.Y))
                            print("Z:", getAxisPosition(Axis.Z))
                            print("R:", getAxisPosition(Axis.R))
                            print("U:", getAxisPosition(Axis.U))
                            print("V:", getAxisPosition(Axis.V))
                            print("PovX:", getAxisPosition(Axis.PovX))
                            print("PovY:", getAxisPosition(Axis.PovY))
                            --]]
            end
        }, 
        InputComponent = {
            onKeyUp = function (this, button)
                --print("Down: ", button)
                if(button == Key.A) then
                    --print("Key was an A")
                else
                    --print("Key was NOT an A")
                end
            end, 
            onKeyDown = function (this, button)
                --print("Up: ", button)
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
            dynamic = false
        }
    }, 

    OtraCosa = {
        PhraseComponent = {
            phrase = "I'm saying hi from this lua Object!! :D"
        }, 
        SpriteComponent = {
            filename = "file.png"
        }
    }, 

    YOtraMas = {
        PhraseComponent = {
            phrase = "I'm saying hi from this lua Object!! :D"
        }, 
        SpriteComponent = {
            filename = "file.png"
        }
    },

    DefaultMap = {
        TiledMapComponent = {
            filename = "res/assets/maps/demo.tmx", 
            mainLayer = 0; 
        }
    },

    DefaultCamera = {
        TransformComponent = {
            x = 0, 
            y = 0
        }, 
        CameraComponent = {
            mainCamera = true,
            zoom = 1.0
        }
    }

}

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                        

                       