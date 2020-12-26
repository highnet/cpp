#include "Texture.h"

Texture::Texture() {

};

Texture::Texture(std::string relativeFilePath) {
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	DDSImage img = loadDDS(relativeFilePath.c_str());
	glCompressedTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
		img.width,
		img.height,
		0, img.size,
		img.data
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture() {

}
