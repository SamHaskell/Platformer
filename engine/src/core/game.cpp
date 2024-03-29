#include "core/game.hpp"
#include "core/resources.hpp"

#include "imgui.h"
#include "imgui-SFML.h"

#include <iomanip>
#include <ctime>
#include <sstream>

ResourceManager g_ResourceManager;

Game::Game() 
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    m_Window = new sf::RenderWindow(sf::VideoMode(1280, 720), "SFML Window", sf::Style::Titlebar, settings);
    
    bool imgui_ok = ImGui::SFML::Init(*m_Window);
    if (!imgui_ok) 
    {
        NT_ERROR("Failed to init IMGUI.");
    }

    m_Clock.Start();
    m_SFMLClock.restart();

    // Load assets, hard coded path to asset metadata file for now.
    ResourceManager::GetInstance().LoadFromManifest("assets/manifest.json");

    m_Running = true;
}

void Game::Tick()
{
    auto scene = m_Scenes[m_CurrentScene];

    f64 dt = m_Clock.Tick();

    sf::Event event;
    while (m_Window->pollEvent(event)) 
    {
        ImGui::SFML::ProcessEvent(*m_Window, event);
        HandleEvent(event);
    }

    ImGui::SFML::Update(*m_Window, m_SFMLClock.restart());

    m_Window->clear(sf::Color::White);

    Update(dt);

    ImGui::SFML::Render(*m_Window);
    
    m_Window->display();
}

void Game::HandleEvent(sf::Event& event)
{
    auto scene = m_Scenes[m_CurrentScene];

    if (event.type == sf::Event::Closed) 
    {
        m_Running = false;
    }

    if (event.type == sf::Event::Resized) 
    {
        m_Window->setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
    }

    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) 
    {
        std::string actionName;
        if (scene->GetAction(event.key.code, actionName) && !ImGui::GetIO().WantCaptureKeyboard) 
        {
            if (!m_CachedKeyState[event.key.code] && (event.type == sf::Event::KeyPressed)) 
            {
                Action action = {
                    .Name = actionName,
                    .Type = ActionType::Begin
                };
                scene->OnAction(action);
            } 
            else if (m_CachedKeyState[event.key.code] && (event.type == sf::Event::KeyReleased))
            {
                Action action = {
                    .Name = actionName,
                    .Type = ActionType::End
                };
                scene->OnAction(action);
            }
        }

        if (event.key.code == sf::Keyboard::F5 && event.type == sf::Event::KeyPressed) 
        {
            std::time_t t = std::time(nullptr);
            std::tm tm = *std::localtime(&t);
            std::stringstream ss;
            ss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
            std::string timestamp = ss.str();
            CaptureScreen("screenshots/" + timestamp + ".png");
        }

        m_CachedKeyState[event.key.code] = (event.type == sf::Event::KeyPressed);
    }

    if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) 
    {
        if (!ImGui::GetIO().WantCaptureMouse) 
        {
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Left:
                {
                    Action action = {
                        .Name = "LeftClick",
                        .Type = (event.type == sf::Event::MouseButtonPressed) ? ActionType::Begin : ActionType::End,
                        .Position = Vec2{(f32)event.mouseButton.x, (f32)event.mouseButton.y}
                    };
                    scene->OnAction(action);
                } break;
                case sf::Mouse::Right:
                {
                    Action action = {
                        .Name = "RightClick",
                        .Type = (event.type == sf::Event::MouseButtonPressed) ? ActionType::Begin : ActionType::End,
                        .Position = Vec2{(f32)event.mouseButton.x, (f32)event.mouseButton.y}
                    };
                    scene->OnAction(action);
                } break;
                case sf::Mouse::Middle:
                {
                    Action action = {
                        .Name = "MiddleClick",
                        .Type = (event.type == sf::Event::MouseButtonPressed) ? ActionType::Begin : ActionType::End,
                        .Position = Vec2{(f32)event.mouseButton.x, (f32)event.mouseButton.y}
                    };
                    scene->OnAction(action);
                } break;
            }
        }
    }
}

void Game::Update(f64 dt) 
{
    auto scene = m_Scenes[m_CurrentScene];

    if (!scene) 
    {
        NT_ERROR("Attempted to simulate a non-existent scene.");
        return;
    }

    scene->Update(dt);
    scene->Render(m_Window);
    scene->DrawGUI();
}

void Game::ChangeScene(const std::string& name, std::shared_ptr<Scene> scene) {
    if (m_Scenes[m_CurrentScene]) 
    {
        m_Scenes[m_CurrentScene]->OnSceneExit();
    }

    m_CurrentScene = name;
    m_Scenes[m_CurrentScene] = scene;
    
    m_Scenes[m_CurrentScene]->OnSceneEnter();
}

void Game::CaptureScreen(const std::string& path)
{
    sf::Texture texture;
    texture.create(m_Window->getSize().x, m_Window->getSize().y);
    texture.update(*m_Window);
    texture.copyToImage().saveToFile(path);
    NT_INFO("Captured screen to %s", path.c_str());
}

Vec2 Game::GetWindowSize() const
{
    return Vec2{(f32)m_Window->getSize().x, (f32)m_Window->getSize().y};
}

Vec2 Game::GetMousePosition() const
{
    return Vec2{(f32)sf::Mouse::getPosition(*m_Window).x, (f32)sf::Mouse::getPosition(*m_Window).y};
}

bool Game::IsMouseButtonDown(sf::Mouse::Button button) const
{
    return sf::Mouse::isButtonPressed(button);
}