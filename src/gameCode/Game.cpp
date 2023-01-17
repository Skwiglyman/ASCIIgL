#include "Game.hpp"

Game::Game()
	: guiCamera(glm::vec2(0, 0), SCR_WIDTH, SCR_HEIGHT)
{
	GUI_Textures["Title"] = new Texture("res/textures/GUI/Title.png");
	GUI_Textures["Start_Sel"] = new Texture("res/textures/GUI/StartSelected.png");
	GUI_Textures["Start_Unsel"] = new Texture("res/textures/GUI/StartUnselected.png");
	GUI_Textures["How_To_Play_Sel"] = new Texture("res/textures/GUI/HowToPlaySelected.png");
	GUI_Textures["How_To_Play_Unsel"] = new Texture("res/textures/GUI/HowToPlayUnselected.png");
	GUI_Textures["GameInfo1"] = new Texture("res/textures/GUI/GameInfo1.png");
	GUI_Textures["GameInfo2"] = new Texture("res/textures/GUI/GameInfo2.png");
	GUI_Textures["Select_Btn"] = new Texture("res/textures/GUI/PressQ.png");
	GUI_Textures["BackInfo"] = new Texture("res/textures/GUI/BackInfo.png");
}

Game::~Game()
{
	for (auto const& [key, val] : GUI_Textures)
	{
		delete val;
	}

	delete Instance;
}

void tempFpsMovement(Camera3D& camera3D)
{
	if (GetKeyState('W') & 0x8000) { camera3D.setCamPos(camera3D.pos + camera3D.getCamFront()); }
	if (GetKeyState('S') & 0x8000) { camera3D.setCamPos(camera3D.pos + camera3D.getCamBack()); }
	if (GetKeyState('A') & 0x8000) { camera3D.setCamPos(camera3D.pos + camera3D.getCamLeft()); }
	if (GetKeyState('D') & 0x8000) { camera3D.setCamPos(camera3D.pos + camera3D.getCamRight()); }

	if (GetKeyState(VK_SPACE) & 0x8000) { camera3D.setCamPos(camera3D.pos + glm::vec3(0, -1, 0)); }
	if (GetKeyState(VK_SHIFT) & 0x8000) { camera3D.setCamPos(camera3D.pos + glm::vec3(0, 1, 0)); }

	if (GetKeyState(VK_UP) & 0x8000) { camera3D.setCamDir(camera3D.yaw, camera3D.pitch - 1); }
	if (GetKeyState(VK_DOWN) & 0x8000) { camera3D.setCamDir(camera3D.yaw, camera3D.pitch + 1); }
	if (GetKeyState(VK_LEFT) & 0x8000) { camera3D.setCamDir(camera3D.yaw - 1, camera3D.pitch); }
	if (GetKeyState(VK_RIGHT) & 0x8000) { camera3D.setCamDir(camera3D.yaw + 1, camera3D.pitch); }
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
	Screen::GetInstance()->InitializeScreen(SCR_WIDTH, SCR_HEIGHT, L"I Don't Wanna Run For Christmas", 2, 2); // 900x540 is the biggest resolution for my monitor

	Screen::GetInstance()->WIREFRAME = false;
	Screen::GetInstance()->BACKFACECULLING = true;
	Screen::GetInstance()->CCW = true;
	Screen::GetInstance()->BLEND = true;

	while (running == true)
	{
		// starting fps timer
		Screen::GetInstance()->StartFPSClock();

		// resetting screen and buffer
		Screen::GetInstance()->ClearScreen();
		Screen::GetInstance()->ClearBuffer(FG_BLACK);

		// do game logic here
		if (gameState == MAIN_MENU)
			RunMainMenu();
		else if (gameState == HOW_TO_PLAY)
			RunHowToPlay();
		else if (gameState == GAME_LORE)
			RunLore();


		// drawing
		Screen::GetInstance()->DrawBorder(FG_WHITE);
		Screen::GetInstance()->OutputBuffer();

		// fps stuff and title of console
		Screen::GetInstance()->EndFPSClock();
		Screen::GetInstance()->SetTitle();
	}

}

void Game::RunMainMenu()
{
	Renderer::Draw2DQuad(vertexShader, *GUI_Textures["Title"], glm::vec2(220, 80), glm::vec2(0, 0), glm::vec2(130, 50), guiCamera, 0);
	Renderer::Draw2DQuad(vertexShader, *GUI_Textures["Select_Btn"], glm::vec2(85, 270), glm::vec2(0, 0), glm::vec2(55, 10), guiCamera, 0);

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
			BtnSelected = 0;
			Sleep(100);
		}
		else
		{
			gameState = HOW_TO_PLAY;
			BtnSelected = 0;
			Sleep(100);
		}
	}


	if (BtnSelected != 0)
	{
		Renderer::Draw2DQuad(vertexShader, *GUI_Textures["How_To_Play_Sel"], glm::vec2(215, 150), glm::vec2(0, 0), glm::vec2(75, 30), guiCamera, 0);
		Renderer::Draw2DQuad(vertexShader, *GUI_Textures["Start_Unsel"], glm::vec2(217, 200), glm::vec2(0, 0), glm::vec2(60, 30), guiCamera, 0);
	}
	else
	{
		Renderer::Draw2DQuad(vertexShader, *GUI_Textures["How_To_Play_Unsel"], glm::vec2(215, 150), glm::vec2(0, 0), glm::vec2(75, 30), guiCamera, 0);
		Renderer::Draw2DQuad(vertexShader, *GUI_Textures["Start_Sel"], glm::vec2(217, 200), glm::vec2(0, 0), glm::vec2(60, 30), guiCamera, 0);
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

	Renderer::Draw2DQuad(vertexShader, *GUI_Textures["GameInfo2"], glm::vec2(215, 120), glm::vec2(0, 0), glm::vec2(175, 100), guiCamera, 0);
	Renderer::Draw2DQuad(vertexShader, *GUI_Textures["BackInfo"], glm::vec2(217, 250), glm::vec2(0, 0), glm::vec2(100, 15), guiCamera, 0);

}

void Game::RunLore()
{
	Renderer::Draw2DQuad(vertexShader, *GUI_Textures["GameInfo1"], glm::vec2(225, 150), glm::vec2(0, 0), glm::vec2(200, 125), guiCamera, 0);
	Screen::GetInstance()->OutputBuffer();
	Sleep(7500);
	gameState = MAZE;
}

void Game::RunMaze()
{
}