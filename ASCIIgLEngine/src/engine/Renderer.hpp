#pragma once

// includes from downloaded libraries
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

// includes from game engine code
#include "Mesh.hpp"
#include "Model.hpp"
#include "Camera3D.hpp"
#include "Camera2D.hpp"

// includes from rendering code
#include "..\renderer\VertexShader.hpp"
#include "..\renderer\Screen.hpp"

#include "engine/Logger.hpp"


class Renderer // this class ust holds all the methods for easy renderings 
{
private:

public:
	// draws mesh specifically for model
	static const void DrawMeshForModel(VERTEX_SHADER VSHADER, Mesh* mesh)
	{
		for (int i = 0; i < mesh->textures.size(); i++)
		{
			if (mesh->textures[i]->texType == "texture_diffuse")
			{
				Screen::GetInstance()->RenderTriangles(VSHADER, mesh->vertices, mesh->textures[i]);
			}
		}
	}

	// draws mesh on its own without a model
	static const void DrawMesh(VERTEX_SHADER VSHADER, Mesh* mesh, glm::vec3 position, glm::vec2 rotation, glm::vec3 size, Camera3D& camera)
	{
		glm::mat4 model = ASCIIgLEngine::CalcModelMatrix(position, rotation, size);

		VSHADER.GLmodel = model;
		VSHADER.GLview = camera.view;
		VSHADER.GLproj = camera.proj;

		for (int i = 0; i < mesh->textures.size(); i++)
		{
			if (mesh->textures[i]->texType == "texture_diffuse")
			{
				Screen::GetInstance()->RenderTriangles(VSHADER, mesh->vertices, mesh->textures[i]);
				return;
			}
		}
	}

	// draws model given transformations and the camera
	static const void DrawModel(VERTEX_SHADER VSHADER, Model& ModelObj, glm::vec3 position, glm::vec2 rotation, glm::vec3 size, Camera3D& camera)
	{
		glm::mat4 model = ASCIIgLEngine::CalcModelMatrix(position, rotation, size);

		VSHADER.GLmodel = model;
		VSHADER.GLview = camera.view;
		VSHADER.GLproj = camera.proj;

		for (size_t i = 0; i < ModelObj.meshes.size(); i++)
		{
			DrawMeshForModel(VSHADER, ModelObj.meshes[i]);
		}
	}

	// draws model given a 3d camera and model matrix
	static const void DrawModel(VERTEX_SHADER VSHADER, Model& ModelObj, glm::mat4 model, Camera3D& camera)
	{
		VSHADER.GLmodel = model;
		VSHADER.GLview = camera.view;
		VSHADER.GLproj = camera.proj;

		for (size_t i = 0; i < ModelObj.meshes.size(); i++)
		{
			DrawMeshForModel(VSHADER, ModelObj.meshes[i]);
		}
	}


	// this function just draws a texture as a 2d gui element, layer dos not work right now
	static const void Draw2DQuad(VERTEX_SHADER VSHADER, Texture& tex, glm::vec2 position, glm::vec2 rotation, glm::vec2 size, Camera2D& camera, int layer)
	{
		Logger::Debug("[DEBUG] Draw2DQuad called. Position: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ") Rotation: (" + std::to_string(rotation.x) + ", " + std::to_string(rotation.y) + ") Size: (" + std::to_string(size.x) + ", " + std::to_string(size.y) + ") Layer: " + std::to_string(layer));
		glm::mat4 model = ASCIIgLEngine::CalcModelMatrix(glm::vec3(position, layer), rotation, glm::vec3(size, 0.0f));

		VSHADER.GLmodel = model;
		VSHADER.GLview = camera.view;
		VSHADER.GLproj = camera.proj;

		Logger::Debug("[DEBUG] Creating quad vertices.");
		std::vector<VERTEX> vertices =
		{
			VERTEX({ -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
			VERTEX({ -1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
			VERTEX({  1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
			VERTEX({  1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
			VERTEX({  1.0f, -1.0f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
			VERTEX({ -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
		};

		Logger::Debug("[DEBUG] Rendering triangles for 2D quad.");
		Screen::GetInstance()->RenderTriangles(VSHADER, vertices, &tex);
	}
};