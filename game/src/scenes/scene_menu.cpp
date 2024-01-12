#include "scene_menu.hpp"
#include "scene_play.hpp"
#include "scene_sandbox.hpp"

#include "../systems/debugsystems.hpp"
#include "../systems/rendersystems.hpp"

#include "core/game.hpp"
#include "core/resources.hpp"
#include "physics/collisions.hpp"

#include "imgui-SFML.h"
#include "imgui.h"

void SceneMenu::AddTestButtons() {
    {
        auto e = m_World.AddEntity("button");
        auto &coll = e->AddComponent<CBoxCollider>(Vec2{256, 64});
        auto &tf = e->AddComponent<CTransform>(Vec2{0, 64}, Vec2{1, 1}, 0.0f);
        auto &button = e->AddComponent<CButton>(
            "Level 1", ResourceManager::GetInstance().GetFont("font_rubik"), 48,
            sf::Color::Black);

        button.OnHoverEnter = [e]() {
            auto &button = e->GetComponent<CButton>();
            button.TextSprite.setFillColor(sf::Color::Red);
        };

        button.OnHoverExit = [e]() {
            auto &button = e->GetComponent<CButton>();
            button.TextSprite.setFillColor(sf::Color::Black);
        };

        button.OnPress = [this]() {
            m_Game->ChangeScene("Level_1",
                                std::make_shared<ScenePlay>(
                                    m_Game, "assets/levels/level_1.json"));
        };
    }

    {
        auto e = m_World.AddEntity("button");
        auto &coll = e->AddComponent<CBoxCollider>(Vec2{256, 64});
        auto &tf = e->AddComponent<CTransform>(Vec2{0, -32}, Vec2{1, 1}, 0.0f);
        auto &button = e->AddComponent<CButton>(
            "Level 2", ResourceManager::GetInstance().GetFont("font_rubik"), 48,
            sf::Color::Black);

        button.OnHoverEnter = [e]() {
            auto &button = e->GetComponent<CButton>();
            button.TextSprite.setFillColor(sf::Color::Red);
        };

        button.OnHoverExit = [e]() {
            auto &button = e->GetComponent<CButton>();
            button.TextSprite.setFillColor(sf::Color::Black);
        };

        button.OnPress = [this]() {
            m_Game->ChangeScene("Level_2",
                                std::make_shared<ScenePlay>(
                                    m_Game, "assets/levels/level_2.json"));
        };
    }

    {
        auto e = m_World.AddEntity("button");
        auto &coll = e->AddComponent<CBoxCollider>(Vec2{256, 64});
        auto &tf = e->AddComponent<CTransform>(Vec2{0, -128}, Vec2{1, 1}, 0.0f);
        auto &button = e->AddComponent<CButton>(
            "Level 3", ResourceManager::GetInstance().GetFont("font_rubik"), 48,
            sf::Color::Black);

        button.OnHoverEnter = [e]() {
            auto &button = e->GetComponent<CButton>();
            button.TextSprite.setFillColor(sf::Color::Red);
        };

        button.OnHoverExit = [e]() {
            auto &button = e->GetComponent<CButton>();
            button.TextSprite.setFillColor(sf::Color::Black);
        };

        button.OnPress = [this]() {
            m_Game->ChangeScene("Level_3",
                                std::make_shared<ScenePlay>(
                                    m_Game, "assets/levels/level_3.json"));
        };
    }

    {
        auto e = m_World.AddEntity("button");
        auto &coll = e->AddComponent<CBoxCollider>(Vec2{256, 64});
        auto &tf = e->AddComponent<CTransform>(Vec2{0, -224}, Vec2{1, 1}, 0.0f);
        auto &button = e->AddComponent<CButton>(
            "Sandbox", ResourceManager::GetInstance().GetFont("font_rubik"), 48,
            sf::Color::Black);

        button.OnHoverEnter = [e]() {
            auto &button = e->GetComponent<CButton>();
            button.TextSprite.setFillColor(sf::Color::Red);
        };

        button.OnHoverExit = [e]() {
            auto &button = e->GetComponent<CButton>();
            button.TextSprite.setFillColor(sf::Color::Black);
        };

        button.OnPress = [this]() {
            m_Game->ChangeScene("Sandbox",
                                std::make_shared<SceneSandbox>(m_Game));
        };
    }
}

void SceneMenu::PhysicsCheckButtons(World &world, f64 dt) {
    auto buttons = world.GetEntitiesWithTag("button");

    for (auto e : buttons) {
        auto &button = e->GetComponent<CButton>();

        // Check mouse position and button collider.

        auto &coll = e->GetComponent<CBoxCollider>();
        auto &tf = e->GetComponent<CTransform>();

        AABB buttonBox = {tf.Position.x - coll.Size.x / 2.0f, tf.Position.y,
                          coll.Size.x, coll.Size.y};

        Vec2 mousePos = m_Game->GetMousePosition();
        Vec2 mouseWorldPos = ScreenToWorld(mousePos);

        bool leftClick = m_Game->IsMouseButtonDown(sf::Mouse::Left);

        if (Physics2D::PointInAABB(mouseWorldPos, buttonBox)) {
            if (!button.IsDown && leftClick) {
                button.IsDown = true;
                button.OnPress();
            } else if (button.IsDown && !leftClick) {
                button.IsDown = false;
                button.OnRelease();
            }

            if (!button.IsHovering) {
                button.IsHovering = true;
                button.OnHoverEnter();
            }
        } else {
            if (button.IsDown) {
                button.IsDown = false;
                button.OnRelease();
            } else if (button.IsHovering) {
                button.IsHovering = false;
                button.OnHoverExit();
            }
        }
    }
}

SceneMenu::SceneMenu(Game *game) : Scene(game) {}

void SceneMenu::OnSceneEnter() {
    RegisterAction(sf::Keyboard::Up, "Up");
    RegisterAction(sf::Keyboard::Left, "Left");
    RegisterAction(sf::Keyboard::Right, "Right");
    RegisterAction(sf::Keyboard::Down, "Down");
    RegisterAction(sf::Keyboard::F1, "ToggleDebugTools");

    AddTestButtons();

    // Place camera at center of screen.

    m_CameraParams.CurrentPosition = Vec2{0, 0};

    m_Camera.setSize(m_CameraParams.FrameWidth, m_CameraParams.FrameHeight);

    m_Camera.setCenter(
        (i32)m_CameraParams.CurrentPosition.x,
        (i32)(m_CameraParams.FrameHeight - m_CameraParams.CurrentPosition.y));
}

void SceneMenu::OnSceneExit() {}

void SceneMenu::OnAction(Action action) {
    if (action.Name == "Up") {

    } else if (action.Name == "Left") {

    } else if (action.Name == "Right") {

    } else if (action.Name == "Down") {

    } else if (action.Name == "ToggleDebugTools" &&
               action.Type == ActionType::Begin) {
        m_ShowDebugTools = !m_ShowDebugTools;
    } else if (action.Name == "LeftClick") {
    }
}

void SceneMenu::Update(f64 dt) {
    m_World.Update();

    if (m_SystemToggles.PhysicsCheckButtons) {
        PhysicsCheckButtons(m_World, dt);
    }
}

void SceneMenu::Render(sf::RenderWindow *window) {
    if (m_SystemToggles.RenderBackground) {
        Systems::RenderBackground(window);
    }

    if (m_SystemToggles.RenderSprites) {
        Systems::RenderSprites(window, m_World, m_Camera, m_CameraParams);
    }

    if (m_SystemToggles.RenderButtons) {
        Systems::RenderButtons(window, m_World, m_Camera, m_CameraParams);
    }

    if (m_SystemToggles.DebugRenderColliders) {
        Systems::DebugRenderColliders(window, m_World, m_Camera,
                                      m_CameraParams);
    }
}

void SceneMenu::OnDrawGUI() {
    if (!ImGui::Begin("Debug")) {
        ImGui::End();
        return;
    }

    if (ImGui::BeginTabBar("##TabBar")) {
        if (ImGui::BeginTabItem("Level"))
        {
            ImGui::Separator();

            ImGui::InputText("Level Path", m_LevelPath, IM_ARRAYSIZE(m_LevelPath));

            if (ImGui::Button("Load Level"))
            {
                m_Game->ChangeScene("Play",
                                    std::make_shared<ScenePlay>(
                                        m_Game, m_LevelPath));
            }
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

            ImGui::Checkbox("UpdatePositions",
                            &m_SystemToggles.UpdatePositions);
            ImGui::Checkbox("UpdateAnimations",
                            &m_SystemToggles.UpdateAnimations);

            ImGui::SeparatorText("OnPhysicsUpdate");

            ImGui::Checkbox("PhysicsCheckButtons",
                            &m_SystemToggles.PhysicsCheckButtons);

            ImGui::SeparatorText("OnRender");

            ImGui::Checkbox("RenderBackground",
                            &m_SystemToggles.RenderBackground);
            ImGui::Checkbox("RenderSprites", &m_SystemToggles.RenderSprites);
            ImGui::Checkbox("RenderButtons", &m_SystemToggles.RenderButtons);

            ImGui::SeparatorText("Debug");

            ImGui::Checkbox("DebugRenderColliders",
                            &m_SystemToggles.DebugRenderColliders);

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