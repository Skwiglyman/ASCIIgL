#include "../renderingEngineCode/Screen.hpp"
#include "../renderingEngineCode/VertexShader.hpp"
#include "../renderingEngineCode/Texture.hpp"

#include "Camera3D.hpp"
#include "Camera2D.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
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