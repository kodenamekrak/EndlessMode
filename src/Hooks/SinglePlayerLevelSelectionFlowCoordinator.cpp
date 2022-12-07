#include "main.hpp"
#include "SongManager.hpp"

#include "bs-utils/shared/utils.hpp"

#include "GlobalNamespace/SinglePlayerLevelSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/LevelSelectionFlowCoordinator.hpp"

using namespace GlobalNamespace;

MAKE_AUTO_HOOK_MATCH(SinglePlayerLevelSelectionFlowCoordinator_ActionButtonWasPressed, &SinglePlayerLevelSelectionFlowCoordinator::ActionButtonWasPressed, void, SinglePlayerLevelSelectionFlowCoordinator* self)
{
    SinglePlayerLevelSelectionFlowCoordinator_ActionButtonWasPressed(self);
    EndlessMode::SongManager::CollectSongs();
}

MAKE_AUTO_HOOK_MATCH(LevelSelectionFlowCoordinator_DidActivate, &LevelSelectionFlowCoordinator::DidActivate, void, LevelSelectionFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    bs_utils::Submission::enable(getModInfo());
    LevelSelectionFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}