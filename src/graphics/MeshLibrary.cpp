#include "graphics/MeshLibrary.h"

bool MeshLibrary::add(const std::string& name, Mesh mesh) {
    std::cout << "[MeshLibrary] Adding mesh: " << name << std::endl;
    if (meshes.count(name)) return false;
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
        // Oberseite
        {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}},
        {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}}
    };

    std::vector<unsigned int> cubeIndices = {
        0, 1, 2, 2, 3, 0,        // vorne
        4, 5, 6, 6, 7, 4,        // hinten
        8, 9,10,10,11, 8,        // links
        12,13,14,14,15,12,        // rechts
        16,17,18,18,19,16,        // unten
        20,21,22,22,23,20         // oben
    };

    add("cube", Mesh(cubeVertices, cubeIndices));

    std::cout << "[MeshLibrary] Loading default meshes done!\n";
}
