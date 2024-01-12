#include "core/resources.hpp"

#include <fstream>

void ResourceManager::AddTexture(const std::string &name, const std::string &path)
{
    sf::Texture texture;
    if (!texture.loadFromFile(path))
    {
        NT_ERROR("Failed to load texture from path: %s", path.c_str());
    }

    if (m_Textures.find(name) != m_Textures.end())
    {
        NT_ERROR("Attempted to load texture with name: %s. Texture already exists!", name.c_str());
        return;
    }

    m_Textures.insert(std::make_pair(name, texture));
}

void ResourceManager::AddSound(const std::string &name, const std::string &path)
{
    sf::SoundBuffer sound;
    if (!sound.loadFromFile(path))
    {
        NT_ERROR("Failed to load texture from path: %s", path.c_str());
    }

    if (m_Sounds.find(name) != m_Sounds.end())
    {
        NT_ERROR("Attempted to load sound with name: %s. Sound already exists!", name.c_str());
        return;
    }

    m_Sounds.insert(std::make_pair(name, sound));
}

void ResourceManager::AddFont(const std::string &name, const std::string &path)
{
    sf::Font font;
    if (!font.loadFromFile(path))
    {
        NT_ERROR("Failed to load font from path: %s", path.c_str());
    }

    if (m_Fonts.find(name) != m_Fonts.end())
    {
        NT_ERROR("Attempted to load font with name: %s. Font already exists!", name.c_str());
        return;
    }

    m_Fonts.insert(std::make_pair(name, font));
}

sf::Texture &ResourceManager::GetTexture(const std::string &name)
{
    return m_Textures[name];
}

sf::SoundBuffer &ResourceManager::GetSound(const std::string &name)
{
    return m_Sounds[name];
}

sf::Font &ResourceManager::GetFont(const std::string &name)
{
    return m_Fonts[name];
}

Animation ResourceManager::GetAnimation(const std::string& name)
{
    return m_Animations[name];
}

void ResourceManager::LoadFromManifest(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        NT_ERROR("Failed to open resource manifest from path: %s", path.c_str());
        return;
    }

    json data = json::parse(file);

    if (!data.contains("resources"))
    {
        NT_ERROR("Failed to parse resource list from JSON source: %s", path.c_str());
        return;
    }

    json resources = data["resources"];
    for (size_t i = 0; i < resources.size(); i++)
    {
        json res = resources[i];

        std::string type = res["type"].template get<std::string>();
        std::string name = res["name"].template get<std::string>();

        if (type == "texture") 
        {
            std::string path = res["path"].template get<std::string>();
            AddTexture(name, path);
        } 
        else if (type == "animation") 
        {
            json anim = res["animation"];
            AddAnimationFromMetadata(name, anim);
        } 
        else if (type == "sound") 
        {
            std::string path = res["path"].template get<std::string>();
            AddSound(name, path);
        } 
        else if (type == "font") 
        {
            std::string path = res["path"].template get<std::string>();
            AddFont(name, path);
        } 
        else 
        {
            NT_ERROR("Failed to parse resource from manifest of type: %s", type.c_str());
        }
    }

    file.close();
}

void ResourceManager::AddAnimationFromMetadata(const std::string& name, json metadata) {
    Animation anim {
        .TextureSourceName = metadata["texture-source"].template get<std::string>(),
        .FrameCount = metadata["frames"].template get<u32>(),
        .FrameTime = 1.0 / metadata["framerate"].template get<f64>(),
        .FrameWidth = metadata["width"].template get<u32>(),
        .FrameHeight = metadata["height"].template get<u32>(),
        .OffsetX = metadata["offset-x"].template get<i32>(),
        .OffsetY = metadata["offset-y"].template get<i32>()
    };
    
    m_Animations.insert(std::make_pair(name, anim));
}
