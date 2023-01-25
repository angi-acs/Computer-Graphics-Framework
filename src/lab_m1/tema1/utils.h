#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace utils
{
    Mesh* CreateGrass(float resolutionX, float resolutionY);
    Mesh* CreateLives();
    Mesh* CreateBullets();
    Mesh* CreateMaxScore();
    Mesh* CreateCurrentScore();
    Mesh* CreateMaxTime();
    Mesh* CreateCurrentTime();
}