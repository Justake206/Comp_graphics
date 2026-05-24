#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader {
public:
    Shader();
    ~Shader();
    
    bool load(const std::string& vertexPath, const std::string& fragmentPath);
    void use();
    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, const glm::mat4& matrix);
    void setUniform(const std::string& name, const glm::vec3& vector);
    
    unsigned int getId() const { return m_id; }
    
private:
    unsigned int m_id;
    bool checkCompileErrors(unsigned int shader, const std::string& type);
};