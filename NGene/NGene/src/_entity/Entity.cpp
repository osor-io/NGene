#include "Entity.h"

Entity::~Entity(){
}

std::string Entity::getType() const {
    return m_type;
}

void Entity::setType(const std::string& type) {
    m_type = type;
}


void Entity::setType(std::string&& type) {
    m_type = std::move(type);
}