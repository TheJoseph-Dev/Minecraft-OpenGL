#pragma once

#include "Renderer.h"

#include "Camera.h"

#include "../../Headers/Debug.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class DrawType {
public:
	DrawType() {};
	virtual ~DrawType() {};
};

struct DrawArrays : public DrawType {
	VertexArray& vertexArray;
	unsigned int trianglesCount;
	Shader& shader;

	DrawArrays(VertexArray& vertexArray, unsigned int trisCount, Shader& shader)
		: vertexArray(vertexArray), trianglesCount(trisCount), shader(shader), DrawType() {};
};

struct DrawElements : public DrawType {
	VertexArray& vertexArray;
	IndexBuffer& indexBuffer;
	Shader& shader;

	DrawElements(VertexArray& vertexArray, IndexBuffer& indexBuffer, Shader& shader) 
		: vertexArray(vertexArray), indexBuffer(indexBuffer), shader(shader), DrawType() {};
	//~DrawElements() : ~DrawType() {};
};

class Window {

	GLFWwindow* window;

	int width;
	int height;

	Renderer renderer;

public:
	Window(int width, int height);
	~Window();

	// Clears the window
	void Clear();

	// Opens the window
	int Open(const char* windowName);

	// Executes the events in the pipeline
	void Show();

	// Ask window to render
	void Render(DrawType* draw);

	// Closes the window
	void Close();

public:
	void SetCallbacks(Camera* camera);

	void CallInputCallback(float deltaTime);

public:
	bool ShouldClose() const { return glfwWindowShouldClose(window); };

	GLFWwindow* GetWindow() const { return window; }
	
	int GetWidth() const { return width; };
	int GetHeight() const { return height; };
};