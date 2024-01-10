#pragma once

#include "core/scene.hpp"

class SceneSandbox : public Scene {
    public:
        SceneSandbox(Game* game);
        ~SceneSandbox() = default;

        void OnSceneEnter() override;
        void OnSceneExit() override;
        void OnAction(Action action) override;
        void Update(f64 dt) override;
        void Render(sf::RenderWindow* window) override;
        void DrawGUI() override;

    private:
        std::shared_ptr<Entity> m_Player;
        bool m_ShowDebugOverlay = false;
};