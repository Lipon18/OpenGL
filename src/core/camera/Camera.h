#ifndef CAMERA_H
#define CAMERA_H

enum E_CameraMovement {
	E_Forward,
	E_Backward,
	E_Left,
	E_Right
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 10.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	glm::mat4 GetViewMatrix();
	void ProcessKeyboard(E_CameraMovement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yoffset);

	glm::vec3 getCameraPosition() const { return m_Position; }
	glm::vec3 getCameraFront() const { return m_Front; }
	glm::vec3 getCameraUp() const { return m_Up; }
	glm::vec3 getCameraRight() const { return m_Right; }
	glm::vec3 getCameraWorldUp() const { return m_WorldUp; }

	float getCamera_YAW() const { return m_Yaw; }
	float getCamera_PITCH() const { return m_Pitch; }
	void setCamera_YAW(float yaw) { m_Yaw = yaw; updateCameraVectors(); }
	void setCamera_PITCH(float pitch) { m_Pitch = pitch; updateCameraVectors(); }

	float getCameraMovementSpeed() const { return m_MovementSpeed; }
	float getCameraMouseSensitivity() const { return m_MouseSensitivity; }
	float getCameraZoom() const { return m_Zoom; }

	void setCameraPosition(const glm::vec3& pos) { m_Position = pos; }

private:
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;

	float m_Yaw;
	float m_Pitch;

	float m_MovementSpeed;
	float m_MouseSensitivity;
	float m_Zoom;

	void updateCameraVectors();
};

#endif // !CAMERA_H