#include "../renderingEngineCode/Screen.hpp"
#include "../renderingEngineCode/VertexShader.hpp"
#include "../renderingEngineCode/Texture.hpp"

#include "../gameEngineCode/Camera3D.hpp"
#include "../gameEngineCode/Camera2D.hpp"
#include "../gameEngineCode/Model.hpp"

#include "Renderer.hpp"


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
	Screen::GetInstance()->InitializeScreen(500, 350, L"I Don't Wanna Run For Christmas - ft Mariah Carey", 2, 2); // 900x540 is the biggest resolution for my monitor
	VERTEX_SHADER vertexShader;

	Camera3D camera(glm::vec3(0.0f, 0.0f, 0.0f), 80, (float)Screen::GetInstance()->SCR_WIDTH / (float)Screen::GetInstance()->SCR_HEIGHT, glm::vec2(90.0f, 0.0f), 0.1,  1000);

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
	Screen::GetInstance()->WIREFRAME = false;
	Screen::GetInstance()->BACKFACECULLING = true;
	Screen::GetInstance()->CCW = true;
	Screen::GetInstance()->BLEND = true;

	Model modelObj("res/models/test/Test.obj");
	
	bool running = true;
	while (running)
	{
		// starting fps timer
		Screen::GetInstance()->StartFPSClock();

		// resetting screen and buffer
		Screen::GetInstance()->ClearScreen();
		Screen::GetInstance()->ClearBuffer(FG_BLACK);

		// do game logic here
		tempFpsMovement(camera);

		// Rendering
		Renderer::DrawModel(vertexShader, modelObj, glm::vec3(0, 0, 100), glm::vec2(0, 0), glm::vec3(10, 10, 10), camera);
		Screen::GetInstance()->DrawBorder(FG_WHITE);

		// drawing
		Screen::GetInstance()->OutputBuffer();

		// fps stuff and title of console
		Screen::GetInstance()->EndFPSClock();
		Screen::GetInstance()->SetTitle();
	}
}