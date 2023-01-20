#include "Game.hpp"

Game::Game()
	: guiCamera(glm::vec2(0, 0), SCR_WIDTH, SCR_HEIGHT)
{
	Textures["Title"] = new Texture("res/textures/GUI/Title.png");
	Textures["Start_Sel"] = new Texture("res/textures/GUI/StartSelected.png");
	Textures["Start_Unsel"] = new Texture("res/textures/GUI/StartUnselected.png");
	Textures["How_To_Play_Sel"] = new Texture("res/textures/GUI/HowToPlaySelected.png");
	Textures["How_To_Play_Unsel"] = new Texture("res/textures/GUI/HowToPlayUnselected.png");
	Textures["GameInfo1"] = new Texture("res/textures/GUI/GameInfo1.png");
	Textures["GameInfo2"] = new Texture("res/textures/GUI/GameInfo2.png");
	Textures["Select_Btn"] = new Texture("res/textures/GUI/PressQ.png");
	Textures["BackInfo"] = new Texture("res/textures/GUI/BackInfo.png");
}

Game::~Game()
{
	for (auto const& [key, val] : Textures)
	{
		delete val;
	}

	for (auto& i : gameObjs)
	{
		delete i;
	}

	delete Instance, player, LevelModel;
}

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

void Game::Run()
{
	Screen::GetInstance()->InitializeScreen(SCR_WIDTH, SCR_HEIGHT, L"I Don't Wanna Run For Christmas", 2, 2); // 900x500 is biggest

	Screen::GetInstance()->WIREFRAME = false;
	Screen::GetInstance()->BACKFACECULLING = true;
	Screen::GetInstance()->CCW = false;
	Screen::GetInstance()->BLEND = true;

	while (running == true)
	{
		// starting fps timer
		Screen::GetInstance()->StartFPSClock();

		// resetting screen and buffer
		Screen::GetInstance()->ClearScreen();
		Screen::GetInstance()->ClearBuffer(FG_BLUE);

		// do game logic here
		if (gameState == MAIN_MENU)
			RunMainMenu();
		else if (gameState == HOW_TO_PLAY)
			RunHowToPlay();
		else if (gameState == GAME_LORE)
			RunLore();
		else if (gameState == MAZE)
			RunMaze();

		// drawing
		Screen::GetInstance()->DrawBorder(FG_WHITE);
		Screen::GetInstance()->OutputBuffer();

		// fps stuff and title of console
		Screen::GetInstance()->EndFPSClock();
		Screen::GetInstance()->SetTitle();
	}

}

void Game::LoadLevel(const std::string path)
{
	LevelModel = new Model("res/models/level/MazeTest.obj");

	gameObjs.push_back(new GameObj(glm::vec3(0, 10, 0), glm::vec2(0, 0), glm::vec3(10, -20, 10), LevelModel));
	player = new Player(glm::vec2(levelHeight / 2, levelWidth / 2), glm::vec2(0, 0), -playerHeight);
}

void Game::RunMainMenu()
{
	Renderer::Draw2DQuad(vertexShader, *Textures["Title"], glm::vec2(220, 80), glm::vec2(0, 0), glm::vec2(130, 50), guiCamera, 0);
	Renderer::Draw2DQuad(vertexShader, *Textures["Select_Btn"], glm::vec2(85, 270), glm::vec2(0, 0), glm::vec2(55, 10), guiCamera, 0);

	if (GetKeyState(VK_UP) & 0x8000)
	{
		BtnSelected = 0;
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		BtnSelected += 1;
	}

	if (GetKeyState('Q') & 0x8000)
	{
		if (BtnSelected != 0)
		{
			gameState = GAME_LORE;
			Sleep(100);
		}
		else
		{
			gameState = HOW_TO_PLAY;
			Sleep(100);
		}
	}


	if (BtnSelected != 0)
	{
		Renderer::Draw2DQuad(vertexShader, *Textures["How_To_Play_Sel"], glm::vec2(215, 150), glm::vec2(0, 0), glm::vec2(75, 30), guiCamera, 0);
		Renderer::Draw2DQuad(vertexShader, *Textures["Start_Unsel"], glm::vec2(217, 200), glm::vec2(0, 0), glm::vec2(60, 30), guiCamera, 0);
	}
	else
	{
		Renderer::Draw2DQuad(vertexShader, *Textures["How_To_Play_Unsel"], glm::vec2(215, 150), glm::vec2(0, 0), glm::vec2(75, 30), guiCamera, 0);
		Renderer::Draw2DQuad(vertexShader, *Textures["Start_Sel"], glm::vec2(217, 200), glm::vec2(0, 0), glm::vec2(60, 30), guiCamera, 0);
	}
	
}

void Game::RunHowToPlay()
{
	if (GetKeyState('Q') & 0x8000)
	{
		gameState = MAIN_MENU;
		Sleep(100);
		BtnSelected = 0;
		
	}

	Renderer::Draw2DQuad(vertexShader, *Textures["GameInfo2"], glm::vec2(215, 120), glm::vec2(0, 0), glm::vec2(175, 100), guiCamera, 0);
	Renderer::Draw2DQuad(vertexShader, *Textures["BackInfo"], glm::vec2(217, 250), glm::vec2(0, 0), glm::vec2(100, 15), guiCamera, 0);

}

void Game::RunLore()
{
	Renderer::Draw2DQuad(vertexShader, *Textures["GameInfo1"], glm::vec2(225, 150), glm::vec2(0, 0), glm::vec2(200, 125), guiCamera, 0);
	Screen::GetInstance()->OutputBuffer();
	Sleep(100);
	gameState = MAZE;
	LoadLevel("res/levels/TestMaze.txt");
}

void Game::RunMaze()
{
	player->Update(gameObjs);

	for (int i = 0; i < gameObjs.size(); i++)
	{
		Renderer::DrawModel(vertexShader,*gameObjs[i]->model, gameObjs[i]->position, gameObjs[i]->rotation, gameObjs[i]->size, player->camera);
	}
}