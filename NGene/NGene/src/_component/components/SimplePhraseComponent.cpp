#include "SimplePhraseComponent.h"

#include <iostream>


SimplePhraseComponent::SimplePhraseComponent(){
    std::cout << "created: " << (void*)this << std::endl;
}


SimplePhraseComponent::~SimplePhraseComponent(){
    std::cout << "deleted: " << (void*)this << std::endl;
}


std::string SimplePhraseComponent::getPhrase() const {
    return m_phrase;
}


void SimplePhraseComponent::setPhrase(const std::string& phrase) {
    m_phrase = phrase;
}


void SimplePhraseComponent::setPhrase(std::string&& phrase) {
    m_phrase = std::move(phrase);
}