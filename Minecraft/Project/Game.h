#pragma once

#include <stdio.h>
#include <iostream>

#include "Source/Headers/Debug.h"
#include "Source/Models/Player.h"
#include "Source/OpenGL/Scene.h"

class Game {

	Scene scene;

	Player player;

public:
	Game();
	~Game();

public:
	void Run();
};