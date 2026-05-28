// ============================================================
// ФРАГМЕНТНЫЙ ШЕЙДЕР
// ============================================================
// ЗАДАНИЕ 3: Модель освещения Фонга (Ambient + Diffuse + Specular)
// ЗАДАНИЕ 4: Normal mapping (преобразование нормалей через TBN)
// ПРОДВИНУТЫЙ УРОВЕНЬ: Specular mapping (разный блеск)
// ИНДИВИДУАЛЬНЫЙ ВАРИАНТ 14: Патинирование (зеленоватый оттенок)
// ============================================================

#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in mat3 TBN;

out vec4 FragColor;

// Текстуры (Задание 1, Задание 4, Продвинутый уровень)
uniform sampler2D uDiffuseMap;   // Диффузная карта (цвет)
uniform sampler2D uNormalMap;    // Normal map (рельеф)
uniform sampler2D uSpecularMap;  // Specular map (разный блеск) - ПРОДВИНУТЫЙ УРОВЕНЬ

uniform vec3 uLightPos;
uniform vec3 uViewPos;

// Материал (Вариант 14 - бронза)
uniform vec3 uMaterialAmbient;
uniform vec3 uMaterialDiffuse;
uniform float uMaterialShininess;

// Источник света
uniform vec3 uLightAmbient;
uniform vec3 uLightDiffuse;
uniform vec3 uLightSpecular;

// Индивидуальный вариант 14: патинирование
uniform float uPatinaTime;

void main() {
    // ============================================================
    // ЗАДАНИЕ 1: ВЫБОРКА ЦВЕТА ИЗ ДИФФУЗНОЙ ТЕКСТУРЫ
    // ============================================================
    vec3 diffuseColor = texture(uDiffuseMap, TexCoord).rgb;
    
    // ============================================================
    // ПРОДВИНУТЫЙ УРОВЕНЬ: ВЫБОРКА БЛЕСКА ИЗ SPECULAR MAP
    // ============================================================
    float specularIntensity = texture(uSpecularMap, TexCoord).r;
    
    // ============================================================
    // ЗАДАНИЕ 4: ПОЛУЧЕНИЕ НОРМАЛИ ИЗ NORMAL MAP
    // ============================================================
    vec3 normal = texture(uNormalMap, TexCoord).rgb;
    normal = normal * 2.0 - 1.0;           // Из [0,1] в [-1,1]
    normal = normalize(TBN * normal);      // Преобразование в мировое пространство
    
    // ============================================================
    // ЗАДАНИЕ 3: МОДЕЛЬ ОСВЕЩЕНИЯ ФОНГА
    // ============================================================
    
    // 1. AMBIENT (фоновое освещение) - всегда есть
    vec3 ambient = uLightAmbient * uMaterialAmbient * diffuseColor;
    
    // 2. DIFFUSE (диффузное освещение) - закон Ламберта
    vec3 lightDir = normalize(uLightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = uLightDiffuse * (diff * uMaterialDiffuse) * diffuseColor;
    
    // 3. SPECULAR (зеркальное освещение) - блики
    vec3 viewDir = normalize(uViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterialShininess);
    vec3 specular = uLightSpecular * (spec * specularIntensity);  // С использованием specular map
    
    // ============================================================
    // ИНДИВИДУАЛЬНЫЙ ВАРИАНТ 14: ПАТИНИРОВАНИЕ (бронза зеленеет)
    // ============================================================
    float patina = sin(uPatinaTime * 1.2) * 0.2 + 0.8;
    vec3 patinaColor = vec3(0.45, 0.65, 0.35);
    
    vec3 result = (ambient + diffuse + specular);
    result = mix(result, result * patinaColor, 0.45 * (1.0 - patina));
    
    FragColor = vec4(result, 1.0);
}