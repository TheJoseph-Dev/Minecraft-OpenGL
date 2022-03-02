#pragma once

#include <stdio.h>
#include <iostream>

#include "MeshGenerator.h"


class Chunk {

	MeshGenerator generator;

	ChunkPos position;
	glm::vec2 chunkSize;

	std::vector<float> vertices = std::vector<float>();
	int trianglesCount = 0;

public:
	Chunk(ChunkPos position, glm::vec2 chunkSize) 
	: position(position), chunkSize(chunkSize), generator(MeshGenerator(chunkSize, position))
	{};

	~Chunk() {};

public:
	void Render() {
		generator.Generate();

		Mesh chunkMesh = generator.GetMesh();

		this->vertices = chunkMesh.vertices;
		this->trianglesCount = chunkMesh.trianglesCount;

		// Translation
		int iCount = 1;
		for (auto& v : vertices) {

			// Tranlates X
			if ((iCount % 5) == 1) {
				v += position.x;
				//print("A");
			}

			// Translates Z
			if ((iCount % 5) == 3) {
				v += position.z;
			}

			iCount++;
		}
	};

public:
	
	std::vector<float> GetVertices() const { return this->vertices; };
	int GetTrianglesCount() const { return this->trianglesCount; };
};
