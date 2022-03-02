#include "Renderer.h"

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    va.Bind();
    ib.Bind();
    shader.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
    
}

void Renderer::Draw(const VertexArray& va, const int trianglesCount, const Shader& shader) const {
    va.Bind();
    shader.Bind();

    GLCall(glDrawArrays(GL_TRIANGLES, 0, trianglesCount));
}


void Renderer::Clear() const
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}