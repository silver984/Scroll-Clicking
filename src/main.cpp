#include <Geode/Geode.hpp>
#include <Geode/modify/CCMouseDispatcher.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "functions.hpp"
#include "data.hpp"
#include "util.hpp"

using namespace geode::prelude;

bool isInLevel = false;
bool scrolledUp = false;

class $modify(CCMouseDispatcher) {
    bool dispatchScrollMSG(float x, float y) {
        if (!CCMouseDispatcher::dispatchScrollMSG(x, y))
            return false;

        RETURN_IF_MOD_DISABLED(true);

        if (x < 0)
            scrolledUp = true;
        else if (x > 0)
            scrolledUp = false;

        if (isInLevel)
            scrolled = true;

        return true;
    }
};

class $modify(PlayLayer) {
    void startGame() {
        RETURN_IF_MOD_DISABLED(PlayLayer::startGame());

        isInLevel = true;
        resetState();

        PlayLayer::startGame();
    }

    void onQuit() {
        RETURN_IF_MOD_DISABLED(PlayLayer::onQuit());

        isInLevel = false;

        PlayLayer::onQuit();
    }
};

class $modify(GJBaseGameLayer) {
    void processCommands(float p0) {
        RETURN_IF_MOD_DISABLED(GJBaseGameLayer::processCommands(p0));

        GJBaseGameLayer::processCommands(p0);

        scrollFn();
    }
};

class $modify(CCScheduler) {
    void update(float dt) {
        toggle = setting<bool>("toggle");

        RETURN_IF_MOD_DISABLED(CCScheduler::update(dt));

        bool directionalMode = setting<bool>("directional-mode");
        if (directionalMode) {
            holdInterval = scrolledUp ? setting<int>("hold-interval") : setting<int>("release-interval");
            releaseInterval = scrolledUp ? setting<int>("release-interval") : setting<int>("hold-interval");
        }
        else {
            holdInterval = setting<int>("hold-interval");
            releaseInterval = setting<int>("release-interval");
        }
        
        p1 = setting<bool>("p1");
        p2 = setting<bool>("p2");
        keyStrP1 = setting<std::string>("key-p1");
        keyStrP2 = setting<std::string>("key-p2");

        CCScheduler::update(dt);
    }
};