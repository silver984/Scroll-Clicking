#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "scroll/listener.hpp"
#include "scroll/functions.hpp"
#include "data.hpp"
#include "conflict.hpp"

using namespace geode::prelude;

class $modify(GJBaseGameLayer) {
    virtual bool init() {
        if (!GJBaseGameLayer::init())
            return false;

        check(); // check for conflicts with other mods

        toggle = Mod::get()->getSettingValue<bool>("switch");
        if (!toggle || isConflicted)
            return true;

        hookWndProc(); // hook into window procedure

        cachedInterval = Mod::get()->getSettingValue<int>("clickInterval");
        isTrueScroll = Mod::get()->getSettingValue<bool>("istrue");
        p1 = Mod::get()->getSettingValue<bool>("p1");
        p2 = Mod::get()->getSettingValue<bool>("p2");

        isGateOpen = false;
        isClicking = false;
        isConflicted = false;

        interval = cachedInterval + 1;
        intervalFrames = 0;
        scrollPasses = 0;
        frames = 0;

        return true;
    }

    void processCommands(float p0) {
        if (isConflicted || !g_hooked || !toggle) {
            GJBaseGameLayer::processCommands(p0);
            return;
        }

        GJBaseGameLayer::processCommands(p0);

        if (isTrueScroll)
            trueScroll();
        else
            constantScroll();
    }
};

class $modify(PlayLayer) {
    void startGame() {
        if (!toggle) {
            PlayLayer::startGame();
            return;
        }
        if (!g_hooked) {
            std::string errorMsg = "Scroll Clicking failed. Check logs.";
            Notification::create(errorMsg, NotificationIcon::Error)->show();
        }

        PlayLayer::startGame();
    }

    void onQuit() {
        if (!toggle) {
            PlayLayer::onQuit();
            return;
        }
        isConflicted = false;
        PlayLayer::onQuit();
    }
};