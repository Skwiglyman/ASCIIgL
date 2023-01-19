#include "Player.hpp"

Player::Player(glm::vec2 xz, glm::vec2 yawPitch, float playerHeight)
	: camera(glm::vec3(xz.x, playerHeight, xz.y), fov, (float)Screen::GetInstance()->SCR_WIDTH / (float)Screen::GetInstance()->SCR_HEIGHT, yawPitch, nearClip, farClip)
{

}

Player::~Player()
{

}

glm::vec3 Player::GetPlayerPos()
{
	return camera.pos;
}

glm::vec3 Player::GetMovement()
{
	glm::vec3 move(0, 0, 0);
	if (GetKeyState('W') & 0x8000) { move += glm::vec3(camera.getCamFront().x, 0, camera.getCamFront().z); }
	if (GetKeyState('S') & 0x8000) { move += glm::vec3(camera.getCamBack().x,  0, camera.getCamBack().z); }
	if (GetKeyState('A') & 0x8000) { move += glm::vec3(camera.getCamLeft().x,  0, camera.getCamLeft().z); }
	if (GetKeyState('D') & 0x8000) { move += glm::vec3(camera.getCamRight().x, 0, camera.getCamRight().z); }
	return move;
}

glm::vec2 Player::GetViewChange()
{
	glm::vec2 view(camera.yaw, camera.pitch);
	float turnRate = 5;

	if (GetKeyState(VK_UP) & 0x8000) { view.y -= turnRate*0.5; }
	if (GetKeyState(VK_DOWN) & 0x8000) { view.y += turnRate*0.5; }
	if (GetKeyState(VK_LEFT) & 0x8000) { view.x -= turnRate; }
	if (GetKeyState(VK_RIGHT) & 0x8000) { view.x += turnRate; }
	return view;
}

void Player::Update(std::vector<GameObj*> obstacles)
{
	glm::vec3 newPos = GetMovement();
	camera.setCamPos(camera.pos + newPos);

	glm::vec2 newDir = GetViewChange();
	camera.setCamDir(newDir.x, newDir.y);
}