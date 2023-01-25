#include "drawing.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

Drawing::Drawing()
{
    dist = 0.6f;

    points =
    {
        glm::vec3(0, 0, 0),
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -2),
        glm::vec3(0, 0, -3),
        glm::vec3(0, 0, -4),
        glm::vec3(0, 0, -5),
        glm::vec3(-0.21, 0, -6),
        glm::vec3(-1, 0, -7),
        glm::vec3(-2.02, 0, -7.45),
        glm::vec3(-3.01, 0, -7.45),
        glm::vec3(-4, 0, -7),
        glm::vec3(-4.8, 0, -5.98),
        glm::vec3(-5, 0, -5),
        glm::vec3(-5, 0, -4),
        glm::vec3(-5, 0, -3),
        glm::vec3(-5, 0, -2),
        glm::vec3(-5, 0, -1),
        glm::vec3(-5, 0, 0),
        glm::vec3(-4.93, 0, 1),
        glm::vec3(-4.72, 0, 2),
        glm::vec3(-4.38, 0, 3),
        glm::vec3(-3.83, 0, 4),
        glm::vec3(-3.05, 0, 5.05),
        glm::vec3(-2, 0, 6),
        glm::vec3(-1, 0, 6.63),
        glm::vec3(0, 0, 7.07),
        glm::vec3(1.02, 0, 7.35),
        glm::vec3(2.02, 0, 7.48),
        glm::vec3(2.97, 0, 7.49),
        glm::vec3(4, 0, 7.35),
        glm::vec3(5.02, 0, 7.06),
        glm::vec3(6, 0, 6.63),
        glm::vec3(7, 0, 6),
        glm::vec3(8.05, 0, 5.05),
        glm::vec3(8.86, 0, 3.97),
        glm::vec3(9.38, 0, 3),
        glm::vec3(9.72, 0, 2.03),
        glm::vec3(9.93, 0, 1),
        glm::vec3(10, 0, 0),
        glm::vec3(10, 0, -1),
        glm::vec3(10, 0, -2),
        glm::vec3(10, 0, -3),
        glm::vec3(10, 0, -4),
        glm::vec3(10, 0, -5),
        glm::vec3(9.8, 0, -5.97),
        glm::vec3(9, 0, -7),
        glm::vec3(7.98, 0, -7.45),
        glm::vec3(7, 0, -7.45),
        glm::vec3(6, 0, -7),
        glm::vec3(5.21, 0, -6),
        glm::vec3(5, 0, -5),
        glm::vec3(5, 0, -4),
        glm::vec3(5, 0, -3),
        glm::vec3(5, 0, -2),
        glm::vec3(5, 0, -1),
        glm::vec3(5, 0, 0),
        glm::vec3(4.79, 0, 1),
        glm::vec3(4, 0, 2),
        glm::vec3(3, 0, 2.45),
        glm::vec3(1.98, 0, 2.45),
        glm::vec3(1, 0, 2),
        glm::vec3(0.2, 0, 0.97),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 0, -1),
    };

    generatePoints();
}

Drawing::~Drawing()
{
}

Mesh* Drawing::CreateGrass()
{
   /*vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(-100, 0, -100), glm::vec3(0.f, 0.8f, 0.f)),
        VertexFormat(glm::vec3(-100, 0, 100), glm::vec3(0.f, 0.8f, 0.f)),
        VertexFormat(glm::vec3(100, 0, 100), glm::vec3(0.f, 0.8f, 0.f)),
        VertexFormat(glm::vec3(100, 0, -100), glm::vec3(0.f, 0.8f, 0.f)),
    };

    vector<unsigned int> indices =
    {
        0, 1, 2,
        2, 3, 0
    };*/

    vector<VertexFormat> vertices;
    vector<unsigned int> indices;
    glm::vec3 color = glm::vec3(0.f, 0.8f, 0.f);
    
    for (int i = -100; i < 100; i = i+5) {
        for (int j = -100; j < 100; j = j+5) {
            vertices.push_back(VertexFormat(glm::vec3(i, 0, j), color));
            vertices.push_back(VertexFormat(glm::vec3(i, 0, j+5), color));
            vertices.push_back(VertexFormat(glm::vec3(i+5, 0, j+5), color));
            vertices.push_back(VertexFormat(glm::vec3(i+5, 0, j), color));
        }
    }

    for (int i = 0; i < vertices.size(); i++) {
        indices.push_back(i);
        indices.push_back(i+1);
        indices.push_back(i+2);

        indices.push_back(i+2);
        indices.push_back(i+3);
        indices.push_back(i);
    }

    Mesh* grass = new Mesh("grass");
    grass->InitFromData(vertices, indices);
    grass->SetDrawMode(GL_TRIANGLE_STRIP);
    return grass;
}

Mesh* Drawing::CreateCircuit()
{
    // Initialize variables
    glm::vec3 D, P, R, A;
    glm::vec3 UP = glm::vec3(0, 1, 0);
    glm::vec3 color = glm::vec3(0.1, 0.1, 0.1);
    int n = 0;

    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    for (int i = 0; i < generatedPoints.size() - 1; i++) {
            D = generatedPoints[i + 1] - generatedPoints[i];
            P = glm::normalize(cross(D, UP));
            R = generatedPoints[i] + dist * P;
            A = generatedPoints[i] - dist * P;

            vertices.push_back(VertexFormat(A, color));
            vertices.push_back(VertexFormat(R, color));

            indices.push_back(n);
            indices.push_back(n + 1);
            n += 2;

            rightLane.push_back(generatedPoints[i] + 0.3f * P);
            leftLane.push_back(generatedPoints[i] - 0.3f * P);
    }

    Mesh* circuit = new Mesh("circuit");
    circuit->InitFromData(vertices, indices);
    circuit->SetDrawMode(GL_TRIANGLE_STRIP);
    return circuit;
}

Mesh* Drawing::CreateLine()
{
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;
    glm::vec3 color = glm::vec3(1, 1, 1);

    for (int i = 0; i < points.size(); i++) {
        vertices.push_back(VertexFormat(points[i], color));
        indices.push_back(i);
    }

    Mesh* line = new Mesh("line");
    line->InitFromData(vertices, indices);
    line->SetDrawMode(GL_LINES);
    return line;
}

void Drawing::CreateTrees()
{
    float dist = 0.8f;
    glm::vec3 D, P;
    glm::vec3 UP = glm::vec3(0, 1, 0);

    for (int i = 0; i < points.size() - 1; i++) {
        D = points[i + 1] - points[i];
        P = glm::normalize(cross(D, UP));
        trees.push_back(points[i] + dist * P);
        trees.push_back(points[i] - dist * P);
    }
}

bool Drawing::isOnRoad(float checkX, float checkZ)
{
    float u, x1, x2, z1, z2, d, x, z;

    for (int i = 0; i < points.size() - 1; i++) {
        x1 = points[i][0];
        z1 = points[i][2];
        x2 = points[i+1][0];
        z2 = points[i+1][2];

        d = (x2 - x1) * (x2 - x1) + (z2 - z1) * (z2 - z1);

        u = ((checkX - x1) * (x2 - x1) + (checkZ - z1) * (z2 - z1)) / d;
        u = max(0.f, min(1.f, u));

        x = x1 + u * (x2 - x1);
        z = z1 + u * (z2 - z1);

        d = (float) sqrt((checkX - x) * (checkX - x) + (checkZ - z) * (checkZ - z));
        if (d < dist) {
            return true;
        }
    }
    return false;
}

void Drawing::generatePoints()
{
    float x1, x2, z1, z2, distance, x, z, d;

    for (int i = 0; i < points.size() - 1; i++) {
        x1 = points[i][0];
        z1 = points[i][2];
        x2 = points[i + 1][0];
        z2 = points[i + 1][2];

        distance = sqrt((x2 - x1) * (x2 - x1) + (z2 - z1) * (z2 - z1));
        d = distance * 0.001f;

        for (float j = 0.f; j < distance; j = j + d) {
            x = x1 + (j * (x2 - x1) / distance);
            z = z1 + (j * (z2 - z1) / distance);
            generatedPoints.push_back(glm::vec3(x, 0, z));
        } 
    }
}
