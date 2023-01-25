#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


class Duck
{
public:
    Duck(glm::vec3 startDrawing);
    ~Duck();

    glm::vec3 start;
    glm::vec3 startDrawing;
    glm::vec3 headOffset;
    int reportedSize;
    Mesh* CreateBody(const std::string& name);
    Mesh* CreateHead(const std::string& name);
    Mesh* CreateBeak(const std::string& name);
    Mesh* CreateLeftWing(const std::string& name);
    Mesh* CreateRightWing(const std::string& name);
};
