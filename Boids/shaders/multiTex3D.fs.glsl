#version 330 core

in vec2 vTexCoords;
uniform sampler2D uTexture1;
uniform sampler2D uTexture2;

out vec3 fFragColor;

void main() {
    vec4 texture=texture(uTexture1, vTexCoords)+texture(uTexture2, vTexCoords);
    fFragColor = vec3(texture[0],texture[1],texture[2]);
} 