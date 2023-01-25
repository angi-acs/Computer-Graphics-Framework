#include "lab_m1/tema1/tema1.h"

#include <iostream>
#include <random>
#include <ctime>
#include <Windows.h>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/duck.h"
#include "lab_m1/tema1/utils.h"

using namespace std;
using namespace m1;

m1::Tema1::Tema1()
{
}

m1::Tema1::~Tema1()
{
}

void m1::Tema1::Init()
{
    resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // Initialize gameplay
    Mesh* grass = utils::CreateGrass((float)resolution.x, (float)resolution.y);
    AddMeshToList(grass);

    Mesh* life = utils::CreateLives();
    AddMeshToList(life);
    lives = 3;

    Mesh* bullet = utils::CreateBullets();
    AddMeshToList(bullet);
    bullets = 3;

    Mesh* maxScore = utils::CreateMaxScore();
    AddMeshToList(maxScore);
    maximum = 10;
    Mesh* currentScore = utils::CreateCurrentScore();
    AddMeshToList(currentScore);
    current = 0;

    Mesh* maxTime = utils::CreateMaxTime();
    AddMeshToList(maxTime);
    Mesh* currentTime = utils::CreateCurrentTime();
    AddMeshToList(currentTime);
    
    // Initialize the translation and angular steps
    glm::vec3 corner = glm::vec3(0, 0, 0);
    translateX = 0;
    translateY = 0;
    startAngularStep = 0.f;
    wingsAngularStep = 0.f;

    // For the random start angle and random starting point
    low = 0;
    high = M_PI * 0.5f;
    startingPoint = 0.f;
    srand(time(NULL));

    // Initialize directions and speeds
    wingsDirection = 0.8f;
    bodyDirectionX = 1.0f;
    bodyDirectionY = 1.0f;
    speedX = 50.f;
    speedY = 150.f;

    // Initialize body parts
    duck = new Duck(corner);

    Mesh* body = duck->CreateBody("body");
    AddMeshToList(body);

    Mesh* head = duck->CreateHead("head");
    AddMeshToList(head);

    Mesh* beak = duck->CreateBeak("beak");
    AddMeshToList(beak);

    Mesh* leftWing = duck->CreateLeftWing("leftwing");
    AddMeshToList(leftWing);

    Mesh* rightWing = duck->CreateRightWing("rightwing");
    AddMeshToList(rightWing);

    // Initialize auxiliary variables
    duckOnScreen = false;
    dyingDuck = false;
    duckScreenTime = 0.f;
    headOffsetX = duck->headOffset[0];
    headOffsetY = duck->headOffset[1];
    duckNumber = 0;
    increasingSpeed = 3;
    canPlay = true;
}

void m1::Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.8, 0.9, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void m1::Tema1::Update(float deltaTimeSeconds)
{  
    modelMatrix = glm::mat3(1);
    leftWingMatrix = glm::mat3(1);
    rightWingMatrix = glm::mat3(1);

    if (canPlay) {

        if (!duckOnScreen) {
            duckOnScreen = true;
            dyingDuck = false;
            duckScreenTime = 0;
            bullets = 3;

            startingPoint = static_cast<float>(rand()) * static_cast<float>(resolution.x * 0.67) / RAND_MAX;
            startAngularStep = low + static_cast<float>(rand()) * static_cast<float>(high - low) / RAND_MAX;

            translateX = 0;         bodyDirectionX = 1;
            translateY = 0;         bodyDirectionY = 1;
            headOffsetX = duck->headOffset[0] + startingPoint;
            headOffsetY = duck->headOffset[1];

            duckNumber++;
            if (duckNumber >= increasingSpeed) {
                speedX *= 1.25;
                speedY *= 1.25;
                increasingSpeed += 3;
            }
        }

        if (duckOnScreen) {
            duckScreenTime += deltaTimeSeconds;

            if (duckScreenTime < 8.f) {
                if (dyingDuck) {
                    if (headOffsetY > duck->headOffset[1]) {
                        RenderDyingDuck(deltaTimeSeconds);
                    }
                    else {
                        duckOnScreen = false;
                        if (current < maximum) {
                            current++;
                        }
                    }
                }
                else
                {
                    RenderFlyingDuck(deltaTimeSeconds);

                    for (int i = 0; i < duckScreenTime; i++) {
                        RenderMesh(meshes["currentTime"], shaders["VertexColor"], glm::vec3(0.f, i * 20.f, 0), glm::vec3(1.f));
                    }
                }
            }
            else {
                if (headOffsetY < resolution.y) {
                    RenderEscapingDuck(deltaTimeSeconds);
                }
                else
                {
                    duckOnScreen = false;
                    current--;
                    lives--;
                    if (lives == 0) {
                        canPlay = false;
                    }

                }
            }
        }
    }

    for (int i = 0; i < lives; i++) {
        RenderMesh(meshes["life"], shaders["VertexColor"], glm::vec3(i * 50.f, 0.f, 0), glm::vec3(1.f));
    }
   
    for (int i = 0; i < bullets; i++) {
        RenderMesh(meshes["bullet"], shaders["VertexColor"], glm::vec3(i * 50.f, 0.f, 0), glm::vec3(1.f));
    }

    for (int i = 0; i < current && i < maximum; i++) {
        RenderMesh(meshes["currentScore"], shaders["VertexColor"], glm::vec3(i * 20.f, 0.f, 0), glm::vec3(1.f));
    }
    RenderMesh(meshes["maxScore"], shaders["VertexColor"], glm::vec3(0.f, 0.f, 0), glm::vec3(1.f));
    RenderMesh(meshes["maxTime"], shaders["VertexColor"], glm::vec3(0.f, 0.f, 0), glm::vec3(1.f));

    glClear(GL_DEPTH_BUFFER_BIT);
    RenderMesh(meshes["grass"], shaders["VertexColor"], glm::vec3(0.f, 0.f, 0), glm::vec3(1.f));
}

void m1::Tema1::FrameEnd()
{
}

void m1::Tema1::OnKeyPress(int key, int mods)
{
}

void m1::Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

}

void m1::Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    mouseY = resolution.y - mouseY;

    if (abs(headOffsetX - mouseX) <= duck->headOffset[0] * sqrtf(2) &&
        abs(headOffsetY - mouseY) <= duck->headOffset[1]) {
        dyingDuck = true;
    }
    bullets--;
    if (bullets == 0 && dyingDuck == false) {
        duckScreenTime = 8.f;
    }
}

void m1::Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void m1::Tema1::RenderFlyingDuck(float deltaTimeSeconds)
{
    modelMatrix *= transform2D::Translate(startingPoint, 0);

    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Rotate(startAngularStep);
    modelMatrix *= transform2D::Translate(- translateX, - translateY);

    translateX += speedX * bodyDirectionX * deltaTimeSeconds;
    translateY += speedY * bodyDirectionY * deltaTimeSeconds;
    modelMatrix *= transform2D::Translate(translateX, translateY);

    headOffsetX += speedX * bodyDirectionX * deltaTimeSeconds;
    headOffsetY += speedY * bodyDirectionY * deltaTimeSeconds;

    if (headOffsetY < 0 || headOffsetY > resolution.y) {
        bodyDirectionY = - bodyDirectionY;
        startAngularStep = 2 * M_PI - startAngularStep;
        headOffsetY += bodyDirectionY * duck->headOffset[1];
    }
    if (headOffsetX < 0 || headOffsetX > resolution.x) {
        bodyDirectionX = - bodyDirectionX;
        startAngularStep = M_PI - startAngularStep;
        headOffsetX += bodyDirectionX * duck->headOffset[0];
    }

    /* The duck is flying */
    RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);
    glClear(GL_DEPTH_BUFFER_BIT);
    RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix);

    rightWingMatrix = modelMatrix;
    wingsAngularStep += wingsDirection * deltaTimeSeconds;
    if (wingsAngularStep > 0.2) {
        wingsDirection = - 0.8f;
    }
    if (wingsAngularStep < 0.05) {
        wingsDirection = 0.8f;
    }
    rightWingMatrix *= transform2D::Translate(duck->reportedSize * 0.35f, duck->reportedSize * 0.15f);
    rightWingMatrix *= transform2D::Rotate(wingsAngularStep);
    rightWingMatrix *= transform2D::Translate(- duck->reportedSize * 0.35f, - duck->reportedSize * 0.15f);
    RenderMesh2D(meshes["rightwing"], shaders["VertexColor"], rightWingMatrix);

    leftWingMatrix = modelMatrix;
    leftWingMatrix *= transform2D::Translate(duck->reportedSize * 0.35f, duck->reportedSize * 0.85f);
    leftWingMatrix *= transform2D::Rotate(- wingsAngularStep);
    leftWingMatrix *= transform2D::Translate(- duck->reportedSize * 0.35f, - duck->reportedSize * 0.85f);
    RenderMesh2D(meshes["leftwing"], shaders["VertexColor"], leftWingMatrix);
}

void m1::Tema1::RenderEscapingDuck(float deltaTimeSeconds)
{
    modelMatrix *= transform2D::Translate(startingPoint, 0);

    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Rotate(M_PI * 0.5f);
    modelMatrix *= transform2D::Translate(-translateX, -translateY);

    bodyDirectionX = 1;
    bodyDirectionY = 1;

    translateX += 0;
    translateY += speedY * 1.25f * bodyDirectionY * deltaTimeSeconds;
    modelMatrix *= transform2D::Translate(translateX, translateY);

    headOffsetX += 0;
    headOffsetY += speedY * 1.25f * bodyDirectionY * deltaTimeSeconds;

    RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);
    glClear(GL_DEPTH_BUFFER_BIT);
    RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix);

    rightWingMatrix = modelMatrix;
    wingsAngularStep += wingsDirection * deltaTimeSeconds;
    if (wingsAngularStep > 0.2f) {
        wingsDirection = - 0.8f;
    }
    if (wingsAngularStep < 0.05f) {
        wingsDirection = 0.8f;
    }
    rightWingMatrix *= transform2D::Translate(duck->reportedSize * 0.35f, duck->reportedSize * 0.15f);
    rightWingMatrix *= transform2D::Rotate(wingsAngularStep);
    rightWingMatrix *= transform2D::Translate(-duck->reportedSize * 0.35f, -duck->reportedSize * 0.15f);
    RenderMesh2D(meshes["rightwing"], shaders["VertexColor"], rightWingMatrix);

    leftWingMatrix = modelMatrix;
    leftWingMatrix *= transform2D::Translate(duck->reportedSize * 0.35f, duck->reportedSize * 0.85f);
    leftWingMatrix *= transform2D::Rotate(-wingsAngularStep);
    leftWingMatrix *= transform2D::Translate(-duck->reportedSize * 0.35f, -duck->reportedSize * 0.85f);
    RenderMesh2D(meshes["leftwing"], shaders["VertexColor"], leftWingMatrix);
}

void m1::Tema1::RenderDyingDuck(float deltaTimeSeconds)
{
    modelMatrix *= transform2D::Translate(startingPoint, 0);

    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Rotate(- M_PI * 0.5f);
    modelMatrix *= transform2D::Translate(-translateX, -translateY);

    bodyDirectionX = -1;
    bodyDirectionY = -1;

    translateX += 0;
    translateY += speedY * 1.25f * bodyDirectionY * deltaTimeSeconds;
    modelMatrix *= transform2D::Translate(translateX, translateY);

    headOffsetX += 0;
    headOffsetY += speedY * 1.25f * bodyDirectionY * deltaTimeSeconds;

    RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);
    glClear(GL_DEPTH_BUFFER_BIT);
    RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["rightwing"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["leftwing"], shaders["VertexColor"], modelMatrix);
}
