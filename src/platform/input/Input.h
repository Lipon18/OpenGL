#ifndef INPUT_H
#define INPUT_H

class Input {
protected:
	virtual void update(GLFWwindow* window) = 0;
	virtual void postUpdate() = 0;
};

#endif // !INPUT_H