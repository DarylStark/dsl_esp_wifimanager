#ifndef __DSL_ESP_WIFIMANAGER_H__
#define __DSL_ESP_WIFIMANAGER_H__

#include <Preferences.h>
#include <list>
#include <string>
#include <sstream>

namespace dsl
{
    namespace esp
    {
        namespace connections
        {
            struct WiFiNetwork
            {
                std::string ssid;
                std::string password;
            };

            class WiFiManager
            {
            public:
                typedef std::list<WiFiNetwork> wifi_list;

            private:
                Preferences __preferences;
                std::string __partition;
                wifi_list __wifi_list;
                bool __dirty;

            public:
                WiFiManager(std::string partition);

                // WiFi network manage methods
                void set(std::string ssid, std::string password);
                const wifi_list &get_wifi_list() const;
                void remove(std::string ssid);
                void clear();

                // Methods to save and load from the ESP
                void load();
                void save();
            };
        };
    };
};

#endif /* __DSL_ESP_WIFIMANAGER_H__ */