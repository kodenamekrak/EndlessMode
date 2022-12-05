#include "main.hpp"
#include "SongManager.hpp"

#include "GlobalNamespace/SinglePlayerLevelSelectionFlowCoordinator.hpp"

using namespace GlobalNamespace;

MAKE_AUTO_HOOK_MATCH(SinglePlayerLevelSelectionFlowCoordinator_ActionButtonWasPressed, &SinglePlayerLevelSelectionFlowCoordinator::ActionButtonWasPressed, void, SinglePlayerLevelSelectionFlowCoordinator* self)
{
    SinglePlayerLevelSelectionFlowCoordinator_ActionButtonWasPressed(self);
    EndlessMode::SongManager::CollectSongs();
}