#version 330

in vec3 vNormal_vs;

out vec3 fFragColor;

void main() {
    fFragColor = normalize(vNormal_vs);
}