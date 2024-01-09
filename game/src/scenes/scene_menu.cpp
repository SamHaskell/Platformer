#include "scene_menu.hpp"
#include "scene_test.hpp"

#include "core/game.hpp"
#include "core/resources.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

SceneMenu::SceneMenu(Game *game) : Scene(game)
{
}

void SceneMenu::OnSceneEnter()
{
    RegisterAction(sf::Keyboard::Up, "Up");
    RegisterAction(sf::Keyboard::Left, "Left");
    RegisterAction(sf::Keyboard::Right, "Right");
    RegisterAction(sf::Keyboard::Down, "Down");

    RegisterAction(sf::Keyboard::Space, "EnterTestScene");

    RegisterAction(sf::Keyboard::F1, "ToggleDebugOverlay");

    NT_INFO("Entering menu scene.");
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
    else if (action.Name == "EnterTestScene")
    {
        NT_INFO("Changing to test scene.");
        m_Game->ChangeScene("TestScene", std::make_shared<SceneTest>(m_Game, "assets/levels/level_1.json"));
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

}

void SceneMenu::Render(sf::RenderWindow *window)
{

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

            ImGui::SeparatorText("OnPhysicsUpdate");

            ImGui::SeparatorText("OnRender");

            ImGui::SeparatorText("Debug");

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