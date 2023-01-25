#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema3/camera.h"
#include "components/transform.h"

namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;

    private:
        void FrameStart() override;
        void RenderTree(float treeX, float treeY, float treeZ);
        void RenderStones(float stonesX, float stonesY, float stonesZ);
        void RenderPillar(float pillarX, float pillarY, float pillarZ);
        void RenderGift(float giftX, float giftY, float giftZ);
        void RenderObstacles(float deltaTimeSeconds);
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void checkCollision();
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

    protected:
        Camera3* camera;
        glm::mat4 projectionMatrix;
        glm::mat4 modelMatrix;
        std::unordered_map<std::string, Texture2D*> mapTextures;
        float angle, planAngle, playerAngle, speed;
        glm::vec4 coords[20];
        int obstacles, frames, mouse;
        glm::ivec2 resolution;
        glm::vec3 lightDirection;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;
        int spotlight;
        bool canPlay;
        glm::vec3 colors[5];
    };
}   // namespace m1