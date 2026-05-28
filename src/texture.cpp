// ============================================================
// ЗАДАНИЕ 1: ЗАГРУЗКА ТЕКСТУР ЧЕРЕЗ stb_image
// ============================================================

#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

Texture::Texture() : m_id(0), m_width(0), m_height(0), m_channels(0) {}

Texture::~Texture() {
    if (m_id) glDeleteTextures(1, &m_id);
}

// ============================================================
// ЗАГРУЗКА ТЕКСТУРЫ ИЗ ФАЙЛА
// Шаг 1: stbi_load() - загружает изображение в память
// Шаг 2: glTexImage2D() - передаёт данные в видеокарту
// Шаг 3: glGenerateMipmap() - ЗАДАНИЕ 5: создаёт уровни mipmap
// Шаг 4: настройка фильтров и режимов обёртки
// ============================================================
bool Texture::loadFromFile(const std::string& path) {
    // Переворачиваем изображение по вертикали (система координат OpenGL)
    stbi_set_flip_vertically_on_load(true);
    
    // ЗАДАНИЕ 1: Загрузка изображения через stb_image
    unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
    
    if (!data) {
        std::cerr << "Ошибка загрузки текстуры: " << path << std::endl;
        return false;
    }
    
    // Создание текстурного объекта OpenGL
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    
    // Определяем формат (RGB или RGBA)
    GLenum format = (m_channels == 4) ? GL_RGBA : GL_RGB;
    
    // ЗАДАНИЕ 1: Передача данных в видеокарту
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
    
    // ЗАДАНИЕ 5: ГЕНЕРАЦИЯ MIPMAP
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // Настройка параметров текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // ЗАДАНИЕ 5: Фильтр с mipmap (трилинейная интерполяция)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_image_free(data);
    return true;
}

void Texture::bind(unsigned int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}