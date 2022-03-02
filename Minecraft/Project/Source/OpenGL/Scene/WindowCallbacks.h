#pragma once

#include "Camera.h"
//#include "Window.h"

#include <GLFW/glfw3.h>

Camera* windowCallbacksCamera = nullptr;
Window* windowCallbacksWindow = nullptr;

extern bool firstMouse = true;

extern float deltaTime = 0.0f;
extern float lastFrame = 0.0f;

class WindowCallbacks {

public:

	static void SetCallbacks() {
		//Callbacks
		glfwSetFramebufferSizeCallback((*windowCallbacksWindow).GetWindow(), framebuffer_size_callback);
		glfwSetCursorPosCallback((*windowCallbacksWindow).GetWindow(), mouse_callback);
		glfwSetScrollCallback((*windowCallbacksWindow).GetWindow(), scroll_callback);
	};

	//View on resize callback
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	};


	static void processInput(GLFWwindow* window, float deltaTime)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			(*windowCallbacksCamera).ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			(*windowCallbacksCamera).ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			(*windowCallbacksCamera).ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			(*windowCallbacksCamera).ProcessKeyboard(RIGHT, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			(*windowCallbacksCamera).ProcessKeyboard(TOP, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			(*windowCallbacksCamera).ProcessKeyboard(DOWN, deltaTime);

	};

	static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
	{
		float lastX = (float)(*windowCallbacksWindow).GetWidth() /  2;
		float lastY = (float)(*windowCallbacksWindow).GetHeight() / 2;

		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		(*windowCallbacksCamera).ProcessMouseMovement(xoffset, yoffset);
	}

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		(*windowCallbacksCamera).ProcessMouseScroll(static_cast<float>(yoffset));
	}
};