#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>

class Tile
{
public:
	Tile() = default;

	Tile(sf::Vector2f const &, sf::Sprite const &, int);

	Tile(Tile const &) = default;

	Tile& operator=(Tile const &) = default;

	~Tile() = default;

	/*	Если плитка не содержит числа, то возвращается true. */
	bool empty() const;

	/*	Устнавливает на плитке новое значение и обновляет спрайт. */
	void setValue(int);

	void setSprite(sf::Sprite const &);

	/*	Возвращает текущее значение плитки. */
	int value() const;

	/*	Устанавливает позицию спрайта относительно левого верхнего края. */
	void setPosition(sf::Vector2f const &);

	/*	Возвращает позицию плитки относительно левого верхнего края. */
	sf::Vector2f position() const;

	/*	Отрисовывает спрайт */
	void draw(sf::RenderWindow &) const;
	
private:
	/*	Позиция плитки на экране */
	sf::Vector2f m_position;

	/*	Отображаемый спрайт */
	sf::Sprite m_sprite;
	
	/* число, отображаемое на плитке - 0, 2, 4,.. 2048 */
	int m_nValue;
};

#endif