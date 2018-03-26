# ToDo Document

In general, smaller @@TODO tasks that are not big or relevant enough to be written here are in the code itself annotated with the tag "@@TODO" in comments throughout the code.

## @@TODO

* Also reload Entity Types when reloading states.
* Implement Save State Light to do things like reloading a state during gameplay (ala checkpoints). WIthout reloading expensive things that will be the same like the map.
* Pool per Component Type
* Implement data table in BehaviourComponent so we can keep state between updates
* Implement and expose LUA enum with general XBOX controller buttons to make calling functions like isButtonPressed easier.


## @@TODO @@MAYBE
* Bake the static parts of the loaded map into a vertex array:
	* Although, keep in mind that this will make it harder to use different textures (different spritesheets with different tilesets) in the same layer D:


## @@DOING

* Read Extent of "Extent Components" of the Colliders in Tiled maps. That is, if we define it, if not we sould just give it a good default extent to check against.
