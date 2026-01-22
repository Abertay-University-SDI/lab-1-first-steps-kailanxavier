#include "Level.h"

Level::Level(sf::RenderWindow& hwnd, Input& in) :
	BaseLevel(hwnd, in)
{
	// initialise game objects
	m_moveDir = MoveDirection::NONE;
	m_spawnedFood = false;

	sf::CircleShape head(m_baseRadius, m_polyCount);
	head.setFillColor(sf::Color::Green);
	head.setPosition({ 300, 300 });

	m_player.push_back(head);
}

// handle user input
void Level::handleInput(float dt)
{
	if (m_input.isLeftMousePressed())
	{
		std::cout << "left mouse pressed" << std::endl;
	}

	if (m_input.isKeyDown(sf::Keyboard::Scancode::W))
	{
		m_moveDir = MoveDirection::UP;
		setMoveDirection(m_moveDir);
	}
	if (m_input.isKeyDown(sf::Keyboard::Scancode::S))
	{
		m_moveDir = MoveDirection::DOWN;
		setMoveDirection(m_moveDir);
	}
	if (m_input.isKeyDown(sf::Keyboard::Scancode::D))
	{
		m_moveDir = MoveDirection::RIGHT;
		setMoveDirection(m_moveDir);
	}
	if (m_input.isKeyDown(sf::Keyboard::Scancode::A))
	{
		m_moveDir = MoveDirection::LEFT;
		setMoveDirection(m_moveDir);
	}
	if (m_input.isKeyDown(sf::Keyboard::Scancode::Space))
	{
		m_moveDir = MoveDirection::NONE;
		setMoveDirection(m_moveDir);
	}
}

// Update game objects
void Level::update(float dt)
{
	if (m_player.empty()) return;

	m_player[0].move(m_inputVector * m_playerSpeed * dt);

	for (size_t i = 1; i < m_player.size(); i++)
	{
		sf::Vector2 prevPos = m_player[i - 1].getPosition();
		sf::Vector2 currentPos = m_player[i].getPosition();

		sf::Vector2f delta = prevPos - currentPos;
		float dist = std::sqrt(std::pow(delta.x, 2) + std::pow(delta.y, 2));

		if (dist > m_spacing)
		{
			sf::Vector2f dir = normalise(delta);
			m_player[i].setPosition(prevPos - dir * m_spacing);
		}
	}

	wrapSnake();

	// spawn food
	if (!m_spawnedFood)
		m_food = spawnFoodRand();

	checkCollision();
}

// Render level
void Level::render()
{
	beginDraw();

	// draw each segment
	for (auto& p : m_player)
		m_window.draw(p);

	if (m_spawnedFood)
		m_window.draw(m_food);

	endDraw();
}

void Level::wrapSnake()
{
	sf::Vector2f windowSize = static_cast<sf::Vector2f>(m_window.getSize());
	float margin = 10.f;

	sf::Vector2f snakeHead = m_player[0].getPosition();

	// wrap x
	if (snakeHead.x > windowSize.x + margin)
		snakeHead.x = -margin;
	else if (snakeHead.x < -margin)
		snakeHead.x = windowSize.x + margin;

	// wrap y
	if (snakeHead.y > windowSize.y + margin)
	snakeHead.y = -margin;
	else if (snakeHead.y < -margin)
		snakeHead.y = windowSize.y + margin;

	m_player[0].setPosition(snakeHead);
}

void Level::checkCollision()
{
	// check collisions
	if (vector2Dist(m_food.getPosition(), m_player[0].getPosition()) < m_food.getRadius() + m_player[0].getRadius())
	{
		std::cout << "we collided" << std::endl;
		m_spawnedFood = false;
		m_foodEaten++;

		addSnakeSegment();
	}
}

void Level::addSnakeSegment()
{
	sf::CircleShape& tail = m_player.back();

	float t = (float)m_player.size();
	float rad = std::max(m_baseRadius - t * 0.8f, m_minRadius);

	sf::Vector2f spawnPos = tail.getPosition();

	if (m_player.size() > 1)
	{
		sf::Vector2f dir = normalise(tail.getPosition() - m_player[m_player.size() - 2].getPosition());
		spawnPos += dir * m_spacing;
	}

	sf::CircleShape seg(rad, m_polyCount);
	seg.setFillColor(sf::Color::Green);
	seg.setPosition(spawnPos);
	seg.setOrigin({ 0.5f, 0.5f });

	m_player.push_back(seg);
}

void Level::setMoveDirection(MoveDirection moveDir)
{
	switch (moveDir)
	{
	case MoveDirection::UP:
		m_inputVector.y = -1.0f;
		m_inputVector.x = 0.0f;
		break;
	case MoveDirection::DOWN:
		m_inputVector.y = 1.0f;
		m_inputVector.x = 0.0f;
		break;
	case MoveDirection::RIGHT:
		m_inputVector.x = 1.0f;
		m_inputVector.y = 0.0f;
		break;
	case MoveDirection::LEFT:
		m_inputVector.x = -1.0f;
		m_inputVector.y = 0.0f;
		break;
	case MoveDirection::NONE:
		m_inputVector.x = 0.0f;
		m_inputVector.y = 0.0f;
		break;
	}
}

sf::CircleShape Level::spawnFoodRand()
{
	m_spawnedFood = true;
	float margin = 150.f;

	int windowX = (int)m_window.getSize().x - margin;
	int windowY = (int)m_window.getSize().y - margin;

	float spawnX = (float)(rand() % windowX);
	float spawnY = (float(rand() % windowY));

	sf::Vector2f spawnPos = { spawnX, spawnY };

	sf::CircleShape food(7.f, 8);
	food.setFillColor(sf::Color::Red);
	food.setPosition(spawnPos);

	std::cout << "Spawned food at: " << food.getPosition().x << ", " << food.getPosition().y << std::endl;
	return food;
}

float Level::vector2Dist(sf::Vector2f a, sf::Vector2f b)
{
	float distance = std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
	return distance;
}

sf::Vector2f Level::normalise(sf::Vector2f a)
{
	float len = std::sqrt(std::pow(a.x, 2) + std::pow(a.y, 2));
	if (len == 0.f) return { 0.f, 0.f };
	return a / len;
}

