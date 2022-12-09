#include "main.hpp"
#include "SongManager.hpp"

#include "bs-utils/shared/utils.hpp"

#include "GlobalNamespace/SinglePlayerLevelSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/LevelSelectionFlowCoordinator.hpp"

#include "System/Action.hpp"

using namespace GlobalNamespace;

bool inPracticeMenu = true;

MAKE_AUTO_HOOK_MATCH(SinglePlayerLevelSelectionFlowCoordinator_StartLevelOrShow360Prompt, &SinglePlayerLevelSelectionFlowCoordinator::StartLevelOrShow360Prompt, void, SinglePlayerLevelSelectionFlowCoordinator* self, System::Action* beforeSceneSwitchCallback, bool practice)
{
    SinglePlayerLevelSelectionFlowCoordinator_StartLevelOrShow360Prompt(self, beforeSceneSwitchCallback, practice);
    if(!practice)
        EndlessMode::SongManager::CollectSongs();
}

MAKE_AUTO_HOOK_MATCH(LevelSelectionFlowCoordinator_DidActivate, &LevelSelectionFlowCoordinator::DidActivate, void, LevelSelectionFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    LevelSelectionFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    bs_utils::Submission::enable(getModInfo());
}