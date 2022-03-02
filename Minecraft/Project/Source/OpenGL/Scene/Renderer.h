#pragma once

#include "../Model_Headers/VertexArray.hpp"
#include "../Model_Headers/VertexBuffer.hpp"
#include "../Model_Headers/IndexBuffer.hpp"
#include "../Model_Headers/Shader.hpp"
#include "../../Headers/Debug.h"

class Renderer {

public:
	void Clear() const;

	// Vertex Array, Index Buffer, Shaders
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	// Vertex Array, n of vertex, Shaders
	void Draw(const VertexArray& va, const int trianglesCount, const Shader& shader) const;
};