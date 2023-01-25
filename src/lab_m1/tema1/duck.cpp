#include "duck.h"

#include <vector>
#include <iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include "math.h"

using namespace std;

Duck::Duck(glm::vec3 startDrawing)
{
    this->startDrawing = startDrawing;
    this->start = startDrawing;
    this->reportedSize = 100;
    this->headOffset = glm::vec3(reportedSize * 1.8, reportedSize, 0);
}

Duck::~Duck()
{
}

Mesh* Duck::CreateBody(const std::string& name)
{   
    glm::vec3 color = glm::vec3(0.3f, 0.1f, 0.0f); // brown
    
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(start, color),
        VertexFormat(start + glm::vec3(0, reportedSize, 0), color),
        VertexFormat(start + glm::vec3(reportedSize * 1.3, reportedSize * 0.5, 0), color)

    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2
    };

    Mesh* body = new Mesh(name);
    body->InitFromData(vertices, indices);
    return body;
}

Mesh* Duck::CreateHead(const std::string& name)
{
    glm::vec3 color = glm::vec3(0.0f, 0.3f, 0.0f); // dark green
    int triangles = 20;
    float r = 30.0;
    start += glm::vec3(reportedSize * 1.3, reportedSize * 0.5, 0);
 
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

    Mesh* head = new Mesh(name);
    head->InitFromData(vertices, indices);
    head->SetDrawMode(GL_TRIANGLE_FAN);
    return head;
}

Mesh* Duck::CreateBeak(const std::string& name)
{
    glm::vec3 color = glm::vec3(0.9f, 0.7f, 0.0f); // yellow

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(start + glm::vec3(0, - reportedSize * 0.2 , 0), color),
        VertexFormat(start + glm::vec3(0, reportedSize * 0.2, 0), color),
        VertexFormat(start + glm::vec3(reportedSize * 0.45, 0, 0), color)

    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2
    };

    Mesh* beak = new Mesh(name);
    beak->InitFromData(vertices, indices);
    return beak;
}

Mesh* Duck::CreateLeftWing(const std::string& name)
{
    glm::vec3 color = glm::vec3(0.3f, 0.1f, 0.0f); // brown
    start = startDrawing;
    start += glm::vec3(reportedSize * 0.35, reportedSize * 0.85, 0);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(start, color),
        VertexFormat(start + glm::vec3(0, reportedSize * 0.5, 0), color),
        VertexFormat(start + glm::vec3(reportedSize * 0.5, - reportedSize * 0.2, 0), color),

    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2
    };

    Mesh* wing = new Mesh(name);
    wing->InitFromData(vertices, indices);
    return wing;
}

Mesh* Duck::CreateRightWing(const std::string& name)
{
    glm::vec3 color = glm::vec3(0.3f, 0.1f, 0.0f); // brown
    start = startDrawing;
    start += glm::vec3(reportedSize * 0.35, reportedSize * 0.15, 0);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(start, color),
        VertexFormat(start + glm::vec3(0, - reportedSize * 0.5, 0), color),
        VertexFormat(start + glm::vec3(reportedSize * 0.5, reportedSize * 0.2, 0), color),

    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2
    };

    Mesh* wing = new Mesh(name);
    wing->InitFromData(vertices, indices);
    return wing;
}
