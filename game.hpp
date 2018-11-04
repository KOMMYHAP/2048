#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include "resourceHandler.hpp"
#include "board.hpp"
//#include "score.hpp"

class Game
{
public:
	Game();

	Game(Game const &) = delete;
	Game& operator=(Game const &) = delete;

	~Game() = default;

	void run();

private:
	void init();

private:
	/*	Менеджер технических ресурсов, таких как шрифты, текстуры и звуки.
		Время жизни должно быть больше, чем у объектов его использующих. */
	ResourceHandler m_resourceHandler;

	/*	Доска, отвечающая за размещение, перемещение и отрисовку плиток. */
	Board m_board;

	/*	Окно, на которое всё будет отрисовано. */
	sf::RenderWindow m_window;
};

#endif // GAME_HPP