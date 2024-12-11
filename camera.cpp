#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Matrix(float FOVdeg, float nearPlane,float farPlane, Shader &shader, const char* uniform)
{
	//Matrix initialization
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);
	
	view = glm::lookAt(Position, Position + Orientation, UP);
	proj = glm::perspective(glm::radians(FOVdeg), (float)width/height, nearPlane, farPlane);

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection*view));
}

