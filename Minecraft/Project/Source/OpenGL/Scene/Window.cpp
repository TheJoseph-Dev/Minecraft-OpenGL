#include "Window.h"
#include "WindowCallbacks.h"

Window::Window(int width, int height) : width(width), height(height), renderer(Renderer()), window(nullptr) {};

Window::~Window() {};


void Window::Clear() { renderer.Clear(); };

int Window::Open(const char* windowName) {
	//GLEW --> Communication with GPU Driver and its OpenGL binaries

	//Init GLFW
	glfwInit();

	//Window Configurations
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	this->window = glfwCreateWindow(width, height, windowName, NULL, NULL);

	if (window == NULL) { print("ERROR: Could't create GLFW window"); glfwTerminate(); return -1; }

	glfwMakeContextCurrent(window);
	//SetCallbacks();

	glfwSwapInterval(1);

	glewExperimental = GL_TRUE;

	// Init GLEW
	if (glewInit() != GLEW_OK) { print("ERROR: Could't Init GLEW"); return -1; }


	// Tell OpenGL the size of the rendering window so OpenGL knows how we want to display the data and coordinates with respect to the window.
	glViewport(0, 0, width, height);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST); // Depth Testing

	// --- End Init

	return 0;
};

void Window::Show() {
	glfwSwapBuffers(window); // Updates the screen swapping between front and back buffers
	glFlush();
	glfwPollEvents(); // Executes all the gl events in queue 
};


void Window::Render(DrawType* draw) { 

	if (typeid(*draw) == typeid(DrawArrays)) {
		//print("It is a DA call");
		DrawArrays* drawArr = dynamic_cast<DrawArrays*>(draw);
		renderer.Draw((*drawArr).vertexArray, (*drawArr).trianglesCount, (*drawArr).shader);
	}

	if (typeid(*draw) == typeid(DrawElements)) {
		//print("It is a DE call");
		DrawElements* drawEl = dynamic_cast<DrawElements*>(draw);

		renderer.Draw((*drawEl).vertexArray, (*drawEl).indexBuffer, (*drawEl).shader);
	}
}; 


void Window::Close() {};


void Window::SetCallbacks(Camera* camera) {
	windowCallbacksCamera = camera;
	windowCallbacksWindow = this;

	WindowCallbacks wCallbacks;
	wCallbacks.SetCallbacks();
};

void Window::CallInputCallback(float deltaTime) {
	WindowCallbacks wCallbacks;
	wCallbacks.processInput(window, deltaTime);
};