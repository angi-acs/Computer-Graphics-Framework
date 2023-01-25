#include "lab_m1/tema3/tema3.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/tema3/camera.h"
#include "lab_m1/tema3/transform3D.h"

using namespace std;
using namespace m1;

m1::Tema3::Tema3()
{
}

m1::Tema3::~Tema3()
{
}

void m1::Tema3::Init()
{
	// Initialize camera
	camera = new Camera3();
	camera->Set(glm::vec3(0, 3, 6.f), glm::vec3(0, 1, 1), glm::vec3(0, 1, 0));
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.f);

    // Create a shader program
    {
        Shader* shader = new Shader("Shader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Load textures
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "my_textures");
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "leaves.jpg").c_str(), GL_REPEAT);
        mapTextures["pine"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "stone.jpg").c_str(), GL_REPEAT);
        mapTextures["stone"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "snow.jpg").c_str(), GL_REPEAT);
        mapTextures["snow"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "wood.jpg").c_str(), GL_REPEAT);
        mapTextures["wood"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "pillar.jpg").c_str(), GL_REPEAT);
        mapTextures["pillar"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "light.jpg").c_str(), GL_REPEAT);
        mapTextures["light"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "gift.jpg").c_str(), GL_REPEAT);
        mapTextures["gift"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "skis.jpg").c_str(), GL_REPEAT);
        mapTextures["skis"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "player.jpg").c_str(), GL_REPEAT);
        mapTextures["player"] = texture;
    }

    // Load predefined meshes
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* tree = new Mesh("cone");
        tree->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models"), "cone.obj");
        meshes[tree->GetMeshID()] = tree;
    }
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("skis");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models"), "skis.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a simple quad
    {
        vector<glm::vec3> vertices
        {
            glm::vec3(1.f,  0.f, 1.f),    // top right
            glm::vec3(1.f,  0.f, -1.f),   // bottom right
            glm::vec3(-1.f, 0.f, -1.f),   // bottom left
            glm::vec3(-1.f, 0.f, 1.f),    // top left
        };

        vector<glm::vec3> normals
        {
            glm::vec3(0, 1, 1),
            glm::vec3(1, 0, 1),
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0)
        };

        vector<glm::vec2> textureCoords
        {
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.f, 1.f),
            glm::vec2(1.f, 1.f),
            glm::vec2(1.f, 0.f)
        };

        vector<unsigned int> indices =
        {
            0, 1, 3,
            1, 2, 3
        };

        Mesh* mesh = new Mesh("quad");
        mesh->InitFromData(vertices, normals, textureCoords, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Initialize variables
    angle = 0.f;
    planAngle = RADIANS(20);
    playerAngle = 0.f;
    speed = 2.f;
    obstacles = 1;
    frames = 0;
    srand(time(NULL));
    resolution = window->GetResolution();
    canPlay = true;

    for (int i = 0; i < 20; i += 4) {
        coords[i] = glm::vec4(0.f, 0.f, 5.f, 0.f);
        coords[i+1] = glm::vec4(0.f, 0.f, 5.f, 1.f);
        coords[i+2] = glm::vec4(0.f, 0.f, 5.f, 2.f);
        coords[i+3] = glm::vec4(0.f, 0.f, 5.f, 3.f);
    }

    // Lights
    lightDirection = glm::vec3(0, -1, 0);
    materialShininess = 50;
    materialKd = 1.f;
    materialKs = 1.f;
    spotlight = 0;

    colors[0] = glm::vec3(0.6, 0.1, 0.5);
    colors[1] = glm::vec3(0.7, 0, 0);
    colors[2] = glm::vec3(0.1, 0.9, 0.2);
    colors[3] = glm::vec3(0, 0.2, 0.9);
    colors[4] = glm::vec3(1, 1, 0.2);
}

void m1::Tema3::FrameStart()
{
    glClearColor(0, 0.9, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void m1::Tema3::RenderTree(float treeX, float treeY, float treeZ)
{
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::RotateOX(planAngle);
    modelMatrix *= transform3D::Translate(treeX, treeY + 1.f, treeZ);
    modelMatrix *= transform3D::Scale(0.6f, 0.7f, 0.6f);
    RenderMesh(meshes["cone"], shaders["Shader"], modelMatrix, mapTextures["pine"]);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::RotateOX(planAngle);
    modelMatrix *= transform3D::Translate(treeX, treeY, treeZ);
    modelMatrix *= transform3D::Scale(0.5f, 0.8f, 0.5f);
    RenderMesh(meshes["box"], shaders["Shader"], modelMatrix, mapTextures["wood"]);
}

void m1::Tema3::RenderStones(float stonesX, float stonesY, float stonesZ)
{
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::RotateOX(planAngle);
    modelMatrix *= transform3D::Translate(stonesX, stonesY, stonesZ);
    modelMatrix *= transform3D::Scale(0.7f, 0.7f, 0.7f);
    RenderMesh(meshes["sphere"], shaders["Shader"], modelMatrix, mapTextures["stone"]);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::RotateOX(planAngle);
    modelMatrix *= transform3D::Translate(stonesX + 0.5f, stonesY, stonesZ);
    modelMatrix *= transform3D::Scale(0.5f, 0.5f, 0.5f);
    RenderMesh(meshes["sphere"], shaders["Shader"], modelMatrix, mapTextures["stone"]);
}

void m1::Tema3::RenderPillar(float pillarX, float pillarY, float pillarZ)
{
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::RotateOX(planAngle);
    modelMatrix *= transform3D::Translate(pillarX, pillarY, pillarZ);
    modelMatrix *= transform3D::Scale(0.2f, 3.f, 0.2f);
    RenderMesh(meshes["box"], shaders["Shader"], modelMatrix, mapTextures["pillar"]);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::RotateOX(planAngle);
    modelMatrix *= transform3D::Translate(pillarX, pillarY + 1.5f, pillarZ);
    modelMatrix *= transform3D::Scale(0.7f, 0.7f, 0.7f);
    RenderMesh(meshes["sphere"], shaders["Shader"], modelMatrix, mapTextures["light"]);
}

void m1::Tema3::RenderGift(float giftX, float giftY, float giftZ)
{
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::RotateOX(planAngle);
    modelMatrix *= transform3D::Translate(giftX, giftY, giftZ);
    modelMatrix *= transform3D::Scale(0.6f, 0.6f, 0.6f);
    RenderMesh(meshes["box"], shaders["Shader"], modelMatrix, mapTextures["gift"]);
}

void m1::Tema3::RenderObstacles(float deltaTimeSeconds)
{
    for (int i = 0; i < obstacles; i++) {
        if (coords[i][2] < -20.f || coords[i][0] == 0.f) {
            float random = -15 + static_cast<float>(rand()) * static_cast<float>(15 - (-15)) / RAND_MAX;
            coords[i][0] = random;
            coords[i][2] = 5.f;
        }

        coords[i][2] -= speed * deltaTimeSeconds * cos(playerAngle);
        coords[i][0] -= speed * deltaTimeSeconds * sin(playerAngle);
        

        if (coords[i][3] == 0.f) {
            RenderTree(coords[i][0], coords[i][1], coords[i][2]);
        }
        else if (coords[i][3] == 1.f) {
            RenderStones(coords[i][0], coords[i][1], coords[i][2]);
        }
        else if (coords[i][3] == 2.f) {
            spotlight = 1;
            RenderPillar(coords[i][0], coords[i][1], coords[i][2]);
            spotlight = 0;
        }
        else if (coords[i][3] == 3.f) {
            RenderGift(coords[i][0], coords[i][1], coords[i][2]);
        }
    }

    frames++;
    if (frames > 50) {
        if (obstacles < 20) {
            obstacles++;
        }
        frames = 0;
    }
}

void m1::Tema3::Update(float deltaTimeSeconds)
{
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::RotateOX(planAngle);
    modelMatrix *= transform3D::RotateOY(playerAngle);
    modelMatrix *= transform3D::Translate(0.f, -0.05f, 0.f);
    modelMatrix *= transform3D::Scale(100.f, 0.f, 100.f);
    RenderMesh(meshes["quad"], shaders["Shader"], modelMatrix, mapTextures["snow"]);

    if (canPlay) {
        checkCollision();

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::RotateOX(planAngle);
        modelMatrix *= transform3D::RotateOY(playerAngle);
        modelMatrix *= transform3D::Translate(-0.2f, 0.f, 0.f);
        modelMatrix *= transform3D::Scale(0.025f, 0.f, 0.025f);
        modelMatrix *= transform3D::RotateOX(-M_PI_2);
        RenderMesh(meshes["skis"], shaders["Shader"], modelMatrix, mapTextures["skis"]);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::RotateOX(planAngle);
        modelMatrix *= transform3D::RotateOY(playerAngle);
        modelMatrix *= transform3D::Translate(0.2f, 0.f, 0.f);
        modelMatrix *= transform3D::Scale(0.025f, 0.f, 0.025f);
        modelMatrix *= transform3D::RotateOX(-M_PI_2);
        RenderMesh(meshes["skis"], shaders["Shader"], modelMatrix, mapTextures["skis"]);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::RotateOX(planAngle);
        modelMatrix *= transform3D::RotateOY(playerAngle);
        modelMatrix *= transform3D::Translate(0.f, 0.25f, 0.f);
        modelMatrix *= transform3D::Scale(0.5f, 0.5f, 0.5f);
        RenderMesh(meshes["box"], shaders["Shader"], modelMatrix, mapTextures["player"]);

        RenderObstacles(deltaTimeSeconds);
    }
}

void m1::Tema3::FrameEnd()
{
}

void m1::Tema3::checkCollision()
{
    for (int i = 0; i < obstacles; i++) {
        if (coords[i][0] > -1.f && coords[i][0] < 1.f &&
            coords[i][2] > -1.f && coords[i][2] < 1.f) {
            if (coords[i][3] == 3.f) {
                coords[i] = glm::vec4(0.f, 0.f, 5.f, 3.f);
            }
            else {
                canPlay = false;

                for (int i = 0; i < 20; i += 4) {
                    coords[i] = glm::vec4(0.f, 0.f, 5.f, 0.f);
                    coords[i + 1] = glm::vec4(0.f, 0.f, 5.f, 1.f);
                    coords[i + 2] = glm::vec4(0.f, 0.f, 5.f, 2.f);
                    coords[i + 3] = glm::vec4(0.f, 0.f, 5.f, 3.f);
                }

                obstacles = 1;
                frames = 0;
            }

        }
    }
}

void m1::Tema3::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    int light_position = glGetUniformLocation(shader->program, "point_light_pos");
    glUniform4fv(light_position, 20, glm::value_ptr(coords[0]));

    int light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(object_color, 5, glm::value_ptr(colors[0]));

    int spotlight_loc = glGetUniformLocation(shader->program, "spotlight");
    glUniform1i(spotlight_loc, spotlight);

    int obstacles_loc = glGetUniformLocation(shader->program, "obstacles");
    glUniform1i(obstacles_loc, obstacles);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void m1::Tema3::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 5.0f;

    if (window->KeyHold(GLFW_KEY_W)) {
        camera->MoveForward(deltaTime * cameraSpeed);
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        angle += deltaTime;
        camera->RotateThirdPerson_OY(deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        camera->MoveForward(-deltaTime * cameraSpeed);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        angle -= deltaTime;
        camera->RotateThirdPerson_OY(-deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_Q)) {
        camera->TranslateUpward(-deltaTime * cameraSpeed);
    }

    if (window->KeyHold(GLFW_KEY_E)) {
        camera->TranslateUpward(deltaTime * cameraSpeed);
    }
}

void m1::Tema3::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_F) {
        canPlay = true;
    }
}

void m1::Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    int posX = mouseX;
    mouse = mouseX;

    int width = resolution.x / 2;

    posX -= width;

    playerAngle = M_PI_2 * posX / width;
}

void m1::Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}
