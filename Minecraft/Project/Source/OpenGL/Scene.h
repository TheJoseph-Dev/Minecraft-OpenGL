#pragma once

#include <stdio.h>
#include <iostream>

#include "Scene/Window.h"
#include "Scene/Camera.h"
#include "../Models/Terrain/Terrain.h"

#include "Model_Headers/Texture.hpp"

#include "../Models/Terrain/Block.h"

//Maths
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Scene {

	Window window;

	Camera camera;

	Terrain terrain;

public:
	Scene();
	~Scene();

	void Start();
};