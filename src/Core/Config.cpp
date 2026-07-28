#include <Core/Config.h>
#include <fstream>
#include <Core/Logger.h>

namespace ShadowRec
{
    void Config::Load(const std::filesystem::path& path)
    {
        m_Path = path;
        if (!std::filesystem::exists(path))
        {
            SHADOWREC_WARN("Config file not found at {}, using defaults", path.string());
            m_Data = nlohmann::json::object();
            return;
        }

        std::ifstream file(path);
        if (!file.is_open())
        {
            SHADOWREC_ERROR("Failed to open config file: {}", path.string());
            return;
        }

        try
        {
            file >> m_Data;
            SHADOWREC_INFO("Config loaded from {}", path.string());
        }
        catch (const nlohmann::json::parse_error& ex)
        {
            SHADOWREC_ERROR("Config parse error at offset {}: {}", ex.byte, ex.what());
            m_Data = nlohmann::json::object();
        }
    }

    void Config::Save(const std::filesystem::path& path)
    {
        if (path.empty())
        {
            if (m_Path.empty())
            {
                SHADOWREC_WARN("No config path set, cannot save");
                return;
            }
            m_Path = path;
        }

        std::ofstream file(path);
        if (!file.is_open())
        {
            SHADOWREC_ERROR("Failed to save config to {}", path.string());
            return;
        }

        file << m_Data.dump(4) << std::endl;
        SHADOWREC_INFO("Config saved to {}", path.string());
    }

    void Config::Set(const std::string& key, const nlohmann::json& value)
    {
        m_Data[key] = value;
    }

std::optional<nlohmann::json> Config::Get(const std::string& key) const
{
    auto it = m_Data.find(key);
    if (it != m_Data.end())
        return *it;
    return std::nullopt;
    }
}