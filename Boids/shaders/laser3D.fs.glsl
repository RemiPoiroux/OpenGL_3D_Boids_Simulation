#version 330 core

in vec3 vPosition_vs;

out vec3 fFragColor;

void main() 
{
    vec3 direction=vec3(0,0,-1);
    float size=0.01;

    float distanceAlongDirection = dot(vPosition_vs, direction);
    float opacity = 1.0 - smoothstep(size - 0.1, size + 0.1, distanceAlongDirection);
    
    fFragColor = vec3(1,0,0) * 6.0 * exp(-12.0 * (sqrt(vPosition_vs[0] * vPosition_vs[0] + vPosition_vs[1] * vPosition_vs[1] + vPosition_vs[2] * vPosition_vs[2])));
    fFragColor *= opacity;
}
