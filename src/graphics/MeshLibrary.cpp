#include "graphics/MeshLibrary.h"

bool MeshLibrary::add(const std::string& name, Mesh mesh) {
    std::cout << "[MeshLibrary] Adding mesh: " << name << std::endl;
    if (meshes.count(name)) {
        std::cerr << "[MeshLibrary] Warning: mesh already exists: " << name << std::endl;
        return false;
    }
    meshes.emplace(name, std::move(mesh));
    return true;
}

const Mesh* MeshLibrary::get(const std::string& name) const {
    std::cout << "[MeshLibrary] Looking for mesh: " << name << std::endl;
    auto it = meshes.find(name);
    if (it != meshes.end()){
        std::cout << "[MeshLibrary] Serving mesh: " << name << std::endl;
        return &it->second;
    }
    std::cout << "[MeshLibrary] Mesh: " << name << " not found!\n";
    return nullptr;
}

void MeshLibrary::loadDefaultMeshes() {
    std::cout << "[MeshLibrary] Loading default meshes...\n";
    // Rechteck
    std::vector<glm::vec3> verts = {
        {-0.5f, -0.5f, 0.0f},
        { 0.5f, -0.5f, 0.0f},
        { 0.5f,  0.5f, 0.0f},
        {-0.5f,  0.5f, 0.0f}
    };
    std::vector<unsigned int> inds = {0, 1, 2, 2, 3, 0};
    //add("rectangle", Mesh(verts, inds));

    // Würfel (3D)
    std::vector<Vertex> cubeVertices = {
        // Vorderseite
        {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}},
        {{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}},
        // Rückseite
        {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}},
        {{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}},
        // Linke Seite
        {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}},
        // Rechte Seite
        {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}},
        // Unterseite
        {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}},
        {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}},
        // Obererseite
        {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}},
        {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}}
    };

    std::vector<unsigned int> cubeIndices = {
        // Vorderseite
        0, 1, 2,   2, 3, 0,
        
        // Rückseite
        4, 6, 5,   4, 7, 6,
        
        // Linke Seite
        8, 9, 10,   8, 10, 11,
    
        // Rechte Seite
        12,15,13,  13,15,14,
    
        // Untererseite
        16,17,18,  16,18,19,
    
        // Obererseite
        20,22,21,  20,23,22
    };

    add("cube", Mesh(cubeVertices, cubeIndices));

    std::cout << "[MeshLibrary] Loading default meshes done!\n";
}
