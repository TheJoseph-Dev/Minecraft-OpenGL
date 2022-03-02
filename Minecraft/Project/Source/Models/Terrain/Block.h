#pragma once

#include <stdio.h>
#include <iostream>

#include <vector>
#include <array>

#include "Chunk.h"
#include "../../Headers/Debug.h"

//Maths
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Noise
#include <FastNoise/FastNoiseLite.h>


// ChunkPos has been declared here to avoid include cycles

struct ChunkPos {
	int x;
	int z;

	ChunkPos(int x, int z) : x(x), z(z) {};

	bool operator<(const ChunkPos& rhs) const noexcept
	{
		return this->x < rhs.x && this->z < rhs.z;
	}

	bool operator==(const ChunkPos& rhs) const noexcept
	{
		return this->x == rhs.x && this->z == rhs.z;
	}
};

enum BlockFace {
	Front,
	Left,
	Back,
	Right,
	Top,
	Bottom
};

enum BlockType {
	Air,
	Grass,
	Dirt,
	Stone
};

struct BlockData {
	std::vector<glm::vec3> vertices;
	int trianglesCount;
};

class Block {

	glm::vec3 position;
	std::vector<std::array<glm::vec3, 6>> vertices;

	//triangles
	//block type

public:

	Block(glm::vec3 position) { this->position = position; };
	~Block() {};

public:
	static BlockType GetBlockType(int ySize, ChunkPos chunkPos, glm::vec3 position) {

		int x = (int)position.x + chunkPos.x;
		int y = (int)position.y;
		int z = (int)position.z + chunkPos.z;

		//print(x << " " << z);
		
		// Terrain Noise
		FastNoiseLite noise;
		noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

		float simplex1 = noise.GetNoise(x * .8f, z * .8f) * 10;
		float simplex2 = noise.GetNoise(x * 3.0f, z * 3.0f) * 10 * (noise.GetNoise(x * .3f, z * .3f) + .5f);

		float heightMap = simplex1 + simplex2;

		//add the 2d noise to the middle of the terrain chunk
		float baseLandHeight = ySize * .5f + heightMap;


		//3d noise for caves and overhangs and such
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		float caveNoise1 = noise.GetNoise(x * 5.0f, y * 10.0f, z * 5.0f);

		noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		float caveMask = noise.GetNoise(x * .3f, z * .3f) + .3f;

		//stone layer heightmap
		float simplexStone1 = noise.GetNoise(x * 1.0f, z * 1.0f) * 10;
		float simplexStone2 = (noise.GetNoise(x * 5.0f, z * 5.0f) + .5f) * 20 * (noise.GetNoise(x * .3f, z * .3f) + .5f);

		float stoneHeightMap = simplexStone1 + simplexStone2;
		float baseStoneHeight = ySize * .25f + stoneHeightMap;

		BlockType blockType = BlockType::Air;

		//print((int)baseLandHeight << " " << y);
		//under the surface, dirt block
		if (y <= (int)baseLandHeight)
		{
			blockType = BlockType::Grass;
		}

		if ((y + 1) <= (int)baseLandHeight && blockType == Grass) {
			blockType = BlockType::Dirt;
		}
		/*if (y <= (int)(baseLandHeight * 0.95)) {
			blockType = BlockType::Dirt;
		}*/
		

		if (y <= (int)baseLandHeight / 1.1) {
			blockType = BlockType::Stone;
		}

		
		if (caveNoise1 > std::max(caveMask, .2f)) {
			blockType = BlockType::Air;
		}

		if (blockType != BlockType::Air && blockType != Grass) {
			//blockType = BlockType::Stone;
		}

		
		// Block
		/*
		if (y > (ySize / 2) || y < 1 || x > 16 || x < 1 || z > 16 || z < 1)
			return Air;
		else
			return Grass;
		*/

		return blockType;
	};

	static std::vector<glm::vec2> GetBlockUVs(BlockType blockType, BlockFace face) {
		if (blockType == Air) { return std::vector<glm::vec2>(); }

		std::vector<std::array<glm::vec2, 6>> UVs;

		if (face == Front) {
			std::array<glm::vec2, 6> faceUVFront;

			switch (blockType) {
			case Grass:
				faceUVFront = {
					//Front
					glm::vec2(0.0f,  0.7f), //0.0f, 0.0f,
					glm::vec2(0.25f, 0.7f), //1.0f, 0.0f,
					glm::vec2(0.25f, 1.0f), //1.0f, 1.0f,
					glm::vec2(0.25f, 1.0f), //1.0f, 1.0f,
					glm::vec2(0.0f,  1.0f), //0.0f, 1.0f,
					glm::vec2(0.0f,  0.7f)  //0.0f, 0.0f
				};
				break;
			case Dirt:
				faceUVFront = {
					//Front
					glm::vec2(0.0f,  0.34f),
					glm::vec2(0.25f, 0.34f),
					glm::vec2(0.25f, 0.6f), 
					glm::vec2(0.25f, 0.6f),
					glm::vec2(0.0f,  0.6f), 
					glm::vec2(0.0f,  0.34f)  
				};
				break;
			case Stone:
				faceUVFront = {
					//Front
					glm::vec2(0.26f,  0.01f),
					glm::vec2(0.48f,  0.01f),
					glm::vec2(0.48f,  0.32f),
					glm::vec2(0.48f,  0.32f),
					glm::vec2(0.26f,  0.32f),
					glm::vec2(0.26f,  0.01f)
				};
				break;
			}


			UVs.push_back(faceUVFront);

			std::vector<glm::vec2> UVVector;

			for (std::array<glm::vec2, 6> UVArray : UVs) {
				for (int i = 0; i < 6; i++)
					UVVector.push_back(UVArray[i]);
			}

			return UVVector;
		}

		if (face == Left) {
			std::array<glm::vec2, 6> faceUVLeft;

			switch (blockType) {
			case Grass:
				faceUVLeft = {
					//Left
					glm::vec2(0.5f,  0.34f), //1.0f, 0.0f,
					glm::vec2(0.5f,  0.6f),  //1.0f, 1.0f,
					glm::vec2(0.25f, 0.6f),  //0.0f, 1.0f,
					glm::vec2(0.25f, 0.6f),  //0.0f, 1.0f,
					glm::vec2(0.25f, 0.34f), //0.0f, 0.0f,
					glm::vec2(0.5f,  0.34f)  //1.0f, 0.0f
				};
				break;
			case Dirt:
				faceUVLeft = {
					glm::vec2(0.25f, 0.34f),
					glm::vec2(0.25f, 0.6f),
					glm::vec2(0.0f,  0.6f),
					glm::vec2(0.0f,  0.6f),
					glm::vec2(0.0f,  0.34f),
					glm::vec2(0.25f, 0.34f)

				};
				break;
			case Stone:
				faceUVLeft = {
					//Left
					glm::vec2(0.48f,  0.01f),
					glm::vec2(0.26f,  0.01f),
					glm::vec2(0.26f,  0.33f),
					glm::vec2(0.26f,  0.33f),
					glm::vec2(0.48f,  0.33f),
					glm::vec2(0.48f,  0.01f)
				};
				break;
			}


			UVs.push_back(faceUVLeft);
			
			std::vector<glm::vec2> UVVector;

			for (std::array<glm::vec2, 6> UVArray : UVs) {
				for (int i = 0; i < 6; i++)
					UVVector.push_back(UVArray[i]);
			}

			return UVVector;
		}

		if (face == Back) {
			std::array<glm::vec2, 6> faceUVBack;

			switch (blockType) {
			case Grass:
				faceUVBack = {
					//Back
					glm::vec2(0.0f,  0.7f), //0.0f, 0.0f,
					glm::vec2(0.25f, 0.7f), //1.0f, 0.0f,
					glm::vec2(0.25f, 1.0f), //1.0f, 1.0f,
					glm::vec2(0.25f, 1.0f), //1.0f, 1.0f,
					glm::vec2(0.0f,  1.0f), //0.0f, 1.0f,
					glm::vec2(0.0f,  0.7f)  //0.0f, 0.0f
				};
				break;
			case Dirt:
				faceUVBack = {
					glm::vec2(0.0f,  0.34f),
					glm::vec2(0.25f, 0.34f),
					glm::vec2(0.25f, 0.6f),
					glm::vec2(0.25f, 0.6f),
					glm::vec2(0.0f,  0.6f),
					glm::vec2(0.0f,  0.34f)

				};

				break;
			case Stone:
				faceUVBack = {
					//Back
					glm::vec2(0.26f,  0.01f),
					glm::vec2(0.47f,  0.01f),
					glm::vec2(0.47f,  0.33f),
					glm::vec2(0.47f,  0.33f),
					glm::vec2(0.26f,  0.33f),
					glm::vec2(0.26f,  0.01f)
				};
				break;
			}


			UVs.push_back(faceUVBack);
			
			std::vector<glm::vec2> UVVector;

			for (std::array<glm::vec2, 6> UVArray : UVs) {
				for (int i = 0; i < 6; i++)
					UVVector.push_back(UVArray[i]);
			}

			return UVVector;
		}

		if (face == Right) {
			std::array<glm::vec2, 6> faceUVRight;

			switch (blockType) {
			case Grass:
				faceUVRight = {
					//Right
					glm::vec2(0.5f,  0.34f), //1.0f, 0.0f,
					glm::vec2(0.5f,  0.6f),  //1.0f, 1.0f,
					glm::vec2(0.25f, 0.6f),  //0.0f, 1.0f,
					glm::vec2(0.25f, 0.6f),  //0.0f, 1.0f,
					glm::vec2(0.25f, 0.34f), //0.0f, 0.0f,
					glm::vec2(0.5f,  0.34f)  //1.0f, 0.0f
				};
				break;
			case Dirt:
				faceUVRight = {
					glm::vec2(0.25f,  0.34f),
					glm::vec2(0.25f,  0.6f),
					glm::vec2(0.0f,   0.6f),
					glm::vec2(0.0f,   0.6f),
					glm::vec2(0.0f,   0.34f),
					glm::vec2(0.25f,  0.34f)
				
				};
				break;
			case Stone:
				faceUVRight = {
					//Right
					glm::vec2(0.48f,  0.01f),
					glm::vec2(0.26f,  0.01f),
					glm::vec2(0.26f,  0.33f),
					glm::vec2(0.26f,  0.33f),
					glm::vec2(0.48f,  0.33f),
					glm::vec2(0.48f,  0.01f)
				};
				break;
			}



			UVs.push_back(faceUVRight);
			
			std::vector<glm::vec2> UVVector;

			for (std::array<glm::vec2, 6> UVArray : UVs) {
				for (int i = 0; i < 6; i++)
					UVVector.push_back(UVArray[i]);
			}

			return UVVector;
		}

		if (face == Top) {
			std::array<glm::vec2, 6> faceUVTop;

			switch (blockType) {
			case Grass:
				faceUVTop = {
					//Top
					glm::vec2(0.5f,  0.6f),  //0.0f, 1.0f,
					glm::vec2(0.75f, 0.6f),  //1.0f, 1.0f,
					glm::vec2(0.75f, 0.34f), //1.0f, 0.0f,
					glm::vec2(0.75f, 0.34f), //1.0f, 0.0f,
					glm::vec2(0.5f,  0.34f), //0.0f, 0.0f,
					glm::vec2(0.5f,  0.6f)  //0.0f, 1.0f
				};
				break;
			case Dirt:
				faceUVTop = {
					glm::vec2(0.0f,  0.6f),
					glm::vec2(0.25f, 0.6f),
					glm::vec2(0.25f, 0.34f),
					glm::vec2(0.25f, 0.34f),
					glm::vec2(0.0f,  0.34f),
					glm::vec2(0.0f,  0.6f)
				};

				break;
			case Stone:
				faceUVTop = {
					//Top
					glm::vec2(0.26f,  0.33f),
					glm::vec2(0.48f,  0.33f),
					glm::vec2(0.48f,  0.01f),
					glm::vec2(0.48f,  0.01f),
					glm::vec2(0.26f,  0.01f),
					glm::vec2(0.26f,  0.33f)
				};
				break;
			}


			UVs.push_back(faceUVTop);
			
			std::vector<glm::vec2> UVVector;

			for (std::array<glm::vec2, 6> UVArray : UVs) {
				for (int i = 0; i < 6; i++)
					UVVector.push_back(UVArray[i]);
			}

			return UVVector;
		}

		if (face == Bottom) {
			std::array<glm::vec2, 6> faceUVBottom;

			switch (blockType) {
			case Grass:
				faceUVBottom = {
					//Bottom
					glm::vec2(0.0f,  0.6f),   //0.0f, 1.0f,
					glm::vec2(0.25f, 0.6f),   //1.0f, 1.0f,
					glm::vec2(0.25f, 0.34f),  //1.0f, 0.0f,
					glm::vec2(0.25f, 0.34f),  //1.0f, 0.0f,
					glm::vec2(0.0f,  0.34f),  //0.0f, 0.0f,
					glm::vec2(0.0f,  0.6f)    //0.0f, 1.0f
				};
				break;
			case Dirt:
				faceUVBottom = {
					//Bottom
					glm::vec2(0.0f,  0.6f),   //0.0f, 1.0f,
					glm::vec2(0.25f, 0.6f),   //1.0f, 1.0f,
					glm::vec2(0.25f, 0.34f),  //1.0f, 0.0f,
					glm::vec2(0.25f, 0.34f),  //1.0f, 0.0f,
					glm::vec2(0.0f,  0.34f),  //0.0f, 0.0f,
					glm::vec2(0.0f,  0.6f)    //0.0f, 1.0f
				};
				break;
			case Stone:
				faceUVBottom = {
					//Bottom
					glm::vec2(0.26f,  0.33f),
					glm::vec2(0.48f,  0.33f),
					glm::vec2(0.48f,  0.01f),
					glm::vec2(0.48f,  0.01f),
					glm::vec2(0.26f,  0.01f),
					glm::vec2(0.26f,  0.33f)
				};
				break;
			}


			UVs.push_back(faceUVBottom);
			
			std::vector<glm::vec2> UVVector;

			for (std::array<glm::vec2, 6> UVArray : UVs) {
				for (int i = 0; i < 6; i++)
					UVVector.push_back(UVArray[i]);
			}

			return UVVector;
		}


		std::vector<glm::vec2> UVVector;

		for (std::array<glm::vec2, 6> UVArray : UVs) {
			for (int i = 0; i < 6; i++)
				UVVector.push_back(UVArray[i]);
		}

		return UVVector;
	};

public:
	void AddFace(const BlockFace face) {
		
		// Try to use glm::vec3
		if (face == Front) {
			std::array<glm::vec3, 6> faceVerticesFront = {
				//Front
				glm::vec3(-0.5f + position.x, -0.5f + position.y,  0.5f + position.z), //  0.0f,  0.7f, //0.0f, 0.0f,
				glm::vec3( 0.5f + position.x, -0.5f + position.y,  0.5f + position.z), //  0.25f, 0.7f, //1.0f, 0.0f,
				glm::vec3( 0.5f + position.x,  0.5f + position.y,  0.5f + position.z), //  0.25f, 1.0f, //1.0f, 1.0f,
				glm::vec3( 0.5f + position.x,  0.5f + position.y,  0.5f + position.z), //  0.25f, 1.0f, //1.0f, 1.0f,
				glm::vec3(-0.5f + position.x,  0.5f + position.y,  0.5f + position.z), //  0.0f,  1.0f, //0.0f, 1.0f,
				glm::vec3(-0.5f + position.x, -0.5f + position.y,  0.5f + position.z)  //  0.0f,  0.7f  //0.0f, 0.0f
			};															   
																		   
			vertices.push_back(faceVerticesFront);						   
																		   
			return;														   
																		   
		}																   
																		   
		if (face == Left) {												   
			std::array< glm::vec3, 6> faceVerticesLeft = {
				//Left													   
				glm::vec3(-0.5f + position.x,  0.5f + position.y,  0.5f + position.z), //   0.5f,  0.34f, //1.0f, 0.0f,
				glm::vec3(-0.5f + position.x,  0.5f + position.y, -0.5f + position.z), //   0.5f,  0.6f,  //1.0f, 1.0f,
				glm::vec3(-0.5f + position.x, -0.5f + position.y, -0.5f + position.z), //   0.25f, 0.6f,  //0.0f, 1.0f,
				glm::vec3(-0.5f + position.x, -0.5f + position.y, -0.5f + position.z), //   0.25f, 0.6f,  //0.0f, 1.0f,
				glm::vec3(-0.5f + position.x, -0.5f + position.y,  0.5f + position.z), //   0.25f, 0.34f, //0.0f, 0.0f,
				glm::vec3(-0.5f + position.x,  0.5f + position.y,  0.5f + position.z)  //   0.5f,  0.34f  //1.0f, 0.0f
			};															    
																		    
			vertices.push_back(faceVerticesLeft);						    
			return;														    
		}																    
																		    
		if (face == Back) {												    
			std::array< glm::vec3, 6> faceVerticesBack = {
				//Back													    
				glm::vec3(-0.5f + position.x, -0.5f + position.y, -0.5f + position.z), //  0.0f,  0.7f, //0.0f, 0.0f,
				glm::vec3( 0.5f + position.x, -0.5f + position.y, -0.5f + position.z), //  0.25f, 0.7f, //1.0f, 0.0f,
				glm::vec3( 0.5f + position.x,  0.5f + position.y, -0.5f + position.z), //  0.25f, 1.0f, //1.0f, 1.0f,
				glm::vec3( 0.5f + position.x,  0.5f + position.y, -0.5f + position.z), //  0.25f, 1.0f, //1.0f, 1.0f,
				glm::vec3(-0.5f + position.x,  0.5f + position.y, -0.5f + position.z), //  0.0f,  1.0f, //0.0f, 1.0f,
				glm::vec3(-0.5f + position.x, -0.5f + position.y, -0.5f + position.z)  //  0.0f,  0.7f  //0.0f, 0.0f
			};															    
																		    
			vertices.push_back(faceVerticesBack);						    
			return;														    
		}																    
																		    
		if (face == Right) {											    
			std::array< glm::vec3, 6> faceVerticesRight = {
				//Right													    
				 glm::vec3(0.5f + position.x,  0.5f + position.y,  0.5f + position.z), //  0.5f,  0.34f, //1.0f, 0.0f,
				 glm::vec3(0.5f + position.x,  0.5f + position.y, -0.5f + position.z), //  0.5f,  0.6f,  //1.0f, 1.0f,
				 glm::vec3(0.5f + position.x, -0.5f + position.y, -0.5f + position.z), //  0.25f, 0.6f,  //0.0f, 1.0f,
				 glm::vec3(0.5f + position.x, -0.5f + position.y, -0.5f + position.z), //  0.25f, 0.6f,  //0.0f, 1.0f,
				 glm::vec3(0.5f + position.x, -0.5f + position.y,  0.5f + position.z), //  0.25f, 0.34f, //0.0f, 0.0f,
				 glm::vec3(0.5f + position.x,  0.5f + position.y,  0.5f + position.z)  //  0.5f,  0.34f  //1.0f, 0.0f
			};

			vertices.push_back(faceVerticesRight);
			return;
		}

		if (face == Top) {
			std::array< glm::vec3, 6> faceVerticesTop = {
				//Top
				glm::vec3(-0.5f + position.x,  0.5f + position.y, -0.5f + position.z), //  0.5f,  0.6f,  //0.0f, 1.0f,
				glm::vec3( 0.5f + position.x,  0.5f + position.y, -0.5f + position.z), //  0.75f, 0.6f,  //1.0f, 1.0f,
				glm::vec3( 0.5f + position.x,  0.5f + position.y,  0.5f + position.z), //  0.75f, 0.34f, //1.0f, 0.0f,
				glm::vec3( 0.5f + position.x,  0.5f + position.y,  0.5f + position.z), //  0.75f, 0.34f, //1.0f, 0.0f,
				glm::vec3(-0.5f + position.x,  0.5f + position.y,  0.5f + position.z), //  0.5f,  0.34f, //0.0f, 0.0f,
				glm::vec3(-0.5f + position.x,  0.5f + position.y, -0.5f + position.z)  //  0.5f,  0.6f   //0.0f, 1.0f
			};															   
																		   
			vertices.push_back(faceVerticesTop);						   
			return;														   
		}																   
																		   
		if (face == Bottom) {											   
			std::array< glm::vec3, 6> faceVerticesBottom = {
				//Bottom												   
				glm::vec3(-0.5f + position.x, -0.5f + position.y, -0.5f + position.z), //  0.0f,  0.6f,   //0.0f, 1.0f,
				glm::vec3( 0.5f + position.x, -0.5f + position.y, -0.5f + position.z), //  0.25f,  0.6f,  //1.0f, 1.0f,
				glm::vec3( 0.5f + position.x, -0.5f + position.y,  0.5f + position.z), //  0.25f,  0.34f, //1.0f, 0.0f,
				glm::vec3( 0.5f + position.x, -0.5f + position.y,  0.5f + position.z), //  0.25f,  0.34f, //1.0f, 0.0f,
				glm::vec3(-0.5f + position.x, -0.5f + position.y,  0.5f + position.z), //  0.0f,  0.34f,  //0.0f, 0.0f,
				glm::vec3(-0.5f + position.x, -0.5f + position.y, -0.5f + position.z)  //  0.0f,  0.6f    //0.0f, 1.0f
			};

			vertices.push_back(faceVerticesBottom);
			return;
		}

	};

	BlockData GetBlockData() const {

		// Vertices
		std::vector<glm::vec3> verticesVector;
		
		for (std::array<glm::vec3, 6> verticesArray : vertices) {
			for (int i = 0; i < 6; i++)
				verticesVector.push_back(verticesArray[i]);
				//print(verticesArray[i]);
		}

		//std::array<float, 180> verticesArr;
		//std::copy_n(verticesVector.begin(), verticesVector.size(), verticesArr.begin());

		// Triangle Count
		const int vCount = verticesVector.size();
		const int stride = 1; //5

		const int trianglesCount = vCount / stride;

		return { verticesVector, trianglesCount };
	};

};