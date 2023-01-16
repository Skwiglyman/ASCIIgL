#include "Game.hpp"

Game::Game()
	: title("res/textures/GUI/Title.png"), startSel("res/textures/GUI/StartSelected.png"), startUnsel("res/textures/GUI/StartUnselected.png"),
	howToSel("res/textures/GUI/HowToPlaySelected.png"), howToUnsel("res/textures/GUI/HowToPlayUnselected.png"),
	gameInfo1("res/textures/GUI/GameInfo1.png"), gameInfo2("res/textures/GUI/GameInfo1.png")
{

}

Game::~Game()
{
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
	Screen::GetInstance()->InitializeScreen(500, 350, L"I Don't Wanna Run For Christmas - ft Mariah Carey", 2, 2); // 900x540 is the biggest resolution for my monitor

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

		// drawing
		Screen::GetInstance()->DrawBorder(FG_WHITE);
		Screen::GetInstance()->OutputBuffer();

		// fps stuff and title of console
		Screen::GetInstance()->EndFPSClock();
		Screen::GetInstance()->SetTitle();
	}

}