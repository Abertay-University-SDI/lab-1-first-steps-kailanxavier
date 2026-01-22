#pragma once

#include "Framework/BaseLevel.h"

class Level : BaseLevel{
public:
	Level(sf::RenderWindow& window, Input& input);
	~Level() override = default;

	void handleInput(float dt) override;
	void update(float dt);
	void render();

private:
	// Default functions for rendering to the screen.
	void wrapSnake();
	void checkCollision();
	void addSnakeSegment();

	// Default variables for level class.
	float m_baseRadius = 10.f;
	float m_minRadius = 2.f;
	float m_spacing = 14.f;
	float m_polyCount = 16.0f;
	std::vector<sf::CircleShape> m_player;

	sf::Vector2f m_inputVector = { 0.0f, 0.0f };
	float m_playerSpeed = 400.f;

private:
	enum MoveDirection {
		NONE,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	MoveDirection m_moveDir;
	void setMoveDirection(MoveDirection moveDir);

private:
	sf::CircleShape m_food;
	bool m_spawnedFood = false;
	sf::CircleShape spawnFoodRand();
	int m_foodEaten = 0;

private:
	// leaving this here for now
	float vector2Dist(sf::Vector2f a, sf::Vector2f b);
	sf::Vector2f normalise(sf::Vector2f a);
};