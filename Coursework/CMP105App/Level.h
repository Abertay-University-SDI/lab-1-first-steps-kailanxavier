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

	// Default variables for level class.
	float m_playerSize = 10.0f;
	float m_polyCount = 16.0f;
	sf::CircleShape m_player;

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
	void spawnFoodRand(sf::CircleShape food);
};