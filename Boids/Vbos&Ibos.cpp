#include "Vbos&Ibos.hpp"
#include "ObjLoad.hpp"

std::vector<glimac::ShapeVertex> cubeVbo()
{
    std::string cubeFile("/home/rempoir/Documents/IMAC/Annee2/Semestre 2/Synthèse d'image/OpenGL_3D_Boids_Simulation/assets/models/background.obj");
    return loadVboFromObj(cubeFile);
}
std::vector<uint32_t> cubeIbo()
{
    std::string cubeFile("/home/rempoir/Documents/IMAC/Annee2/Semestre 2/Synthèse d'image/OpenGL_3D_Boids_Simulation/assets/models/background.obj");
    return loadIboFromObj(cubeFile);
}