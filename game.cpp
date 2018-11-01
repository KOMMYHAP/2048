#include <cassert>
#include <iostream>
#include <exception>

#include "game.hpp"

Game::Game()
{
	m_bIsInitialized = false;

	try {
		init();	
	} catch (std::runtime_error const &err) {
		std::cerr << err.what() << std::endl;
	} catch (std::logic_error const &err) {
		std::cerr << " <!> " << err.what() << std::endl;
	}
}

void Game::init()
{
	std::cout << "Инициализация игры... ";

	// Было бы неплохо размер экрана не хардкодить.
	unsigned int const
		tileW = 128,
		tileH = 128,
		tileSpace = 16,
		nTileRows = 4,
		nTileColumns = 4,
		offsetLeft = 16,
		offsetRight = 16,
		offsetTop = 64,
		offsetBottom = 16;

	std::string const
		title = "2048 by White";

	unsigned int const
		screenW = 
			tileW * nTileRows + 
			tileSpace * (nTileRows - 1) + offsetLeft + offsetRight,
		screenH = 
			tileH * nTileColumns + 
			tileSpace * (nTileColumns - 1) + offsetBottom + offsetTop;

	m_window.create(sf::VideoMode(screenW, screenH), title);

	// А также убрать привязку фпс и скорости обновления доски
	m_window.setFramerateLimit(10);

	// Если вынести настройки в отдельный класс, то 
	// можно будет конфигурировать хоть через файл
	// m_settings.init();
	// m_resourceHandler.init(m_settings);
	m_resourceHandler.init();
	// m_board.init(m_resouceHandler, m_settings);
	m_board.init(m_resourceHandler, {offsetLeft, offsetTop});
//	m_score.init(m_resourceHandler);

	m_bIsInitialized = true;
	std::cout << "Ок." << std::endl;
}

void Game::run()
{
	if (!m_bIsInitialized)
		return;

	while (m_window.isOpen()) {
		sf::Event event;
		while (m_window.pollEvent(event)) {
			if (event.type == sf::Event::Closed
				|| (event.type == sf::Event::KeyPressed 
				&& event.key.code == sf::Keyboard::Escape)) 
				m_window.close();
		}
		if (m_window.isOpen()) {
			m_board.update(event);

			m_window.clear();
			m_board.draw(m_window);
//			m_score.draw(m_window);
			m_window.display();
		}
	}
}