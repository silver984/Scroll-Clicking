#include <Geode/modify/CCScheduler.hpp>
#include "globals.hpp"

using namespace geode::prelude;

template <typename T>
T getSettings(std::string name) {
    static std::unordered_map<std::string, T> cache;
    static std::mutex mutex;

    {
        std::lock_guard lock(mutex);
        if (!cache.contains(name)) {
            T val = Mod::get()->getSettingValue<T>(name);
            cache[name] = val;

            listenForSettingChangesV3<T>(name, [name](T newVal) {
                std::lock_guard lock(mutex);
                cache[name] = newVal;
                });
        }
        return cache[name];
    }
}

class $modify(CCScheduler) {
    void update(float dt) {
        // note: im just gonna use CCScheduler instead cuz listenForSettingChangesV3 is kinda scuffed
        // it works better... i think grrrr.... -silver

        static auto& settings = globals.settings;

        settings.isModEnabled = getSettings<bool>("toggle");
        if (!settings.isModEnabled) return CCScheduler::update(dt);

        settings.isDirectionalMode = getSettings<bool>("directional-mode");
        settings.invertDirectionalMode = getSettings<bool>("inverted-directional-mode");
        settings.isVelocityMode = getSettings<bool>("velocity-mode");
        settings.isStableScroll = getSettings<bool>("stable-scroll");
        settings.affectP1 = getSettings<bool>("p1");
        settings.affectP2 = getSettings<bool>("p2");

        int hold = getSettings<int>("hold-interval");
        int release = getSettings<int>("release-interval");

        if (settings.isDirectionalMode) {
            auto& scroll = globals.scrollState;
            if (!settings.invertDirectionalMode) {
                settings.holdInterval = scroll.scrolledUp ? hold : release;
                settings.releaseInterval = scroll.scrolledUp ? release : hold;
            }
            else {
                settings.holdInterval = scroll.scrolledUp ? release : hold;
                settings.releaseInterval = scroll.scrolledUp ? hold : release;
            }
        }
        else {
            settings.holdInterval = hold;
            settings.releaseInterval = release;
        }

        settings.maxVelocity = getSettings<int>("max-velocity");

        CCScheduler::update(dt);
    }
};