
Entities = {
    Cosa = {
        TransformComponent = {
            x = 0, 
            y = 10
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
                    --transform.position.x = transform.position.x + (x * 3 * deltaTime) 
                    --transform.position.y = transform.position.y + (y * 3 * deltaTime) 
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
                print("Down: ", button)
                if(button == Key.A) then
                    print("Key was an A")
                else
                    print("Key was NOT an A")
                end
            end, 
            onKeyDown = function (this, button)
                print("Up: ", button)
            end, 
            onButtonUp = function (this, button)
                print("Button Up: ", button)
            end, 
            onButtonDown = function (this, button)
                transform = this:getTransformComponent() 
                if(transform) then
                    print("Accessing our transform: ", transform.position.y) 
                end
                print("Button Down: ", button)
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
        }}, 
        OtraCosa = {
            PhraseComponent = {
                phrase = "I'm saying hi from this lua Object!! :D"
            }, 
            SpriteComponent = {
                filename = "file.png"
            }}, 
            YOtraMas = {
                PhraseComponent = {
                    phrase = "I'm saying hi from this lua Object!! :D"
                }, 
                SpriteComponent = {
                    filename = "file.png"
                }}}

                

                

               