#include "Player.h"

Player::Player(): x(0), y(0), z(0) {

};

Player::~Player() {

};

//

void Player::Move(int x, int y, int z) {
	this->x = x;
	this->y = y;
	this->z = z;
};

void Player::Jump() {

};

//

int* Player::GetPosition() const {
	int* position = new int[3];
	position[0] = this->x;
	position[1] = this->y;
	position[2] = this->z;

	return position;
};