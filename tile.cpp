#include "tile.hpp"

Tile::Tile(sf::Vector2f const &position, sf::Sprite const &sprite, int value)
{
	setPosition(position);
	setValue(value);
	setSprite(sprite);
}

bool Tile::empty() const
{
	return m_nValue == 0;
}

void Tile::setValue(int value)
{
	m_nValue = value;
}

int Tile::value() const
{
	return m_nValue;
}

void Tile::setPosition(sf::Vector2f const &position)
{
	m_position = position;
}

sf::Vector2f Tile::position() const
{
	return m_position;
}

void Tile::setSprite(sf::Sprite const &sprite)
{
	m_sprite = sprite;
	m_sprite.setPosition(m_position);
}

void Tile::draw(sf::RenderWindow &window) const
{
	window.draw(m_sprite);
}