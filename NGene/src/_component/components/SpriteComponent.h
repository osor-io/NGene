/**

The general documentation for the components will be in
the Component.h and CompomentTemplate.h respectively. Here you
can find the documentation of the general members that every component
should have such as the serialization methods (to and from Json),
the ones used to expose the component to LUA scripts or draw its inspector
for the developer mode.

@see ComponentTemplate.h
@see Component.h

*/

#pragma once
#include "../ComponentTemplate.h"
#include <SFML\Graphics.hpp>


/**

Component that represents a visible sprite that represents a particular
entity. This sprite, as far as this component is concerned, is a static
2D image but can be set by other systems and changed between frames to 
create animations.

*/
class SpriteComponent : public ComponentTemplate<SpriteComponent> {
    friend auto meta::registerMembers<SpriteComponent>();
public:
    SpriteComponent(EntityId id);
    SpriteComponent(EntityId id, const sol::table& table);
    ~SpriteComponent();

    json to_json() override;
    void load_json(const json& j) override;


    // ====== BEG OF REQUIREMENTS ======
    void draw_component_inspector() override;
    static void expose_to_lua();
    // ====== END OF REQUIREMENTS ======


    /**

    These are just general getters and setters for the parameters that define 
    the sprite. We use an rvalue optimization because it is likely that we
    will set sprites directly from a newly created Sprite object which
    would optimize this set a bit.

    */

    std::string get_filename() const;
    sf::Sprite* get_sprite_ptr();
    sf::Sprite get_sprite() const;
    void set_sprite(const sf::Sprite& sprite);
    void set_sprite_rval(sf::Sprite&& sprite);

    /**
    
    Load the default sprite based on the texture found
    when constructing the component.
    
    */
    void load_sprite();

    // ====== BEG OF MEMBERS ======

    /**
    
    THe original name of the file that contained the sprite being
    rendered from this component.

    */
    std::string m_filename;
    
    /**
    
    The sprite that will be rendered regarding this entity.
    
    */
    sf::Sprite m_sprite;

    /**
    
    The original texture that contained the image that at the
    same time contained the sprite that would be rendered
    regarding this entity.

    */
    sf::Texture* m_texture;

    /**
    
    The 'Z' or "depth" layer that this sprite would be in.
    This is used to properly order the rendered sprites during gameplay
    and correctly have some in front of others when it makes sense.
    
    */
    unsigned int m_layer{ 1000 };

    // ====== END OF MEMBERS ======


};

template<>
inline auto meta::registerName<SpriteComponent>() {
    return "SpriteComponent";
}

template<>
inline auto meta::registerMembers<SpriteComponent>() {
    return members(
        member("filename", &SpriteComponent::m_filename),
        member("layer", &SpriteComponent::m_layer)
    );
}

