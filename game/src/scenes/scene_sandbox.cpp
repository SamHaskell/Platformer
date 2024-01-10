#include "scene_sandbox.hpp"
#include "scene_menu.hpp"

#include "core/game.hpp"

#include "imgui.h"
#include "imgui-SFML.h"

SceneSandbox::SceneSandbox(Game *game) : Scene(game)
{

}

void SceneSandbox::OnSceneEnter()
{
    // Register actions.

    RegisterAction(sf::Keyboard::Up, "Up");
    RegisterAction(sf::Keyboard::Left, "Left");
    RegisterAction(sf::Keyboard::Right, "Right");
    RegisterAction(sf::Keyboard::Down, "Down");
    RegisterAction(sf::Keyboard::Escape, "MainMenu");
    RegisterAction(sf::Keyboard::F1, "ToggleDebugOverlay");
}

void SceneSandbox::OnSceneExit()
{

}

void SceneSandbox::OnAction(Action action)
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

void SceneSandbox::Update(f64 dt)
{
    m_World.Update();


}

void SceneSandbox::Render(sf::RenderWindow *window)
{

}

void SceneSandbox::DrawGUI()
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
        if (ImGui::BeginTabItem("Settings"))
        {
            ImGui::Checkbox("Debug Overlay", &useOverlay);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}