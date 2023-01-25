#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/camera.h"
#include "lab_m1/tema2/drawing.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void RenderScene(float deltaTimeSeconds, const glm::mat4 projectionMatrix, Camera* camera);
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        bool checkCollision();
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::mat4& projectionMatrix, Camera* camera);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

    protected:
        Camera* camera;
        Camera* minimap;
        glm::mat4 perspectiveProjection, orthoProjection;
        ViewportArea miniViewportArea;
        glm::mat4 modelMatrix;
        float speed, translateZ, translateX, angle, scale;
        Drawing* drawing;
        int o0, o1, o2, o3, o4, o5, o6, obstaclesNO;
        vector<glm::vec3> obstacles;
        float obstacleX, obstacleZ;
    };
}   // namespace m1
