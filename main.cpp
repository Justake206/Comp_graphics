#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <random>

#include "src/shader.h"
#include "src/texture.h"
#include "src/cube.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    // Инициализация GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lab4: Bronze (Variant 14)", NULL, NULL);
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
    glEnable(GL_DEPTH_TEST);
    
    Shader shader;
    if (!shader.load("shaders/vertex.glsl", "shaders/fragment.glsl")) {
        std::cerr << "Ошибка загрузки шейдеров" << std::endl;
        return -1;
    }
    
    Texture texture;
    if (!texture.loadFromFile("textures/bronze.jpg")) {
        std::cerr << "Текстура не загружена" << std::endl;
    }
    
    Cube cube;
    
    // Случайное вращение (очень медленное)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-0.003f, 0.003f);
    
    float rotX = 0, rotY = 0, rotZ = 0;
    float rotSpeedX = dis(gen);
    float rotSpeedY = dis(gen);
    float rotSpeedZ = dis(gen) * 0.5f;
    bool rotating = true;
    
    // Более яркое освещение
    glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
    glm::vec3 viewPos(0.0f, 0.0f, 3.5f);
    float lightAngle = 0.0f;
    float patinaTime = 0.0f;
    
    shader.use();
    // Увеличены коэффициенты отражения для яркости
    shader.setUniform("uMaterialAmbient", glm::vec3(0.40f, 0.30f, 0.20f));
    shader.setUniform("uMaterialDiffuse", glm::vec3(1.00f, 0.80f, 0.60f));
    shader.setUniform("uLightAmbient", glm::vec3(0.35f, 0.25f, 0.20f));
    shader.setUniform("uLightDiffuse", glm::vec3(1.20f, 1.10f, 0.90f));
    shader.setUniform("uTexture", 0);
    
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        
        glClearColor(0.15f, 0.15f, 0.20f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if (rotating) {
            rotX += rotSpeedX;
            rotY += rotSpeedY;
            rotZ += rotSpeedZ;
        }
        
        lightAngle += 0.005f;
        lightPos.x = sin(lightAngle) * 2.2f;
        lightPos.y = cos(lightAngle * 0.7f) * 1.8f;
        lightPos.z = cos(lightAngle) * 2.2f;
        patinaTime += 0.008f;
        
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
        shader.setUniform("uPatinaTime", patinaTime);
        
        texture.bind(0);
        cube.draw();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    cube.clear();
    glfwTerminate();
    return 0;
}