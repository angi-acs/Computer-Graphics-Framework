#include "lab_m1/tema1/utils.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* utils::CreateGrass(float resolutionX, float resolutionY)
{
    glm::vec3 color = glm::vec3(0.f, 0.7f, 0.2f); // green
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, 0, 0), color),
        VertexFormat(glm::vec3(0, resolutionY / 4, 0), color),
        VertexFormat(glm::vec3(resolutionX, resolutionY / 4, 0), color),
        VertexFormat(glm::vec3(resolutionX, 0, 0), color)

    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 3, 2
    };

    Mesh* grass = new Mesh("grass");
    grass->InitFromData(vertices, indices);
    return grass;
}

Mesh* utils::CreateLives()
{
    glm::vec3 color = glm::vec3(1.f, 0.f, 0.f); // red
    int triangles = 20;
    float r = 20.0;
    glm::vec3 start = glm::vec3(30, 650, 0);

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i < triangles; i++)
    {
        float theta = 2 * M_PI * i / triangles;
        float x = r * cos(theta);
        float y = r * sin(theta);

        vertices.push_back(VertexFormat(start + glm::vec3(x, y, 0), color));
        indices.push_back(i);
    }

    Mesh* life = new Mesh("life");
    life->InitFromData(vertices, indices);
    life->SetDrawMode(GL_TRIANGLE_FAN);
    return life;
}

Mesh* utils::CreateBullets()
{
    glm::vec3 color = glm::vec3(0.f, 0.7f, 0.2f); // green

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(20, 580, 0), color),
        VertexFormat(glm::vec3(20, 620, 0), color),
        VertexFormat(glm::vec3(40, 620, 0), color),
        VertexFormat(glm::vec3(40, 580, 0), color)
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 3, 2
    };

    Mesh* bullet = new Mesh("bullet");
    bullet->InitFromData(vertices, indices);
    bullet->SetDrawMode(GL_LINE_LOOP);
    return bullet;
}

Mesh* utils::CreateMaxScore()
{
    glm::vec3 color = glm::vec3(0.f, 0.f, 0.8f); // dark blue

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(20, 680, 0), color),
        VertexFormat(glm::vec3(20, 710, 0), color),
        VertexFormat(glm::vec3(220, 710, 0), color),
        VertexFormat(glm::vec3(220, 680, 0), color)
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 3, 2
    };

    Mesh* maxScore = new Mesh("maxScore");
    maxScore->InitFromData(vertices, indices);
    maxScore->SetDrawMode(GL_LINE_LOOP);
    return maxScore;
}

Mesh* utils::CreateCurrentScore()
{
    glm::vec3 color = glm::vec3(0.f, 0.f, 0.8f); // dark blue

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(20, 680, 0), color),
        VertexFormat(glm::vec3(20, 710, 0), color),
        VertexFormat(glm::vec3(40, 710, 0), color),
        VertexFormat(glm::vec3(40, 680, 0), color)
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 3, 2
    };

    Mesh* currentScore = new Mesh("currentScore");
    currentScore->InitFromData(vertices, indices);
    return currentScore;
}

Mesh* utils::CreateMaxTime()
{
    glm::vec3 color = glm::vec3(0.9f, 0.7f, 0.0f); // yellow

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(1240, 550, 0), color),
        VertexFormat(glm::vec3(1240, 710, 0), color),
        VertexFormat(glm::vec3(1270, 710, 0), color),
        VertexFormat(glm::vec3(1270, 550, 0), color)
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 3, 2
    };

    Mesh* maxTime = new Mesh("maxTime");
    maxTime->InitFromData(vertices, indices);
    maxTime->SetDrawMode(GL_LINE_LOOP);
    return maxTime;
}

Mesh* utils::CreateCurrentTime()
{
    glm::vec3 color = glm::vec3(0.9f, 0.7f, 0.0f); // yellow

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(1240, 550, 0), color),
        VertexFormat(glm::vec3(1240, 570, 0), color),
        VertexFormat(glm::vec3(1270, 570, 0), color),
        VertexFormat(glm::vec3(1270, 550, 0), color)
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 3, 2
    };

    Mesh* currentTime = new Mesh("currentTime");
    currentTime->InitFromData(vertices, indices);
    return currentTime;
}
