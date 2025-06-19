#pragma once
#include <unordered_map>
#include <string>
#include "graphics/Mesh.h"

class MeshLibrary {
public:
    bool add(const std::string& name, Mesh mesh);
    const Mesh* get(const std::string& name) const;
    void loadDefaultMeshes();  // Rechteck, Kreis, Dreieck usw.

private:
    std::unordered_map<std::string, Mesh> meshes;
};
