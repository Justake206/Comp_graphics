#pragma once

#include <glad/glad.h>
#include <string>

class Texture {
public:
    Texture();
    ~Texture();
    
    bool loadFromFile(const std::string& path);
    void bind(unsigned int unit = 0);
    void unbind();
    
    unsigned int getId() const { return m_id; }
    
private:
    unsigned int m_id;
    int m_width, m_height, m_channels;
};