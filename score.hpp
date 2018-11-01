#ifndef SCORE_HPP
#define SCORE_HPP

#include <SFML/Graphics>

#include "resourceHandler.hpp"

class Score
{
public:
	void init(ResourceHandler const &);

	/*	Сброс счетчика очков до нуля */
	void reset();

	/*	Геттер и сеттер для очков */
	unsigned int get() const;
	void set(unsigned int);

	/*	Увеличивает количество очков на заданную величину */
	void inc(unsigned int);

	void draw(sf::RenderWindow &);
private:
	/*	Количество очков на данный момент */
	unsigned int m_value;

	/*	Отображаемый текст */
	sf::Text m_text;

	/*	Отображаемый спрайт */
	sf::Sprite m_sprite;
};

#endif