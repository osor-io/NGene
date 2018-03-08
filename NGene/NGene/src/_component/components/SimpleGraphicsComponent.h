#pragma once
#include "../Component.h"
#include <string>

class SimpleGraphicsComponent : public Component {
public:
	SimpleGraphicsComponent();
	~SimpleGraphicsComponent();

	std::string getFilename() const;
	void setFilename(const std::string& filename);
	void setFilename(std::string&& filename);

private:
	std::string m_filename;
};

