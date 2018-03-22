# ToDo Document

## @@TODO


* Also reload Entity Types when reloading states.
* Implement Save State Light to do things like reloading a state during gameplay (ala checkpoints). WIthout reloading expensive things that will be the same like the map.
* Think how we should organize the different colliders of the map to fill when reading the lua file of the map.
	* Right now I think it might be best to just edit the colliders in the editor and place them there in a new entity for each one: THe entity type for map collider could have a behaviour component that makes the extent significantly bigger than the collider size (or have the threshold bigger for the extent of those entities).
* Pool per Component Type

## @@DOING

* Read Tiled maps from lua into new members of the Tiled Map Component

