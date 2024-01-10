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
    RegisterAction(sf::Keyboard::Up, "Up");
    RegisterAction(sf::Keyboard::Left, "Left");
    RegisterAction(sf::Keyboard::Right, "Right");
    RegisterAction(sf::Keyboard::Down, "Down");
    RegisterAction(sf::Keyboard::Escape, "MainMenu");
    RegisterAction(sf::Keyboard::F1, "ToggleDebugTools");
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
    else if (action.Name == "ToggleDebugTools" && action.Type == ActionType::Begin)
    {
        m_ShowDebugTools = !m_ShowDebugTools;
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

void SceneSandbox::OnDrawGUI()
{
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
            ImGui::Checkbox("Debug Overlay", &m_ShowDebugOverlay);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}