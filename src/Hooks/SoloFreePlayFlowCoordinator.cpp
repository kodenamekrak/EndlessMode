#include "main.hpp"
#include "SongManager.hpp"
#include "ModConfig.hpp"

#include "GlobalNamespace/SoloFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/LevelCompletionResults.hpp"
#include "GlobalNamespace/IReadonlyBeatmapData.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/ResultsViewController.hpp"

#include "conditional-dependencies/shared/main.hpp"

MAKE_AUTO_HOOK_MATCH(SoloFreePlayFlowCoordinator_ProcessLevelCompletionResultsAfterLevelDidFinish, &GlobalNamespace::SoloFreePlayFlowCoordinator::ProcessLevelCompletionResultsAfterLevelDidFinish, void, GlobalNamespace::SoloFreePlayFlowCoordinator* self, GlobalNamespace::LevelCompletionResults* levelCompletionResults, GlobalNamespace::IReadonlyBeatmapData* transformedBeatmapData, GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap, GlobalNamespace::GameplayModifiers* gameplayModifiers, bool practice)
{
    bool isReplaying;
    auto fun = CondDeps::Find<bool>("replay", "IsInReplay");
    if (fun)
        isReplaying = fun.value()();
    SoloFreePlayFlowCoordinator_ProcessLevelCompletionResultsAfterLevelDidFinish(self, levelCompletionResults, transformedBeatmapData, difficultyBeatmap, gameplayModifiers, practice);

    if((levelCompletionResults->levelEndStateType == 1 || (levelCompletionResults->levelEndStateType == 2 && getModConfig().ContinueAfterFail.GetValue())) && EndlessMode::SongManager::GetShouldContinue() && !practice && !isReplaying)
    {
        self->resultsViewController->ContinueButtonPressed();
        EndlessMode::SongManager::GetAndStartNextSong();
    }
}