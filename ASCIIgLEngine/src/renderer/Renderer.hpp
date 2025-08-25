#pragma once

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

#include "../engine/Mesh.hpp"
#include "../engine/Model.hpp"
#include "../engine/Texture.hpp"
#include "../engine/Camera3D.hpp"
#include "../engine/Camera2D.hpp"
#include "VertexShader.hpp"
#include "Screen.hpp"

class Renderer
{
private:
    Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

public:
    static Renderer& GetInstance() {
        static Renderer instance;
        return instance;
    }

    static void DrawMesh(VERTEX_SHADER& VSHADER, const Mesh* mesh);
    static void DrawMesh(VERTEX_SHADER& VSHADER, const Mesh* mesh, const glm::vec3 position, const glm::vec2 rotation, const glm::vec3 size, const Camera3D& camera);
    static void DrawModel(VERTEX_SHADER& VSHADER, const Model& ModelObj, const glm::vec3 position, const glm::vec2 rotation, const glm::vec3 size, const Camera3D& camera);
    static void DrawModel(VERTEX_SHADER& VSHADER, const Model& ModelObj, const glm::mat4 model, const Camera3D& camera);
    static void Draw2DQuad(VERTEX_SHADER& VSHADER, const Texture& tex, const glm::vec2 position, const glm::vec2 rotation, const glm::vec2 size, const Camera2D& camera, int layer);
    static void DrawScreenBorder(short col);

    static void RenderTriangles(const VERTEX_SHADER& VSHADER, const std::vector<VERTEX>& vertices, const Texture* tex);
    static void DrawLine(int x1, int y1, int x2, int y2, CHAR pixel_type, short col);
    static void DrawTriangleWireFrame(const VERTEX& v1, const VERTEX& v2, const VERTEX& v3, CHAR pixel_type, short col);
    static void DrawTriangleTextured(const VERTEX& vert1, const VERTEX& vert2, const VERTEX& vert3, const Texture* tex);

    static VERTEX HomogenousPlaneIntersect(const VERTEX& v2, const VERTEX& v1, const int component, const bool Near);
    static std::vector<VERTEX> Clipping(const std::vector<VERTEX>& vertices, const int component, const bool Near);
    static void PerspectiveDivision(VERTEX& clipCoord);
    static void ClippingHelper(const std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped);
    static bool BackFaceCull(const VERTEX& v1, const VERTEX& v2, const VERTEX& v3, bool CCW);
    static void ViewPortTransform(VERTEX& vertice);
    static glm::mat4 CalcModelMatrix(const glm::vec3 position, const glm::vec2 rotation, const glm::vec3 size);

    static float GrayScaleRGB(const glm::vec3 rgb);
    static CHAR_INFO GetColGlyph(const float GreyScale);

    inline static bool WIREFRAME = false;
    inline static bool BACKFACECULLING = true;
    inline static bool CCW = false;
};