#include <stdio.h>
#include <iostream>
#include "../Headers/Debug.h"

class Player {

	int x;
	int y;
	int z;

public:
	Player();
	~Player();

	void Move(int x, int y, int z);
	void Jump();

public:
	int* GetPosition() const; // Returns x and y in an array

};