#include "scene_menu.hpp"
#include "scene_play.hpp"

#include "../systems/debugsystems.hpp"
#include "../systems/rendersystems.hpp"

#include "core/game.hpp"
#include "core/resources.hpp"
#include "physics/collisions.hpp"

#include "imgui.h"
#include "imgui-SFML.h"

void SceneMenu::AddTestButton()
{
    auto e = m_World.AddEntity("button");
    
    auto coll = e->AddComponent<CBoxCollider>(Vec2{256, 64});
    
    e->AddComponent<CTransform>(Vec2{0, 0}, Vec2{1, 1}, 0.0f);

    auto& button = e->AddComponent<CButton>();

    button.OnHoverEnter = [this]() {
        NT_INFO("Hovering over button.");
    };

    button.OnHoverExit = [this]() {
        NT_INFO("No longer hovering over button.");
    };

    button.OnPress = [this]() {
        NT_INFO("Changing to test scene.");
        m_Game->ChangeScene("TestScene", std::make_shared<ScenePlay>(m_Game, "assets/levels/level_1.json"));
    };

    button.OnRelease = [this]() {
        NT_INFO("Released button.");
    };
}

void SceneMenu::PhysicsCheckButtons(f64 dt)
{
    auto buttons = m_World.GetEntitiesWithTag("button");

    for (auto e : buttons)
    {
        auto& button = e->GetComponent<CButton>();

        // Check mouse position and button collider.

        auto& coll = e->GetComponent<CBoxCollider>();
        auto& tf = e->GetComponent<CTransform>();

        AABB buttonBox = {
            tf.Position.x - coll.Size.x / 2.0f,
            tf.Position.y,
            coll.Size.x,
            coll.Size.y
        };

        Vec2 mousePos = m_Game->GetMousePosition();
        Vec2 mouseWorldPos = ScreenToWorld(mousePos);

        bool leftClick = m_Game->IsMouseButtonDown(sf::Mouse::Left);

        if (Physics2D::PointInAABB(mouseWorldPos, buttonBox))
        {
            NT_INFO("Button Found!");

            if (!button.IsDown && leftClick)
            {
                button.IsDown = true;
                button.OnPress();
            }
            else if (button.IsDown && !leftClick)
            {
                button.IsDown = false;
                button.OnRelease();
            }

            if (!button.IsHovering)
            {
                button.IsHovering = true;
                button.OnHoverEnter();
            }
        }
        else
        {
            if (button.IsDown)
            {
                button.IsDown = false;
                button.OnRelease();
            }
            else if (button.IsHovering)
            {
                button.IsHovering = false;
                button.OnHoverExit();
            }

        }
    }
}

SceneMenu::SceneMenu(Game *game) : Scene(game)
{
}

void SceneMenu::OnSceneEnter()
{
    RegisterAction(sf::Keyboard::Up, "Up");
    RegisterAction(sf::Keyboard::Left, "Left");
    RegisterAction(sf::Keyboard::Right, "Right");
    RegisterAction(sf::Keyboard::Down, "Down");
    RegisterAction(sf::Keyboard::Space, "EnterPlayScene");
    RegisterAction(sf::Keyboard::F1, "ToggleDebugOverlay");

    AddTestButton();

    NT_INFO("Entering menu scene.");

    // Place camera at center of screen.

    m_CameraParams.CurrentPosition = Vec2{0, 0};

    m_Camera.setSize(m_CameraParams.FrameWidth, m_CameraParams.FrameHeight);

    m_Camera.setCenter(
        (i32) m_CameraParams.CurrentPosition.x, 
        (i32)(m_CameraParams.FrameHeight - m_CameraParams.CurrentPosition.y)
    );
}

void SceneMenu::OnSceneExit()
{
    NT_INFO("Exiting test scene.");
}

void SceneMenu::OnAction(Action action)
{
    if (action.Name == "Up")
    {

    }
    else if (action.Name == "Left")
    {

    }
    else if (action.Name == "Right")
    {

    }
    else if (action.Name == "Down")
    {

    }
    else if (action.Name == "EnterPlayScene")
    {
        NT_INFO("Changing to play scene.");
        m_Game->ChangeScene("PlayScene", std::make_shared<ScenePlay>(m_Game, "assets/levels/level_1.json"));
    }
    else if (action.Name == "ToggleDebugOverlay" && action.Type == ActionType::Begin)
    {

    }
    else if (action.Name == "LeftClick")
    {

    }
}

void SceneMenu::Update(f64 dt)
{
    m_World.Update();

    if (m_SystemToggles.PhysicsCheckButtons)
    {
        PhysicsCheckButtons(dt);
    }
}

void SceneMenu::Render(sf::RenderWindow *window)
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
}

void SceneMenu::DrawGUI()
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

            ImGui::Checkbox("UpdatePositions", &m_SystemToggles.UpdatePositions);
            ImGui::Checkbox("UpdateAnimations", &m_SystemToggles.UpdateAnimations);

            ImGui::SeparatorText("OnPhysicsUpdate");

            ImGui::Checkbox("PhysicsCheckButtons", &m_SystemToggles.PhysicsCheckButtons);

            ImGui::SeparatorText("OnRender");

            ImGui::Checkbox("RenderBackground", &m_SystemToggles.RenderBackground);
            ImGui::Checkbox("RenderSprites", &m_SystemToggles.RenderSprites);

            ImGui::SeparatorText("Debug");

            ImGui::Checkbox("DebugRenderColliders", &m_SystemToggles.DebugRenderColliders);

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