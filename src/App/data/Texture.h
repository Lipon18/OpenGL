#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

struct Texture {
	unsigned int id = 0;
	std::string type;
	std::string path;
};

#endif // !TEXTURE_H