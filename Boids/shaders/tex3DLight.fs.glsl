#version 330 core

in vec3 vPosition_o;
in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uDirLightDir_vs;
uniform vec3 uDirLightColor;

const int MAX_POINT_LIGHTS = 5;
uniform vec3 uPointLightsPositions[MAX_POINT_LIGHTS];
uniform vec3 uPointLightsColors[MAX_POINT_LIGHTS];
uniform float uPointLightsIntensities[MAX_POINT_LIGHTS];
uniform int uNumPointLights;

uniform sampler2D uTexture;

uniform mat4 uDepthMVP;
uniform sampler2D uShadowMap;

out vec4 fFragColor;
vec3 blinnPhongDir(vec3 position, vec3 normal, vec3 lightDir, vec3 lightIntensity, vec3 kd, vec3 ks, float shininess) 
{
    vec3 viewDir = normalize(-position);
    vec3 halfVector = normalize(lightDir + viewDir);

    float diffuse = max(0.0, dot(normal, lightDir));
    float specular = pow(max(0.0, dot(normal, halfVector)), shininess);

    return kd * lightIntensity * diffuse + ks * lightIntensity * specular;
}

vec3 blinnPhongPoint(vec3 position, vec3 normal, vec3 kd, vec3 ks, float shininess, vec3 uLightIntensity, vec3 uLightPos_vs) 
{
    vec3 viewDir = normalize(- position);
    vec3 lightDir = normalize(uLightPos_vs - position);
    float distanceToLight = length(uLightPos_vs - position);
    vec3 attenuatedLightIntensity = uLightIntensity / (distanceToLight * distanceToLight);
    vec3 halfVector = normalize(lightDir + viewDir);

    float diffuse = max(0.0, dot(normal, lightDir));
    float specular = pow(max(0.0, dot(normal, halfVector)), shininess);

    return kd * attenuatedLightIntensity * diffuse + ks * attenuatedLightIntensity * specular;
}

void main() 
{
    vec4 textureColor = texture(uTexture, vTexCoords);
    vec3 diffuseColor = textureColor.xyz;
    vec3 normal = normalize(vNormal_vs);
    vec3 lightDir = normalize(uDirLightDir_vs);

    vec3 color=vec3(0,0,0);

    //Dir light shadows
    vec4 shadowCoord = uDepthMVP * vec4(vPosition_o, 1.0);
    vec3 projCoords = shadowCoord.xyz / shadowCoord.w;
    projCoords = projCoords * 0.5 + 0.5;
    float depth = texture(uShadowMap, projCoords.xy).x;
    float shadowFactor = (projCoords.z > depth + 0.01) ? 0 : 1.0;

    color = blinnPhongDir(vPosition_vs, normal, lightDir, uDirLightColor, uKd, uKs, uShininess) * shadowFactor;


    for (int i = 0; i < uNumPointLights; i++) 
    {
        color += blinnPhongPoint(vPosition_vs, normal, uKd, uKs, uShininess, uPointLightsIntensities[i]*uPointLightsColors[i], uPointLightsPositions[i]);       
    }

    fFragColor = vec4(color * diffuseColor, textureColor.w);
}