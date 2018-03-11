# Notes

## Ideas

### Raspberry Pi

 - Port the Engine/Game to a Raspberry Pi to simulate the constraints of console development and make things such as the custom memory management make sense.
 	- CLion with the RPi: http://foreglance.com/20160414/setup-and-use-c-ide-clion-on-raspberry-pi-with-ubuntu-mate/
 	- Cross-Compiling for the RPi: https://blog.kitware.com/cross-compiling-for-raspberry-pi/
 	- SFML for RPi: https://github.com/mickelson/sfml-pi
 	- Use Visual Studio to develop for the RPi: https://raspberrypi.stackexchange.com/questions/58797/raspberry-pi-c-ide
 	- Article about LUA in general which mentions LuaJIT to RPi: https://cellux.github.io/articles/introduction-to-luajit-part-1/

### ECS
 - Post about how to deal with inter-system dependencies: https://softwareengineering.stackexchange.com/a/362232
 - Article about ECS and system communicastion, actually good job Wikipedia!: https://en.wikipedia.org/wiki/Entity%E2%80%93component%E2%80%93system#Inter_system_communication
 - I really think I can store any dependency in the data (components) and not with new functions or calls between systems, for example, why call the sound system when the collision systems detects a component if we can just set that a collision has happened in the collision component and have the soundsystem read the "onCollision" sound in the component.
 	- The physics system, when trying to move something and that collides which the collision system catches, then the collision system has to be able to access the necessary data to correct that collision back to normal, that is, it can calculate how much one collider got inside the other and move them so the collision is not happening anymore.
 - To multithread maybe it is a good idea to have multiple copies of the world and have the renderer render the last one that is ready while other threads update the newer ones.
 	- Naughty Dog talk about frames might be usefull for this: https://www.gdcvault.com/play/1022186/Parallelizing-the-Naughty-Dog-Engine / https://archive.org/details/GDC2015Gyrling_201508