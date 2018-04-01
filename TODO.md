# ToDo Document

In general, smaller @@TODO tasks that are not big or relevant enough to be written here are in the code itself annotated with the tag "@@TODO" in comments throughout the code.

## @@TODO

* **Read Extent of "Extent Components"** of the Colliders in Tiled maps. That is, if we define it, if not we sould just give it a good default extent to check against.
* Allow for **more than one instance of the same component** type in the same entity. Previous getComponent functions should still be there and returning the first instance of a component but for some specific components like collision components the functionality of having more than one seems necessary (collider for physical extent, other for dealing damage, etc).
* Add **triggers to the collision component**:
	* Add a new type to the enum in the collision component that is a trigger and a new attribute to the component that holds the functions to call in relation to that trigger (on trigger enter, leave, etc).
	* Add a boolean to the collision component to know if the current collider can trigger or not, that is, should be checked in the collisions against triggers.
	* Add the check between moving entities that can trigger and triggers in the collision system.
* **Render Main Map layer** just after all the in-game sprites for things like grass being on top of the gameplay elements.
* Implement **Save State Light** to do things like reloading a state during gameplay (ala checkpoints). WIthout reloading expensive things that will be the same like the map.
* **Pool per Component Type**
* Implement **data table in BehaviourComponent** so we can keep state between updates
* Implement and expose **LUA enum with general XBOX controller buttons** to make calling functions like isButtonPressed easier.

## @@TODO @@MAYBE

* **Bake the static parts of the loaded map** into a vertex array:
	* Although, keep in mind that this will make it harder to use different textures (different spritesheets with different tilesets) in the same layer D:

## @@DOING

* Checking correct behaviour and tweaking the platformer physics component (mario like movement).
	* Also, deciding whether the player should have full control in the air and in the movement should just accelerate to max speed when pressing the button or accelerate slowly like in mario games.