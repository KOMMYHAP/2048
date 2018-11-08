#include <cassert>
#include <iostream>
#include <exception>
#include <limits>

#include "game.hpp"

Game::Game()
{
	try {
		init();
		reset();
	} catch (std::runtime_error const &err) {
		std::cerr << err.what() << std::endl;
	} catch (std::logic_error const &err) {
		std::cerr << " <!> " << err.what() << std::endl;
	}
}

void Game::init()
{	
	m_resourceHandler.init();
	m_board.init(m_resourceHandler);

	m_window.create(sf::VideoMode(128 * 4 + 8 * 5, 128 * 4 + 8 * 5), "2048");
	m_window.setFramerateLimit(60);
}

void Game::reset()
{
	m_bGameInProcess = true;
	m_board.reset();
}

void Game::requestNewGame()
{
	std::cout << "Game over with " << m_board.score() << " score points." << std::endl;
	bool isAnswer = false;
	while (!isAnswer) {
		std::cout << "Start a new game? (y/n): ";
		int c = std::cin.get();
		isAnswer = c == 'n' || c == 'y';
		if (c == 'y')
			reset();

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin.clear();
	}
}

void Game::run()
{
	sf::Event event;
	while (m_bGameInProcess) {
		while (m_window.pollEvent(event)) {
			if (event.type == sf::Event::Closed 
					|| event.type == sf::Event::KeyPressed
					&& event.key.code == sf::Keyboard::Escape) {
				m_bGameInProcess = false;
			}
			m_board.update(event);
			m_bGameInProcess &= !m_board.isGameOver();
			if (!m_bGameInProcess)
				break;
		}
		if (m_bGameInProcess) { 
			m_window.clear();
			m_board.draw(m_window);
			m_window.display();
		} else {
			requestNewGame();
		}
	}
	m_window.close();
}