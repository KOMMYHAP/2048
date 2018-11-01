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
	auto res = m_textures.insert(std::make_pair(alias, texture));
	if (!res.second) {
		throw std::logic_error(std::string("Texture \"") + path + "\" was already loaded");
	}
}

void ResourceHandler::init()
{
	std::string const dir = "resources/";
	// Загружаем текструры для плиток:
	loadTexture("0", dir + "0");
	loadTexture("1", dir + "2");
	loadTexture("2", dir + "4");
	loadTexture("3", dir + "8");
	loadTexture("4", dir + "16");
	loadTexture("5", dir + "32");
	loadTexture("6", dir + "64");
	loadTexture("7", dir + "128");
	loadTexture("8", dir + "256");
	loadTexture("9", dir + "512");
	loadTexture("10", dir + "1024");
	loadTexture("11", dir + "2048");

	// Загружаем текструры для заднего фона:
	loadTexture("background", dir + "base");
}

sf::Texture const & ResourceHandler::texture(std::string const &name) const
{
	auto it = m_textures.find(name);
	if (it == m_textures.end()) {
		throw std::runtime_error(std::string("Texture \"") + name + "\" not found");
	}
	return it->second;
}