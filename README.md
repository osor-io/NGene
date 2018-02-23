# NGene
Personal project dedicated to an Engine+Game exploring a few interesting concepts and design decisions.

## Concepts

* ECS system with declaration of entities and components in LUA. Explore the benefits of also defining all the game specific behaviour on LUA instead of C++, leaving C++ just for generic engine code.
* Integrate information about and modification of entities and components into a debug GUI (Combination of ImGUI, ECS, LUA and metaprogramming to access Class information).
* Override Global New and Delete to use my own memory allocation. Consider options like "Memory Pool Per Object Type" or "Small Memory Allocators"
	* Memory allocators with a stack of free positions.
