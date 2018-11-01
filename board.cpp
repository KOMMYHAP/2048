#include "board.hpp"

#include <iomanip>
#include <iostream>

#include <cassert>
#include <string>
#include <cstdlib> // std::rand

void Board::init(ResourceHandler const &handler, sf::Vector2f offset)
{
	/*	<! Загружаем все необходимые спрайты !> */
	int const nTotalSpritesNumber = 12;
	std::string textureName;
	for (int i = 0; i < nTotalSpritesNumber; ++i) {
		textureName = std::to_string(i);
		m_sprites.emplace_back(handler.texture(textureName));
	}
	m_background.setTexture(handler.texture("background"));

	/*	<! Размещаем плитки и устанавливаем им соответствующий индекс !> */
	{ // создаем "доску" - двумерный массив плиток
		size_t const w = 4, h = 4; // from settings
		m_tiles.resize(h);
		for (auto &row : m_tiles)
			row.resize(w);
	}
	// Запоминаем смещение доски от края:
	m_offset = offset;

	// Размер плитки установим равным размеру спрайта
	auto pTexture = m_sprites[0].getTexture();
	assert(pTexture != nullptr);
	sf::Vector2u tileSize = pTexture->getSize();
	assert(tileSize.x == tileSize.y);

	// Установим расстояние между плитками величиной в 1/16 ширины плитки
	unsigned int const frontierW = tileSize.x / 16;

	// Разместим все плитки на доске
	sf::Vector2f position;
	for (size_t h = 0; h < m_tiles.size(); ++h) {
		position.y = m_offset.y + (tileSize.y + frontierW) * h + frontierW;	
		for (size_t w = 0; w < m_tiles[h].size(); ++w) {
			position.x = m_offset.x + (tileSize.x + frontierW) * w + frontierW;
			m_tiles[h][w].setPosition(position);
		}
	}

	// Разместим фон на экране
	m_background.setPosition(m_offset);

	/*	<! Разместим все плитки в соответствии с новой игрой !> */
	reset();
}

void Board::reset()
{
	assert(m_sprites.size() == 12);

	for (auto &row : m_tiles)
		for (auto &tile : row) {
			tile.setValue(0);
			tile.setSprite(m_sprites[0]);
		}

	/* <! Генерируем две начальные плитки. !> */
	createTile();
	createTile();
}

/*	Проверяет наличие места на доске для размещение новой плитки. */
bool Board::isFull() const
{
	size_t counter = 0;
	for (auto const &row : m_tiles)
		for (auto const &tile : row)
			if (!tile.empty())
				++counter;

	size_t nTotalTilesNumber = 0;
	for (auto const &row : m_tiles)
		nTotalTilesNumber += row.size();

	return counter == nTotalTilesNumber;
}

/*	Добавляет плитку на доску в новой позиции */
void Board::createTile()
{
	// check pre conditions
	assert(m_tiles.size() > 0);
	assert(!isFull());

	// Определим с помощью рандома какую плитку будем менять
	Tile *pTile = nullptr;
	{
		size_t percent = rand() % 100;
		size_t number = 0;
		while (percent >= 15) {
			percent = rand() % 100;
			number += 1;
		}
		bool isGenerated = false;
		while (!isGenerated) {
			for (auto &row : m_tiles) {
				for (auto &tile : row) {
					if (number == 0) {
						if (tile.empty()) {
							pTile = &tile;
							isGenerated = true;
							break;
						}
						++number;
					}
					--number;
				}
				if (isGenerated)
					break;
			}
		}
	}
	assert(pTile != nullptr);
	

	// Сгенерируем значение для плитки: 50% - '2', 35% - '4', 15% - '8'
	unsigned int const percent = rand() % 100;
	unsigned int const value = (percent < 50) ? 2 : (percent < 85) ? 4 : 8;
	unsigned int const spriteIndex = ValueToSpriteIndex(value);

	std::cout << "Created tile " << value << ", sprite #" << spriteIndex << "." << std::endl;

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

void Board::update(sf::Event const &event/*, GameState &state*/)
{
	if (event.type != sf::Event::KeyPressed)
		return;

	// 	if (std::chrono::seconds::now() <= m_lastUpdate + m_timeout)
	//		return;
	//	m_lastUpdate = std::chono::seconds::now();

	auto dir = Direction::Up;
	switch (event.key.code) {
		case sf::Keyboard::Up:
			dir = Direction::Up;
			break;
		case sf::Keyboard::Down:
			dir = Direction::Down;
			break;
		case sf::Keyboard::Left:
			dir = Direction::Left;
			break;
		case sf::Keyboard::Right:
			dir = Direction::Right;
			break;
		default:
			return;
	}

	move(dir);
	
	if (isFull()) {
		std::cout << "Lost!" << std::endl;
		return;
	}
	
	createTile();

	auto PrintBoard = [this]() {
		for (auto const &row : m_tiles) {
			for (auto const &tile : row) {
				std::cout << std::setw(3) << tile.value() << " ";
			}
			std::cout << "\n";
		}
	};
	PrintBoard();
}

void Board::move(Direction dir)
{
	std::vector<std::vector<Tile *>> board;
	std::vector<Tile *> row;

	size_t const 
		maxH = m_tiles.size(),
		maxW = m_tiles[0].size();

	switch (dir)
	{
		case Direction::Left:
			for (size_t h = 0; h < maxH; ++h) {
				for (size_t w = 0; w < maxW; ++w)
					row.push_back(&m_tiles[h][w]);
				board.emplace_back(std::move(row));
			}
			break;
		case Direction::Right:
			for (size_t h = 0; h < maxH; ++h) {
				for (size_t w = maxW; w > 0; --w)
					row.push_back(&m_tiles[h][w - 1]);
				board.emplace_back(std::move(row));
			}
			break;
		case Direction::Up:
			for (size_t w = 0; w < maxW; ++w) {
				for (size_t h = 0; h < maxH; ++h)
					row.push_back(&m_tiles[h][w]);
				board.emplace_back(std::move(row));
			}
			break;
		case Direction::Down:
			for (size_t w = 0; w < maxW; ++w) {
				for (size_t h = maxH; h > 0; --h)
					row.push_back(&m_tiles[h - 1][w]);
				board.emplace_back(std::move(row));
			}
			break;
		default:
			assert(!"Unreachable code.");
			return;
	}

	for (auto &row : board)
		moveRowOnLeft(row);
}

unsigned int Board::ValueToSpriteIndex(unsigned int value) const
{
	if (value == 0)
		return 0;
	unsigned int index = 1;
	while (value > 2) {
		value >>= 1;
		++index;
	}
	assert(index < m_sprites.size());
	return index;
}

void Board::moveRowOnLeft(std::vector<Tile *> &row)
{
	size_t place = 0, x = 1;
	for (; x < row.size(); ++x) {
		assert(place < x);
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