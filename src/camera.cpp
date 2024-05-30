#include "camera.h"
#include <iostream>


void Camera::update(float nearClip, float farClip, float fov) {
    glm::mat4 V = glm::mat4(1.f);
    glm::mat4 P = glm::mat4(1.f);
    V = glm::lookAt(position, position + direction, up);
    P = glm::perspective(fov, static_cast<float>(window_size.width) / static_cast<float>(window_size.height), nearClip, farClip);
    mat = P * V;
}
void Camera::exportMatrix(Shader& shader, const char* uniform_name) {
    glUniformMatrix4fv(shader.u(uniform_name), 1, GL_FALSE, glm::value_ptr(mat));
}
void Camera::processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += move_speed * direction;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position += move_speed * -glm::normalize(glm::cross(direction, up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position += move_speed * -direction;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += move_speed * glm::normalize(glm::cross(direction, up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		position += move_speed * up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		position += move_speed * -up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		move_speed = 0.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		move_speed = 0.1f;
	}


	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick)
		{
			glfwSetCursorPos(window, ((float)window_size.width / 2), ((float)window_size.height / 2));
			firstClick = false;
            double mouseX;
            double mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            lastMousePos.x = mouseX;
            lastMousePos.y = mouseY;
		}
        
		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = turn_speed * (mouseX - lastMousePos.x);
		float rotY = turn_speed * (mouseY - lastMousePos.y);
        lastMousePos.x = mouseX;
        lastMousePos.y = mouseY;

		glm::vec3 newdirection = glm::rotate(direction, glm::radians(-rotY), glm::normalize(glm::cross(direction, up)));

		if (abs(glm::angle(newdirection, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			direction = newdirection;
		}

		direction = glm::rotate(direction, glm::radians(-rotX), up);

	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}

Camera::Camera(int width, int height, vec3 pos, vec3 lookat) {
    direction = glm::normalize(lookat - pos);
    window_size.width = width;
    window_size.height = height;
    position = pos;
}
