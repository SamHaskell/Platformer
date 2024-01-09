#pragma once

#include "core/world.hpp"
#include "core/actions.hpp"

class Game;

struct CameraParams
{
    f32 FrameWidth = 1280;
    f32 FrameHeight = 720;
    f32 BoxWidth = 128;
    f32 BoxHeight = 128;
    f32 DampTimeX = 0.15f;
    f32 DampTimeY = 0.3f;
    Vec2 CurrentPosition;
    Vec2 CurrentTarget;
};

class Scene {
    public:
        Scene(Game* game) : m_Game(game) {}
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
        Vec2 ScreenToWorld(Vec2 screenPos);
        Vec2 WorldToScreen(Vec2 worldPos);

        Game* m_Game;
        World m_World;
        sf::View m_Camera;
        CameraParams m_CameraParams;
        std::map<u32, std::string> m_ActionMap;
        bool m_Paused;
};