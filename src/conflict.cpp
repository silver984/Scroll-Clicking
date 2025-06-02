#include <Geode/Geode.hpp>
#include "data.hpp"

using namespace geode::prelude;

void check() {
    struct ConflictingModInfo {
        std::string settingKey;
        bool conflictValue;
    };

    std::unordered_map<std::string, ConflictingModInfo> conflictingMods = {
        { "syzzi.click_between_frames", { "soft-toggle", false } },
        { "alphalaneous.click_after_frames", { "soft-toggle", false } },
    };

    for (const auto& [modID, info] : conflictingMods) {
        if (Loader::get()->isModLoaded(modID)) {
            auto* mod = Loader::get()->getLoadedMod(modID);

            if (mod->getSettingValue<bool>(info.settingKey) == info.conflictValue) {
                log::error("Mod can't run with {} ({}) enabled.", mod->getName(), modID);
                Notification::create(fmt::format("Incompatible mod detected: {}", mod->getName()), NotificationIcon::Error)->show();

                isConflicted = true;
                break;
            }
        }
    }
}