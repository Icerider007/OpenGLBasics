#include "camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane,float farPlane, Shader &shader, const char* uniform)
{
	//Matrix initialization
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);
	
	view = glm::lookAt(Position, Position + Orientation, UP);
	proj = glm::perspective(glm::radians(FOVdeg), (float)width/height, nearPlane, farPlane);

	GLuint matrixLoc = glGetUniformLocation(shader.ID, uniform);
	glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, glm::value_ptr(proj*view));
}

void Camera::Inputs(GLFWwindow *window){
	//Keyboard Input		
		//Forward and back
		if( (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) && (glfwGetKey(window,GLFW_KEY_S) == GLFW_RELEASE) ){
			Position += speed * Orientation;
		}
		else if( (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) && (glfwGetKey(window,GLFW_KEY_W) == GLFW_RELEASE)){
			Position += speed * -Orientation;
		}
		//End of Forward and back

		//Strafe
		if( (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && (glfwGetKey(window,GLFW_KEY_A) == GLFW_RELEASE)){
			Position += speed * glm::normalize(glm::cross(Orientation, UP));
		}	
		else if( (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)){
			Position += speed * -glm::normalize(glm::cross(Orientation, UP));
		}
		//End of Strafe

		//Vertical
		if( (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)){
			Position += speed * UP;
		}
		else if( (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)){
			Position += speed * -UP;
		}
		//End of vertical

		//Speed modifier
		if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
			speed = 0.4f;
		}
		else if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE){
			speed = 0.1f;
		}
		//Speed end
	
	//Mouse input
	//Puts mouse in the game and makes camera rotate according to its screen position
		if((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) || !(firstClick)){
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			if(firstClick){
				glfwSetCursorPos(window, width/2, height/2);
				firstClick = false;
			}
			
			double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			float rotY = sensitivity * (float)(mouseX - (height/2))/height;
			float rotX = sensitivity * (float)(mouseY - (width/2))/width;

			glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, UP)));

			if(!((glm::angle(newOrientation,UP) <= glm::radians(5.0f)) or (glm::angle(newOrientation, -UP) <= glm::radians(5.0f)))){
				Orientation = newOrientation;
			}
			
			Orientation = glm::rotate(Orientation, glm::radians(-rotY), UP);
			
			glfwSetCursorPos(window, width/2, height/2);
		}
	//Takes mouse outside the game window
	//Need to add a pause menu or something because if you hold LMB and ESC
	//like a sociopath, the game freezes
		if( (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) && (firstClick != true)){
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			firstClick = true;
		}	
}
