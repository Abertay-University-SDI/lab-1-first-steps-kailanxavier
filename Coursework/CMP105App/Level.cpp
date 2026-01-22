#include "Level.h"

Level::Level(sf::RenderWindow& hwnd, Input& in) :
	BaseLevel(hwnd, in)
{
	// initialise game objects
	m_player = sf::CircleShape(m_playerSize, m_polyCount);
	m_player.setFillColor(sf::Color::Green);
	m_player.setPosition({ 300, 300 });

	m_moveDir = MoveDirection::NONE;
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
	m_player.setPosition(m_player.getPosition() + (m_inputVector * m_playerSpeed * dt));
	sf::Vector2f windowSize = static_cast<sf::Vector2f>(m_window.getSize());
	float margin = 10.f;

	// wrap right
	if (m_player.getPosition().x > windowSize.x + margin)
	{
		m_player.setPosition({ 0.f, m_player.getPosition().y });
	}
	// wrap left
	else if (m_player.getPosition().x < -margin)
	{
		m_player.setPosition({ windowSize.x + margin, m_player.getPosition().y });
	}

	// wrap bottom
	if (m_player.getPosition().y > windowSize.y + margin)
	{
		m_player.setPosition({ m_player.getPosition().x, -margin });
	}
	// wrap top
	else if (m_player.getPosition().y < -margin)
	{
		m_player.setPosition({ m_player.getPosition().x, windowSize.y + margin });
	}

	if (!m_spawnedFood)
	{
		spawnFoodRand(m_food);
	}
}

// Render level
void Level::render()
{
	beginDraw();
	m_window.draw(m_player);

	endDraw();
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

void Level::spawnFoodRand(sf::CircleShape food)
{
	m_spawnedFood = true;
	food.setFillColor(sf::Color::Red);
	food.setRadius(5.0f);
	int windowX = (int)m_window.getSize().x;
	int windowY = (int)m_window.getSize().y;

	sf::Vector2f spawnPos = { (float)(rand() % windowX), (float)(rand() % windowY) };
	food.setPosition(spawnPos);
	std::cout << "Spawned food at: " << windowX << ", " << windowY << std::endl;
}

