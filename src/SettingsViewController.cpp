#include "main.hpp"
#include "SettingsViewController.hpp"
#include "ModConfig.hpp"

#include "questui/shared/BeatSaberUI.hpp"

DEFINE_TYPE(EndlessMode, SettingsViewController);

void EndlessMode::SettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    if(firstActivation)
    {
        auto settingsContainer = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform());

        QuestUI::BeatSaberUI::CreateToggle(settingsContainer->get_transform(), "Continue after fail", getModConfig().ContinueAfterFail.GetValue(), [](bool value)
        {
            getModConfig().ContinueAfterFail.SetValue(value, true);
        });
    }
}