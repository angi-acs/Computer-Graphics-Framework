#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/tema2/camera.h"
#include "lab_m1/tema2/transform3D.h"
#include "lab_m1/tema2/drawing.h"

using namespace std;
using namespace m1;

m1::Tema2::Tema2()
{
}

m1::Tema2::~Tema2()
{
}

void m1::Tema2::Init()
{
    // Initialize cameras
	camera = new Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    minimap = new Camera();
    minimap->Set(glm::vec3(0, 4, 1), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(1000, 20, resolution.x / 5.f, resolution.y / 5.f);

    // Initialize projection matrix
    perspectiveProjection = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.f);
    orthoProjection = glm::ortho(-16.f, 16.f, -12.f, 12.f, 0.01f, 200.f);

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* myShader = new Shader("MyShader");
        myShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "NormalVertexShader.glsl"), GL_VERTEX_SHADER);
        myShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "NormalFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        myShader->CreateAndLink();
        shaders[myShader->GetName()] = myShader;
    }
    {
        Shader* shader = new Shader("Shader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Load predefined meshes
    Mesh* car = new Mesh("car");
    car->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models"), "Car.obj");
    meshes[car->GetMeshID()] = car;

    Mesh* tree = new Mesh("tree");
    tree->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models"), "tree.obj");
    meshes[tree->GetMeshID()] = tree;

    // Initialize new meshes
    drawing = new Drawing();
    Mesh* grass = drawing->CreateGrass();
    AddMeshToList(grass);

    Mesh* circuit = drawing->CreateCircuit();
    AddMeshToList(circuit);

    Mesh* line = drawing->CreateLine();
    AddMeshToList(line);

    drawing->CreateTrees();

    // Initialize variables
    speed = 9.0f;
    translateZ = 0.f;
    translateX = 0.f;
    angle = 0.f;
    scale = 3.5f;
    o0 = 0;
    o1 = drawing->generatedPoints.size() / 2;
    o2 = drawing->generatedPoints.size() / 3;
    o3 = 2 * drawing->generatedPoints.size() / 3;
    o4 = 3 * drawing->generatedPoints.size() / 4;
    o5 = 5 * drawing->generatedPoints.size() / 6;
    o6 = drawing->generatedPoints.size() / 4;
    obstaclesNO = 7;

    for (int i = 0; i < obstaclesNO; i++) {
        obstacles.push_back(glm::vec3(0.f, 0.f, 0.f));
    }
}

void m1::Tema2::FrameStart()
{
    glClearColor(0, 0.9, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void m1::Tema2::RenderScene(float deltaTimeSeconds, const glm::mat4 projectionMatrix, Camera* camera)
{
    // Landscape
    modelMatrix = glm::mat4(1);
    RenderMesh(meshes["grass"], shaders["Shader"], modelMatrix, projectionMatrix, camera);
    glClear(GL_DEPTH_BUFFER_BIT);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Scale(scale, 0, scale);
    RenderMesh(meshes["circuit"], shaders["Shader"], modelMatrix, projectionMatrix, camera);
    glClear(GL_DEPTH_BUFFER_BIT);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Scale(scale, 0, scale);
    RenderMesh(meshes["line"], shaders["Shader"], modelMatrix, projectionMatrix, camera);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Trees
    for (int i = 3; i < drawing->trees.size(); i = i + 3) {
        modelMatrix = glm::mat4(1);
        float x = drawing->trees[i][0] * scale;
        float z = drawing->trees[i][2] * scale;
        modelMatrix *= transform3D::Translate(x, 0, z);
        modelMatrix *= transform3D::Scale(0.1, 0.1, 0.1);
        RenderMesh(meshes["tree"], shaders["MyShader"], modelMatrix, projectionMatrix, camera);
    }

    // Player's car
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(translateX, 0.f, translateZ);
    modelMatrix *= transform3D::RotateOY(angle);
    modelMatrix *= transform3D::RotateOY(M_PI);
    modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    RenderMesh(meshes["car"], shaders["MyShader"], modelMatrix, projectionMatrix, camera);

    // Obstacles
    o0 = o0 % drawing->rightLane.size();
    obstacles[0][0] = drawing->rightLane[o0][0] * scale;
    obstacles[0][2] = drawing->rightLane[o0][2] * scale;
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(obstacles[0][0], 0.f, obstacles[0][2]);
    modelMatrix *= transform3D::RotateOY(angle);
    modelMatrix *= transform3D::RotateOY(M_PI);
    modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    RenderMesh(meshes["car"], shaders["MyShader"], modelMatrix, projectionMatrix, camera);
    o0 += 20;

    o1 = o1 % drawing->rightLane.size();
    obstacles[1][0] = drawing->rightLane[o1][0] * scale;
    obstacles[1][2] = drawing->rightLane[o1][2] * scale;
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(obstacles[1][0], 0.f, obstacles[1][2]);
    modelMatrix *= transform3D::RotateOY(angle);
    modelMatrix *= transform3D::RotateOY(M_PI);
    modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    RenderMesh(meshes["car"], shaders["MyShader"], modelMatrix, projectionMatrix, camera);
    o1 += 5;

    if (o2 < 5) {
        o2 = drawing->leftLane.size() - 1;
    }
    obstacles[2][0] = drawing->leftLane[o2][0] * scale;
    obstacles[2][2] = drawing->leftLane[o2][2] * scale;
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(obstacles[2][0], 0.f, obstacles[2][2]);
    modelMatrix *= transform3D::RotateOY(angle);
    modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    RenderMesh(meshes["car"], shaders["MyShader"], modelMatrix, projectionMatrix, camera);
    o2 -= 5;

    if (o3 < 10) {
        o3 = drawing->leftLane.size() - 1;
    }
    obstacles[3][0] = drawing->leftLane[o3][0] * scale;
    obstacles[3][2] = drawing->leftLane[o3][2] * scale;
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(obstacles[3][0], 0.f, obstacles[3][2]);
    modelMatrix *= transform3D::RotateOY(angle);
    modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    RenderMesh(meshes["car"], shaders["MyShader"], modelMatrix, projectionMatrix, camera);
    o3 -= 10;

    if (o4 < 30) {
        o4 = drawing->leftLane.size() - 1;
    }
    obstacles[4][0] = drawing->leftLane[o4][0] * scale;
    obstacles[4][2] = drawing->leftLane[o4][2] * scale;
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(obstacles[4][0], 0.f, obstacles[4][2]);
    modelMatrix *= transform3D::RotateOY(angle);
    modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    RenderMesh(meshes["car"], shaders["MyShader"], modelMatrix, projectionMatrix, camera);
    o4 -= 30;

    if (o5 < 10) {
        o5 = drawing->leftLane.size() - 1;
    }
    obstacles[5][0] = drawing->leftLane[o5][0] * scale;
    obstacles[5][2] = drawing->leftLane[o5][2] * scale;
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(obstacles[5][0], 0.f, obstacles[5][2]);
    modelMatrix *= transform3D::RotateOY(angle);
    modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    RenderMesh(meshes["car"], shaders["MyShader"], modelMatrix, projectionMatrix, camera);
    o5 -= 10;

    o6 = o6 % drawing->rightLane.size();
    obstacles[6][0] = drawing->rightLane[o6][0] * scale;
    obstacles[6][2] = drawing->rightLane[o6][2] * scale;
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(obstacles[6][0], 0.f, obstacles[6][2]);
    modelMatrix *= transform3D::RotateOY(angle);
    modelMatrix *= transform3D::RotateOY(M_PI);
    modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    RenderMesh(meshes["car"], shaders["MyShader"], modelMatrix, projectionMatrix, camera);
    o6 += 15;
}

void m1::Tema2::Update(float deltaTimeSeconds)
{
    RenderScene(deltaTimeSeconds, perspectiveProjection, camera);
    
    // Draw again in minimap
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    RenderScene(deltaTimeSeconds, orthoProjection, minimap);
}

void m1::Tema2::FrameEnd()
{
}

bool m1::Tema2::checkCollision()
{
    for (int i = 0; i < obstaclesNO; i++) {
        float r = sqrt((translateX - obstacles[i][0]) * (translateX - obstacles[i][0]) +
                        (translateZ - obstacles[i][2]) * (translateZ - obstacles[i][2]));
        if (r < 1.7f) {
            return true;
        }
    }    
    return false;
}

void m1::Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::mat4& projectionMatrix, Camera* camera)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int location = glGetUniformLocation(shader->program, "car_pos");
    glm::vec3 car_pos = glm::vec3(translateX, 0, translateZ);
    glUniform3fv(location, 1, glm::value_ptr(car_pos));

    mesh->Render();
}

void m1::Tema2::OnInputUpdate(float deltaTime, int mods)
{ 
    float checkX, checkZ;

    if (window->KeyHold(GLFW_KEY_W)) {
        // Translate the camera forward
        checkX = (translateX - deltaTime * speed * sin(angle)) / scale;
        checkZ = (translateZ - deltaTime * speed * cos(angle)) / scale;
        if (drawing->isOnRoad(checkX, checkZ) && !checkCollision()) {

            translateZ -= deltaTime * speed * cos(angle);
            translateX -= deltaTime * speed * sin(angle);
            camera->MoveForward(deltaTime * speed);
            minimap->Set(glm::vec3(0, 4, 1) + glm::vec3(translateX, 0, translateZ),
                glm::vec3(translateX, 1, translateZ), glm::vec3(0, 1, 0));
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        // Translate the camera backward
        checkX = (translateX + deltaTime * speed * sin(angle)) / scale;
        checkZ = (translateZ + deltaTime * speed * cos(angle)) / scale;
        if (drawing->isOnRoad(checkX, checkZ) && !checkCollision()) {

            translateZ += deltaTime * speed * cos(angle);
            translateX += deltaTime * speed * sin(angle);
            camera->MoveForward(-deltaTime * speed);
            minimap->Set(glm::vec3(0, 4, 1) + glm::vec3(translateX, 0, translateZ),
                glm::vec3(translateX, 1, translateZ), glm::vec3(0, 1, 0));
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        // Translate the camera to the left
        angle += deltaTime;
        camera->RotateThirdPerson_OY(deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        // Translate the camera to the right
        angle -= deltaTime;
        camera->RotateThirdPerson_OY(-deltaTime);
    }
}

void m1::Tema2::OnKeyPress(int key, int mods)
{
}

void m1::Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void m1::Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}