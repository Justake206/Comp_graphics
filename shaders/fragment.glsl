#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec3 uLightPos;
uniform vec3 uViewPos;

uniform vec3 uMaterialAmbient;
uniform vec3 uMaterialDiffuse;
uniform vec3 uLightAmbient;
uniform vec3 uLightDiffuse;

uniform float uPatinaTime;

void main() {
    vec4 texColor = texture(uTexture, TexCoord);
    
    // Ambient
    vec3 ambient = uLightAmbient * uMaterialAmbient * texColor.rgb;
    
    // Diffuse (Ламберт)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = uLightDiffuse * (diff * uMaterialDiffuse) * texColor.rgb;
    
    // Патинирование (бронза зеленеет)
    float patina = sin(uPatinaTime * 1.2) * 0.2 + 0.8;
    vec3 patinaColor = vec3(0.45, 0.65, 0.35);
    
    vec3 result = (ambient + diffuse);
    result = mix(result, result * patinaColor, 0.45 * (1.0 - patina));
    
    FragColor = vec4(result, 1.0);
}