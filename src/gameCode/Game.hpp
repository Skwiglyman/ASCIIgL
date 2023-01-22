#pragma once

#include "..\renderingEngineCode\Screen.hpp"

#include "../gameEngineCode/Camera2D.hpp"
#include "../gameEngineCode/Renderer.hpp"
#include "../gameEngineCode/GameObj.hpp"

#include "Player.hpp"
#include "Enemies.hpp"

#include <map>
#include <Windows.h>
#include "MMSystem.h"

class Game
{
private:
	enum STATE
	{
		MAIN_MENU,
		HOW_TO_PLAY,
		GAME_LORE,
		MAZE,
		CAUGHT,
		WIN,
	};

	unsigned int BtnSelected = 0;

	Game();
	static inline Game* Instance = nullptr;

	unsigned int gameState = MAIN_MENU;

	unsigned int SCR_WIDTH = 450;
	unsigned int SCR_HEIGHT = 300;

	bool running = true;
	VERTEX_SHADER vertexShader;

	// LOADING LEVEL
	std::string levelData = "";
	int levelXSize = 300;
	int levelZSize = 300;
	int levelHeight = 300;

	void LoadLevel(const std::string path);


	// MENU TEXTURES
	std::map<std::string, Texture*> Textures;

	// Game Objects
	GameObj* Level = nullptr;
	Camera2D guiCamera;

	Model* LevelModel;

	void RunMainMenu();
	void RunHowToPlay();
	void RunLore();
	void RunMaze();

public:

	Game(const Game& obj) = delete;
	~Game();

	void Run();

	static Game* GetInstance();

	Player* player = nullptr;

};