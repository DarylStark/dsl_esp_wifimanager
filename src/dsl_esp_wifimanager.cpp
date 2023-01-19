#include "dsl_esp_wifimanager.h"

namespace dsl
{
    namespace esp
    {
        namespace connections
        {
            WiFiManager::WiFiManager(std::string partition)
                : __partition(partition), __dirty(true)
            {
                // TODO: Add autosave
            }

            void WiFiManager::set(std::string ssid, std::string password)
            {
                // If is already in there, just adjust the password
                for (auto &network : __wifi_list)
                {
                    if (network.ssid == ssid)
                    {
                        network.password = password;
                        __dirty = true;
                        return;
                    }
                }

                // Not on the list, add it
                __wifi_list.push_back({ssid, password});
                __dirty = true;
            }

            const WiFiManager::wifi_list &WiFiManager::get_wifi_list() const
            {
                return __wifi_list;
            }

            void WiFiManager::remove(std::string ssid)
            {
                for (wifi_list::iterator s = __wifi_list.begin(); s != __wifi_list.end(); s++)
                {
                    if ((*s).ssid == ssid)
                    {
                        __wifi_list.erase(s);
                        break;
                    }
                }
                __dirty = true;
            }

            void WiFiManager::clear()
            {
                __wifi_list.clear();
                __dirty = true;
            }

            void WiFiManager::load()
            {
                __preferences.begin(__partition.c_str());

                // Retrieve and save the WiFis
                __wifi_list.clear();
                uint16_t wifi_count = __preferences.getUInt("count", 0);
                for (uint16_t counter = 0; counter < wifi_count; ++counter)
                {
                    std::stringstream field_ssid;
                    field_ssid << "ssid_" << counter;
                    std::stringstream field_password;
                    field_password << "password_" << counter;
                    std::string ssid = __preferences.getString(field_ssid.str().c_str()).c_str();
                    std::string password = __preferences.getString(field_password.str().c_str()).c_str();

                    if (ssid.length() > 0)
                    {
                        WiFiNetwork network;
                        network.ssid = ssid;
                        network.password = password;
                        __wifi_list.emplace_back(network);
                    }
                }

                __dirty = false;
                __preferences.end();
            }

            void WiFiManager::save()
            {
                if (!__dirty)
                    return;

                __preferences.begin(__partition.c_str());
                __preferences.clear();
                __preferences.putUInt("count", __wifi_list.size());

                uint16_t counter = 0;
                for (const auto &wifi : __wifi_list)
                {
                    std::stringstream field_ssid;
                    field_ssid << "ssid_" << counter;
                    std::stringstream field_password;
                    field_password << "password_" << counter;

                    // Save it
                    __preferences.putString(field_ssid.str().c_str(), wifi.ssid.c_str());
                    __preferences.putString(field_password.str().c_str(), wifi.password.c_str());

                    counter++;
                }

                __dirty = false;
                __preferences.end();
            }
        };
    };
};