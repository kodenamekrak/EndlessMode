#include "main.hpp"
#include "SongManager.hpp"

#include "bs-utils/shared/utils.hpp"

// Level collection
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/LevelCollectionNavigationController.hpp"
#include "GlobalNamespace/IBeatmapLevelPack.hpp"
#include "GlobalNamespace/IBeatmapLevelCollection.hpp"

// Collecting new level
#include "GlobalNamespace/BeatmapLevelsModel.hpp"
#include "GlobalNamespace/BeatmapLevelsModel_-GetBeatmapLevelAsync-d__38.hpp"
#include "GlobalNamespace/IBeatmapLevelData.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"

#include "System/Threading/Tasks/Task.hpp"
#include "System/Threading/Tasks/Task_1.hpp"
#include "System/Action_1.hpp"


// Starting new level
#include "GlobalNamespace/SinglePlayerLevelSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp"
#include "GlobalNamespace/LevelCompletionResults.hpp"
#include "GlobalNamespace/MenuTransitionsHelper.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/IDifficultyBeatmapSet.hpp"
#include "GlobalNamespace/IBeatmapLevel.hpp"
#include "GlobalNamespace/PlayerSpecificSettings.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/ColorSchemesSettings.hpp"
#include "GlobalNamespace/OverrideEnvironmentSettings.hpp"
#include "GlobalNamespace/PlayerData.hpp"
#include "GlobalNamespace/PlayerDataModel.hpp"


// Saving scores
#include "GlobalNamespace/IReadonlyBeatmapData.hpp"
#include "GlobalNamespace/SoloFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/PlayerAllOverallStatsData.hpp"
#include "GlobalNamespace/PlayerAllOverallStatsData_PlayerOverallStatsData.hpp"
#include "GlobalNamespace/PlayerLevelStatsData.hpp"
#include "GlobalNamespace/LevelCompletionResultsHelper.hpp"
#include "GlobalNamespace/ResultsViewController.hpp"


#include "custom-types/shared/delegate.hpp"

#include "System/Action_2.hpp"

#include "UnityEngine/Resources.hpp"

#include <random>

using namespace GlobalNamespace;

ArrayW<IPreviewBeatmapLevel*> previewBeatmapLevels;
bool shouldContinue;

namespace EndlessMode::SongManager
{

    void SetShouldContinue(bool value)
    {
        shouldContinue = value;
    }

    bool GetShouldContinue()
    {
        return shouldContinue;
    }

    void CollectSongs()
    {
        auto levelCollectionNavigationController = UnityEngine::Resources::FindObjectsOfTypeAll<LevelCollectionNavigationController*>().FirstOrDefault();

        previewBeatmapLevels = levelCollectionNavigationController->levelPack->i_IAnnotatedBeatmapLevelCollection()->get_beatmapLevelCollection()->get_beatmapLevels();
        getLogger().info("Collected %lu beatmaps", previewBeatmapLevels.Length());
    }


    // Basically same as base game but doesnt call ResultsViewController since it causes issues
    void LevelCompleted(LevelCompletionResults* completionResults, IReadonlyBeatmapData* transformedBeatmapData, IDifficultyBeatmap* difficultyBeatmap, GameplayModifiers* gameplayModifiers)
    {
        getLogger().info("Saving score for song");
        auto soloFreePlayFlowCoordinator = UnityEngine::Resources::FindObjectsOfTypeAll<SoloFreePlayFlowCoordinator*>().FirstOrDefault();
        auto playerDataModel = soloFreePlayFlowCoordinator->playerDataModel;

        playerDataModel->playerData->playerAllOverallStatsData->UpdateSoloFreePlayOverallStatsData(completionResults, difficultyBeatmap);
        playerDataModel->Save();

        auto playerLevelStatsData = playerDataModel->playerData->GetPlayerLevelStatsData(difficultyBeatmap);
        bool newHighScore = playerLevelStatsData->highScore < completionResults->modifiedScore;

        playerDataModel->playerData->IncreaseNumberOfGameplays(playerLevelStatsData);

        playerLevelStatsData->UpdateScoreData(completionResults->modifiedScore, completionResults->maxCombo, completionResults->fullCombo, completionResults->rank);
        playerDataModel->Save();

        if(shouldContinue)
            GetAndStartNextSong();
    }

    void StartNewSong(IDifficultyBeatmap* difficultyBeatmap)
    {
        getLogger().info("Starting new song");
        bs_utils::Submission::disable(getModInfo());

        auto singlePlayerLevelSelectionFlowCoordinator = UnityEngine::Resources::FindObjectsOfTypeAll<SinglePlayerLevelSelectionFlowCoordinator*>().FirstOrDefault();
        auto playerData = UnityEngine::Resources::FindObjectsOfTypeAll<PlayerDataModel*>().FirstOrDefault()->playerData;

        std::function<void(StandardLevelScenesTransitionSetupDataSO*, LevelCompletionResults*)> levelFinished = [singlePlayerLevelSelectionFlowCoordinator](StandardLevelScenesTransitionSetupDataSO* transitionSetupData, LevelCompletionResults* completionResults)
        {
            if(completionResults->levelEndStateType == LevelCompletionResults::LevelEndStateType::Cleared)
                LevelCompleted(completionResults, transitionSetupData->get_transformedBeatmapData(), transitionSetupData->difficultyBeatmap, transitionSetupData->gameplayModifiers);
            // Still show level failed screen
            // else if(completionResults->levelEndStateType == LevelCompletionResults::LevelEndStateType::Failed)
            //     UnityEngine::Resources::FindObjectsOfTypeAll<ResultsViewController*>().FirstOrDefault()->Init(completionResults, transitionSetupData->get_transformedBeatmapData(), transitionSetupData->difficultyBeatmap, false, false);
        };
        auto levelFinishedDelegate = custom_types::MakeDelegate<System::Action_2<StandardLevelScenesTransitionSetupDataSO*, LevelCompletionResults*>*>(levelFinished);

        singlePlayerLevelSelectionFlowCoordinator->menuTransitionsHelper->StartStandardLevel(
            "Standard",
            difficultyBeatmap,
            difficultyBeatmap->get_level()->i_IPreviewBeatmapLevel(),
            playerData->overrideEnvironmentSettings,
            playerData->colorSchemesSettings->GetSelectedColorScheme(),
            playerData->gameplayModifiers,
            playerData->playerSpecificSettings,
            nullptr,
            "Back",
            false,
            false,
            nullptr,
            levelFinishedDelegate,
            nullptr
        );
    }

    void GetAndStartNextSong()
    {
        if(!previewBeatmapLevels)
            return;
        
        auto beatmapLevelsModel = UnityEngine::Resources::FindObjectsOfTypeAll<BeatmapLevelsModel*>().FirstOrDefault();

        std::function<void(System::Threading::Tasks::Task_1<BeatmapLevelsModel::GetBeatmapLevelResult>*)> gotBeatmapLevel = [](System::Threading::Tasks::Task_1<BeatmapLevelsModel::GetBeatmapLevelResult>* task)
        {
            auto result = task->get_Result();
            if(result.isError)
            {
                getLogger().info("Error getting beatmap");
                return;
            }
            ArrayW<IDifficultyBeatmapSet*> beatmapSets = result.beatmapLevel->get_beatmapLevelData()->get_difficultyBeatmapSets();
            ArrayW<IDifficultyBeatmap*> difficultyBeatmaps = beatmapSets[0]->get_difficultyBeatmaps();
            StartNewSong(difficultyBeatmaps.Last());
        };
        auto gotBeatmapLevelDelegate = custom_types::MakeDelegate<System::Action_1<System::Threading::Tasks::Task*>*>(gotBeatmapLevel);

        auto beatmapTask = beatmapLevelsModel->GetBeatmapLevelAsync(previewBeatmapLevels[rand() % (previewBeatmapLevels.Length() - 1)]->get_levelID(), System::Threading::CancellationToken::get_None());
        
        reinterpret_cast<System::Threading::Tasks::Task*>(beatmapTask)->ContinueWith(gotBeatmapLevelDelegate);
    }
}