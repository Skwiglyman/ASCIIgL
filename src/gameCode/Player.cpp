#include "Player.hpp"

Player::Player(glm::vec2 xz, glm::vec2 yawPitch)
	: camera(glm::vec3(xz.x, -playerHeight, xz.y), fov, (float)Screen::GetInstance()->SCR_WIDTH / (float)Screen::GetInstance()->SCR_HEIGHT, yawPitch, nearClip, farClip)
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

	if (move == glm::vec3(0, 0, 0))
		return glm::vec3(0, 0, 0);
	return glm::normalize(move);
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

void Player::Update(GameObj* Level)
{
	glm::vec3 newPos = GetMovement() * Screen::GetInstance()->GetDeltaTime();
	if (CollideLevel(glm::vec3(camera.pos.x + newPos.x, camera.pos.y, camera.pos.z), Level) == false)
	{
		camera.setCamPos(glm::vec3(camera.pos.x + newPos.x, camera.pos.y, camera.pos.z));
	}
	if (CollideLevel(glm::vec3(camera.pos.x, camera.pos.y, camera.pos.z + newPos.z), Level) == false)
	{
		camera.setCamPos(glm::vec3(camera.pos.x, camera.pos.y, camera.pos.z + newPos.z));
	}

	glm::vec2 newDir = GetViewChange();
	camera.setCamDir(newDir.x, newDir.y);
}

bool Player::CollideLevel(glm::vec3 move, GameObj* Level)
{
	glm::vec2 p1, p2, p3, p4;
	glm::vec2 newMove = glm::vec2(move.x, move.z);
	int levelSize = Level->size.x;
	int levelOffset = -Level->size.x;
	p1 = glm::vec2(levelOffset, levelOffset);
	p2 = glm::vec2(levelOffset, levelSize);
	p3 = glm::vec2(levelSize, levelSize);
	p4 = glm::vec2(levelSize, levelOffset);

	glm::vec3 col1 = ASCIIgLEngine::lineCircleCol2D(newMove, playerHitBoxRad, p1, p2);
	if (col1.x == 1)
		return true;

	glm::vec3 col2 = ASCIIgLEngine::lineCircleCol2D(newMove, playerHitBoxRad, p2, p3);
	if (col2.x == 1)
		return true;

	glm::vec3 col3 = ASCIIgLEngine::lineCircleCol2D(newMove, playerHitBoxRad, p3, p4);
	if (col3.x == 1)
		return true;

	glm::vec3 col4 = ASCIIgLEngine::lineCircleCol2D(newMove, playerHitBoxRad, p4, p1);
	if (col4.x == 1)
		return true;

	return false;
}