#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <optional>
#include <stdexcept>

namespace ShadowRec
{
    class Config
    {
    public:
        static Config& Get()
        {
            static Config instance;
            return instance;
        }

        void Load(const std::filesystem::path& path);
        void Save(const std::filesystem::path& path);

        void Set(const std::string& key, const nlohmann::json& value);
        std::optional<nlohmann::json> Get(const std::string& key) const;

        template <typename T>
        T Get(const std::string& key, const T& defaultValue) const
        {
            auto it = m_Data.find(key);
            if (it != m_Data.end() && !it->is_null())
                return it->get<T>();
            return defaultValue;
        }

        const nlohmann::json& Raw() const { return m_Data; }
        nlohmann::json& Raw() { return m_Data; }

        bool Has(const std::string& key) const
        {
            return m_Data.contains(key);
        }

        void Remove(const std::string& key)
        {
            m_Data.erase(key);
        }

    private:
        Config() = default;
        nlohmann::json m_Data;
        std::filesystem::path m_Path;
    };
}