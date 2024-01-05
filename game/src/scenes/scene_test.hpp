#pragma once

#include "core/scene.hpp"
#include "core/entity.hpp"

class SceneTest : public Scene {
    public:
        SceneTest(const Game* game);
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

        void UpdateGravity(f64 dt);
        void UpdatePositions(f64 dt);
        void UpdatePlayerMovement(f64 dt);
        void UpdatePlayerAnimationState(f64 dt);
        void UpdateAnimations(f64 dt);

        void PhysicsCheckCollisions(f64 dt);

        void RenderSprites(sf::RenderWindow* window);

        void DebugRenderColliders(sf::RenderWindow *window);

        void LoadLevel(const std::string& path);

    private:

        struct SystemToggles
        {
            bool UpdateGravity = true;
            bool UpdatePositions = true;
            bool UpdatePlayerMovement = true;
            bool UpdatePlayerAnimationState = true;
            bool UpdateAnimations = true;

            bool PhysicsCheckCollisions = true;

            bool RenderSprites = true;

            bool DebugRenderColliders = false;
        } m_SystemToggles;

        bool m_ShowDebugOverlay = false;

        std::shared_ptr<Entity> m_Player;
        sf::View m_Camera;
};