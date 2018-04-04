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

namespace config {
    constexpr auto max_phrase_size = 256;
}

/**

Component that contains a phrase that might be relevant or could be shown
during gameplay. It is basically a very simple component used at the beginning of
development to test how this component-based arquitecture would work but it could
be used during gameplay for very simple phrase based interactions like a sign or
something like that.

*/
class PhraseComponent : public ComponentTemplate<PhraseComponent> {
    friend auto meta::registerMembers<PhraseComponent>();
public:
    PhraseComponent(EntityId id);
    PhraseComponent(EntityId id, const sol::table& table);
    ~PhraseComponent();

    json to_json() override;
    void load_json(const json& j) override;

    std::string get_phrase() const;
    void set_phrase(const std::string& phrase);

    // ====== BEG OF REQUIREMENTS ======
    void draw_component_inspector() override;
    static void expose_to_lua();
    // ====== END OF REQUIREMENTS ======


    // ====== BEG OF MEMBERS ======

    /**
    
    The phrase that the entity containing this component would "say" during
    a particular interaction or in a particular moment.

    */
    std::string m_phrase;

    // ====== END OF MEMBERS ======

};

template<>
inline auto meta::registerName<PhraseComponent>() {
    return "PhraseComponent";
}

template<>
inline auto meta::registerMembers<PhraseComponent>() {
    return members(
        member("phrase", &PhraseComponent::m_phrase)
    );
}
