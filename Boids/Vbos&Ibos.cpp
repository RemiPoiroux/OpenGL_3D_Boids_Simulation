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

std::vector<glimac::ShapeVertex> xwingVbo()
{
    std::string xwingFile("/home/rempoir/Documents/IMAC/Annee2/Semestre 2/Synthèse d'image/OpenGL_3D_Boids_Simulation/assets/models/Xwing.obj");
    return loadVboFromObj(xwingFile);
}
std::vector<uint32_t> xwingIbo()
{
    std::string xwingFile("/home/rempoir/Documents/IMAC/Annee2/Semestre 2/Synthèse d'image/OpenGL_3D_Boids_Simulation/assets/models/Xwing.obj");
    return loadIboFromObj(xwingFile);
}
std::vector<glimac::ShapeVertex> xwingReactorsVbo()
{
    std::string xwingFile("/home/rempoir/Documents/IMAC/Annee2/Semestre 2/Synthèse d'image/OpenGL_3D_Boids_Simulation/assets/models/XwingReactors.obj");
    return loadVboFromObj(xwingFile);
}
std::vector<uint32_t> xwingReactorsIbo()
{
    std::string xwingFile("/home/rempoir/Documents/IMAC/Annee2/Semestre 2/Synthèse d'image/OpenGL_3D_Boids_Simulation/assets/models/XwingReactors.obj");
    return loadIboFromObj(xwingFile);
}

std::vector<glimac::ShapeVertex> tieHDVbo()
{
    std::string tieFile("/home/rempoir/Documents/IMAC/Annee2/Semestre 2/Synthèse d'image/OpenGL_3D_Boids_Simulation/assets/models/tieHD.obj");
    return loadVboFromObj(tieFile);
}
std::vector<uint32_t> tieHDIbo()
{
    std::string tieFile("/home/rempoir/Documents/IMAC/Annee2/Semestre 2/Synthèse d'image/OpenGL_3D_Boids_Simulation/assets/models/tieHD.obj");
    return loadIboFromObj(tieFile);
}
std::vector<glimac::ShapeVertex> tieLDVbo()
{
    std::string tieFile("/home/rempoir/Documents/IMAC/Annee2/Semestre 2/Synthèse d'image/OpenGL_3D_Boids_Simulation/assets/models/tieLD.obj");
    return loadVboFromObj(tieFile);
}
std::vector<uint32_t> tieLDIbo()
{
    std::string tieFile("/home/rempoir/Documents/IMAC/Annee2/Semestre 2/Synthèse d'image/OpenGL_3D_Boids_Simulation/assets/models/tieLD.obj");
    return loadIboFromObj(tieFile);
}

std::vector<glimac::ShapeVertex> asteroidHDVbo()
{
    std::string tieFile("/home/rempoir/Documents/IMAC/Annee2/Semestre 2/Synthèse d'image/OpenGL_3D_Boids_Simulation/assets/models/asteroidHD.obj");
    return loadVboFromObj(tieFile);
}
std::vector<uint32_t> asteroidHDIbo()
{
    std::string tieFile("/home/rempoir/Documents/IMAC/Annee2/Semestre 2/Synthèse d'image/OpenGL_3D_Boids_Simulation/assets/models/asteroidHD.obj");
    return loadIboFromObj(tieFile);
}
std::vector<glimac::ShapeVertex> asteroidLDVbo()
{
    std::string tieFile("/home/rempoir/Documents/IMAC/Annee2/Semestre 2/Synthèse d'image/OpenGL_3D_Boids_Simulation/assets/models/asteroidLD.obj");
    return loadVboFromObj(tieFile);
}
std::vector<uint32_t> asteroidLDIbo()
{
    std::string tieFile("/home/rempoir/Documents/IMAC/Annee2/Semestre 2/Synthèse d'image/OpenGL_3D_Boids_Simulation/assets/models/asteroidLD.obj");
    return loadIboFromObj(tieFile);
}