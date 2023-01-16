#pragma once

#include "..\renderingEngineCode\Screen.hpp"

#include "Camera2D.hpp"
#include "Camera3D.hpp"
#include "Renderer.hpp"

#include <map>

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
	std::map<std::string, Texture*> GUI_Textures;

	Camera2D guiCamera;
	void RunMainMenu();

public:

	Game(const Game& obj) = delete;
	~Game();

	void Run();

	static Game* GetInstance();

};