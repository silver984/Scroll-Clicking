#include <Geode/modify/CCMouseDispatcher.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include "globals.hpp"

using namespace geode::prelude;

/*
    note: yes i know this hook looks unnecessary lol
    this makes it so that you cannot buffer a scroll input before the level actually starts itself
    (music starts, cube starts moving, etc.)
    also for no pause buffering

    if anyone has a better way, please do change this :p -silver
*/
bool canScroll = false;
class $modify(PlayLayer) {
    void startGame() {
        canScroll = true;
        PlayLayer::startGame();
    }

    void onQuit() {
        canScroll = false;
        PlayLayer::onQuit();
    }

    void pauseGame(bool p0) {
        canScroll = false;
        PlayLayer::pauseGame(p0);
    }

    void resume() {
        canScroll = true;
        PlayLayer::resume();
    }
};

class $modify(LevelEditorLayer) {
    void onPlaytest() {
        canScroll = true;
        LevelEditorLayer::onPlaytest();
    }

    void onResumePlaytest() {
        canScroll = true;
        LevelEditorLayer::onResumePlaytest();
    }

    void onStopPlaytest() {
        canScroll = false;
        LevelEditorLayer::onStopPlaytest();
    }
};

class $modify(CCMouseDispatcher) {
    bool dispatchScrollMSG(float x, float y) {
        if (!CCMouseDispatcher::dispatchScrollMSG(x, y))
            return false;

        if (!globals.settings.isModEnabled || !canScroll)
            return true;

        static auto& scroll = globals.scrollState;
        scroll.scrolledUp = (x < 0);
        scroll.scrolled = true;

        return true;
    }
};