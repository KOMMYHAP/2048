#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include "resourceHandler.hpp"
#include "board.hpp"

class Game
{
public:
	Game();

	Game(Game const &) = delete;
	Game& operator=(Game const &) = delete;

	~Game() = default;

	/*	Запуск основного игрового цикла */
	void run();

private:
	/* 	Инициализация ресурсов игры. Выполняется единожды для объекта. */
	void init();

	/*	Сброс состояния игры для начала новой. */
	void reset();

	void requestNewGame();

private:
	bool m_bGameInProcess;

	/*	Менеджер технических ресурсов, таких как шрифты, текстуры и звуки.
		Время жизни должно быть больше, чем у объектов его использующих. */
	ResourceHandler m_resourceHandler;

	/*	Доска, отвечающая за размещение, перемещение и отрисовку плиток. */
	Board m_board;

	/*	Окно, на которое всё будет отрисовано. */
	sf::RenderWindow m_window;
};

#endif // GAME_HPP