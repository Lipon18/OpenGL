#ifndef CUBEMAP_H
#define CUBEMAP_H

class Cubemap {
public:
	Cubemap(const std::vector<std::string>& faces);
	void bind(GLuint uint = 0);

	GLuint getID() const { return m_ID; }
private:
	GLuint m_ID;
};

#endif // !CUBEMAP_H