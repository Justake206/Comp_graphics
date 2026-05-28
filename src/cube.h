// ============================================================
// ЗАДАНИЕ 2: МОДЕЛЬ КУБА
// ============================================================

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

// Структура вершины с позицией, нормалью, UV и касательной
struct Vertex {
    glm::vec3 position;   // Позиция в 3D
    glm::vec3 normal;     // Нормаль к поверхности
    glm::vec2 texCoord;   // ЗАДАНИЕ 2: UV-координаты для текстуры
    glm::vec3 tangent;    // ЗАДАНИЕ 4: Касательная для normal mapping
};

class Cube {
public:
    Cube();
    void setupMesh();
    void draw();
    void clear();
    
    static std::vector<Vertex> getVertices();
    static std::vector<unsigned int> getIndices();
    static void computeTangents(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    
private:
    unsigned int VAO, VBO, EBO;
    int m_indexCount;
};