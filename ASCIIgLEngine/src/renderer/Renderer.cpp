#include "Renderer.hpp"

#include "engine/Logger.hpp"
#include "Screen.hpp"
#include "RenderEnums.hpp"

void Renderer::DrawMesh(VERTEX_SHADER& VSHADER, const Mesh* mesh) {
    for (int i = 0; i < mesh->textures.size(); i++) {
        if (mesh->textures[i]->texType == "texture_diffuse") {
            RenderTriangles(VSHADER, mesh->vertices, mesh->textures[i]);
        }
    }
}

void Renderer::DrawMesh(VERTEX_SHADER& VSHADER, const Mesh* mesh, const glm::vec3 position, const glm::vec2 rotation, const glm::vec3 size, const Camera3D& camera) {
    glm::mat4 model = CalcModelMatrix(position, rotation, size);

    VSHADER.SetModel(model);
    VSHADER.SetView(camera.view);
    VSHADER.SetProj(camera.proj);

    for (int i = 0; i < mesh->textures.size(); i++) {
        if (mesh->textures[i]->texType == "texture_diffuse") {
            RenderTriangles(VSHADER, mesh->vertices, mesh->textures[i]);
        }
    }
}

void Renderer::DrawModel(VERTEX_SHADER& VSHADER, const Model& ModelObj, const glm::vec3 position, const glm::vec2 rotation, const glm::vec3 size, const Camera3D& camera) {
    glm::mat4 model = CalcModelMatrix(position, rotation, size);

    VSHADER.SetModel(model);
    VSHADER.SetView(camera.view);
    VSHADER.SetProj(camera.proj);

    for (size_t i = 0; i < ModelObj.meshes.size(); i++) {
        DrawMesh(VSHADER, ModelObj.meshes[i]);
    }
}


void Renderer::DrawModel(VERTEX_SHADER& VSHADER, const Model& ModelObj, const glm::mat4 model, const Camera3D& camera)
{
    VSHADER.SetModel(model);
    VSHADER.SetView(camera.view);
    VSHADER.SetProj(camera.proj);

    for (size_t i = 0; i < ModelObj.meshes.size(); i++) {
        DrawMesh(VSHADER, ModelObj.meshes[i]);
    }
}

void Renderer::Draw2DQuad(VERTEX_SHADER& VSHADER, const Texture& tex, const glm::vec2 position, const glm::vec2 rotation, const glm::vec2 size, const Camera2D& camera, int layer)
{
    glm::mat4 model = CalcModelMatrix(glm::vec3(position, layer), rotation, glm::vec3(size, 0.0f));

    VSHADER.SetModel(model);
    VSHADER.SetView(camera.view);
    VSHADER.SetProj(camera.proj);

    std::vector<VERTEX> vertices = {
        VERTEX({ -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
        VERTEX({ -1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f}), // top-left
        VERTEX({  1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
        VERTEX({  1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f}), // top-right
        VERTEX({  1.0f, -1.0f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f}), // bottom-right
        VERTEX({ -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f}), // bottom-left
    };

    Renderer::RenderTriangles(VSHADER, vertices, &tex);
}

void Renderer::RenderTriangles(const VERTEX_SHADER& VSHADER, const std::vector<VERTEX>& vertices, const Texture* tex) {
    if (vertices.size() < 3 || vertices.size() % 3 != 0) {
        return;
    }

    std::vector<VERTEX> verticesCopy = vertices;

    for (int k = 0; k < verticesCopy.size(); k++) { 
        VSHADER.GLUse(verticesCopy[k]); 
    }

    std::vector<VERTEX> CLIPPED_COORDS;
    ClippingHelper(verticesCopy, CLIPPED_COORDS);
    for (int i = 0; i < CLIPPED_COORDS.size(); i += 3)
    {
        for (int k = 0; k < 3; k++)
        {
            PerspectiveDivision(CLIPPED_COORDS[i + k]);
            ViewPortTransform(CLIPPED_COORDS[i + k]);
        }
        bool cull = (BACKFACECULLING == true ? BackFaceCull(CLIPPED_COORDS[i], CLIPPED_COORDS[i + 1], CLIPPED_COORDS[i + 2], CCW) : true);
        if (cull)
        {
            if (WIREFRAME == true || tex == nullptr) {
                DrawTriangleWireFrame(CLIPPED_COORDS[i], CLIPPED_COORDS[i + 1], CLIPPED_COORDS[i + 2], PIXEL_SOLID, FG_WHITE);
            }
            else {
                DrawTriangleTextured(CLIPPED_COORDS[i], CLIPPED_COORDS[i + 1], CLIPPED_COORDS[i + 2], tex);
            }
        }
    }
}

void Renderer::ViewPortTransform(VERTEX& vertice) {
	// transforms vertice from [-1 to 1] to [0 to scr_dim]
	glm::vec4 newPos = glm::vec4(((vertice.X() + 1.0f) / 2.0f) * Screen::GetInstance().GetWidth(), ((vertice.Y() + 1.0f) / 2.0f) * Screen::GetInstance().GetHeight(), vertice.Z(), vertice.W());
	vertice.SetXYZW(newPos);
}

void Renderer::DrawLine(const int x1, const int y1, const int x2, const int y2, const CHAR pixel_type, const short col) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int incx = (x2 > x1) ? 1 : -1;
    int incy = (y2 > y1) ? 1 : -1;
    int x = x1, y = y1;

    Screen::GetInstance().PlotPixel(x, y, pixel_type, col);

    if (dx > dy) {
        int e = 2 * dy - dx;
        for (int i = 0; i < dx; ++i) {
            x += incx;
            if (e >= 0) {
                y += incy;
                e += 2 * (dy - dx);
            } else {
                e += 2 * dy;
            }
            Screen::GetInstance().PlotPixel(x, y, pixel_type, col);
        }
    } else {
        int e = 2 * dx - dy;
        for (int i = 0; i < dy; ++i) {
            y += incy;
            if (e >= 0) {
                x += incx;
                e += 2 * (dx - dy);
            } else {
                e += 2 * dx;
            }
            Screen::GetInstance().PlotPixel(x, y, pixel_type, col);
        }
    }
}

void Renderer::DrawTriangleWireFrame(const VERTEX& v1, const VERTEX& v2, const VERTEX& v3, CHAR pixel_type, short col) {
	// RENDERING LINES BETWEEN VERTICES
	DrawLine((int) v1.X(), (int) v1.Y(), (int) v2.X(), (int) v2.Y(), pixel_type, col);
	DrawLine((int) v2.X(), (int) v2.Y(), (int) v3.X(), (int) v3.Y(), pixel_type, col);
	DrawLine((int) v3.X(), (int) v3.Y(), (int) v1.X(), (int) v1.Y(), pixel_type, col);
}

void Renderer::DrawTriangleTextured(const VERTEX& vert1, const VERTEX& vert2, const VERTEX& vert3, const Texture* tex) {
    int texWidth = tex->GetWidth();
    int texHeight = tex->GetHeight();
    if (texWidth == 0 || texHeight == 0)
    {
        Logger::Error("Invalid texture size. Width: " + std::to_string(texWidth) + ", Height: " + std::to_string(texHeight));
        return;
    }

    int x1 = vert1.X(), x2 = vert2.X(), x3 = vert3.X();
    int y1 = vert1.Y(), y2 = vert2.Y(), y3 = vert3.Y();
    float w1 = vert1.UVW(), w2 = vert2.UVW(), w3 = vert3.UVW();
    float u1 = vert1.U(), u2 = vert2.U(), u3 = vert3.U();
    float v1 = vert1.V(), v2 = vert2.V(), v3 = vert3.V();

    // Sort vertices by y
    if (y2 < y1) { std::swap(y1, y2); std::swap(x1, x2); std::swap(u1, u2); std::swap(v1, v2); std::swap(w1, w2); }
    if (y3 < y1) { std::swap(y1, y3); std::swap(x1, x3); std::swap(u1, u3); std::swap(v1, v3); std::swap(w1, w3); }
    if (y3 < y2) { std::swap(y2, y3); std::swap(x2, x3); std::swap(u2, u3); std::swap(v2, v3); std::swap(w2, w3); }

    // Precompute steps
    int dy1 = y2 - y1, dx1 = x2 - x1;
    int dy2 = y3 - y1, dx2 = x3 - x1;
    float du1 = u2 - u1, dv1 = v2 - v1, dw1 = w2 - w1;
    float du2 = u3 - u1, dv2 = v3 - v1, dw2 = w3 - w1;

    float dax_step = dy1 ? dx1 / (float)abs(dy1) : 0;
    float dbx_step = dy2 ? dx2 / (float)abs(dy2) : 0;
    float du1_step = dy1 ? du1 / (float)abs(dy1) : 0;
    float dv1_step = dy1 ? dv1 / (float)abs(dy1) : 0;
    float dw1_step = dy1 ? dw1 / (float)abs(dy1) : 0;
    float du2_step = dy2 ? du2 / (float)abs(dy2) : 0;
    float dv2_step = dy2 ? dv2 / (float)abs(dy2) : 0;
    float dw2_step = dy2 ? dw2 / (float)abs(dy2) : 0;

    // Upper part
    if (dy1)
    {
        for (int i = y1; i <= y2 && i < Screen::GetInstance().GetHeight(); i++)
        {
            int ax = x1 + (float)(i - y1) * dax_step;
            int bx = x1 + (float)(i - y1) * dbx_step;
            float tex_su = u1 + (float)(i - y1) * du1_step;
            float tex_sv = v1 + (float)(i - y1) * dv1_step;
            float tex_sw = w1 + (float)(i - y1) * dw1_step;
            float tex_eu = u1 + (float)(i - y1) * du2_step;
            float tex_ev = v1 + (float)(i - y1) * dv2_step;
            float tex_ew = w1 + (float)(i - y1) * dw2_step;
            if (ax > bx) { std::swap(ax, bx); std::swap(tex_su, tex_eu); std::swap(tex_sv, tex_ev); std::swap(tex_sw, tex_ew); }
            float tstep = (bx != ax) ? 1.0f / ((float)(bx - ax)) : 0.0f;
            float t = 0.0f;
            for (int j = ax; j < bx && j < Screen::GetInstance().GetWidth(); j++)
            {
                float tex_w = (1.0f - t) * tex_sw + t * tex_ew;
                if (tex_w > Screen::GetInstance().depthBuffer[i * Screen::GetInstance().GetWidth() + j]) {
                    float tex_uw = ((1.0f - t) * tex_su + t * tex_eu) / tex_w;
                    float tex_vw = ((1.0f - t) * tex_sv + t * tex_ev) / tex_w;
                    float texWidthProd = tex_uw * texWidth;
                    float texHeightProd = tex_vw * texHeight;
                    if (tex_uw < 1 && tex_vw < 1) {
                        float blendedGrayScale = tex->GetPixelCol(texWidthProd, texHeightProd);
                        Screen::GetInstance().PlotPixel(glm::vec2(j, i), GetColGlyph(blendedGrayScale));
                        Screen::GetInstance().depthBuffer[i * Screen::GetInstance().GetWidth() + j] = tex_w;
                    }
                }
                t += tstep;
            }
        }
    }

    // Lower part
    dy1 = y3 - y2; dx1 = x3 - x2;
    du1 = u3 - u2; dv1 = v3 - v2; dw1 = w3 - w2;
    dax_step = dy1 ? dx1 / (float)abs(dy1) : 0;
    dbx_step = dy2 ? dx2 / (float)abs(dy2) : 0;
    du1_step = dy1 ? du1 / (float)abs(dy1) : 0;
    dv1_step = dy1 ? dv1 / (float)abs(dy1) : 0;
    dw1_step = dy1 ? dw1 / (float)abs(dy1) : 0;
    if (dy1)
    {
        for (int i = y2; i <= y3 && i < Screen::GetInstance().GetHeight(); i++)
        {
            int ax = x2 + (float)(i - y2) * dax_step;
            int bx = x1 + (float)(i - y1) * dbx_step;
            float tex_su = u2 + (float)(i - y2) * du1_step;
            float tex_sv = v2 + (float)(i - y2) * dv1_step;
            float tex_sw = w2 + (float)(i - y2) * dw1_step;
            float tex_eu = u1 + (float)(i - y1) * du2_step;
            float tex_ev = v1 + (float)(i - y1) * dv2_step;
            float tex_ew = w1 + (float)(i - y1) * dw2_step;
            if (ax > bx) { std::swap(ax, bx); std::swap(tex_su, tex_eu); std::swap(tex_sv, tex_ev); std::swap(tex_sw, tex_ew); }
            float tstep = (bx != ax) ? 1.0f / ((float)(bx - ax)) : 0.0f;
            float t = 0.0f;
            for (int j = ax; j < bx && j < Screen::GetInstance().GetWidth(); j++)
            {
                float tex_w = (1.0f - t) * tex_sw + t * tex_ew;
                if (tex_w > Screen::GetInstance().depthBuffer[i * Screen::GetInstance().GetWidth() + j])
                {
                    float tex_uw = ((1.0f - t) * tex_su + t * tex_eu) / tex_w;
                    float tex_vw = ((1.0f - t) * tex_sv + t * tex_ev) / tex_w;
                    float texWidthProd = tex_uw * texWidth;
                    float texHeightProd = tex_vw * texHeight;
                    if (tex_uw < 1 && tex_vw < 1) {
                        float blendedGrayScale = tex->GetPixelCol(texWidthProd, texHeightProd);
                        Screen::GetInstance().PlotPixel(glm::vec2(j, i), GetColGlyph(blendedGrayScale));
                        Screen::GetInstance().depthBuffer[i * Screen::GetInstance().GetWidth() + j] = tex_w;
                    }
                }
                t += tstep;
            }
        }
    }
}

void Renderer::DrawScreenBorder(const short col) {
	// DRAWING BORDERS
	DrawLine(1, 1, Screen::GetInstance().GetWidth() - 1, 1, PIXEL_SOLID, col);
	DrawLine(Screen::GetInstance().GetWidth() - 1, 1, Screen::GetInstance().GetWidth() - 1, Screen::GetInstance().GetHeight() - 1, PIXEL_SOLID, col);
	DrawLine(Screen::GetInstance().GetWidth() - 1, Screen::GetInstance().GetHeight() - 1, 1, Screen::GetInstance().GetHeight() - 1, PIXEL_SOLID, col);
	DrawLine(1, 1, 1, Screen::GetInstance().GetHeight() - 1, PIXEL_SOLID, col);
}

void Renderer::Clipping(const std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped, const int component, const bool Near) {
    for (int i = 0; i < static_cast<int>(vertices.size()); i += 3)
    {
        std::vector<int> inside;
        std::vector<int> outside;

        VERTEX temp[6];
        int newTri = 0;

        // Use direct array indexing for w and component
        auto isInside = [&](const VERTEX& v) {
            float w = v.W();
            float val = v.data[component];
            return Near ? (val > -w) : (val < w);
        };

        if (isInside(vertices[i]))      inside.push_back(i);      else outside.push_back(i);
        if (isInside(vertices[i + 1]))  inside.push_back(i + 1);  else outside.push_back(i + 1);
        if (isInside(vertices[i + 2]))  inside.push_back(i + 2);  else outside.push_back(i + 2);

        if (inside.size() == 3)
        {
            clipped.push_back(vertices[inside[0]]);
            clipped.push_back(vertices[inside[1]]);
            clipped.push_back(vertices[inside[2]]);
            continue;
        }
        else if (inside.size() == 1)
        {
            VERTEX newPos1 = HomogenousPlaneIntersect(vertices[inside[0]], vertices[outside[0]], component, Near);
            VERTEX newPos2 = HomogenousPlaneIntersect(vertices[inside[0]], vertices[outside[1]], component, Near);

            temp[outside[0] - i] = newPos1;
            temp[outside[1] - i] = newPos2;
            temp[inside[0] - i] = vertices[inside[0]];
            newTri = 3;
        }
        else if (inside.size() == 2)
        {
            VERTEX newPos1 = HomogenousPlaneIntersect(vertices[inside[0]], vertices[outside[0]], component, Near);
            VERTEX newPos2 = HomogenousPlaneIntersect(vertices[inside[1]], vertices[outside[0]], component, Near);

            // triangle 1
            temp[inside[0] - i] = vertices[inside[0]];
            temp[inside[1] - i] = vertices[inside[1]];
            temp[outside[0] - i] = newPos1;

            // triangle 2
            temp[outside[0] - i + 3] = newPos2;
            temp[inside[0] - i + 3] = newPos1;
            temp[inside[1] - i + 3] = vertices[inside[1]];
            newTri = 6;
        }
        // If all outside, do nothing

        for (int k = 0; k < newTri; k++)
            clipped.push_back(temp[k]);
    }
}

void Renderer::ClippingHelper(const std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped) {
	std::vector<VERTEX> c1, c2, c3, c4, c5;
	// calls clipping on every clip plane
	Clipping(vertices, c1, 2, true);
	Clipping(c1, c2, 2, false);
	Clipping(c2, c3, 1, true);
	Clipping(c3, c4, 1, false);
	Clipping(c4, c5, 0, true);
	Clipping(c5, clipped, 0, false);
}

CHAR_INFO Renderer::GetColGlyph(const float GreyScale) {
	// returns colour combination at the fastest possible speed using gray scale value
	static const CHAR_INFO vals[16]
	{
		CHAR_INFO{ PIXEL_QUARTER, FG_BLACK}, CHAR_INFO{ PIXEL_QUARTER, FG_DARK_GREY},
		CHAR_INFO{ PIXEL_QUARTER, FG_GREY}, CHAR_INFO{ PIXEL_QUARTER, FG_WHITE},

		CHAR_INFO{ PIXEL_HALF, FG_BLACK}, CHAR_INFO{ PIXEL_HALF, FG_DARK_GREY},
		CHAR_INFO{ PIXEL_HALF, FG_GREY}, CHAR_INFO{ PIXEL_HALF, FG_WHITE},

		CHAR_INFO{ PIXEL_THREEQUARTERS, FG_BLACK}, CHAR_INFO{ PIXEL_THREEQUARTERS, FG_DARK_GREY},
		CHAR_INFO{ PIXEL_THREEQUARTERS, FG_GREY}, CHAR_INFO{ PIXEL_THREEQUARTERS, FG_WHITE},

		CHAR_INFO{ PIXEL_SOLID, FG_BLACK}, CHAR_INFO{ PIXEL_SOLID, FG_DARK_GREY},
		CHAR_INFO{ PIXEL_SOLID, FG_GREY}, CHAR_INFO{ PIXEL_SOLID, FG_WHITE},
	};

	return vals[static_cast<long>(std::min(GreyScale * 15.99f, 16.0f))]; // has to be 15.99 or it overflows
}

float Renderer::GrayScaleRGB(const glm::vec3 rgb)
{
	return (0.3 * rgb.x + 0.6 * rgb.y + 0.1 * rgb.z); // grayscales based on how much we see that wavelength of light instead of just averaging
}

bool Renderer::BackFaceCull(const VERTEX& v1, const VERTEX& v2, const VERTEX& v3, bool CCW) { // determines if the triangle is in the correct winding order or not

	// it calculates it based on glm cross because if the perpendicular z is less than 0, the triangle is pointing away
	glm::vec3 U = v2.GetXYZ() - v1.GetXYZ();
	glm::vec3 V = v3.GetXYZ() - v1.GetXYZ();

	float crossZ;

	if (CCW == true)
		crossZ = glm::cross(U, V).z;
	else
		crossZ = glm::cross(V, U).z;

	return crossZ > 0.0f;
}

void Renderer::PerspectiveDivision(VERTEX& clipCoord) {
    // Divide position and UVW by w for perspective correction
    float w = clipCoord.W();
    clipCoord.data[0] /= w; // x
    clipCoord.data[1] /= w; // y
    clipCoord.data[2] /= w; // z
    clipCoord.data[3] = w;  // w stays the same

    glm::vec3 uvw = clipCoord.GetUVW();
    clipCoord.SetUVW(glm::vec3(uvw.x / w, uvw.y / w, 1.0f / w));
}

VERTEX Renderer::HomogenousPlaneIntersect(const VERTEX& v2, const VERTEX& v1, const int component, const bool Near) {
    // Interpolates on the line between both vertices to get a new point on the line between them
    // v2 is the vertex that is actually visible, v1 is behind the near plane

    VERTEX newVert = v1;
    VERTEX v = v1;

    v.SetXYZW(v1.GetXYZW() - v2.GetXYZW());

    float i0 = v1.data[component];
    float w0 = v1.data[3]; // w is at index 3

    float vi = v.data[component];
    float vw = v.data[3];

    float t;
    if (Near)
        t = (i0 + w0) / (vw + vi); // near clipping
    else
        t = (i0 - w0) / (vi - vw); // far clipping

    for (int i = 0; i < 6; i++)
        newVert.data[i] = glm::mix(v1.data[i], v2.data[i], t); // interpolate attributes

    return newVert;
}

glm::mat4 Renderer::CalcModelMatrix(const glm::vec3 position, const glm::vec2 rotation, const glm::vec3 size) {
	// this function just saves lines as this needs to be calculated alot
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z));
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z));
	model = glm::scale(model, size);

	return model;
}