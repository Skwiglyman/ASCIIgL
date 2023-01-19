#pragma once

#include "..\renderingEngineCode\Screen.hpp"

#include "Camera2D.hpp"
#include "Renderer.hpp"
#include "Player.hpp"
#include "GameObj.hpp"

#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <random>

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
	int levelWidth = 100;
	int levelHeight = 100;

	void LoadLevel(const std::string path);


	// MENU TEXTURES
	std::map<std::string, Texture*> Textures;

	// Game Objects
	std::vector<GameObj*> gameObjs;
	Camera2D guiCamera;

	Model* WallModel;

	void RunMainMenu();
	void RunHowToPlay();
	void RunLore();
	void RunMaze();

public:

	Game(const Game& obj) = delete;
	~Game();

	void Run();

	static Game* GetInstance();

	int playerHeight = 10;
	Player* player = nullptr;

};