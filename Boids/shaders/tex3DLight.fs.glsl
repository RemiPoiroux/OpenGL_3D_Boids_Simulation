#version 330 core

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uDirLightDir_vs;
uniform vec3 uDirLightColor;

uniform sampler2D uTexture;

out vec4 fFragColor;

vec3 blinnPhong(vec3 position, vec3 normal, vec3 lightDir, vec3 lightIntensity, vec3 kd, vec3 ks, float shininess) 
{
    vec3 viewDir = normalize(-position);
    vec3 halfVector = normalize(lightDir + viewDir);

    float diffuse = max(0.0, dot(normal, lightDir));
    float specular = pow(max(0.0, dot(normal, halfVector)), shininess);

    return kd * lightIntensity * diffuse + ks * lightIntensity * specular;
}

void main() 
{
    vec4 texture = texture(uTexture, vTexCoords);
    vec3 diffuseColor = texture.xyz;

    vec3 normal = normalize(vNormal_vs);
    vec3 lightDir = normalize(uDirLightDir_vs);
    vec3 color = blinnPhong(vPosition_vs, normal, lightDir, uDirLightColor, uKd, uKs, uShininess);

    fFragColor = vec4(color * diffuseColor, texture.w);
}