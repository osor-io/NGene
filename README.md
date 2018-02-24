# NGene
Personal project dedicated to an Engine+Game exploring a few interesting concepts and design decisions.

## Concepts

* ECS system with declaration of entities and components in LUA. Explore the benefits of also defining all the game specific behaviour on LUA instead of C++, leaving C++ just for generic engine code.
* Integrate information about and modification of entities and components into a debug GUI (Combination of ImGUI, ECS, LUA and metaprogramming to access Class information).
* Override Global New and Delete to use my own memory allocation. Consider options like "Memory Pool Per Object Type" or "Small Memory Allocators"
	* Memory allocators with a stack of free positions.
* Define and keep components specific to the scope of the project. If we want a 2D platformer then we should define the components especific for that. Avoiding going general when we don't need to.
* Define components in a similar way that Unity does it. Just defining data attributes in the interface outside the engine (The editor/scripts in Unity and Lua in this project).
	*  Most of the general behaviour should be defined in the engine with the components leaving only very specific game behaviour in the Lua side of things.