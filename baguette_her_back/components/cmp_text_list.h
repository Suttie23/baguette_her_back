#pragma once
#include <SFML/Graphics/Text.hpp>
#include <ecm.h>

class TextListComponent : public Component {
public:
	bool changed;
	TextListComponent() = delete;
	explicit TextListComponent(Entity* p, const std::vector<std::string> str);
	
	void update(double dt) override;

	void render() override;

	~TextListComponent() override = default;
	void addText(std::string& str);
	sf::Text getText();
	void setDefault(std::string str);
protected:
	std::shared_ptr<sf::Font> _font;
	std::string _string;
	sf::Text _text;
	std::vector<sf::Text> _text_list;
private:
	unsigned int _text_list_index;
};
