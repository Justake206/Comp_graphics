// ============================================================
// ЛАБОРАТОРНАЯ РАБОТА №4 - ПРОДВИНУТЫЙ УРОВЕНЬ (ВАРИАНТ 14)
// ============================================================
// Базовый уровень: текстуры, UV, вращение
// Средний уровень: + normal map, mipmap, полная модель Фонга
// Продвинутый уровень: + specular map, TBN, касательные
// ============================================================

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <random>
#include <cmath>

#include "src/shader.h"
#include "src/texture.h"
#include "src/cube.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// ============================================================
// ЗАДАНИЕ 1: ЗАГРУЗКА ТЕКСТУР (stb_image)
// Базовый уровень
// Создаёт диффузную текстуру (цвет) - бронза
// ============================================================
GLuint createDiffuseMap() {
    const int size = 512;
    unsigned char* data = new unsigned char[size * size * 4];
    
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int i = (y * size + x) * 4;
            
            // Бронзовый цвет (индивидуальный вариант 14)
            int r = 180 + (rand() % 40);
            int g = 110 + (rand() % 30);
            int b = 50 + (rand() % 30);
            
            // Металлические полосы
            if ((x / 32 + y / 32) % 2 == 0) {
                r = std::min(255, r + 20);
                g = std::min(200, g + 15);
            }
            
            // Шум для текстуры металла
            r += (rand() % 20) - 10;
            g += (rand() % 15) - 7;
            b += (rand() % 15) - 7;
            
            r = std::min(255, std::max(0, r));
            g = std::min(255, std::max(0, g));
            b = std::min(255, std::max(0, b));
            
            data[i] = r;
            data[i+1] = g;
            data[i+2] = b;
            data[i+3] = 255;
        }
    }
    
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    // ЗАДАНИЕ 5: MIPMAP (средний/продвинутый уровень)
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    delete[] data;
    return texture;
}

// ============================================================
// ЗАДАНИЕ 4: NORMAL MAPPING (средний/продвинутый уровень)
// Создаёт карту нормалей для рельефа
// ============================================================
GLuint createNormalMap() {
    const int size = 512;
    unsigned char* data = new unsigned char[size * size * 4];
    
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int i = (y * size + x) * 4;
            
            // Базовая нормаль (0.5, 0.5, 1.0) в диапазоне 0-1
            int r = 128;
            int g = 128;
            int b = 255;
            
            // Царапины для эффекта рельефа
            if (sin(x * 0.05f) * sin(y * 0.05f) > 0.7f) {
                r = 100;
                g = 100;
                b = 200;
            }
            
            data[i] = r;
            data[i+1] = g;
            data[i+2] = b;
            data[i+3] = 255;
        }
    }
    
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    // MIPMAP для normal map
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    delete[] data;
    return texture;
}

// ============================================================
// ПРОДВИНУТЫЙ УРОВЕНЬ: SPECULAR MAPPING
// Создаёт карту блеска (разные участки блестят по-разному)
// ============================================================
GLuint createSpecularMap() {
    const int size = 512;
    unsigned char* data = new unsigned char[size * size * 4];
    
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int i = (y * size + x) * 4;
            
            // Базовый блеск
            int spec = 80 + (rand() % 120);
            
            // Полосы с повышенным блеском
            if ((x / 32 + y / 32) % 2 == 0) {
                spec = 200 + (rand() % 55);
            }
            
            // Углубления с пониженным блеском
            if ((x / 16) % 2 == 0 && (y / 16) % 2 == 0) {
                spec = 40 + (rand() % 40);
            }
            
            spec = std::min(255, std::max(0, spec));
            
            data[i] = spec;
            data[i+1] = spec;
            data[i+2] = spec;
            data[i+3] = 255;
        }
    }
    
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    delete[] data;
    return texture;
}

int main() {
    // ============================================================
    // ИНИЦИАЛИЗАЦИЯ GLFW (создание окна)
    // ============================================================
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, 
        "Lab4: Продвинутый уровень - Бронза (Diffuse + Normal + Specular Map)", NULL, NULL);
    if (!window) {
        std::cerr << "Ошибка окна" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Ошибка GLAD" << std::endl;
        return -1;
    }
    
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);  // Включаем Z-буфер
    
    // ============================================================
    // ЗАГРУЗКА ШЕЙДЕРОВ (вершинный и фрагментный)
    // ============================================================
    Shader shader;
    if (!shader.load("shaders/vertex.glsl", "shaders/fragment.glsl")) {
        std::cerr << "Ошибка загрузки шейдеров" << std::endl;
        return -1;
    }
    
    // ============================================================
    // ЗАДАНИЕ 1: СОЗДАНИЕ ТРЁХ ТЕКСТУР
    // ============================================================
    GLuint diffuseMap = createDiffuseMap();   // Диффузная карта (цвет)
    GLuint normalMap = createNormalMap();     // Normal map (рельеф)
    GLuint specularMap = createSpecularMap(); // Specular map (блеск)
    
    // ============================================================
    // ЗАДАНИЕ 2: СОЗДАНИЕ КУБА (36 вершин, UV-координаты)
    // ============================================================
    Cube cube;
    
    // ============================================================
    // ИНДИВИДУАЛЬНЫЙ ВАРИАНТ 14: СЛУЧАЙНОЕ ВРАЩЕНИЕ
    // ============================================================
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-0.002f, 0.002f);
    
    float rotX = 0, rotY = 0, rotZ = 0;
    float rotSpeedX = dis(gen);
    float rotSpeedY = dis(gen);
    float rotSpeedZ = dis(gen) * 0.5f;
    bool rotating = true;
    
    // ============================================================
    // ПАРАМЕТРЫ ОСВЕЩЕНИЯ (блуждающий свет)
    // ============================================================
    glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
    glm::vec3 viewPos(0.0f, 0.0f, 4.5f);
    float lightAngle = 0.0f;
    float patinaTime = 0.0f;  // для эффекта патинирования
    
    // ============================================================
    // ЗАДАНИЕ 3: МОДЕЛЬ ФОНГА (передача uniform-переменных)
    // ============================================================
    shader.use();
    shader.setUniform("uMaterialAmbient", glm::vec3(0.35f, 0.25f, 0.20f));
    shader.setUniform("uMaterialDiffuse", glm::vec3(1.00f, 0.80f, 0.60f));
    shader.setUniform("uMaterialShininess", 32.0f);
    
    shader.setUniform("uLightAmbient", glm::vec3(0.40f, 0.30f, 0.25f));
    shader.setUniform("uLightDiffuse", glm::vec3(1.20f, 1.10f, 0.90f));
    shader.setUniform("uLightSpecular", glm::vec3(1.00f, 1.00f, 1.00f));
    
    // Привязка текстурных юнитов
    shader.setUniform("uDiffuseMap", 0);
    shader.setUniform("uNormalMap", 1);
    shader.setUniform("uSpecularMap", 2);  // ПРОДВИНУТЫЙ УРОВЕНЬ
    
    std::cout << "=== ПРОДВИНУТЫЙ УРОВЕНЬ: Normal Map + Specular Map ===" << std::endl;
    
    // ============================================================
    // ГЛАВНЫЙ ЦИКЛ РЕНДЕРИНГА
    // ============================================================
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        
        glClearColor(0.10f, 0.10f, 0.15f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // ЗАДАНИЕ 2: ВРАЩЕНИЕ КУБА
        if (rotating) {
            rotX += rotSpeedX;
            rotY += rotSpeedY;
            rotZ += rotSpeedZ;
        }
        
        // БЛУЖДАЮЩИЙ СВЕТ (движение по эллипсу)
        lightAngle += 0.005f;
        lightPos.x = sin(lightAngle) * 2.2f;
        lightPos.y = cos(lightAngle * 0.7f) * 1.8f;
        lightPos.z = cos(lightAngle) * 2.2f;
        
        // ИНДИВИДУАЛЬНЫЙ ВАРИАНТ 14: ПАТИНИРОВАНИЕ
        patinaTime += 0.012f;
        shader.setUniform("uPatinaTime", patinaTime);
        
        // МАТРИЦЫ ТРАНСФОРМАЦИИ (Model, View, Projection)
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, rotX, glm::vec3(1,0,0));
        model = glm::rotate(model, rotY, glm::vec3(0,1,0));
        model = glm::rotate(model, rotZ, glm::vec3(0,0,1));
        
        glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0,0,0), glm::vec3(0,1,0));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        
        shader.setUniform("uModel", model);
        shader.setUniform("uView", view);
        shader.setUniform("uProjection", projection);
        shader.setUniform("uLightPos", lightPos);
        shader.setUniform("uViewPos", viewPos);
        
        // ПРИВЯЗКА ТРЁХ ТЕКСТУР (Diffuse, Normal, Specular)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        
        cube.draw();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    cube.clear();
    glfwTerminate();
    return 0;
}
