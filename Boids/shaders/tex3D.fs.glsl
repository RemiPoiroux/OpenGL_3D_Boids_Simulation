#version 330 core

in vec2 vTexCoords;
uniform sampler2D uTexture;

out vec3 fFragColor;

void main() {
    vec4 texture=texture(uTexture, vTexCoords);
    fFragColor = vec3(texture[0],texture[1],texture[2]);
} 