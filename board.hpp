#ifndef BOARD_HPP
#define BOARD_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <vector>

#include "resourceHandler.hpp"
#include "tile.hpp"


class Board
{
public:
	/*	Инициализация должна происходить единожды за всё время работы. */
	void init(ResourceHandler const &, sf::Vector2f offset);

	/*	Доска сбрасывается для новой игры. */
	void reset();

	bool isFull() const;

	/*	Если событие содержит нужную нам клавишу, то передаём управление в 
		функцию move. */
	void update(sf::Event const &/*, GameState &state*/);

	/*	Отрисовка доски и плиток на экране */
	void draw(sf::RenderWindow &) const;

private:
	/*	Добавляет плитку на доску в новой позиции.
		Значение плитки будет {2, 4, 8} с вероятностью соответственно {50%, 35%, 15%}
		(!) Требуется наличие хотя бы одной свободной позиции. */
	void createTile();

	enum class Direction {
		Left, Right, Up, Down
	};
	
	/*	Производит сдвиг всех плиток в соответсвующем направлении. */
	void move(Direction);

	/*	Производит сдвиг всех плиток в массиве, сформированном 
		по следующему правилу: 
		... */
	void moveRowOnLeft(std::vector<Tile *> &);

	unsigned int ValueToSpriteIndex(unsigned int) const;

private:
	sf::Vector2f m_offset;
	sf::Sprite m_background;
	std::vector<sf::Sprite> m_sprites;
	std::vector<std::vector<Tile>> m_tiles;
};

#endif