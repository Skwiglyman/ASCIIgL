#pragma once

// includes from downloaded libraries
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// includes from default c++ libraries
#include <Windows.h>
#include <functional>
#include <algorithm>
#include <vector>
#include <iostream>

// I don't even know what this does black magic screen shit
#define WIN32_LEAN_AND_MEAN

// includes from other project files
#include "Camera3D.hpp"

class Screen
{
private:
	HANDLE hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE); // handle thing (I got no idea what this is)

	COORD dwBufferSize; // size of buffer
	COORD dwBufferCoord; // starting coord of buffer
	SMALL_RECT rcRegion; // rectangle of coords of buffer { 0, 0, width - 1, height - 1 }

	static void vec4ToVert(glm::vec4 p, std::vector<float>* vert);
	static void vec3ToVert(glm::vec3 p, std::vector<float>* vert);
	static void vec2ToVert(glm::vec2 p, std::vector<float>* vert);
	
	std::vector<float> clipToScreen(std::vector<float> p);
	static glm::vec3 calcNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);


public:
	CHAR_INFO* pixelBuffer; // pixel output buffer

	int SCR_WIDTH, SCR_HEIGHT; // defining the width and height of the screen

	Screen(int width, int height); 
	~Screen();

	void ClearScreen(); // clears screen (does not clear buffer)
	void ClearBuffer(); // clears buffer
	void OutputBuffer(); // draws to screen
	void PlotPixel(glm::vec2 p); // plotting pixel onto screen buffer
	void DrawLine(int x1, int y1, int x2, int y2);
	
	template<typename T> void RenderTriangles(T& VERTEX_SHADER, std::vector<std::vector<float>> vertices) // this takes in a custom vertex shader by user, and vertices
	{
		// the vertices need to atleast have an xyz coords for anything to display, normals are not needed neccesarily, 1 2 3 are x y z
		std::vector<std::vector<float>> transformed; // holds all the transformed vertices after vertex shader runs and its in screen space

		for (int i = 0; i < vertices.size(); i++)
		{
			//transforming vertices using vertex shader
			std::vector<float> newVert = VERTEX_SHADER.Use(vertices[i]);

			// perspective division
			newVert[0] /= newVert[3];
			newVert[1] /= newVert[3];
			newVert[2] /= newVert[3];
			newVert[3] = 1.0f;

			// changing from clip space to screen space and adding it to transformed coords
			std::vector<float> screenCoords = clipToScreen(newVert);
			transformed.push_back(screenCoords);
		}

		// if there are enough info for xyz coords
		if (transformed.size() >= 3)
		{
			for (size_t i = 0; i < transformed.size(); i+=3) // loop through all triangles
			{
				// BACKFACE CULLING
				// calculate normals using vertices in WORLD SPACE, NOT VIEW
				glm::vec3 p1 = glm::vec3(VERTEX_SHADER.model * glm::vec4(vertices[i][0], vertices[i][1], vertices[i][2], 1.0f));
				glm::vec3 p2 = glm::vec3(VERTEX_SHADER.model * glm::vec4(vertices[i + 1][0], vertices[i + 1][1], vertices[i + 1][2], 1.0f));
				glm::vec3 p3 = glm::vec3(VERTEX_SHADER.model * glm::vec4(vertices[i + 2][0], vertices[i + 2][1], vertices[i + 2][2], 1.0f));

				glm::vec3 normal = calcNormal(p1, p2, p3); // Getting normal of surface triangle
				glm::vec3 cameraRay = p1 - VERTEX_SHADER.camera->pos; // ITS WORLD SPACE BECAUSE CAM POS IS IN WORLD SPACE

				if (glm::dot(normal, cameraRay) > 0.0f) // if the surface is visible, draw
				{
					// drawing lines between vertices
					DrawLine(transformed[i][0], transformed[i][1], transformed[i + 1][0], transformed[i + 1][1]);
					DrawLine(transformed[i + 1][0], transformed[i + 1][1], transformed[i + 2][0], transformed[i + 2][1]);
					DrawLine(transformed[i + 2][0], transformed[i + 2][1], transformed[i][0], transformed[i][1]);
				}
			}
		}

		// drawing borders
		DrawLine(0, 0, SCR_WIDTH - 1, 0);
		DrawLine(SCR_WIDTH - 1, 0, SCR_WIDTH - 1, SCR_HEIGHT - 1);
		DrawLine(SCR_WIDTH - 1, SCR_HEIGHT - 1, 0, SCR_HEIGHT - 1);
		DrawLine(0, 0, 0, SCR_HEIGHT - 1);
	}

	typedef struct Def_Vertex_Shader
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
		Camera3D* camera;

		std::vector<float> Use(std::vector<float> vertice)
		{
			// 0 1 2 3 xyzw coords
			// 4 5 6 normals

			// output vertice
			std::vector<float> newVert;

			// calculations
			glm::vec4 newPos = proj * view * model * glm::vec4(vertice[0], vertice[1], vertice[2], 1.0f);
			//glm::vec3 newNormal = glm::vec4(vertice[4], vertice[5], vertice[6], 0.0f);

			// adding new components to vertice
			vec4ToVert(newPos, &newVert);
			//vec3ToVert(newNormal, &newVert);

			return newVert;
		}
	} DEF_VERTEX_SHADER;
};