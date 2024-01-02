#pragma once

#include "core/world.hpp"
#include "core/actions.hpp"

class Game;

class Scene {
    public:
        Scene(const Game* game) : m_Game(game) {}
        ~Scene() = default;

        void RegisterAction(i32 keycode, std::string name);
        bool GetAction(i32 keycode, std::string& name);

        virtual void OnSceneEnter() = 0;
        virtual void OnSceneExit() = 0;
        virtual void OnAction(Action action) = 0;
        virtual void Update(f64 dt) = 0;
        virtual void Render(sf::RenderWindow* window) = 0;
        virtual void DrawGUI() = 0;

    protected:
        const Game* m_Game;
        World m_World;
        std::map<u32, std::string> m_ActionMap;
        bool m_Paused;
};