#include "main.hpp"
#include "SettingsViewController.hpp"
#include "ModConfig.hpp"
#include "questui/shared/QuestUI.hpp"

static ModInfo modInfo;

ModInfo& getModInfo() {
    return modInfo;
}

Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load();
    getLogger().info("Completed setup!");
}

extern "C" void load() {
    il2cpp_functions::Init();

    QuestUI::Init();
    QuestUI::Register::RegisterModSettingsViewController<endlessmode::SettingsViewController*>(modInfo, "EndlessMode");

    getModConfig().Init(modInfo);

    getLogger().info("Installing hooks...");
    Hooks::InstallHooks(getLogger());
    getLogger().info("Installed all hooks!");
}