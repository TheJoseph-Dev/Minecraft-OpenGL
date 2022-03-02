#pragma once

#include "Block.h"
//#include "Chunk.h"

#include <vector>
#include <array>

//Maths
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct Vertex {
    glm::vec3 position;
    glm::vec2 UVs;
    
    Vertex(glm::vec3 position, glm::vec2 UVs)
        : position(position), UVs(UVs) {};

public:
    std::array<float, 5> ToArray() { return { position.x, position.y, position.z, UVs.x, UVs.y }; };
};

struct Mesh {
    std::vector<float> vertices;
    int trianglesCount;
    std::vector<float> UVs; //Texture Coordinates

    Mesh(std::vector<float> vertices, int trianglesCount, std::vector<float> UVs = std::vector<float>())
        : vertices(vertices), trianglesCount(trianglesCount), UVs(UVs) {};
};


//TODO: Allocate blocks as a single dimensional array, so it can run faster

//template<int tXSize, int tYSize>
class MeshGenerator {

    std::vector<std::vector<glm::vec3>> vertices;
    int trianglesCount;
    std::vector<std::vector<glm::vec2>> UVs;

    //bool* blocks;
    BlockType*** blocks;
    //bool blocks[tXSize + 2][tYSize + 2][tXSize + 2];
    //bool blocks[18][66][18];
    
    //[xSize + 2, ySize + 2, zSize + 2]
    

    ChunkPos chunkPos;

    int xSize, zSize;
    int ySize;

public:
	MeshGenerator(glm::vec2 size, ChunkPos chunkPos) : chunkPos(chunkPos) {
        this->xSize = size.x;
        this->ySize = size.y;
        this->zSize = size.x;
    };
	~MeshGenerator() {};

private:
    void AllocateBlocks()
    {   
        // 3D Array 
        
        blocks = new BlockType** [xSize + 2];

        for (int i = 0; i < xSize + 2; i++) {

            blocks[i] = new BlockType* [ySize + 2];

            for (int j = 0; j < ySize + 2; j++) {
                blocks[i][j] = new BlockType[zSize + 2];
            }
        }
        
    };

    void DeallocateBlocks()
    {
        // 3D array
        
        for (int i = 0; i < xSize + 2; i++) {
            for (int j = 0; j < ySize + 2; j++) {
                delete[] blocks[i][j];
            }

            delete[] blocks[i];
        }

        delete[] blocks;
        
    };

    void SetupBlocks()
    {
        //this.blocks = new BlockType[xSize + 2, ySize + 2, zSize + 2];
        //memset(blocks, false, sizeof(blocks)); // Initialize with false, just in stack allocated arrays
        

        //Sets up whether block is gonna be air or solid
        for (int x = 0; x < xSize + 2; x++)
        {
            for (int y = 0; y < ySize + 1; y++)
            {
                for (int z = 0; z < zSize + 2; z++)
                {
                    glm::vec3 blockPos = glm::vec3(x, y, z);
                    //print(x << ", " << y << ", " << z);
                    auto bType = Block::GetBlockType(ySize, chunkPos, blockPos);

                    //bool value = (bType == Air) ? false : true;
                    blocks[x][y][z] = bType;

                } //z
            } //y
        } //x

    };

public:
    void Generate() {
        
        this->vertices = std::vector<std::vector<glm::vec3>>();
        this->trianglesCount = 0;

        AllocateBlocks();
        SetupBlocks();


        for (int x = 1; x < xSize + 1; x++) //Starts in one to have block to analise when the chunk renders
        {
            for (int y = 0; y < ySize; y++)
            {
                for (int z = 1; z < zSize + 1; z++)
                {

                    if (blocks[x][y][z] != Air)
                    {
                        BlockType curBlockType = blocks[x][y][z];
                        Block block(glm::vec3(x, y, z));
                        //Block::GetBlockType(ySize, chunkPos, glm::vec3(x, y, z)); Takes a second longer calling here


                        if (blocks[x][y][z + 1] == Air) {
                            block.AddFace(Front);
                            //print("Adding Front");
                            this->UVs.push_back(Block::GetBlockUVs(curBlockType, Front));
                        }

                        if (blocks[x][y][z - 1] == Air) {
                            block.AddFace(Back);
                            //print("Adding Back");
                            this->UVs.push_back(Block::GetBlockUVs(curBlockType, Back));
                        }

                        if (blocks[x - 1][y][z] == Air) {
                            block.AddFace(Left);
                            //print("Adding Left");
                            this->UVs.push_back(Block::GetBlockUVs(curBlockType, Left));
                        }

                        if (blocks[x + 1][y][z] == Air) {
                            block.AddFace(Right);
                            //print("Adding Right");
                            this->UVs.push_back(Block::GetBlockUVs(curBlockType, Right));
                        }

                        if (blocks[x][y + 1][z] == Air) {
                            block.AddFace(Top);
                            //print("Adding Top");
                            this->UVs.push_back(Block::GetBlockUVs(curBlockType, Top));
                        }

                        if (y > 0) {
                            if (blocks[x][y - 1][z] == Air) {
                                block.AddFace(Bottom);
                                //print("Adding Bottom");
                                this->UVs.push_back(Block::GetBlockUVs(curBlockType, Bottom));
                            }
                        }

                        
                        BlockData bData = block.GetBlockData();
                        
                        this->vertices.push_back(bData.vertices);
                        this->trianglesCount += bData.trianglesCount;
                    }

                    //print(x << ", " << y << ", " << z);
                } //z
            } //y
        } //x

        DeallocateBlocks();
    };

public:
    Mesh GetMesh() const {

        std::vector<float> mVertices = GetVertices();


        // TODO: Put UVs adn Vertices in a single array again

        Mesh mesh(mVertices, trianglesCount);
        return mesh;
    }

    std::vector<float> GetVertices() const {
        std::vector<float> verticesVector;

        std::vector<glm::vec3> vec3Vector;
        for (std::vector<glm::vec3> v : vertices) {
            for (int i = 0; i < v.size(); i++) {
                //verticesVector.push_back(v.at(i).x);
                vec3Vector.push_back(v.at(i));
            }
            
        }

        std::vector<glm::vec2> vec2Vector;
        for (std::vector<glm::vec2> uv : UVs) {
            for (int i = 0; i < uv.size(); i++) {
                vec2Vector.push_back(uv.at(i));
            }

        }

        for (int i = 0; i < vec3Vector.size(); i++) {
            verticesVector.push_back(vec3Vector.at(i).x);
            verticesVector.push_back(vec3Vector.at(i).y);
            verticesVector.push_back(vec3Vector.at(i).z);

            verticesVector.push_back(vec2Vector.at(i).x);
            verticesVector.push_back(vec2Vector.at(i).y);
        }

        return verticesVector;
    };

    int GetTrianglesCount() const { return this->trianglesCount; };
};