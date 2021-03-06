# Notes

## Cool Stuff

- Continuous collision detection using **Minkowski Differences**.
- **Chunk system** to optimize collision detection and relevant entities.
- **Editor/Developer Mode** to allow for quick iteration times, testing and development of the content.
- **State Saving/Loading/Reloading** to manage the attributes and state of all the entities and its components on a given moment.

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
 - To multithread maybe it is a good idea to have multiple copies of the world and have the renderer render the last one that is ready while other threads update the newer ones. Or just do a job system (or just use ASync) to paralelize work INSIDE the systems, dividing the tasks into equal blocks there.
 	- Naughty Dog talk about frames might be usefull for this: https://www.gdcvault.com/play/1022186/Parallelizing-the-Naughty-Dog-Engine / https://archive.org/details/GDC2015Gyrling_201508
 - Great simple article that explains how to tie behaviour of the system into a combination of components: https://www.gamedev.net/articles/programming/general-and-gameplay-programming/understanding-component-entity-systems-r3013/


## Other Notes/Resources/Ideas

 - 2D: Plattformers: http://higherorderfun.com/blog/2012/05/20/the-guide-to-implementing-2d-platformers/
 - Implement Children Entities from the component and retrieve the position and previous position from the transform by calling
 our own position() or previous_position() which will call our parents's until we reach a top entity.
 	- EntityManager should own all the entities with unique pointers but have also a list of the top ones for conveniency and for developer mode.
 - Build a good jump! Or even a better one: https://youtu.be/hG9SzQxaCm8
 - Why not read tracks to make the music? (THere are actually quite a few reasons not to do so but: https://github.com/cmatsuoka/libxmp )

