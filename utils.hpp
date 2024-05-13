#pragma once

#include <string>
#include <vector>
#include <fstream>

unsigned int packColor(const unsigned int r, const unsigned int g, const unsigned int b, const unsigned int a = 255);

void unpackColor(const unsigned int buffer, unsigned int &r, unsigned int &g, unsigned int &b, unsigned int &a);

void dropPpmImage(const std::string &path, const std::vector<unsigned int> buffer, const int width, const int height);