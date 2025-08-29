#pragma once

// includes from downloaded libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp> // for glm::to_string

// includes from default c++ libraries
#include <vector>
#include <iostream>

#include <ASCIIgL/renderer/Vertex.hpp>

#include <ASCIIgL/engine/Logger.hpp>

typedef struct Vertex_Shader {
    glm::mat4 GLmodel = glm::mat4(1.0f);
    glm::mat4 GLview = glm::mat4(1.0f);
    glm::mat4 GLproj = glm::mat4(1.0f);
    glm::mat4 GLmvp = glm::mat4(1.0f); // precomputed MVP matrix

    // Internal helper to update the MVP matrix
    void UpdateMVP() {
        GLmvp = GLproj * GLview * GLmodel;
    }

    // Getters
    const glm::mat4& GetModel() const { return GLmodel; }
    const glm::mat4& GetView()  const { return GLview; }
    const glm::mat4& GetProj()  const { return GLproj; }
    const glm::mat4& GetMVP()   const { return GLmvp; }

    // Setters
    void SetModel(const glm::mat4& model) { GLmodel = model; UpdateMVP(); }
    void SetView(const glm::mat4& view)   { GLview  = view;  UpdateMVP(); }
    void SetProj(const glm::mat4& proj)   { GLproj  = proj;  UpdateMVP(); }

    void GLUse(VERTEX& vertice) const {
        vertice.SetXYZW(GLmvp * vertice.GetXYZW());
    }
} VERTEX_SHADER;