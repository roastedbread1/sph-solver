#pragma once
#include <GLAD/glad.h>
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.5f;
const float SENS = 0.1f;
const float ZOOM = 45.0f;

struct Camera
{
	///attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUP;

	///euler angles
	float yaw;
	float pitch;

	///camera options
	float movementSpeed;
	float mouseSens;
	float zoom;
};

void InitCamera(Camera* camera);
void InitCameraScalar(Camera* camera, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
glm::mat4 GetViewMatrix(Camera* camera);
void UpdateCameraVectors(Camera* camera);

void ProcessKeyboard(Camera* camera, Camera_Movement direction, float delta_time);
void ProcessMouseMovement(Camera* camera, float xOffset, float yOffset, GLboolean constraintPitch = true);
void ProcessMouseScroll(Camera* camera, float yOffset);