#pragma once

#include "core/scene.hpp"
#include "core/entity.hpp"

struct TileData
{
    std::string TextureSource;
    u32 Width;
    u32 Height;
    i32 OffsetX;
    i32 OffsetY;
};

struct CameraParams
{
    f32 FrameWidth = 1280;
    f32 FrameHeight = 720;
    f32 BoxWidth = 64;
    f32 BoxHeight = 128;
    f32 DampTimeX = 0.15f;
    f32 DampTimeY = 0.3f;
    Vec2 CurrentPosition;
    Vec2 CurrentTarget;
};

class SceneTest : public Scene {
    public:
        SceneTest(Game* game, const char* levelPath);
        ~SceneTest() = default;

        void OnSceneEnter() override;
        void OnSceneExit() override;
        void OnAction(Action action) override;
        void Update(f64 dt) override;
        void Render(sf::RenderWindow* window) override;
        void DrawGUI() override;

    private:
        void SpawnPlayer();
        Vec2 ScreenToWorld(Vec2 screenPos);
        Vec2 WorldToScreen(Vec2 worldPos);

        void UpdateGravity(f64 dt);
        void UpdatePositions(f64 dt);
        void UpdatePlayerMovement(f64 dt);
        void UpdatePlayerAnimationState(f64 dt);
        void UpdateAnimations(f64 dt);
        void UpdateCamera(f64 dt);

        void PhysicsCheckCollisions(f64 dt);

        void RenderBackground(sf::RenderWindow* window);
        void RenderSprites(sf::RenderWindow* window);

        void DebugRenderWorldGrid(sf::RenderWindow* window);
        void DebugRenderColliders(sf::RenderWindow *window);
        void DebugRenderCamera(sf::RenderWindow* window);

        void LoadLevel(const std::string& path);

    private:

        struct SystemToggles
        {
            bool UpdateGravity = true;
            bool UpdatePositions = true;
            bool UpdatePlayerMovement = true;
            bool UpdatePlayerAnimationState = true;
            bool UpdateAnimations = true;
            bool UpdateCamera = true;
            bool PhysicsCheckCollisions = true;
            bool RenderBackground = true;
            bool RenderSprites = true;
            bool DebugRenderWorldGrid = true;
            bool DebugRenderColliders = true;
            bool DebugRenderCamera = true;
        } 
        m_SystemToggles;

        bool m_ShowDebugOverlay = true;
        std::map<std::string, TileData> m_TileData;
        std::shared_ptr<Entity> m_Player;
        sf::View m_Camera;
        CameraParams m_CameraParams;
};