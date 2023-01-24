#pragma once

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

#include "Mesh.hpp"
#include "Model.hpp"
#include "Camera3D.hpp"

#include "..\renderingEngineCode\VertexShader.hpp"
#include "..\renderingEngineCode\Screen.hpp"


class Renderer
{
private:

public:
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

	static const void Draw2DQuad(VERTEX_SHADER VSHADER, Texture& tex, glm::vec2 position, glm::vec2 rotation, glm::vec2 size, Camera2D& camera, int layer)
	{
		glm::mat4 model = ASCIIgLEngine::CalcModelMatrix(glm::vec3(position, layer), rotation, glm::vec3(size, 0.0f));

		VSHADER.GLmodel = model;
		VSHADER.GLview = camera.view;
		VSHADER.GLproj = camera.proj;

		std::vector<VERTEX> vertices =
		{
			VERTEX({ -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
			VERTEX({ -1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
			VERTEX({  1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
			VERTEX({  1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
			VERTEX({  1.0f, -1.0f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
			VERTEX({ -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
		};

		Screen::GetInstance()->RenderTriangles(VSHADER, vertices, &tex);
	}
};