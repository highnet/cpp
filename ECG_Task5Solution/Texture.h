#pragma once
#ifndef  Texture_h
#define Texture_h
#include <GL\glew.h>
#include <string>
#include "Utils.h"
class Texture {
public:
	GLuint handle;
	Texture::Texture();
	Texture::Texture(std::string relativeFilePath);
	Texture::~Texture();
};
#endif /Texture_h/