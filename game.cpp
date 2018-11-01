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
	m_window.create(sf::VideoMode(128 * 4 + 8 * 5, 128 * 4 + 8 * 5), "2048");
	m_window.setFramerateLimit(60);

	m_resourceHandler.init();
	m_board.init(m_resourceHandler);
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
			m_window.display();
		}
	}
}