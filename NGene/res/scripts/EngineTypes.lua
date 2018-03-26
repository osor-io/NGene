
EngineEntities = {

	MapCollider = {
		TransformComponent = {
            x = 0, 
            y = 0
        },
        ExtentComponent = {
            extentX = 400, 
            extentY = 400, 
            offsetX = 0, 
            offsetY = 0
        },
		CollisionComponent = {
	 			extentX = 0, 
	            extentY = 0, 
	            offsetX = 0, 
	            offsetY = 0,
	            type = ColliderType.TERRAIN
		}
	}

}

print("Loaded Internal Engine Types")
