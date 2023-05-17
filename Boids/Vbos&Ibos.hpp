#pragma once

#include <vector>
#include "glimac/common.hpp"

std::vector<glimac::ShapeVertex> cubeVbo();
std::vector<uint32_t>            cubeIbo();

std::vector<glimac::ShapeVertex> xwingVbo();
std::vector<uint32_t>            xwingIbo();

std::vector<glimac::ShapeVertex> xwingReactorsVbo();
std::vector<uint32_t>            xwingReactorsIbo();

std::vector<glimac::ShapeVertex> laserVbo();
std::vector<uint32_t>            laserIbo();

std::vector<glimac::ShapeVertex> tieHDVbo();
std::vector<uint32_t>            tieHDIbo();
std::vector<glimac::ShapeVertex> tieLDVbo();
std::vector<uint32_t>            tieLDIbo();

std::vector<glimac::ShapeVertex> asteroidHDVbo();
std::vector<uint32_t>            asteroidHDIbo();
std::vector<glimac::ShapeVertex> asteroidLDVbo();
std::vector<uint32_t>            asteroidLDIbo();