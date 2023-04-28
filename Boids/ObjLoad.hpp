#pragma once

#include <iostream>
#include <vector>
#include "glimac/common.hpp"

std::vector<glimac::ShapeVertex> loadVboFromObj(const std::string& objFilePath);
std::vector<unsigned int>        loadIboFromObj(const std::string& objFilePath);