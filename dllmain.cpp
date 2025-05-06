#include <memory>
#include <Windows.h>
#include "Plugin.hpp"
#include <string>
#include <C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.9\include\nvml.h>

using namespace uevr;

typedef int (*GetGPUUsageFn)();

class UEVRPlugin : public Plugin {
public:
    HMODULE hModule = nullptr;    
    int screenpercentage = 50;
    float sinceincrease = 0;
    float sincedecrease = 0;
    int framesoverbudget = 0;

    int get_gpu_usage() {
        static bool initialized = false;
        static nvmlDevice_t device;

        if (!initialized) {
            if (nvmlInit() != NVML_SUCCESS) return -1;
            if (nvmlDeviceGetHandleByIndex(0, &device) != NVML_SUCCESS) return -1;
            initialized = true;
        }

        nvmlUtilization_t utilization;
        if (nvmlDeviceGetUtilizationRates(device, &utilization) == NVML_SUCCESS) {
            return utilization.gpu;
        }

        return -1;
    }

    UEVRPlugin() = default;

    void on_dllmain(HANDLE handle) override {     
    }

    void on_initialize() override {        
        API::get()->log_info("Plugin initialized.");
    }

    void on_post_engine_tick(UEVR_UGameEngineHandle engine, float delta) override {        
        sinceincrease = sinceincrease + delta;
        sincedecrease = sincedecrease + delta;
        int usage = get_gpu_usage();        
        if (usage == -1) {
            return;
        }
        //aim to keep the usage between 82 & 92 percent
        //increase infrequently only by 5%, every 5 seconds at most, to prevent too many hitches
        //decrease often and by 10%, every 0.5 seconds if needed, so we're not below target too long
        if (usage <= 82 && sinceincrease > 0.5 && screenpercentage < 100) {
            screenpercentage = screenpercentage + 2;
            std::wstring command = L"r.ScreenPercentage ";
            command.append(std::to_wstring(screenpercentage));
            API::get()->sdk()->functions->execute_command(command.c_str());

            API::get()->log_info("Usage now %d%%. Increased res to: %d%% after %.2f secs", usage, screenpercentage, sinceincrease);
            sinceincrease = 0;
        }
        if (usage >= 92 && screenpercentage >= 20) {
            framesoverbudget = framesoverbudget + 1;
            if (framesoverbudget > 9)
            {
                screenpercentage = screenpercentage - 3;
                std::wstring command = L"r.ScreenPercentage ";
                command.append(std::to_wstring(screenpercentage));
                API::get()->sdk()->functions->execute_command(command.c_str());

                API::get()->log_info("Usage now %d%%. Decreased res to: %d%% after %.2f secs", usage, screenpercentage, sincedecrease);
                sincedecrease = 0;
                framesoverbudget = 0;
            }
        }
        else {
            framesoverbudget = 0;
        }

        /*else if (usage >= 92 && sincedecrease > 0.1) {            
            screenpercentage = screenpercentage - 3;
            std::wstring command = L"r.ScreenPercentage ";
            command.append(std::to_wstring(screenpercentage));
            API::get()->sdk()->functions->execute_command(command.c_str());

            API::get()->log_info("Usage now %d%%. Decreased res to: %d%% after %.2f secs", usage, screenpercentage, sincedecrease);
            sincedecrease = 0;
        }     */  
    }    
};

extern "C" __declspec(dllexport) Plugin* create_plugin() {
    return new UEVRPlugin();
}

// Actually creates the plugin. Very important that this global is created.
// The fact that it's using std::unique_ptr is not important, as long as the constructor is called in some way.
std::unique_ptr<UEVRPlugin> g_plugin{ new UEVRPlugin() };
