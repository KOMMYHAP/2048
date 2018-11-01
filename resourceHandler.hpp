#ifndef RESOURCE_HANDLER
#define RESOURCE_HANDLER

#include <SFML/Graphics.hpp>

#include <string>
#include <map>

class ResourceHandler
{
public:
	/*	Инициализация (загрузка) текстур и шрифтов и сохранение их в памяти.
		Если какой-то из объектов не найден в процессе инициализации,
		то будет брошено исключение std::runtime_error. */
	void init();

	/*	Запрос текстуры/шрифта по заданному имени. 
		Если нужного объекта не было найдено - выбрасывается исключение.*/
	sf::Texture const & texture(std::string const &alias) const;
	sf::Font const & font(std::string const &alias) const;

private:
	void loadTexture(std::string const &alias, std::string const &path);
	void loadFont(std::string const &alias, std::string const &path);

private:
	std::map<sf::String, sf::Texture> m_textures;
	std::map<sf::String, sf::Font> m_fonts;
};

#endif