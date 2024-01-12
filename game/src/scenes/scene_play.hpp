#pragma once

#include "core/scene.hpp"
#include "core/entity.hpp"

#include "../systems/debugsystems.hpp"
#include "../systems/rendersystems.hpp"
#include "../systems/updatesystems.hpp"
#include "../systems/physicssystems.hpp"

class ScenePlay : public Scene {
    public:
        ScenePlay(Game* game, const std::string& levelPath);
        ~ScenePlay() = default;

        void OnSceneEnter() override;
        void OnSceneExit() override;
        void OnAction(Action action) override;
        void Update(f64 dt) override;
        void Render(sf::RenderWindow* window) override;
        void OnDrawGUI() override;

    private:
        void DebugRenderWorldGrid(sf::RenderWindow* window);
        void DebugRenderCamera(sf::RenderWindow* window);
        
        void SpawnPlayer();

        void SerializeLevel(const std::string& path);
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
            bool DebugRenderWorldGrid = false;
            bool DebugRenderColliders = false;
            bool DebugRenderCamera = false;
        } 
        m_SystemToggles;

        std::string m_LevelPath;
        char m_LevelSerializationPath[1024];

        bool m_PaintTiles;
        TileData m_CurrentSelectedTile;
        sf::Sprite m_CurrentSelectedTileSprite;

        std::map<std::string, TileData> m_TileData;
        std::shared_ptr<Entity> m_Player;
};