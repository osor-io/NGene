# ToDo Document

In general, smaller @@TODO tasks that are not big or relevant enough to be written here are in the code itself annotated with the tag "@@TODO" in comments throughout the code.

## @@TODO

* Extend Collision Component and System to support different types of colliders besides just static or dynamic (e.g. One-Way Colliders).
* Also reload Entity Types when reloading states.
* Implement Save State Light to do things like reloading a state during gameplay (ala checkpoints). WIthout reloading expensive things that will be the same like the map.
* Think how we should organize the different colliders of the map to fill when reading the lua file of the map.
	* Right now I think it might be best to just edit the colliders in the editor and place them there in a new entity for each one: THe entity type for map collider could have a behaviour component that makes the extent significantly bigger than the collider size (or have the threshold bigger for the extent of those entities).
* Pool per Component Type
* Implement data table in BehaviourComponent so we can keep state between updates
* Implement and expose LUA enum with general XBOX controller buttons to make calling functions like isButtonPressed easier.


## @@TODO @@MAYBE
* Bake the static parts of the loaded map into a vertex array:
	* Although, keep in mind that this will make it harder to use different textures (different spritesheets with different tilesets) in the same layer D:


## @@DOING

* Read Extent of "Extent Components" of the Colliders in Tiled maps.
