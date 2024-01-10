#pragma once

#include "core/scene.hpp"
#include "core/entity.hpp"

class SceneMenu : public Scene {
    public:
        SceneMenu(Game* game);
        ~SceneMenu() = default;

        void OnSceneEnter() override;
        void OnSceneExit() override;
        void OnAction(Action action) override;
        void Update(f64 dt) override;
        void Render(sf::RenderWindow* window) override;
        void DrawGUI() override;

    private:
        void PhysicsCheckButtons(World& world, f64 dt);
        void AddTestButtons();

        struct SystemToggles
        {
            bool UpdatePositions = true;
            bool UpdateAnimations = true;
            bool PhysicsCheckButtons = true;
            bool RenderBackground = true;
            bool RenderSprites = true;
            bool RenderButtons = true;
            bool DebugRenderWorldGrid = true;
            bool DebugRenderColliders = true;
        } 
        m_SystemToggles;

        bool m_ShowDebugOverlay = true;
};