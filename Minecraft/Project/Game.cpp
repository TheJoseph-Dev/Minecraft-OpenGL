#include "Game.h"

Game::Game() {

};

Game::~Game() {

};


void Game::Run() {
	print("Game is running!");
	int* position = player.GetPosition();
	
	scene.Start();
};