#pragma once
#include "../ComponentTemplate.h"

class PhraseComponent : public ComponentTemplate<PhraseComponent> {
    friend auto meta::registerMembers<PhraseComponent>();
public:
    PhraseComponent(EntityId id);
    PhraseComponent(EntityId id, const sol::table& table);
    ~PhraseComponent();

    std::string getPhrase() const;
    void setPhrase(const std::string& phrase);

    void drawComponentInspector() override;

    static void exposeToLua();

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
