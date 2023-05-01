#pragma once

#include <vector>
#include "glimac/common.hpp"

std::vector<glimac::ShapeVertex> cubeVbo();
std::vector<uint32_t>            cubeIbo();

std::vector<glimac::ShapeVertex> xwingVbo();
std::vector<uint32_t>            xwingIbo();

std::vector<glimac::ShapeVertex> xwingReactorsVbo();
std::vector<uint32_t>            xwingReactorsIbo();

std::vector<glimac::ShapeVertex> tieHDVbo();
std::vector<uint32_t>            tieHDIbo();