#include "../projectCode/Screen.hpp"
#include "../projectCode/Camera3D.hpp"

void tempFpsMovement(Camera3D& camera3D)
{
	if (GetKeyState('W') & 0x8000) { camera3D.setCamPos(camera3D.position + camera3D.getCamFront()); }
	if (GetKeyState('S') & 0x8000) { camera3D.setCamPos(camera3D.position + camera3D.getCamBack()); }
	if (GetKeyState('A') & 0x8000) { camera3D.setCamPos(camera3D.position + camera3D.getCamLeft()); }
	if (GetKeyState('D') & 0x8000) { camera3D.setCamPos(camera3D.position + camera3D.getCamRight()); }

	if (GetKeyState(VK_UP) & 0x8000) { camera3D.setCamDir(camera3D.yaw, camera3D.pitch - 5); }
	if (GetKeyState(VK_BACK) & 0x8000) { camera3D.setCamDir(camera3D.yaw, camera3D.pitch + 5); }
	if (GetKeyState(VK_LEFT) & 0x8000) { camera3D.setCamDir(camera3D.yaw - 5, camera3D.pitch); }
	if (GetKeyState(VK_RIGHT) & 0x8000) { camera3D.setCamDir(camera3D.yaw + 5, camera3D.pitch); }
}

int main()
{
	Screen screen(200, 200);
	Screen::DEF_VERTEX_SHADER vertexShader;

	Camera3D camera(glm::vec3(0.0f, 10.0f, 0.0f), 80, screen.SCR_WIDTH / screen.SCR_HEIGHT, glm::vec2(0.0f, 0.0f), 0.01f,  1280);

	glm::vec3 position(10, 10, 10);
	glm::vec3 size(5, 5, 5);
	glm::vec2 rotation(45, 45);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z));
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z));
	model = glm::scale(model, size);

	std::vector<float*> vertices = {
		{ 0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		 				   
		{ 0.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		 				   
		{ 0.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f },
		 				   
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		 				   
		{ 0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f },
		 				   
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f }
	};

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