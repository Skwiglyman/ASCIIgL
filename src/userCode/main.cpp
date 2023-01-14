#include "../projectCode/Screen.hpp"
#include "../projectCode/Camera3D.hpp"
#include "../projectCode/Camera2D.hpp"
#include "../projectCode/VertexShader.hpp"
#include "../projectCode/Texture.hpp"

#include <random>

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

int main()
{
	Screen* screen = Screen::GetInstance();
	screen->InitializeScreen(400, 400, L"I Don't Wanna Run For Christmas - ft Mariah Carey", 2, 2); // 900x540 is the biggest resolution for my monitor
	VERTEX_SHADER vertexShader;

	Camera3D camera(glm::vec3(0.0f, 0.0f, 0.0f), 80, (float) screen->SCR_WIDTH / (float) screen->SCR_HEIGHT, glm::vec2(90.0f, 0.0f), 0.1,  1000);

	std::vector<VERTEX> vertices {
		VERTEX({ -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // Bottom-left
		VERTEX({  1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
		VERTEX({  1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
		VERTEX({  1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
		VERTEX({ -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
		VERTEX({ -1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
													 
		VERTEX({ -1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
		VERTEX({  1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
		VERTEX({  1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
		VERTEX({  1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
		VERTEX({ -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
		VERTEX({ -1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
			 										 
		VERTEX({ -1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
		VERTEX({ -1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
		VERTEX({ -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
		VERTEX({ -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
		VERTEX({ -1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
		VERTEX({ -1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
	 												 
		VERTEX({  1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
		VERTEX({  1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
		VERTEX({  1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right         
		VERTEX({  1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
		VERTEX({  1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
		VERTEX({  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left     
													 
		VERTEX({ -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
		VERTEX({  1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
		VERTEX({  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
		VERTEX({  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
		VERTEX({ -1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
		VERTEX({ -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
													 
		VERTEX({ -1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
		VERTEX({  1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
		VERTEX({  1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right     
		VERTEX({  1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
		VERTEX({ -1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
		VERTEX({ -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left     
	};
	screen->WIREFRAME = false;
	screen->BACKFACECULLING = true;
	screen->CCW = true;
	screen->BLEND = true;

	Model modelObj("res/models/test/Test.obj");
	
	bool running = true;
	while (running)
	{
		// starting fps timer
		screen->StartFPSClock();

		// resetting screen and buffer
		screen->ClearScreen();
		screen->ClearBuffer();

		// do game logic here
		tempFpsMovement(camera);

		// Rendering
		screen->DrawModel(vertexShader, modelObj, glm::vec3(0, 0, 100), glm::vec2(0, 0), glm::vec3(10, 10, 10), camera);
		screen->DrawBorder();

		// drawing
		screen->OutputBuffer();

		// fps stuff and title of console
		screen->EndFPSClock();
		screen->SetTitle();
	}
}