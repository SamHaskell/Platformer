#pragma once

#include "core/defines.hpp"
#include "core/clock.hpp"

#include "core/scene.hpp"
#include "core/world.hpp"
#include "core/entity.hpp"

#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

class Game {
    public:
        Game();
        ~Game() = default;
        
        void Tick();
        void HandleEvent(sf::Event& event);
        bool ShouldClose() { return !m_Running; }
        void Update(f64 dt);

        void ChangeScene(const std::string& name, std::shared_ptr<Scene> scene);

        Vec2 GetWindowSize() const;

    private:
        Clock m_Clock;
        sf::Clock m_SFMLClock;
        sf::RenderWindow* m_Window;
        bool m_Running;

        std::map<std::string, std::shared_ptr<Scene>> m_Scenes;
        std::unordered_map<sf::Keyboard::Key, bool> m_CachedKeyState;
        std::string m_CurrentScene;
};