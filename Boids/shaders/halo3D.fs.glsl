#version 330 core

in vec3 vPosition_vs;

out vec3 fFragColor;

void main() 
{
    fFragColor = vec3(0.5,0,0.1) * 6 * exp(-12*(sqrt(vPosition_vs[0]*vPosition_vs[0]+vPosition_vs[1]*vPosition_vs[1]+vPosition_vs[2]*vPosition_vs[2])));
}