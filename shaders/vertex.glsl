// ============================================================
// ВЕРШИННЫЙ ШЕЙДЕР
// ============================================================
// ЗАДАНИЕ 2: Передача UV-координат
// ЗАДАНИЕ 4: TBN-матрица для normal mapping (продвинутый уровень)
// ============================================================

#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;  // ПРОДВИНУТЫЙ УРОВЕНЬ: касательная

out vec2 TexCoord;      // UV-координаты для текстуры (Задание 2)
out vec3 FragPos;       // Позиция фрагмента для освещения
out mat3 TBN;           // ПРОДВИНУТЫЙ УРОВЕНЬ: матрица для normal mapping

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    vec4 worldPos = uModel * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    
    // ============================================================
    // ЗАДАНИЕ 4: ВЫЧИСЛЕНИЕ TBN-МАТРИЦЫ (normal mapping)
    // ============================================================
    vec3 T = normalize(vec3(uModel * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(uModel * vec4(aNormal, 0.0)));
    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);
    
    // ЗАДАНИЕ 2: Передача UV-координат во фрагментный шейдер
    TexCoord = aTexCoord;
    
    gl_Position = uProjection * uView * worldPos;
}