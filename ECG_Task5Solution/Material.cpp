#pragma once
#include "Material.h"

Material::Material() {

}

Material::Material(float r, float g, float b, float ka, float kd, float ks, int a) {
	baseColor.x = r;
	baseColor.y = g;
	baseColor.z = b;
	k_ambient = ka;
	k_diffuse = kd;
	k_specular = ks;
	alpha = a;
}

Material::~Material() {

}
