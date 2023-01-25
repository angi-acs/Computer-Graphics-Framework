#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema1/duck.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnKeyPress(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;

        void RenderFlyingDuck(float deltaTimeSeconds);
        void RenderEscapingDuck(float deltaTimeSeconds);
        void RenderDyingDuck(float deltaTimeSeconds);

    protected:
        Duck* duck;
        glm::mat3 modelMatrix;
        glm::mat3 leftWingMatrix;
        glm::mat3 rightWingMatrix;
        glm::ivec2 resolution;
        float translateX, translateY;
        float wingsAngularStep, startAngularStep;
        float wingsDirection, bodyDirectionX, bodyDirectionY;
        float speedX, speedY;
        float low, high;
        float headOffsetX, headOffsetY;
        bool duckOnScreen, dyingDuck, canPlay;
        float duckScreenTime;
        float startingPoint;
        int duckNumber, increasingSpeed;
        int lives, bullets, maximum, current;
    };
}