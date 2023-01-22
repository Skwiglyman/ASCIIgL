

#include "Game.hpp"

int main()
{
	//std::vector<VERTEX> vertices {
	//	VERTEX({ -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // Bottom-left
	//	VERTEX({  1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
	//	VERTEX({  1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
	//	VERTEX({  1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
	//	VERTEX({ -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
	//	VERTEX({ -1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
	//												 
	//	VERTEX({ -1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
	//	VERTEX({  1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
	//	VERTEX({  1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
	//	VERTEX({  1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
	//	VERTEX({ -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
	//	VERTEX({ -1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
	//		 										 
	//	VERTEX({ -1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
	//	VERTEX({ -1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
	//	VERTEX({ -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
	//	VERTEX({ -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
	//	VERTEX({ -1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
	//	VERTEX({ -1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
	// 												 
	//	VERTEX({  1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
	//	VERTEX({  1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
	//	VERTEX({  1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right         
	//	VERTEX({  1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
	//	VERTEX({  1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
	//	VERTEX({  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left     
	//												 
	//	VERTEX({ -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
	//	VERTEX({  1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
	//	VERTEX({  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
	//	VERTEX({  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
	//	VERTEX({ -1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
	//	VERTEX({ -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
	//												 
	//	VERTEX({ -1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
	//	VERTEX({  1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
	//	VERTEX({  1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right     
	//	VERTEX({  1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
	//	VERTEX({ -1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
	//	VERTEX({ -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left     
	//};

	Game::GetInstance()->Run();
	

}