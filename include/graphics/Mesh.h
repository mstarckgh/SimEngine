#pragma once

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>


struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();

    // Kein Kopieren erlaubt (wegen GPU-Ressourcen)
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // Move erlaubt
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void bind() const;
    void draw() const;

    // Getter für physikalische Berechnungen (CPU-Daten)
    const std::vector<Vertex>& getCpuVertices() const { return cpuVertices; }
    const std::vector<unsigned int>& getCpuIndices()  const { return cpuIndices; }

private:
    // GPU-Seite
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    // CPU-Seite
    std::vector<Vertex> cpuVertices;
    std::vector<unsigned int> cpuIndices;
    int indexCount = 0;
};