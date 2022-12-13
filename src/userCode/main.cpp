#include "../projectCode/Screen.hpp"
#include "../projectCode/Camera3D.hpp"
#include "../projectCode/Camera2D.hpp"

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
	Screen screen(900, 540); // 900x540 is the biggest resolution for my monitor
	Screen::DEF_VERTEX_SHADER vertexShader;

	Camera3D camera(glm::vec3(0.0f, 0.0f, 0.0f), 80, (float) screen.SCR_WIDTH / (float) screen.SCR_HEIGHT, glm::vec2(0.0f, 0.0f), 0.01f,  1280);
	//Camera2D camera(glm::vec2(0, 0), screen.SCR_WIDTH, screen.SCR_HEIGHT);

	glm::vec3 position(80, 50, 80);
	glm::vec3 size(50, 50, 50);
	glm::vec2 rotation(0, 0);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z));
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z));
	model = glm::scale(model, size);

	std::vector<std::vector<float>> vertices {
		{ -1.0f, -1.0f, -1.0f}, // Bottom-left
		{  1.0f,  1.0f, -1.0f}, // top-right
		{  1.0f, -1.0f, -1.0f}, // bottom-right         
		{  1.0f,  1.0f, -1.0f}, // top-right
		{ -1.0f, -1.0f, -1.0f}, // bottom-left
		{ -1.0f,  1.0f, -1.0f}, // top-left
		// Front
		{ -1.0f, -1.0f,  1.0f}, // bottom-left
		{  1.0f, -1.0f,  1.0f}, // bottom-right
		{  1.0f,  1.0f,  1.0f}, // top-right
		{  1.0f,  1.0f,  1.0f}, // top-right
		{ -1.0f,  1.0f,  1.0f}, // top-left
		{ -1.0f, -1.0f,  1.0f}, // bottom-left
		// Left
		{ -1.0f,  1.0f,  1.0f}, // top-right
		{ -1.0f,  1.0f, -1.0f}, // top-left
		{ -1.0f, -1.0f, -1.0f}, // bottom-left
		{ -1.0f, -1.0f, -1.0f}, // bottom-left
		{ -1.0f, -1.0f,  1.0f}, // bottom-right
		{ -1.0f,  1.0f,  1.0f}, // top-right
		// Right
		{  1.0f,  1.0f,  1.0f}, // top-left
		{  1.0f, -1.0f, -1.0f}, // bottom-right
		{  1.0f,  1.0f, -1.0f}, // top-right         
		{  1.0f, -1.0f, -1.0f}, // bottom-right
		{  1.0f,  1.0f,  1.0f}, // top-left
		{  1.0f, -1.0f,  1.0f}, // bottom-left     
		// Bottom
		{ -1.0f, -1.0f, -1.0f}, // top-right
		{  1.0f, -1.0f, -1.0f}, // top-left
		{  1.0f, -1.0f,  1.0f}, // bottom-left
		{  1.0f, -1.0f,  1.0f}, // bottom-left
		{ -1.0f, -1.0f,  1.0f}, // bottom-right
		{ -1.0f, -1.0f, -1.0f}, // top-right
		// Top f1.0
		{ -1.0f,  1.0f, -1.0f}, // top-left
		{  1.0f,  1.0f,  1.0f}, // bottom-right
		{  1.0f,  1.0f, -1.0f}, // top-right     
		{  1.0f,  1.0f,  1.0f}, // bottom-right
		{ -1.0f,  1.0f, -1.0f}, // top-left
		{ -1.0f,  1.0f,  1.0f}, // bottom-left     
	};



	vertexShader.camera = &camera;
	
	bool running = true;
	while (running)
	{
		// resetting screen and buffer
		screen.ClearScreen();
		screen.ClearBuffer();

		// do game logic here
		tempFpsMovement(camera);

		vertexShader.model = model;
		vertexShader.view = camera.view;
		vertexShader.proj = camera.proj;

		// Rendering
		screen.RenderTriangles(vertexShader, vertices);
		
		// drawing
		screen.OutputBuffer();
	}
}