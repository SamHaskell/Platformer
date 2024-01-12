#include "scene_play.hpp"
#include "scene_menu.hpp"

#include "gui.hpp"

#include "core/game.hpp"
#include "core/resources.hpp"
#include "core/scene.hpp"

#include "maths/maths.hpp"
#include "physics/collisions.hpp"

#include "imgui-SFML.h"
#include "imgui.h"

#include <fstream>
#include <map>

ScenePlay::ScenePlay(Game *game, const std::string &levelPath)
    : Scene(game), m_LevelPath(levelPath) {
    std::strcpy(m_LevelSerializationPath, levelPath.c_str());
}

void ScenePlay::OnSceneEnter() {
    // Register actions.

    RegisterAction(sf::Keyboard::Up, "Up");
    RegisterAction(sf::Keyboard::Left, "Left");
    RegisterAction(sf::Keyboard::Right, "Right");
    RegisterAction(sf::Keyboard::Down, "Down");
    RegisterAction(sf::Keyboard::Space, "Jump");
    RegisterAction(sf::Keyboard::Escape, "MainMenu");
    RegisterAction(sf::Keyboard::F1, "ToggleDebugTools");

    LoadLevel(m_LevelPath);

    for (auto &entry : std::filesystem::recursive_directory_iterator("assets")) {
        if (entry.path().extension() != ".level") {
            continue;
        }

        m_LevelPaths.push_back(entry.path().string());
    }
}

void ScenePlay::OnSceneExit() {}

void ScenePlay::OnAction(Action action) {
    auto &playerActions = m_Player->GetComponent<CPlayerActions>();

    if (action.Name == "Up") {
        playerActions.Up = (action.Type == ActionType::Begin);
    } else if (action.Name == "Left") {
        playerActions.Left = (action.Type == ActionType::Begin);
    } else if (action.Name == "Right") {
        playerActions.Right = (action.Type == ActionType::Begin);
    } else if (action.Name == "Down") {
        playerActions.Down = (action.Type == ActionType::Begin);
    } else if (action.Name == "Jump") {
        playerActions.Jump = (action.Type == ActionType::Begin);
    } else if (action.Name == "ToggleDebugTools" &&
               action.Type == ActionType::Begin) {
        m_ShowDebugTools = !m_ShowDebugTools;
    } else if (action.Name == "MainMenu" && action.Type == ActionType::Begin) {
        m_Game->ChangeScene("MainMenu", std::make_shared<SceneMenu>(m_Game));
    } else if (action.Name == "LeftClick") {
        if (action.Type == ActionType::Begin) {
            if (m_PaintTiles) {
                Vec2 mousePos = ScreenToWorld(
                    {(f32)action.Position.x, (f32)action.Position.y}, m_Camera,
                    *m_Game->GetWindow());
                mousePos = Vec2{(f32)std::floor(mousePos.x / 32.0f),
                                (f32)std::floor(mousePos.y / 32.0f)} *
                           32.0f;
                mousePos.x += 16.0f;

                auto e = m_World.AddEntity("tile");

                e->AddComponent<CTransform>(mousePos, Vec2{2.0f, 2.0f}, 0.0f);
                e->AddComponent<CSprite>(
                    ResourceManager::GetInstance().GetTexture(
                        m_CurrentSelectedTile.TextureSource),
                    m_CurrentSelectedTile.OffsetX,
                    m_CurrentSelectedTile.OffsetY, m_CurrentSelectedTile.Width,
                    m_CurrentSelectedTile.Height, 1.0f);

                e->AddComponent<CTile>(m_CurrentSelectedTile.Name,
                                       m_CurrentSelectedTile.TextureSource,
                                       m_CurrentSelectedTile.Width,
                                       m_CurrentSelectedTile.Height,
                                       m_CurrentSelectedTile.OffsetX,
                                       m_CurrentSelectedTile.OffsetY);

                e->AddComponent<CBoxCollider>(
                    Vec2{(f32)m_CurrentSelectedTile.Width,
                         (f32)m_CurrentSelectedTile.Height});
            }
        } else if (action.Type == ActionType::End) {
        }
    }
}

void ScenePlay::Update(f64 dt) {
    m_World.Update();

    if (m_SystemToggles.UpdateGravity) {
        Systems::UpdateGravity(m_World, dt);
    }

    if (m_SystemToggles.UpdatePlayerMovement) {
        Systems::UpdatePlayerMovement(m_Player, dt);
    }

    if (m_SystemToggles.PhysicsCheckCollisions) {
        Systems::PhysicsCheckCollisions(m_World, dt);
    }

    if (m_SystemToggles.UpdatePositions) {
        Systems::UpdatePositions(m_World, dt);
    }

    Systems::UpdateTileSprites(m_World, dt);

    if (m_SystemToggles.UpdatePlayerAnimationState) {
        Systems::UpdatePlayerAnimationState(m_Player, dt);
    }

    if (m_SystemToggles.UpdateAnimations) {
        Systems::UpdateAnimations(m_World, dt);
    }

    if (m_SystemToggles.UpdateCamera) {
        Systems::UpdateCamera(m_World, dt, m_Camera, m_CameraParams);
    }
}

void ScenePlay::Render(sf::RenderWindow *window) {
    if (m_SystemToggles.RenderBackground) {
        Systems::RenderBackground(window);
    }

    if (m_SystemToggles.RenderSprites) {
        Systems::RenderSprites(window, m_World, m_Camera, m_CameraParams);
    }

    if (m_SystemToggles.DebugRenderColliders) {
        Systems::DebugRenderColliders(window, m_World, m_Camera,
                                      m_CameraParams);
    }

    if (m_SystemToggles.DebugRenderWorldGrid) {
        Systems::DebugRenderWorldGrid(window, m_WorldGrid, m_Camera,
                                      m_CameraParams);
    }

    if (m_SystemToggles.DebugRenderCamera) {
        Systems::DebugRenderCamera(window, m_Camera, m_CameraParams);
    }

    if (m_PaintTiles) {
        Systems::EditorRenderSelectedTileSprite(
            window, m_World, m_Camera, m_CameraParams, m_CurrentSelectedTile,
            m_CurrentSelectedTileSprite,
            ScreenToWorld(m_Game->GetMousePosition(), m_Camera, *window));
    }
}

void ScenePlay::OnDrawGUI() {
    if (!ImGui::Begin("Debug")) {
        ImGui::End();
        return;
    }

    if (ImGui::BeginTabBar("##TabBar")) {
        if (ImGui::BeginTabItem("Level")) {
            ImGui::Separator();

            ImGui::InputText("Level Path", m_LevelSerializationPath,
                             IM_ARRAYSIZE(m_LevelSerializationPath));

            if (ImGui::Button("Save Level")) {
                SerializeLevel(m_LevelSerializationPath);
            }

            ImGui::Separator();

            if (ImGui::Button("Reset Level")) {
                LoadLevel(m_LevelPath);
            }

            GUI::DrawLevelSelector(m_CurrentLevelIndex, m_LevelPaths,
                                   [&](const std::string &path) {
                                       LoadLevel(path);
                                   });

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Entities")) {
            ImGui::Separator();

            ImGui::Text("Entity Count: %u", m_World.GetEntityCount());

            ImGui::Separator();

            for (auto e : m_World.GetEntities()) {
                ImGui::Text("Entity: %s", e->GetTag().c_str());
            }

            ImGui::Separator();

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Systems")) {
            ImGui::SeparatorText("OnUpdate");

            ImGui::Checkbox("Update Gravity", &m_SystemToggles.UpdateGravity);
            ImGui::Checkbox("Update Positions",
                            &m_SystemToggles.UpdatePositions);
            ImGui::Checkbox("Update Player Movement",
                            &m_SystemToggles.UpdatePlayerMovement);
            ImGui::Checkbox("Update Player Animation State",
                            &m_SystemToggles.UpdatePlayerAnimationState);
            ImGui::Checkbox("Update Animations",
                            &m_SystemToggles.UpdateAnimations);

            ImGui::SeparatorText("OnPhysicsUpdate");

            ImGui::Checkbox("Physics Check Collisions",
                            &m_SystemToggles.PhysicsCheckCollisions);

            ImGui::SeparatorText("OnRender");

            ImGui::Checkbox("Render Background",
                            &m_SystemToggles.RenderBackground);
            ImGui::Checkbox("Render Sprites", &m_SystemToggles.RenderSprites);

            ImGui::SeparatorText("Debug");

            ImGui::Checkbox("Debug Render World Grid",
                            &m_SystemToggles.DebugRenderWorldGrid);
            ImGui::Checkbox("Debug Render Colliders",
                            &m_SystemToggles.DebugRenderColliders);
            ImGui::Checkbox("Debug Render Camera",
                            &m_SystemToggles.DebugRenderCamera);

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Tiles")) {
            ImGui::Separator();

            ImGui::Checkbox("Paint Tiles", &m_PaintTiles);

            ImGui::Separator();

            for (auto &[key, value] : m_TileData) {
                sf::Sprite sprite(ResourceManager::GetInstance().GetTexture(
                                      value.TextureSource),
                                  sf::IntRect(value.OffsetX, value.OffsetY,
                                              value.Width, value.Height));

                if (ImGui::ImageButton(key.c_str(), sprite,
                                       sf::Vector2f(32, 32))) {
                    m_CurrentSelectedTile = value;
                    m_CurrentSelectedTileSprite = sprite;
                }

                ImGui::SameLine();

                ImGui::Text("%s", key.c_str());
            }

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Settings")) {
            ImGui::Checkbox("Debug Overlay", &m_ShowDebugOverlay);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}

void ScenePlay::SpawnPlayer() {
    if (m_Player != nullptr) {
        m_Player->Destroy();
    }

    m_Player = m_World.AddEntity("player");

    m_Player->AddComponent<CTransform>(Vec2{16, 64}, Vec2{2, 2}, 0.0f);

    m_Player->AddComponent<CVelocity>(Vec2{0, 0}, 0.0f);

    m_Player->AddComponent<CSprite>(
        ResourceManager::GetInstance().GetTexture("tex_player"), 0, 0, 32, 32,
        2.0f);

    m_Player->AddComponent<CPlayerActions>();

    m_Player->AddComponent<CSpriteAnimator>(
        ResourceManager::GetInstance().GetAnimation("anim_player_idle"));

    m_Player->AddComponent<CGravity>(1600.0f);

    m_Player->AddComponent<CBoxCollider>(Vec2{24, 22});

    m_Player->AddComponent<CPlayerController>(400.0f, 800.0f);
}

void ScenePlay::SerializeLevel(const std::string &path) {
    std::ofstream file(path);
    if (!file.is_open()) {
        NT_ERROR("Failed to open level file to write to: %s", path.c_str());
        return;
    }

    json data;
    {
        std::vector<json> tileset;
        for (auto &[key, value] : m_TileData) {
            json tile;
            tile["name"] = key;
            tile["texture-source"] = value.TextureSource;
            tile["width"] = value.Width;
            tile["height"] = value.Height;
            tile["offset-x"] = value.OffsetX;
            tile["offset-y"] = value.OffsetY;
            tileset.push_back(tile);
        }
        data["tileset"] = tileset;

        std::vector<json> tiles;
        for (auto e : m_World.GetEntitiesWithTag("tile")) {
            if (e->HasComponent<CTransform>() && e->HasComponent<CTile>()) {
                json tile;
                tile["type"] = e->GetComponent<CTile>().Name;
                tile["x"] = std::floor(
                    e->GetComponent<CTransform>().Position.x / 32.0f);
                tile["y"] = std::floor(
                    e->GetComponent<CTransform>().Position.y / 32.0f);
                tiles.push_back(tile);
            }
        }
        data["tiles"] = tiles;

        std::vector<json> decorations;
        for (auto e : m_World.GetEntitiesWithTag("decoration")) {
            if (e->HasComponent<CTransform>() && e->HasComponent<CTile>()) {
                json tile;
                tile["type"] = e->GetComponent<CTile>().Name;
                tile["x"] = std::floor(
                    e->GetComponent<CTransform>().Position.x / 32.0f);
                tile["y"] = std::floor(
                    e->GetComponent<CTransform>().Position.y / 32.0f);
                decorations.push_back(tile);
            }
        }
        data["decorations"] = decorations;
    }

    // Write to file.

    file << data.dump(4);
    file.close();
}

void ScenePlay::LoadLevel(const std::string &path) {
    m_LevelPath = path;

    m_World = World();

    SpawnPlayer();

    m_CameraParams.CurrentPosition =
        m_Player->GetComponent<CTransform>().Position;

    m_Camera.setSize(m_CameraParams.FrameWidth, m_CameraParams.FrameHeight);

    m_Camera.setCenter(
        (i32)m_CameraParams.CurrentPosition.x,
        (i32)(m_CameraParams.FrameHeight - m_CameraParams.CurrentPosition.y));

    std::ifstream file(path);
    if (!file.is_open()) {
        NT_ERROR("Failed to open level data from path: %s", path.c_str());
        return;
    }

    json data = json::parse(file);

    if (data.contains("tileset")) {
        // Process the tile types

        json tileset = data["tileset"];
        for (auto &[key, value] : tileset.items()) {
            std::string name = value["name"];
            std::string textureSource = value["texture-source"];
            u32 width = value["width"];
            u32 height = value["height"];
            i32 offsetX = value["offset-x"];
            i32 offsetY = value["offset-y"];

            m_TileData[name] = {name,   textureSource, width,
                                height, offsetX,       offsetY};
        }
    }

    if (data.contains("tiles")) {
        // Instantiate tile entities

        json tiles = data["tiles"];
        for (auto &[key, value] : tiles.items()) {
            std::string type = value["type"];
            TileData tile = m_TileData[type];

            Vec2 position;
            position.x = (8.0f * 2.0f) + (f32)value["x"] * 2.0 * tile.Width;
            position.y = (f32)value["y"] * 2.0 * tile.Height;

            auto e = m_World.AddEntity("tile");
            e->AddComponent<CTransform>(position, Vec2{2.0f, 2.0f}, 0.0f);
            e->AddComponent<CSprite>(
                ResourceManager::GetInstance().GetTexture(tile.TextureSource),
                tile.OffsetX, tile.OffsetY, tile.Width, tile.Height, 1.0f);

            e->AddComponent<CTile>(type, tile.TextureSource, tile.Width,
                                   tile.Height, tile.OffsetX, tile.OffsetY);

            e->AddComponent<CBoxCollider>(
                Vec2{(f32)tile.Width, (f32)tile.Height});
        }
    }

    if (data.contains("decorations")) {
        // Instantiate decoration entities

        json decs = data["decorations"];
        for (auto &[key, value] : decs.items()) {
            std::string type = value["type"];
            TileData tile = m_TileData[type];

            Vec2 position;
            position.x = (8.0f * 2.0f) + (f32)value["x"] * 2.0 * tile.Width;
            position.y = (f32)value["y"] * 2.0 * tile.Height;

            auto e = m_World.AddEntity("decoration");
            e->AddComponent<CTransform>(position, Vec2{2.0f, 2.0f}, 0.0f);
            e->AddComponent<CSprite>(
                ResourceManager::GetInstance().GetTexture(tile.TextureSource),
                tile.OffsetX, tile.OffsetY, tile.Width, tile.Height, 0.0f);

            e->AddComponent<CTile>(type, tile.TextureSource, tile.Width,
                                   tile.Height, tile.OffsetX, tile.OffsetY);
        }
    }
}