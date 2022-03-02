#pragma once

#include <stdio.h>
#include <iostream>

#include <map>
#include <array>

#include "Chunk.h"

class Terrain {

	std::map<ChunkPos, Chunk> chunks = std::map<ChunkPos, Chunk>();
	
	std::vector<float> terrainVertices = std::vector<float>();
	int terrainTrianglesCount = 0;

	const glm::vec2 chunkSize = glm::vec2(16, 64);

public:
	Terrain() {};
	~Terrain() {};

public:
	void GenerateOne(ChunkPos chunkPos = { 0, 0 }) {
		Chunk chunk(chunkPos, chunkSize);
		chunk.Render();
		
		this->terrainVertices = chunk.GetVertices();
		this->terrainTrianglesCount = chunk.GetTrianglesCount();
		print("Vertices Count: " << terrainVertices.size());
		print("Triangles Count: " << terrainTrianglesCount);
	};

	void Generate(glm::vec3 playerPosition) {

		int x = (int)playerPosition.x;
		int z = (int)playerPosition.z;

		const int chunkWidth = chunkSize.x;

		int curChunkPosX = (int)((x / chunkWidth) * chunkWidth);
		int curChunkPosZ = (int)((z / chunkWidth) * chunkWidth);


		const int chunksSize = 6;

		//Renders start chunks
		for (int i = curChunkPosX - (chunkWidth * chunksSize) /* First chunk */; i <= curChunkPosX + (chunkWidth * chunksSize) /* Until the last chunk */; i += chunkWidth)
		{
			for (int j = curChunkPosZ - (chunkWidth * chunksSize); j <= curChunkPosZ + (chunkWidth * chunksSize); j += chunkWidth)
			{
				ChunkPos pos = ChunkPos(i, j);


				if (chunks.count(pos) <= 1)
				{
					Chunk chunk = Chunk(pos, chunkSize);
					chunks.insert(std::make_pair(pos, chunk));

					chunk.Render();

					std::vector<float> cVertices = chunk.GetVertices();
					this->terrainVertices.insert(terrainVertices.end(), cVertices.begin(), cVertices.end()); // Appends chunk vertices to the main vector

					this->terrainTrianglesCount += chunk.GetTrianglesCount();
					//print(pos.x << " " << pos.z);
				}

				//print("---" << pos.x << " " << pos.z);
			}//
		}//

		print("Vertices Count: " << terrainVertices.size());
		print("Triangles Count: " << terrainTrianglesCount);
	};

	std::vector<float> GetVertices() const { return this->terrainVertices; };
	int GetTriangles() const { return this->terrainTrianglesCount; };
};