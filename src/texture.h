// ============================================================
// ЗАДАНИЕ 1: ЗАГРУЗКА ТЕКСТУР (stb_image)
// ============================================================

#pragma once

#include <glad/glad.h>
#include <string>

class Texture {
public:
    Texture();
    ~Texture();
    
    // Загрузка текстуры из файла (использует stb_image)
    bool loadFromFile(const std::string& path);
    
    // Привязка текстуры к указанному текстурному юниту
    void bind(unsigned int unit = 0);
    void unbind();
    
    unsigned int getId() const { return m_id; }
    
private:
    unsigned int m_id;
    int m_width, m_height, m_channels;
};