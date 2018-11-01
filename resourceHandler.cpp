#include "resourceHandler.hpp"

#include <vector>
#include <utility>
#include <string>
#include <exception>

void ResourceHandler::loadTexture(std::string const &alias, std::string const &path)
{
	sf::Texture texture;
	if (!texture.loadFromFile(path)) {
		throw std::runtime_error(std::string("Cannot load texture \"") + path + "\"");
	}
	auto [it, success] = m_textures.emplace(alias, std::move(texture));
	if (!success) {
		throw std::logic_error(std::string("Texture \"") + path + "\" was already loaded");
	}
}

void ResourceHandler::loadFont(std::string const &alias, std::string const &path)
{

}

void ResourceHandler::init()
{
	/*	<! Загрузка текстур из папки resources !> */
	// Определим функции для определения формата
	// имени файла и псевдонима исходя из номера плитки.
	auto format_filename = [](unsigned int i) {
		return i != 0 ? std::to_string(1 << i) : std::to_string(0);
	};
	auto format_name = [](unsigned int i) {
		return std::to_string(i);
	};

	// Загружаем текструры для плиток:
	int const nTotalTexturesNumber = 12;
	std::string const dir = "resources/";
	for (int i = 0; i < nTotalTexturesNumber; ++i) {
		loadTexture(format_name(i), dir + format_filename(i));
	}

	// Загружаем текструры для заднего фона:
	loadTexture("background", "resources/base");

	/* <! Загрузка шрифтов !> */
	// ...
}

sf::Texture const & ResourceHandler::texture(std::string const &name) const
{
	auto it = m_textures.find(name);
	if (it == m_textures.end()) {
		throw std::runtime_error(std::string("Texture \"") + name + "\" not found");
	}
	return it->second;
}

sf::Font const & ResourceHandler::font(std::string const &name) const
{
	auto it = m_fonts.find(name);
	if (it == m_fonts.end()) {
		throw std::runtime_error(std::string("Font \"") + name + "\" not found");
	}
	return it->second;
}