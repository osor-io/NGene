#pragma once
#include "../Component.h"
#include <string>
#include <meta.h>

class SimplePhraseComponent : public Component {
    friend auto meta::registerMembers<SimplePhraseComponent>();
public:
    SimplePhraseComponent();
    ~SimplePhraseComponent();

    std::string getPhrase() const;
    void setPhrase(const std::string& phrase);
    void setPhrase(std::string&& phrase);

private:
    std::string m_phrase;
};

template<>
inline auto meta::registerName<SimplePhraseComponent>() {
    return "SimplePhraseComponent";
}

template<>
inline auto meta::registerMembers<SimplePhraseComponent>() {
    return members(
        member("Phrase", &SimplePhraseComponent::m_phrase)
    );
}
