#pragma once

class Game
{
private:
	enum STATE
	{
		MAIN_MENU,
		HOW_TO_PLAY,
		GAME_LORE,
		MAZE,
	};

	Game();
	static inline Game* Instance = nullptr;

	unsigned int gameState = MAIN_MENU;

	unsigned int SCR_WIDTH = 500;
	unsigned int SCR_HEIGHT = 300;

public:
	Game(const Game& obj) = delete;
	~Game();

	Game* Game::GetInstance()
	{
		if (Instance == nullptr)
		{
			Instance = new Game();
			return Instance;
		}
		else
		{
			return Instance;
		}
	}

};