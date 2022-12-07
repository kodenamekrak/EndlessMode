#include "main.hpp"
#include "SongManager.hpp"

#include "GlobalNamespace/PauseMenuManager.hpp"
#include "GlobalNamespace/LevelBar.hpp"

#include "questui/shared/BeatSaberUI.hpp"

UnityEngine::UI::Toggle* toggle;

MAKE_AUTO_HOOK_MATCH(PauseMenuManager_Start, &GlobalNamespace::PauseMenuManager::Start, void, GlobalNamespace::PauseMenuManager* self) {
    PauseMenuManager_Start(self);

    toggle = QuestUI::BeatSaberUI::CreateToggle(self->levelBar->get_transform(), "Continue Songs", EndlessMode::SongManager::GetShouldContinue(), [](bool value)
    {
        EndlessMode::SongManager::SetShouldContinue(value);
    });
}