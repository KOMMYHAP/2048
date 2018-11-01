#include <cassert>
#include <iostream>
#include <exception>

#include "game.hpp"

Game::Game()
{
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
	// Было бы неплохо размер экрана не хардкодить.
	m_window.create(sf::VideoMode(128 * 4 + 16 * 3 + 32, 128 * 4 + 16 * 3 + 80), "2048");

	// А также убрать привязку фпс и скорости обновления доски
	m_window.setFramerateLimit(10);

	// Если вынести настройки в отдельный класс, то 
	// можно будет конфигурировать хоть через файл
	// m_settings.init();
	// m_resourceHandler.init(m_settings);
	m_resourceHandler.init();
	// m_board.init(m_resouceHandler, m_settings);
	m_board.init(m_resourceHandler, {16, 64});
//	m_score.init(m_resourceHandler);
}

void Game::run()
{
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