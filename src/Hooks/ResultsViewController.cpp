#include "main.hpp"
#include "SongManager.hpp"
#include "GlobalNamespace/ResultsViewController.hpp"
#include "GlobalNamespace/LevelCompletionResults.hpp"
#include "GlobalNamespace/IReadonlyBeatmapData.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"

MAKE_AUTO_HOOK_MATCH(ResultsViewController_Init, &GlobalNamespace::ResultsViewController::DidActivate, void, GlobalNamespace::ResultsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    ResultsViewController_Init(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    EndlessMode::SongManager::GetAndStartNextSong();
}