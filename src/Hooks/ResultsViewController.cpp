#include "main.hpp"
#include "SongManager.hpp"

#include "GlobalNamespace/ResultsViewController.hpp"
#include "questui/shared/BeatSaberUI.hpp"

MAKE_AUTO_HOOK_MATCH(ResultsViewController_DidActivate, &GlobalNamespace::ResultsViewController::DidActivate, void, GlobalNamespace::ResultsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    ResultsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if(firstActivation)
    {
        QuestUI::BeatSaberUI::CreateUIButton(self->get_transform(), "KEEP PLAYING", []()
        {
            EndlessMode::SongManager::SetShouldContinue(true);
            EndlessMode::SongManager::GetAndStartNextSong();
        });
    }
}