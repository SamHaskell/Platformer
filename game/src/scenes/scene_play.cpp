#include "scene_play.hpp"
#include "scene_menu.hpp"

#include "../systems/debugsystems.hpp"
#include "../systems/rendersystems.hpp"
#include "../systems/updatesystems.hpp"
#include "../systems/physicssystems.hpp"

#include "core/game.hpp"
#include "core/resources.hpp"
#include "core/scene.hpp"
#include "maths/maths.hpp"
#include "physics/collisions.hpp"

#include "imgui.h"
#include "imgui-SFML.h"

#include <map>
#include <fstream>

ScenePlay::ScenePlay(Game *game, const char* levelPath) : Scene(game)
{
    LoadLevel(levelPath);
}

void ScenePlay::OnSceneEnter()
{
    // Register actions.

    RegisterAction(sf::Keyboard::Up, "Up");
    RegisterAction(sf::Keyboard::Left, "Left");
    RegisterAction(sf::Keyboard::Right, "Right");
    RegisterAction(sf::Keyboard::Down, "Down");
    RegisterAction(sf::Keyboard::Space, "Jump");
    RegisterAction(sf::Keyboard::Escape, "MainMenu");
    RegisterAction(sf::Keyboard::F1, "ToggleDebugOverlay");

    SpawnPlayer();

    m_CameraParams.CurrentPosition = m_Player->GetComponent<CTransform>().Position;

    m_Camera.setSize(m_CameraParams.FrameWidth, m_CameraParams.FrameHeight);

    m_Camera.setCenter(
        (i32) m_CameraParams.CurrentPosition.x, 
        (i32)(m_CameraParams.FrameHeight - m_CameraParams.CurrentPosition.y)
    );
}

void ScenePlay::OnSceneExit()
{

}

void ScenePlay::OnAction(Action action)
{
    auto &playerActions = m_Player->GetComponent<CPlayerActions>();

    if (action.Name == "Up")
    {
        playerActions.Up = (action.Type == ActionType::Begin);
    }
    else if (action.Name == "Left")
    {
        playerActions.Left = (action.Type == ActionType::Begin);
    }
    else if (action.Name == "Right")
    {
        playerActions.Right = (action.Type == ActionType::Begin);
    }
    else if (action.Name == "Down")
    {
        playerActions.Down = (action.Type == ActionType::Begin);
    }
    else if (action.Name == "Jump")
    {
        playerActions.Jump = (action.Type == ActionType::Begin);
    }
    else if (action.Name == "ToggleDebugOverlay" && action.Type == ActionType::Begin)
    {
        m_ShowDebugOverlay = !m_ShowDebugOverlay;
    }
    else if (action.Name == "MainMenu" && action.Type == ActionType::Begin)
    {
        m_Game->ChangeScene("MainMenu", std::make_shared<SceneMenu>(m_Game));
    }
    else if (action.Name == "LeftClick")
    {
        if (action.Type == ActionType::Begin)
        {
        }
        else if (action.Type == ActionType::End)
        {
        }
    }
}

void ScenePlay::Update(f64 dt)
{
    m_World.Update();

    if (m_SystemToggles.UpdateGravity)
    {
        Systems::UpdateGravity(m_World, dt);
    }

    if (m_SystemToggles.UpdatePlayerMovement)
    {
        Systems::UpdatePlayerMovement(m_Player, dt);
    }

    if (m_SystemToggles.PhysicsCheckCollisions)
    {
        Systems::PhysicsCheckCollisions(m_World, dt);
    }

    if (m_SystemToggles.UpdatePositions)
    {
        Systems::UpdatePositions(m_World, dt);
    }

    if (m_SystemToggles.UpdatePlayerAnimationState)
    {
        Systems::UpdatePlayerAnimationState(m_Player, dt);
    }

    if (m_SystemToggles.UpdateAnimations)
    {
        Systems::UpdateAnimations(m_World, dt);
    }

    if (m_SystemToggles.UpdateCamera)
    {
        Systems::UpdateCamera(m_World, dt, m_Camera, m_CameraParams);
    }
}

void ScenePlay::Render(sf::RenderWindow *window)
{
    if (m_SystemToggles.RenderBackground)
    {
        Systems::RenderBackground(window);
    }

    if (m_SystemToggles.RenderSprites)
    {
        Systems::RenderSprites(window, m_World, m_Camera, m_CameraParams);
    }

    if (m_SystemToggles.DebugRenderColliders)
    {
        Systems::DebugRenderColliders(window, m_World, m_Camera, m_CameraParams);
    }

    if (m_SystemToggles.DebugRenderWorldGrid)
    {
        DebugRenderWorldGrid(window);
    }

    if (m_SystemToggles.DebugRenderCamera)
    {
        DebugRenderCamera(window);
    }
}

void ScenePlay::DrawGUI()
{
    if (!m_ShowDebugOverlay)
    {
        return;
    }

    ImGuiIO &io = ImGui::GetIO();
    ImVec2 mousePos = io.MousePos;
    f32 deltaTime = io.DeltaTime;

    static bool useOverlay = false;

    if (useOverlay)
    {
        ImGui::Begin("Overlay Panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
        ImGui::SetWindowPos(ImVec2(10, 10));
        ImGui::SetWindowSize(ImVec2(250, 50));

        ImGui::Text("Mouse Position: (%.1f, %.1f)", mousePos.x, mousePos.y);
        ImGui::Text("Frame Time: %.3f (ms)", deltaTime * 1000.0f);
        ImGui::PopStyleVar();
        ImGui::End();
    }

    if (!ImGui::Begin("Debug"))
    {
        ImGui::End();
        return;
    }

    if (ImGui::BeginTabBar("##TabBar"))
    {
        if (ImGui::BeginTabItem("Entities"))
        {
            ImGui::Separator();

            ImGui::Text("Entity Count: %u", m_World.GetEntityCount());

            ImGui::Separator();

            for (auto e : m_World.GetEntities())
            {
                ImGui::Text("Entity: %s", e->GetTag().c_str());
            }

            ImGui::Separator();

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Systems"))
        {
            ImGui::SeparatorText("OnUpdate");

            ImGui::Checkbox("Update Gravity", &m_SystemToggles.UpdateGravity);
            ImGui::Checkbox("Update Positions", &m_SystemToggles.UpdatePositions);
            ImGui::Checkbox("Update Player Movement", &m_SystemToggles.UpdatePlayerMovement);
            ImGui::Checkbox("Update Player Animation State", &m_SystemToggles.UpdatePlayerAnimationState);
            ImGui::Checkbox("Update Animations", &m_SystemToggles.UpdateAnimations);

            ImGui::SeparatorText("OnPhysicsUpdate");

            ImGui::Checkbox("Physics Check Collisions", &m_SystemToggles.PhysicsCheckCollisions);

            ImGui::SeparatorText("OnRender");

            ImGui::Checkbox("Render Background", &m_SystemToggles.RenderBackground);
            ImGui::Checkbox("Render Sprites", &m_SystemToggles.RenderSprites);

            ImGui::SeparatorText("Debug");

            ImGui::Checkbox("Debug Render World Grid", &m_SystemToggles.DebugRenderWorldGrid);
            ImGui::Checkbox("Debug Render Colliders", &m_SystemToggles.DebugRenderColliders);
            ImGui::Checkbox("Debug Render Camera", &m_SystemToggles.DebugRenderCamera);

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Tiles"))
        {
            ImGui::Separator();

            for (auto &[key, value] : m_TileData)
            {
                // Render a selectable image button of the tile

                ImGui::Text("%s", key.c_str());
                ImGui::Text("Texture: %s", value.TextureSource.c_str());

                sf::Sprite sprite(ResourceManager::GetInstance().GetTexture(value.TextureSource), sf::IntRect(value.OffsetX, value.OffsetY, value.Width, value.Height));

                if (ImGui::ImageButton(
                    key.c_str(),
                    sprite,
                    sf::Vector2f(64, 64)))
                {
                    // TODO: Select tile
                }

                ImGui::Separator();
            }

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Settings"))
        {
            ImGui::Checkbox("Debug Overlay", &useOverlay);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}

void ScenePlay::SpawnPlayer()
{
    if (m_Player != nullptr)
    {
        m_Player->Destroy();
    }

    m_Player = m_World.AddEntity("player");

    m_Player->AddComponent<CTransform>(
        Vec2{16, 64},
        Vec2{2, 2},
        0.0f);

    m_Player->AddComponent<CVelocity>(
        Vec2{0, 0},
        0.0f);

    m_Player->AddComponent<CSprite>(
        ResourceManager::GetInstance().GetTexture("tex_player"),
        0, 0,
        32, 32,
        2.0f);

    m_Player->AddComponent<CPlayerActions>();

    m_Player->AddComponent<CSpriteAnimator>(
        ResourceManager::GetInstance().GetAnimation("anim_player_idle"));

    m_Player->AddComponent<CGravity>(1600.0f);

    m_Player->AddComponent<CBoxCollider>(Vec2{24, 22});

    m_Player->AddComponent<CPlayerController>(400.0f, 800.0f);
}

void ScenePlay::DebugRenderWorldGrid(sf::RenderWindow *window)
{
    window->setView(window->getDefaultView());

    sf::Vector2i cameraSize = (sf::Vector2i)m_Camera.getSize();
    sf::Vector2i cameraPos = (sf::Vector2i)m_Camera.getCenter();

    i32 gridLeft = ((cameraPos.x - cameraSize.x / 2) / 32) * 32 - 32;
    i32 gridRight = ((cameraPos.x + cameraSize.x / 2) / 32) * 32 + 32;

    i32 gridTop = ((cameraSize.y / 2 - cameraPos.y) / 32) * 32 - 32;
    i32 gridBottom = ((cameraSize.y + cameraSize.y / 2 - cameraPos.y) / 32) * 32 + 32;

    sf::Color col = sf::Color::Black;
    col.a = 80;

    for (i32 i = gridLeft; i <= gridRight; i += 32)
    {
        Vec2 screenTop = WorldToScreen({(f32)i, (f32)(gridTop)});
        Vec2 screenBottom = WorldToScreen({(f32)i, (f32)(gridBottom)});

        sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(screenTop.x, screenTop.y), col),
                sf::Vertex(sf::Vector2f(screenBottom.x, screenBottom.y), col)};
        window->draw(line, 2, sf::Lines);
    }

    for (i32 j = gridTop; j <= gridBottom; j += 32)
    {
        Vec2 screenLeft = WorldToScreen({(f32)gridLeft, (f32)j});
        Vec2 screenRight = WorldToScreen({(f32)gridRight, (f32)j});

        sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(screenLeft.x, screenLeft.y), col),
                sf::Vertex(sf::Vector2f(screenRight.x, screenRight.y), col)};
        window->draw(line, 2, sf::Lines);
    }

    // Draw coords in corners of grid cells

    col.a = 255;

    sf::Text text;
    text.setFont(ResourceManager::GetInstance().GetFont("font_noto"));
    text.setCharacterSize(8);
    text.setFillColor(col);

    for (i32 i = gridLeft; i <= gridRight; i += 32)
    {
        for (i32 j = gridTop; j <= gridBottom; j += 32)
        {
            Vec2 screenPos = WorldToScreen({(f32)i + 2, (f32)j + 30});
            text.setString(std::to_string(i / 32) + ", " + std::to_string(j / 32));
            text.setPosition(screenPos.x, screenPos.y);
            window->draw(text);
        }
    }
}

void ScenePlay::DebugRenderCamera(sf::RenderWindow *window)
{
    window->setView(m_Camera);

    sf::Color col = sf::Color::Green;
    col.a = 255;

    sf::RectangleShape rect(sf::Vector2f(m_CameraParams.BoxWidth, m_CameraParams.BoxHeight));
    
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(col);
    rect.setOutlineThickness(0.2f);
    rect.setOrigin(m_CameraParams.BoxWidth / 2.0f, m_CameraParams.BoxHeight / 2.0f);

    rect.setPosition(
        (i32)m_CameraParams.CurrentPosition.x, 
        m_CameraParams.FrameHeight - (i32)m_CameraParams.CurrentPosition.y
    );
    
    window->draw(rect, sf::BlendAlpha);
}

void ScenePlay::LoadLevel(const std::string &path)
{
    m_World = World();
    NT_INFO("Loading level from file: %s", path.c_str());

    std::ifstream file(path);
    if (!file.is_open())
    {
        NT_ERROR("Failed to open level data from path: %s", path.c_str());
        return;
    }

    json data = json::parse(file);

    if (data.contains("tileset"))
    {
        // Process the tile types

        json tileset = data["tileset"];
        for (auto &[key, value] : tileset.items())
        {
            std::string name = value["name"];
            std::string textureSource = value["texture-source"];
            u32 width = value["width"];
            u32 height = value["height"];
            i32 offsetX = value["offset-x"];
            i32 offsetY = value["offset-y"];

            m_TileData[name] = {
                textureSource,
                width,
                height,
                offsetX,
                offsetY};
        }
    }

    if (data.contains("tiles"))
    {
        // Instantiate tile entities

        json tiles = data["tiles"];
        for (auto &[key, value] : tiles.items())
        {
            std::string type = value["type"];
            TileData tile = m_TileData[type];

            Vec2 position;
            position.x = (8.0f * 2.0f) + (f32)value["x"] * 2.0 * tile.Width;
            position.y = (f32)value["y"] * 2.0 * tile.Height;

            auto e = m_World.AddEntity("tile");
            e->AddComponent<CTransform>(position, Vec2{2.0f, 2.0f}, 0.0f);
            e->AddComponent<CSprite>(
                ResourceManager::GetInstance().GetTexture(tile.TextureSource),
                tile.OffsetX,
                tile.OffsetY,
                tile.Width,
                tile.Height,
                1.0f);

            e->AddComponent<CBoxCollider>(Vec2{(f32)tile.Width, (f32)tile.Height});
        }
    }

    if (data.contains("decorations"))
    {
        // Instantiate decoration entities

        json decs = data["decorations"];
        for (auto &[key, value] : decs.items())
        {
            std::string type = value["type"];
            TileData tile = m_TileData[type];

            Vec2 position;
            position.x = (8.0f * 2.0f) + (f32)value["x"] * 2.0 * tile.Width;
            position.y = (f32)value["y"] * 2.0 * tile.Height;

            auto e = m_World.AddEntity("decoration");
            e->AddComponent<CTransform>(position, Vec2{2.0f, 2.0f}, 0.0f);
            e->AddComponent<CSprite>(
                ResourceManager::GetInstance().GetTexture(tile.TextureSource),
                tile.OffsetX,
                tile.OffsetY,
                tile.Width,
                tile.Height,
                0.0f);
        }
    }
}