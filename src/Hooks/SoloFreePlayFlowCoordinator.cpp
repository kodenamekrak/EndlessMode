#include "main.hpp"
#include "SongManager.hpp"

#include "GlobalNamespace/SoloFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/LevelCompletionResults.hpp"
#include "GlobalNamespace/IReadonlyBeatmapData.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/ResultsViewController.hpp"

MAKE_AUTO_HOOK_MATCH(SoloFreePlayFlowCoordinator_ProcessLevelCompletionResultsAfterLevelDidFinish, &GlobalNamespace::SoloFreePlayFlowCoordinator::ProcessLevelCompletionResultsAfterLevelDidFinish, void, GlobalNamespace::SoloFreePlayFlowCoordinator* self, GlobalNamespace::LevelCompletionResults* levelCompletionResults, GlobalNamespace::IReadonlyBeatmapData* transformedBeatmapData, GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap, GlobalNamespace::GameplayModifiers* gameplayModifiers, bool practice)
{
    SoloFreePlayFlowCoordinator_ProcessLevelCompletionResultsAfterLevelDidFinish(self, levelCompletionResults, transformedBeatmapData, difficultyBeatmap, gameplayModifiers, practice);

    if(levelCompletionResults->levelEndStateType == GlobalNamespace::LevelCompletionResults::LevelEndStateType::Cleared && EndlessMode::SongManager::GetShouldContinue())
    {
        self->resultsViewController->ContinueButtonPressed();
        EndlessMode::SongManager::GetAndStartNextSong();
    }
}