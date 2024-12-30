#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaderClass.h"

class Camera{
	public:
	//Stores all the main vectors we need
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);

	// Variables
	int width;
	int height;
	//This is to prevent the camera from jumping around
	bool firstClick;

	float speed = 0.1f;
	float sensitivity = 100.0f;

	// Functions
	Camera(int width, int height, glm::vec3 position);

	void Matrix(float FOVdeg, float nearPlane,float farPlane, Shader &shader, const char* uniform);
	void Inputs(GLFWwindow *window);
};

#endif
