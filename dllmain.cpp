#include <memory>
#include <Windows.h>
#include "Plugin.hpp"
#include <string>

using namespace uevr;

typedef int (*GetGPUUsageFn)();

class UEVRPlugin : public Plugin {
public:
    HMODULE hModule = nullptr;
    GetGPUUsageFn get_gpu_usage = nullptr;
    int screenpercentage = 50;
    float sinceincrease = 0;
    float sincedecrease = 0;

    UEVRPlugin() = default;

    void on_dllmain(HANDLE handle) override {
        

        //API::get()->log_info("DLL and function loaded successfully.");
        
        //API::get()->log_info("no");
                
    }

    void on_initialize() override {
        hModule = LoadLibraryW(L"AutoScalerNvidia.dll");
        if (!hModule) {
            API::get()->log_error("Failed to load AutoScalerNvidia.dll");
            return;
        }

        get_gpu_usage = (GetGPUUsageFn)GetProcAddress(hModule, "get_gpu_usage");
        if (!get_gpu_usage) {
            API::get()->log_error("Function not found in DLL.");
            FreeLibrary(hModule);
            hModule = nullptr;
            return;
        }
        API::get()->log_info("Plugin initialized.");
    }

    void on_post_engine_tick(UEVR_UGameEngineHandle engine, float delta) override {
        if (get_gpu_usage) {
            sinceincrease = sinceincrease + delta;
            sincedecrease = sincedecrease + delta;
            int usage = get_gpu_usage();
            API::get()->log_info("GPU Usage: %d%%", usage);

            if (usage <= 82 && sinceincrease > 5) {
                sinceincrease = 0;
                screenpercentage = screenpercentage + 5;
                std::wstring command = L"r.ScreenPercentage ";
                command.append(std::to_wstring(screenpercentage));
                API::get()->sdk()->functions->execute_command(command.c_str());

                API::get()->log_info("Increased to: %d%%", screenpercentage);
            }
            if (usage >= 92 && sincedecrease > 1) {
                sincedecrease = 0;
                screenpercentage = screenpercentage - 10;
                std::wstring command = L"r.ScreenPercentage ";
                command.append(std::to_wstring(screenpercentage));
                API::get()->sdk()->functions->execute_command(command.c_str());

                API::get()->log_info("Decreased to: %d%%", screenpercentage);
            }
        }
        else {
            API::get()->log_info("No gpu usage function :(");
        }
    }

    /*void on_shutdown() override {
        if (hModule) {
            FreeLibrary(hModule);
            hModule = nullptr;
        }
    }*/
};

extern "C" __declspec(dllexport) Plugin* create_plugin() {
    return new UEVRPlugin();
}


// Actually creates the plugin. Very important that this global is created.
// The fact that it's using std::unique_ptr is not important, as long as the constructor is called in some way.
std::unique_ptr<UEVRPlugin> g_plugin{ new UEVRPlugin() };
