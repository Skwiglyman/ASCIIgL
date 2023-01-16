#pragma once

#include "..\renderingEngineCode\Screen.hpp"

#include "Camera2D.hpp"
#include "Camera3D.hpp"

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

	bool running = true;
	VERTEX_SHADER vertexShader;


	// MENU TEXTURES
	Texture title, startSel, startUnsel, howToSel, howToUnsel, gameInfo1, gameInfo2;

public:

	Game(const Game& obj) = delete;
	~Game();

	void Run();

	Game* GetInstance();

};