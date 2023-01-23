#include "Player.hpp"


Player::Player(glm::vec2 xz, glm::vec2 yawPitch)
	: camera(glm::vec3(xz.x, -playerHeight, xz.y), fov, (float)Screen::GetInstance()->SCR_WIDTH / (float)Screen::GetInstance()->SCR_HEIGHT, yawPitch, nearClip, farClip)
{

}

Player::~Player()
{
	//honestly this is probably causing a memory leak at this point
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

	if (move == glm::vec3(0, 0, 0)) // this is here because since the move can be so small, if it isn't rounded down to 0 it crashes on glm::normalizea
		return glm::vec3(0, 0, 0);
	return glm::normalize(move);
}

glm::vec2 Player::GetViewChange() // it is a tradgedy that I couldn't get this to work with mouse inputs
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
	glm::vec3 newPos = GetMovement() * Screen::GetInstance()->GetDeltaTime(); // the deltatime ensures that you are going at the same speed at any FPS
	newPos = Sprinting(newPos);

	// checks if you are colliding om the x axis first, then the z axis
	if (CollideLevel(glm::vec3(camera.pos.x + newPos.x, camera.pos.y, camera.pos.z), Level) == false)
	{
		camera.setCamPos(glm::vec3(camera.pos.x + newPos.x, camera.pos.y, camera.pos.z));
	}
	if (CollideLevel(glm::vec3(camera.pos.x, camera.pos.y, camera.pos.z + newPos.z), Level) == false)
	{
		camera.setCamPos(glm::vec3(camera.pos.x, camera.pos.y, camera.pos.z + newPos.z));
	}

	// setting new direction of camera
	glm::vec2 newDir = GetViewChange();
	camera.setCamDir(newDir.x, newDir.y);
}

glm::vec3 Player::Sprinting(glm::vec3 move)
{
	// basically the stamina system is simple
	// you start with a certain amount, you lose faster than you regen when sprinting
	// and if you run out fully, you can't sprint again until its full

	// this controls the thing where you can't sprint until its full if you empty it
	if (stamina < 0)
	{
		tired = true;
		stamina = 0;
	}

	// this is the flag which ends the refill state
	else if (stamina > maxStamina)
	{
		stamina = maxStamina;
		tired = false;
	}

	// the get key state just ensures that you don't regen while sprinting
	if (!(GetKeyState(VK_SHIFT) & 0x8000) || tired == true)
	{
		stamina += staminaRegen;
	}

	// this is when you are actually sprinting, it increases your speed by a factor
	if (GetKeyState(VK_SHIFT) & 0x8000 && stamina > 0 && tired == false)
	{
		move *= sprintFactor;
		stamina -= staminaLoss;
	}

	return move;
}

bool Player::CollideLevel(glm::vec3 move, GameObj* Level) // this method checks collision on the player and the walls of the level
{
	glm::vec2 p1, p2, p3, p4;
	glm::vec2 newMove = glm::vec2(move.x, move.z);
	int levelSize = Level->size.x;
	int levelOffset = -Level->size.x;
	p1 = glm::vec2(levelOffset, levelOffset);
	p2 = glm::vec2(levelOffset, levelSize);
	p3 = glm::vec2(levelSize, levelSize);
	p4 = glm::vec2(levelSize, levelOffset);

	// all of these points are different corners on the level

	// these methods check if the player circles
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