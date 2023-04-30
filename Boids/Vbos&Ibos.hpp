#pragma once

#include <vector>
#include "glimac/common.hpp"

std::vector<glimac::ShapeVertex> cubeVbo();
std::vector<uint32_t>            cubeIbo();

std::vector<glimac::ShapeVertex> tieHDVbo();
std::vector<uint32_t>            tieHDIbo();