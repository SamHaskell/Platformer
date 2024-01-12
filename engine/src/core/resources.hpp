#pragma once

#include "core/defines.hpp"
#include "core/singleton.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "json.hpp"
using json = nlohmann::json;

struct Animation 
{
    std::string TextureSourceName;
    u32 FrameCount;
    f64 FrameTime;
    u32 FrameWidth;
    u32 FrameHeight;
    i32 OffsetX;
    i32 OffsetY;

    bool operator==(const Animation& other) const {
        return TextureSourceName == other.TextureSourceName &&
            FrameCount == other.FrameCount &&
            FrameTime == other.FrameTime &&
            FrameWidth == other.FrameWidth &&
            FrameHeight == other.FrameHeight &&
            OffsetX == other.OffsetX &&
            OffsetY == other.OffsetY;
    }
};

class ResourceManager : public Singleton<ResourceManager> 
{
    public:
        void AddTexture(const std::string& name, const std::string& path);
        void AddSound(const std::string& name, const std::string& path);
        void AddFont(const std::string& name, const std::string& path);
        void AddAnimationFromMetadata(const std::string& name, json metadata);

        sf::Texture& GetTexture(const std::string& name);
        sf::SoundBuffer& GetSound(const std::string& name);
        sf::Font& GetFont(const std::string& name);

        // For now just copy animations since they are pretty lightweight.
        Animation GetAnimation(const std::string& name);

        void LoadFromManifest(const std::string& path);

    private:
        std::map<std::string, sf::Texture> m_Textures;
        std::map<std::string, sf::SoundBuffer> m_Sounds;
        std::map<std::string, sf::Font> m_Fonts;
        std::map<std::string, Animation> m_Animations;
};