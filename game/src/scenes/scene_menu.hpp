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
        bool m_ShowDebugOverlay = true;
};