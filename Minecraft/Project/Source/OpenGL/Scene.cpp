#include "Scene.h"

//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

#include "../GlobalVars.h"


Scene::Scene(): window(Window(1024, 720)), camera(Camera(glm::vec3(0, 0, 0))), terrain(Terrain()) {

};

Scene::~Scene() {

};


void Scene::Start() {
	
	int isOpen = window.Open("Minecraft");
	if (isOpen != 0) { print("Could not Open Window"); return; }

	// Setup

	//Texture texture("Project/Resources/Textures/MyBox.jpg");
	Texture grassTexture("Project/Resources/Textures/Textures2.png");
	grassTexture.Bind();
	

	terrain.Generate(glm::vec3(0, 0, 0));
	std::vector<float> vertices = terrain.GetVertices();
	int trianglesCount = terrain.GetTriangles();

	const int vSize = vertices.size() * sizeof(float);


	VertexArray va;

	// Vertex Buffer
	VertexBuffer vb(vertices.data(), vSize/*sizeof(vertices)*/);

	// Layout
	VertexBufferLayout layout;
	layout.Push<float>(3); // Position
	layout.Push<float>(2); // Texture Coordinates

	va.AddBuffer(vb, layout);

	// Shaders
	Shader shader("Project/Resources/Shaders/Shaders.glsl");
	shader.Bind();

	shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
	shader.SetUniformMat4f("u_MVP", glm::mat4(1.0f));

	shader.SetUniform1i("u_Texture", 0);

	// Camera
	this->camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));
	
	window.SetCallbacks(&camera);

	// MVP and Maths
	glm::mat4 view = glm::mat4(1.0f); // Camera
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f); // Transformations


	// Main Loop
	while (!window.ShouldClose()) {
		window.Clear();

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		window.CallInputCallback(deltaTime);

		projection = glm::perspective(glm::radians(camera.Zoom), (float)window.GetWidth() / (float)window.GetHeight(), 0.1f, 100.0f);
		view = camera.GetViewMatrix(); //glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -15.0f, 0.0f));
		model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 mvp = (projection * view * model);

		shader.SetUniformMat4f("u_MVP", mvp);

		DrawArrays terrainDraw(va, trianglesCount, shader);
		window.Render(&terrainDraw);

		/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		mvp = (projection * view * model);

		shader.SetUniformMat4f("u_MVP", mvp);

		DrawArrays newCubeDraw(va, trianglesCount, shader);
		//window.Render(&newCubeDraw);
		*/

		window.Show();
	};

	//window.Close();
};


/*
float vertices[] = {
		//	x, y

		// 6 Quads --> 6 vertices --> 36 vertices

		//Back
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		//Front
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		//Left
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		//Right
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		//Bottom
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		//Top
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
*/