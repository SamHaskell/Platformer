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

        void UpdatePositions(f64 dt);
        void UpdatePlayerAnimationState(f64 dt);
        void UpdateAnimations(f64 dt);
        void RenderSprites(sf::RenderWindow* window);

        void LoadLevel(const std::string& path);

    private:
        std::shared_ptr<Entity> m_Player;
};