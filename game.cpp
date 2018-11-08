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
	m_bGameInProcess = true;
	
	m_window.create(sf::VideoMode(128 * 4 + 8 * 5, 128 * 4 + 8 * 5), "2048");
	m_window.setFramerateLimit(60);

	m_resourceHandler.init();
	m_board.init(m_resourceHandler);
}

void Game::run()
{
	sf::Event event;
	while (m_bGameInProcess) {
		while (m_window.pollEvent(event)) {
			m_board.update(event);
			m_bGameInProcess = !m_board.isGameOver();
		}
		if (m_bGameInProcess) { 
			m_window.clear();
			m_board.draw(m_window);
			m_window.display();
		}
	}
	std::cout << "Game over." << std::endl;
	m_window.close();
}