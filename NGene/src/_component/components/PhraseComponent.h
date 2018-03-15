#pragma once
#include "../ComponentTemplate.h"

namespace config {
    constexpr auto max_phrase_size = 256;
}

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

    void draw_component_inspector() override;

    static void expose_to_lua();

    std::string m_phrase;
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
