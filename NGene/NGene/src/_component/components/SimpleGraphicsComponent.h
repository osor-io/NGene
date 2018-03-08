#pragma once
#include "../Component.h"
#include <string>
#include <meta.h>

class SimpleGraphicsComponent : public Component {
    friend auto meta::registerMembers<SimpleGraphicsComponent>();
public:
    SimpleGraphicsComponent();
    ~SimpleGraphicsComponent();

    std::string getFilename() const;
    void setFilename(const std::string& filename);
    void setFilename(std::string&& filename);

private:
    std::string m_filename;
};

template<>
inline auto meta::registerName<SimpleGraphicsComponent>() {
    return "SimpleGraphicsComponent";
}

template<>
inline auto meta::registerMembers<SimpleGraphicsComponent>() {
    return members(
        member("File Name", &SimpleGraphicsComponent::m_filename)
    );
}

