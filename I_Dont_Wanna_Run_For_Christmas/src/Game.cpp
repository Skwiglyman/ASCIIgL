#include "Game.hpp"
#include "engine/Logger.hpp"

Game::Game()
	: guiCamera(glm::vec2(0, 0), SCR_WIDTH, SCR_HEIGHT)
{

	// don't mind me just loading in 100030423 textures for my simple gui
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

	// stamina bar gui textures
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
	// this destructor just deletes everything
	for (auto const& [key, val] : Textures)
	{
		delete val;
	}

	for (auto i : enemies)
	{
		delete i;
	}

	delete Instance, player, LevelModel, Level, PresentModel, MariahModel, Mariah2Model;
}

Game* Game::GetInstance()
{
	// just returns the instance of the game class, and if there isn't one creates one
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
    Logger::Info("[INFO] Game loop starting.");
    const int screenInitResult = Screen::GetInstance()->InitializeScreen(SCR_WIDTH, SCR_HEIGHT, L"I Don't Wanna Run For Christmas", 2, 2);

    Logger::Debug("[DEBUG] Screen::InitializeScreen returned: " + std::to_string(screenInitResult));
    Screen::GetInstance()->WIREFRAME = false;
    Screen::GetInstance()->BACKFACECULLING = true;
    Screen::GetInstance()->CCW = false;
    Screen::GetInstance()->BLEND = true;

    Logger::Info("[INFO] Playing background music.");
	BOOL soundResult = PlaySound(TEXT(".\\res\\audio\\Man.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	if (soundResult)
		Logger::Info("[INFO] Background music started successfully.");
	else
		Logger::Error("[ERROR] Failed to start background music.");

    // main gameloop
    while (running == true)
    {
        Logger::Debug("[DEBUG] Frame start. GameState: " + std::to_string(gameState));

        Screen::GetInstance()->StartFPSClock();
		Logger::Debug("[DEBUG] Clearing screen and buffers.");
        Screen::GetInstance()->ClearScreen();
        Screen::GetInstance()->ClearBuffer(BG_BLACK);

        // do game logic here
        switch (gameState) {
            case MAIN_MENU:
                Logger::Debug("[DEBUG] Running Main Menu.");
                RunMainMenu();
                break;
            case HOW_TO_PLAY:
                Logger::Debug("[DEBUG] Running How To Play.");
                RunHowToPlay();
                break;
            case GAME_LORE:
                Logger::Debug("[DEBUG] Running Game Lore.");
                RunLore();
                break;
            case MAZE:
                Logger::Debug("[DEBUG] Running Maze.");
                RunMaze();
                break;
            case CAUGHT:
                Logger::Debug("[DEBUG] Running Lost.");
                RunLost();
                break;
            case WIN:
                Logger::Debug("[DEBUG] Running Win.");
                RunWin();
                break;
            default:
                Logger::Debug("[WARN] Unknown game state: " + std::to_string(gameState));
                break;
        }

		Logger::Debug("[DEBUG] Drawing border.");
        Screen::GetInstance()->DrawBorder(FG_WHITE);

		Logger::Debug("[DEBUG] Outputting buffer.");
        Screen::GetInstance()->OutputBuffer();

        Screen::GetInstance()->EndFPSClock();
        Screen::GetInstance()->SetTitle();

        Logger::Debug("[DEBUG] Frame end.");
    }
    Logger::Info("[INFO] Game loop ended.");
}

void Game::LoadLevel()
{
	LevelModel = new Model("res/models/level2/MazeTest.obj");
	MariahModel = new Model("res/models/mariah/mariah.obj");
	Mariah2Model = new Model("res/models/Mariah2/mariah.obj");
	PresentModel = new Model("res/models/Present/present.obj");

	initLevel();
}

void Game::RunMainMenu()
{
	Logger::Debug("[DEBUG] Drawing main menu title and select button.");
	Renderer::Draw2DQuad(vertexShader, *Textures["Title"], glm::vec2(220, 80), glm::vec2(0, 0), glm::vec2(130, 50), guiCamera, 0);
	Renderer::Draw2DQuad(vertexShader, *Textures["Select_Btn"], glm::vec2(105, 270), glm::vec2(0, 0), glm::vec2(100, 15), guiCamera, 0);

	if (GetKeyState(VK_UP) & 0x8000)
	{
		Logger::Debug("[DEBUG] VK_UP pressed. BtnSelected set to 0.");
		BtnSelected = 0;
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		Logger::Debug("[DEBUG] VK_DOWN pressed. BtnSelected incremented.");
		BtnSelected += 1;
	}

	if (GetKeyState('Q') & 0x8000)
	{
		Logger::Debug("[DEBUG] Q pressed. BtnSelected=" + std::to_string(BtnSelected));
		if (BtnSelected != 0)
		{
			Logger::Debug("[DEBUG] Switching to GAME_LORE.");
			gameState = GAME_LORE;
			Sleep(100);
		}
		else
		{
			Logger::Debug("[DEBUG] Switching to HOW_TO_PLAY.");
			gameState = HOW_TO_PLAY;
			Sleep(100);
		}
	}

	Logger::Debug("[DEBUG] BtnSelected=" + std::to_string(BtnSelected));
	if (BtnSelected != 0)
	{
		Logger::Debug("[DEBUG] Drawing How_To_Play_Sel and Start_Unsel.");
		Renderer::Draw2DQuad(vertexShader, *Textures["How_To_Play_Sel"], glm::vec2(215, 150), glm::vec2(0, 0), glm::vec2(75, 30), guiCamera, 0);
		Renderer::Draw2DQuad(vertexShader, *Textures["Start_Unsel"], glm::vec2(217, 200), glm::vec2(0, 0), glm::vec2(60, 30), guiCamera, 0);
	}
	else
	{
		Logger::Debug("[DEBUG] Drawing How_To_Play_Unsel and Start_Sel.");
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
	Sleep(7500);
	Screen::GetInstance()->StartFPSClock();
	gameState = MAZE;
	LoadLevel();

}

void Game::RunMaze()
{
	player->Update(Level);
	MariahAI();

	for (int i = 0; i < enemies.size(); i++)
	{
		glm::mat4 model = glm::inverse(glm::lookAt(glm::vec3(enemies[i]->position.x, enemies[i]->position.y, enemies[i]->position.z), 
			glm::vec3(player->GetPlayerPos().x, enemies[i]->position.y, player->GetPlayerPos().z), glm::vec3(0, 1, 0)));
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
		if (presents[i]->collected != true)
		{
			glm::mat4 model = glm::inverse(glm::lookAt(glm::vec3(presents[i]->position.x, presents[i]->position.y, presents[i]->position.z), 
				glm::vec3(player->GetPlayerPos().x, presents[i]->position.y, player->GetPlayerPos().z), glm::vec3(0, 1, 0)));
			model = glm::scale(model, glm::vec3(-presents[i]->size.x, presents[i]->size.y, presents[i]->size.z));

			Renderer::DrawModel(vertexShader, *presents[i]->model, model, player->camera);

			bool hit = ASCIIgLEngine::pointCircle2D(glm::vec2(presents[i]->position.x, presents[i]->position.z), glm::vec2(player->GetPlayerPos().x, player->GetPlayerPos().z), player->playerHitBoxRad);
			if (hit == true)
			{
				presents[i]->collected = true;

				enemies.push_back(new Enemy(glm::vec3(0, -20, 0), glm::vec3(10, -10, 0), Mariah2Model));
			}
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
		
		delete player, Level;

		for (int i = 0; i < enemies.size(); i++)
		{
			delete enemies[i];
		}

		for (int i = 0; i < presents.size(); i++)
		{
			delete presents[i];
		}

		presents.clear();
		enemies.clear();
		
		initLevel();

		Sleep(100);
	}
}

void Game::MariahAI()
{
	float chaseSpeed = 0.9f;
	float patrolSpeed = 3.0;
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->aiState == Enemy::CHASE)
		{
			glm::vec3 move = glm::normalize(player->GetPlayerPos() - enemies[i]->position) * Screen::GetInstance()->GetDeltaTime() * chaseSpeed;
			enemies[i]->position += glm::vec3(move.x, 0, move.z);
		}

		if (enemies[i]->aiState == Enemy::PATROL)
		{
			glm::vec2 pos1 = glm::vec2(enemies[i]->position.x, enemies[i]->position.z);
			glm::vec2 pos2 = glm::vec2(enemies[i]->patrolDest.x, enemies[i]->patrolDest.z);

			if (ASCIIgLEngine::pointCircle2D(pos1, pos2, enemies[i]->destRadius))
			{
				if (enemies[i]->patrolDest == enemies[i]->patrolEnd)
					enemies[i]->patrolDest = enemies[i]->patrolStart;
				else
					enemies[i]->patrolDest = enemies[i]->patrolEnd;
			}
			glm::vec3 move = glm::normalize(enemies[i]->patrolDest - enemies[i]->position) * Screen::GetInstance()->GetDeltaTime() * patrolSpeed;
			enemies[i]->position += glm::vec3(move.x, 0, move.z);
		}
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

void Game::initLevel()
{
	Level = new GameObj(glm::vec3(0, 0, 0), glm::vec2(0, 0), glm::vec3(levelXSize, -levelHeight, levelZSize), LevelModel);
	player = new Player(glm::vec2(0, levelZSize / 2), glm::vec2(-90, 0));
	player->stamina = player->maxStamina;
	
	float wOff = 20;

	glm::vec3 size = glm::vec3(10, -8, 0);
	enemies.push_back(new Enemy(glm::vec3(0, -20, 0), size, Mariah2Model));

	glm::vec3 sp1 = glm::vec3(levelXSize / 2, -20, 0);
	enemies.push_back(new Enemy(sp1, size, MariahModel, Enemy::PATROL, sp1, glm::vec3(levelXSize / 2, -20, levelZSize - wOff)));
	enemies.push_back(new Enemy(sp1, size, MariahModel, Enemy::PATROL, sp1, glm::vec3(levelXSize / 2, -20, -levelZSize + wOff)));

	glm::vec3 sp2 = glm::vec3(levelXSize - wOff, -20, 0);
	enemies.push_back(new Enemy(sp2, size, MariahModel, Enemy::PATROL, sp2, glm::vec3(levelXSize - wOff, -20, levelZSize - wOff)));
	enemies.push_back(new Enemy(sp2, size, MariahModel, Enemy::PATROL, sp2, glm::vec3(levelXSize - wOff, -20, -levelZSize + wOff)));

	glm::vec3 sp3 = glm::vec3(-levelXSize / 2, -20, 0);
	enemies.push_back(new Enemy(sp3, size, MariahModel, Enemy::PATROL, sp3, glm::vec3(-levelXSize / 2, -20, levelZSize - wOff)));
	enemies.push_back(new Enemy(sp3, size, MariahModel, Enemy::PATROL, sp3, glm::vec3(-levelXSize / 2, -20, -levelZSize + wOff)));
																						  
	glm::vec3 sp4 = glm::vec3(-levelXSize + wOff, -20, 0);								  
	enemies.push_back(new Enemy(sp4, size, MariahModel, Enemy::PATROL, sp4, glm::vec3(-levelXSize + wOff, -20, levelZSize - wOff)));
	enemies.push_back(new Enemy(sp4, size, MariahModel, Enemy::PATROL, sp4, glm::vec3(-levelXSize + wOff, -20, -levelZSize + wOff)));

	glm::vec3 ep6 = glm::vec3(0, -20, levelZSize - wOff);
	enemies.push_back(new Enemy(ep6, size, MariahModel, Enemy::PATROL, glm::vec3(levelXSize - wOff, -20, levelZSize - wOff), ep6));
	enemies.push_back(new Enemy(ep6, size, MariahModel, Enemy::PATROL, glm::vec3(-levelXSize + wOff, -20, levelZSize - wOff), ep6));

	glm::vec3 ep8 = glm::vec3(0, -20, -levelZSize + wOff);
	enemies.push_back(new Enemy(ep8, size, MariahModel, Enemy::PATROL, glm::vec3(levelXSize - wOff, -20, -levelZSize + wOff), ep8));
	enemies.push_back(new Enemy(ep8, size, MariahModel, Enemy::PATROL, glm::vec3(-levelXSize + wOff, -20, -levelZSize + wOff), ep8));


	presents.push_back(new Present(glm::vec3(levelXSize - 30, -15, 0), glm::vec3(10, -10, 0), PresentModel));
	presents.push_back(new Present(glm::vec3(levelXSize - 30, -15, levelZSize - 30), glm::vec3(10, -10, 0), PresentModel));
	presents.push_back(new Present(glm::vec3(levelXSize - 30, -15, -levelZSize + 30), glm::vec3(10, -10, 0), PresentModel));

	presents.push_back(new Present(glm::vec3(-levelXSize + 30, -15, 0), glm::vec3(10, -10, 0), PresentModel));
	presents.push_back(new Present(glm::vec3(-levelXSize + 30, -15, levelZSize - 30), glm::vec3(10, -10, 0), PresentModel));
	presents.push_back(new Present(glm::vec3(-levelXSize + 30, -15, -levelZSize + 30), glm::vec3(10, -10, 0), PresentModel));
}