#pragma once
#include "../Component.h"
#include <string>
#include <meta.h>
#include <sol.hpp>

class PhraseComponent : public Component {
    friend auto meta::registerMembers<PhraseComponent>();
public:
    PhraseComponent();
    PhraseComponent(const sol::table& table);
    ~PhraseComponent();

    static void exposeToLua();

    std::string getPhrase() const;
    void setPhrase(const std::string& phrase);

private:
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
