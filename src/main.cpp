#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "ScrollListener.hpp"

using namespace geode::prelude;

bool isGateOpen;
bool isClicking;
bool conflicted;
bool didShowError = false;
bool p1;
bool p2;
bool toggle;
int initPasses = 0;
int interval;
int cachedInterval;
int intervalFrames;
int frames;

void hookWndProc();

class $modify(PlayLayer) {
    void startGame() {
        if (!toggle) {
            PlayLayer::startGame();
            return;
        }

        if (!g_hooked)
            Notification::create("Couldn't initialize Scroll Clicking! Please check logs.", NotificationIcon::Error)->show();

        PlayLayer::startGame();
    }

    void onQuit() {
        if (!toggle) {
            PlayLayer::onQuit();
            return;
        }

        conflicted = false;

        PlayLayer::onQuit();
    }
};

class $modify(GJBaseGameLayer) {
    virtual bool init() {
        if (!GJBaseGameLayer::init())
            return false;

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

                    conflicted = true;
                    break;
                }
            }
        }

        toggle = Mod::get()->getSettingValue<bool>("switch");
        if (!toggle || conflicted)
            return true;

        hookWndProc();

        isGateOpen = false;
        isClicking = false;
        conflicted = false;
        interval = 3;
        cachedInterval = Mod::get()->getSettingValue<int>("clickInterval");
        p1 = Mod::get()->getSettingValue<bool>("p1");
        p2 = Mod::get()->getSettingValue<bool>("p2");
        intervalFrames = 0;
        frames = 0;

        return true;
    }

    void processCommands(float p0) {
        if (conflicted || !g_hooked || !toggle) {
            GJBaseGameLayer::processCommands(p0);
            return;
        }

        GJBaseGameLayer::processCommands(p0);

        if (g_mouseScrolled) {
            g_mouseScrolled = false;
            isGateOpen = true;
            interval = cachedInterval + 1;
            frames = 0;
        }

        intervalFrames++;
        frames++;

        if (isGateOpen) {
            if (frames <= interval * 2) {
                if (intervalFrames >= interval) {
                    isClicking = !isClicking;
                    intervalFrames = 0;
                }
                if (p1)
                    CCKeyboardDispatcher::get()->dispatchKeyboardMSG(enumKeyCodes::KEY_Space, isClicking, false);
                if (p2)
                    CCKeyboardDispatcher::get()->dispatchKeyboardMSG(enumKeyCodes::KEY_Up, isClicking, false);
            }
            else {
                if (p1)
                    CCKeyboardDispatcher::get()->dispatchKeyboardMSG(enumKeyCodes::KEY_Space, false, false);
                if (p2)
                    CCKeyboardDispatcher::get()->dispatchKeyboardMSG(enumKeyCodes::KEY_Up, false, false);

                isGateOpen = false;
            }
        }
    }
};