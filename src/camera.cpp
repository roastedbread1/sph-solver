#include "camera.h"

void InitCamera(Camera* camera)
{
	camera->position = glm::vec3(0.0f, 0.0f, 0.0f);
	camera->worldUP = glm::vec3(0.0f, 1.0f, 0.0f);
	camera->yaw = YAW;
	camera->pitch = PITCH;
	camera->front = glm::vec3(0.0f, 0.0f, -1.0f);
	camera->movementSpeed = SPEED;
	camera->mouseSens = SENS;
	camera->zoom = ZOOM;
	UpdateCameraVectors(camera);
}

void InitCameraScalar(Camera* camera, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
{
	camera->position = glm::vec3(posX, posY, posZ);
	camera->worldUP = glm::vec3(upX, upY, upZ);
	camera->yaw = yaw;
	camera->pitch = pitch;
	UpdateCameraVectors(camera);


	camera->movementSpeed = SPEED;
	camera->mouseSens = SENS;
	camera->zoom = ZOOM;
}

glm::mat4 GetViewMatrix(Camera* camera)
{
	return glm::lookAt(camera->position, camera->position + camera->front, camera->up);
}

void UpdateCameraVectors(Camera* camera)
{
	///get the new front vector
	glm::vec3 front;
	front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
	front.y = sin(glm::radians(camera->pitch));
	front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));

	camera->front = glm::normalize(front);

	///get the new right and up vector
	///normalize the vectors, because their lengths gets closer to 0 the more you look up or down which results in slower speed
	camera->right = glm::normalize(glm::cross(camera->front, camera->worldUP));
	camera->up = glm::normalize(glm::cross(camera->right, camera->front));
}


void ProcessKeyboard(Camera* camera, Camera_Movement direction, float delta_time)
{
	float velocity = camera->movementSpeed * delta_time;

	if (direction == FORWARD)
	{
		camera->position += camera->front * velocity;
	}

	if (direction == BACKWARD)
	{
		camera->position -= camera->front * velocity;
	}

	if (direction == LEFT)
	{
		camera->position -= camera->right * velocity;
	}

	if (direction == RIGHT)
	{
		camera->position += camera->right * velocity;
	}
}

void ProcessMouseMovement(Camera* camera, float xOffset, float yOffset, GLboolean constraintPitch)
{
	xOffset *= camera->mouseSens;
	yOffset *= camera->mouseSens;

	camera->yaw += xOffset;
	camera->pitch += yOffset;

	if (constraintPitch)
	{
		if (camera->pitch > 89.0f)
		{
			camera->pitch = 89.0f;
		}
		if (camera->pitch < -89.0f)
		{
			camera->pitch = -89.0f;
		}

		UpdateCameraVectors(camera);
	}
}

void ProcessMouseScroll(Camera* camera, float yOffset)
{

	camera->zoom -= (float)yOffset;

	if (camera->zoom < 1.0f)
	{
		camera->zoom = 1.0f;
	}

	if (camera->zoom > 45.0f)
	{
		camera->zoom = 45.0f;
	}
}


