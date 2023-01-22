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
	Textures["Lost"] = new Texture("res/textures/GUI/Lost.png");
	Textures["Win"] = new Texture("res/textures/GUI/Win.png");

	Textures["Tired"] = new Texture("res/textures/GUI/Tired.png");
	Textures["Stamina1"] = new Texture("res/textures/GUI/Stamina1.png");
	Textures["Stamina2"] = new Texture("res/textures/GUI/Stamina2.png");
	Textures["Stamina3"] = new Texture("res/textures/GUI/Stamina3.png");
	Textures["Stamina4"] = new Texture("res/textures/GUI/Stamina4.png");
	Textures["Stamina5"] = new Texture("res/textures/GUI/Stamina5.png");
	Textures["Stamina6"] = new Texture("res/textures/GUI/Stamina6.png");
}

Game::~Game()
{
	for (auto const& [key, val] : Textures)
	{
		delete val;
	}

	for (auto i : enemies)
	{
		delete i;
	}

	delete Instance, player, LevelModel, Level, PresentModel, MariahModel;
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

	PlaySound(TEXT("C:\\Users\\Skwig\\OneDrive\\Desktop\\Programs\\C_Programs\\3d_Ascii_Games\\ASCIIgL\\res\\audio\\Man.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_LOOP);

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
		else if (gameState == CAUGHT)
			RunLost();
		else if (gameState == WIN)
			RunWin();

		// drawing
		Screen::GetInstance()->DrawBorder(FG_WHITE);
		Screen::GetInstance()->OutputBuffer();

		// fps stuff and title of console
		Screen::GetInstance()->EndFPSClock();
		Screen::GetInstance()->SetTitle();
	}

}

void Game::LoadLevel()
{
	LevelModel = new Model("res/models/level2/MazeTest.obj");
	MariahModel = new Model("res/models/mariah/mariah.obj");
	PresentModel = new Model("res/models/Present/present.obj");

	Level = new GameObj(glm::vec3(0, 0, 0), glm::vec2(0, 0), glm::vec3(levelXSize, -levelHeight, levelZSize), LevelModel);
	player = new Player(glm::vec2(0, 0), glm::vec2(0, 0));

	float enemyHeightOffset = 20;
	enemies.push_back(new Enemy(glm::vec3(0, -enemyHeightOffset, levelZSize / 2), glm::vec3(10, -10, 0), MariahModel));

	presents.push_back(new Present(glm::vec3(levelXSize-30, -15, 0), glm::vec3(10, -10, 0), PresentModel));
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
	LoadLevel();
}

void Game::RunMaze()
{
	player->Update(Level);
	MariahAI();

	for (int i = 0; i < enemies.size(); i++)
	{
		glm::mat4 model = glm::inverse(glm::lookAt(glm::vec3(enemies[i]->position.x, enemies[i]->position.y, enemies[i]->position.z), glm::vec3(player->GetPlayerPos().x, enemies[i]->position.y, player->GetPlayerPos().z), glm::vec3(0, 1, 0)));
		model = glm::scale(model, glm::vec3(-enemies[i]->size.x, enemies[i]->size.y, enemies[i]->size.z));

		Renderer::DrawModel(vertexShader, *enemies[i]->model, model, player->camera);

		bool hit = ASCIIgLEngine::pointCircle2D(glm::vec2(enemies[i]->position.x, enemies[i]->position.z), glm::vec2(player->GetPlayerPos().x, player->GetPlayerPos().z), player->playerHitBoxRad);
		if (hit == true)
		{
			gameState = CAUGHT;
		}
	}

	for (int i = 0; i < presents.size(); i++)
	{
		glm::mat4 model = glm::inverse(glm::lookAt(glm::vec3(presents[i]->position.x, presents[i]->position.y, presents[i]->position.z), glm::vec3(player->GetPlayerPos().x, presents[i]->position.y, player->GetPlayerPos().z), glm::vec3(0, 1, 0)));
		model = glm::scale(model, glm::vec3(-presents[i]->size.x, presents[i]->size.y, presents[i]->size.z));

		Renderer::DrawModel(vertexShader, *presents[i]->model, model, player->camera);

		bool hit = ASCIIgLEngine::pointCircle2D(glm::vec2(presents[i]->position.x, presents[i]->position.z), glm::vec2(player->GetPlayerPos().x, player->GetPlayerPos().z), player->playerHitBoxRad);
		if (hit == true)
		{
			presents[i]->collected = true;
		}
	}

	glm::vec2 barSize = glm::vec2(50, 20);
	glm::vec2 barPos = glm::vec2(380, 270);

	if (player->stamina > player->maxStamina * 0.99f)
		Renderer::Draw2DQuad(vertexShader, *Textures["Stamina6"], barPos, glm::vec2(0, 0), barSize, guiCamera, 0);
	
	else if (player->stamina > player->maxStamina * 0.83f)
		Renderer::Draw2DQuad(vertexShader, *Textures["Stamina5"], barPos, glm::vec2(0, 0), barSize, guiCamera, 0);
	
	else if (player->stamina > player->maxStamina * 0.66f)
		Renderer::Draw2DQuad(vertexShader, *Textures["Stamina4"], barPos, glm::vec2(0, 0), barSize, guiCamera, 0);
	
	else if (player->stamina > player->maxStamina * 0.5)
		Renderer::Draw2DQuad(vertexShader, *Textures["Stamina3"], barPos, glm::vec2(0, 0), barSize, guiCamera, 0);
	
	else if (player->stamina > player->maxStamina * 0.33f)
		Renderer::Draw2DQuad(vertexShader, *Textures["Stamina2"], barPos, glm::vec2(0, 0), barSize, guiCamera, 0);

	else if (player->stamina > player->maxStamina * 0.16)
		Renderer::Draw2DQuad(vertexShader, *Textures["Stamina1"], barPos, glm::vec2(0, 0), barSize, guiCamera, 0);

	else
		Renderer::Draw2DQuad(vertexShader, *Textures["Tired"], barPos, glm::vec2(0, 0), barSize, guiCamera, 0);

	int presentsCollected = GetPresentsCollected();
	if (presentsCollected == presents.size() && presentsCollected != 0)
		gameState = WIN;

	Renderer::DrawModel(vertexShader,*Level->model, Level->position, Level->rotation, Level->size, player->camera);
}

void Game::RunLost()
{
	Renderer::Draw2DQuad(vertexShader, *Textures["Lost"], glm::vec2(225, 150), glm::vec2(0, 0), glm::vec2(200, 125), guiCamera, 0);

	if (GetKeyState('R') & 0x8000)
	{
		gameState = MAZE;
		player->camera.setCamPos(glm::vec3(0, -player->playerHeight, 0));
		for (int i = 0; i < enemies.size(); i++)
		{
			enemies[i]->position = glm::vec3(0, -10, levelZSize / 2);
		}
		
		Sleep(100);
	}
}

void Game::MariahAI()
{
	float mariahSpeedFactor = 0.7f;
	for (int i = 0; i < enemies.size(); i++)
	{
		glm::vec3 move = glm::normalize(player->GetPlayerPos() - enemies[i]->position) * Screen::GetInstance()->GetDeltaTime() * mariahSpeedFactor;
		enemies[i]->position += glm::vec3(move.x, 0, move.z);
	}
}

int Game::GetPresentsCollected()
{
	int presentsCollected = 0;

	for (int i = 0; i < presents.size(); i++)
	{
		if (presents[i]->collected == true)
			presentsCollected += 1;
	}
	return presentsCollected;
}

void Game::RunWin()
{
	Renderer::Draw2DQuad(vertexShader, *Textures["Win"], glm::vec2(225, 150), glm::vec2(0, 0), glm::vec2(200, 125), guiCamera, 0);
}