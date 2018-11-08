#include "board.hpp"

#include <string>
#include <cstdlib> // std::rand

void Board::init(ResourceHandler const &handler)
{
	/*	<! Загружаем все необходимые спрайты !> */
	for (int i = 0; i < 12; ++i) {
		m_sprites.push_back(sf::Sprite(handler.texture(std::to_string(i))));
	}
	m_background.setTexture(handler.texture("background"));

	/*	<! Размещаем плитки и устанавливаем им соответствующий индекс !> */
	m_tiles.resize(4);
	for (auto &row : m_tiles)
		row.resize(4);

	// Разместим все плитки на доске
	sf::Vector2f position;
	for (int h = 0; h < 4; ++h) {
		position.y = (128 + 8) * h + 8;
		for (int w = 0; w < 4; ++w) {
			position.x = (128 + 8) * w + 8;
			m_tiles[h][w].setPosition(position);
		}
	}

	/*	<! Разместим все плитки в соответствии с новой игрой !> */
	reset();
}

void Board::reset()
{
	m_bBlocked = false;
	m_bGameIsOver = false;

	for (auto &row : m_tiles)
		for (auto &tile : row) {
			tile.setValue(0);
			tile.setSprite(m_sprites[0]);
		}

	/* <! Генерируем две начальные плитки. !> */
	createTile();
	createTile();
}

bool Board::isGameOver() const
{
	return m_bGameIsOver;
}

/*	Добавляет плитку на доску в новой позиции */
void Board::createTile()
{
	// Определим с помощью рандома какую плитку будем менять
	int index = rand() % 16;
	while (!m_tiles[index / 4][index % 4].empty())
		index = rand() % 16;

	Tile *pTile = &m_tiles[index / 4][index % 4];
	
	// Сгенерируем значение для плитки: 50% - '2', 35% - '4', 15% - '8'
	int percent = rand() % 100;
	int value = (percent < 50) ? 2 : (percent < 85) ? 4 : 8;
	int spriteIndex = ValueToSpriteIndex(value);

	// Обновим соответсвующую плитку в нашем массиве
	pTile->setValue(value);
	pTile->setSprite(m_sprites[spriteIndex]);
}

void Board::draw(sf::RenderWindow &window) const
{
	window.draw(m_background);
	for (auto const &row : m_tiles)
		for (auto const &tile : row)
			tile.draw(window);
}

void Board::update(sf::Event const &event)
{
	if (isGameOver())
		return;

	switch (event.type) {
		case sf::Event::KeyPressed:
			if (m_bBlocked)
				return;
			m_bBlocked = true;
			break;
		case sf::Event::KeyReleased:
			m_bBlocked = false;
		default:
			return;
	}

	const auto code = event.key.code;
	if (code != sf::Keyboard::Left
		&& code != sf::Keyboard::Right
		&& code != sf::Keyboard::Up
		&& code != sf::Keyboard::Down) {
		return;
	}

	move(code);

	createTile();

	checkGameOver();
}

void Board::checkGameOver()
{
	if (!isFull())
		return;
	
	m_bGameIsOver = false;

	auto const cachedTiles = m_tiles;
	bool deadlock = true;

	move(sf::Keyboard::Left);
	deadlock = isFull();
	m_tiles = cachedTiles;
	if (!deadlock)
		return;

	move(sf::Keyboard::Right);
	deadlock = isFull();
	m_tiles = cachedTiles;
	if (!deadlock)
		return;

	move(sf::Keyboard::Up);
	deadlock = isFull();
	m_tiles = cachedTiles;
	if (!deadlock)
		return;

	move(sf::Keyboard::Down);
	deadlock = isFull();
	m_tiles = cachedTiles;
	if (!deadlock)
		return;

	m_bGameIsOver = true;
}

bool Board::isFull() const
{
	int counter = 0;
	for (int h = 0; h < 4; ++h)
		for (int w = 0; w < 4; ++w) {
			if (m_tiles[h][w].empty())
				break;
			counter += 1;
		}
	return counter == 16;
}

void Board::move(sf::Keyboard::Key dir)
{
	std::vector<std::vector<Tile *>> board;
	std::vector<Tile *> row;

	switch (dir)
	{
		case sf::Keyboard::Left:
			for (size_t h = 0; h < 4; ++h) {
				for (size_t w = 0; w < 4; ++w)
					row.push_back(&m_tiles[h][w]);
				board.emplace_back(std::move(row));
			}
			break;
		case sf::Keyboard::Right:
			for (size_t h = 0; h < 4; ++h) {
				for (size_t w = 4; w > 0; --w)
					row.push_back(&m_tiles[h][w - 1]);
				board.emplace_back(std::move(row));
			}
			break;
		case sf::Keyboard::Up:
			for (size_t w = 0; w < 4; ++w) {
				for (size_t h = 0; h < 4; ++h)
					row.push_back(&m_tiles[h][w]);
				board.emplace_back(std::move(row));
			}
			break;
		case sf::Keyboard::Down:
			for (size_t w = 0; w < 4; ++w) {
				for (size_t h = 4; h > 0; --h)
					row.push_back(&m_tiles[h - 1][w]);
				board.emplace_back(std::move(row));
			}
			break;
	}

	for (auto &row : board)
		moveRowOnLeft(row);
}

unsigned int Board::ValueToSpriteIndex(int value) const
{
	if (value == 0)
		return 0;

	int index = 1;
	while (value > 2) {
		value = value / 2;
		++index;
	}
	return index;
}

void Board::moveRowOnLeft(std::vector<Tile *> &row)
{
	int place = 0, x = 1;
	for (; x < row.size(); ++x) {
		if (row[x]->empty())
			continue;

		auto const valueP = row[place]->value();
		auto const valueX = row[x]->value();
		if (valueP == valueX) {
			row[place]->setValue(valueP * 2);
			row[place]->setSprite(m_sprites[ValueToSpriteIndex(valueP * 2)]);
			row[x]->setValue(0);
			row[x]->setSprite(m_sprites[0]);
			place++;
		} else { // valueP != valueX
			if (place + 1 == x && !row[place]->empty()) {
				place += 1;
				continue;
			}
			if (!row[place]->empty())
				place++;
			row[place]->setValue(valueX);
			row[place]->setSprite(m_sprites[ValueToSpriteIndex(valueX)]);
			row[x]->setValue(0);
			row[x]->setSprite(m_sprites[0]);
			place++;
		}
	}
}