// ============================================================
// РЕАЛИЗАЦИЯ КЛАССА SHADER
// ============================================================

#include "shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() : m_id(0) {}

Shader::~Shader() {
    if (m_id) glDeleteProgram(m_id);
}

// ============================================================
// ЗАГРУЗКА И КОМПИЛЯЦИЯ ШЕЙДЕРОВ
// Чтение .glsl файлов → компиляция → линковка
// ============================================================
bool Shader::load(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode, fragmentCode;
    std::ifstream vFile, fFile;
    
    vFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try {
        vFile.open(vertexPath);
        fFile.open(fragmentPath);
        std::stringstream vStream, fStream;
        vStream << vFile.rdbuf();
        fStream << fFile.rdbuf();
        vFile.close();
        fFile.close();
        vertexCode = vStream.str();
        fragmentCode = fStream.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "Ошибка чтения шейдеров" << std::endl;
        return false;
    }
    
    const char* vCode = vertexCode.c_str();
    const char* fCode = fragmentCode.c_str();
    
    // Компиляция вершинного шейдера
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vCode, NULL);
    glCompileShader(vertex);
    if (!checkCompileErrors(vertex, "VERTEX")) return false;
    
    // Компиляция фрагментного шейдера
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fCode, NULL);
    glCompileShader(fragment);
    if (!checkCompileErrors(fragment, "FRAGMENT")) return false;
    
    // Линковка шейдерной программы
    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);
    
    int success;
    char infoLog[512];
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_id, 512, NULL, infoLog);
        std::cerr << "Ошибка линковки: " << infoLog << std::endl;
        return false;
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return true;
}

void Shader::use() {
    glUseProgram(m_id);
}

void Shader::setUniform(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setUniform(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setUniform(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniform(const std::string& name, const glm::vec3& vector) {
    glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(vector));
}

bool Shader::checkCompileErrors(unsigned int shader, const std::string& type) {
    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cerr << "Ошибка компиляции " << type << ": " << infoLog << std::endl;
        return false;
    }
    return true;
}