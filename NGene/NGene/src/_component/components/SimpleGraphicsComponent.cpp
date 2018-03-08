#include "SimpleGraphicsComponent.h"


SimpleGraphicsComponent::SimpleGraphicsComponent(){
}


SimpleGraphicsComponent::~SimpleGraphicsComponent(){
}


std::string SimpleGraphicsComponent::getFilename() const {
    return m_filename;
}


void SimpleGraphicsComponent::setFilename(const std::string& filename) {
    m_filename = filename;
}


void SimpleGraphicsComponent::setFilename(std::string&& filename) {
    m_filename = std::move(filename);
}