#pragma once
#include "../Component.h"
#include <string>

class SimplePhraseComponent : public Component {
public:
	SimplePhraseComponent();
	~SimplePhraseComponent();

	std::string getPhrase() const;
	void setPhrase(const std::string& phrase);
	void setPhrase(std::string&& phrase);

private:
	std::string m_phrase;
};

