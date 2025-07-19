#include <Geode/Geode.hpp>
#include <Geode/modify/CCMouseDispatcher.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "ScrollClickHandler.hpp"

using namespace geode::prelude;

class $modify(CCMouseDispatcher) {
    bool dispatchScrollMSG(float x, float y) {
        if (!CCMouseDispatcher::dispatchScrollMSG(x, y))
            return false;

        auto& handler = ScrollClickHandler::get();
        if (!GJBaseGameLayer::get() || !handler.toggle)
            return true;

        handler.scrolledUp = (x < 0);
        handler.scrolled = true;

        return true;
    }
};

class $modify(GJBaseGameLayer) {
    bool init() override {
        if (!GJBaseGameLayer::init())
            return false;

        if (ScrollClickHandler::get().toggle)
            ScrollClickHandler::get().resetState();

        return true;
    }

    void processCommands(float p0) {
        GJBaseGameLayer::processCommands(p0);

        auto& handler = ScrollClickHandler::get();
        if (handler.toggle)
            handler.scrollFn();
    }
};


$execute {
    auto& handler = ScrollClickHandler::get();

    listenForSettingChangesV3<bool>("toggle", [&](bool v) {
        handler.toggle = v;
    });
    listenForSettingChangesV3<bool>("p1", [&](bool v) {
        handler.p1 = v;
    });
    listenForSettingChangesV3<bool>("p2", [&](bool v) {
        handler.p2 = v;
    });
    listenForSettingChangesV3<std::string>("key-p1", [&](std::string v) {
        handler.keyStrP1 = std::move(v);
    });
    listenForSettingChangesV3<std::string>("key-p2", [&](std::string v) {
        handler.keyStrP2 = std::move(v);
    });
    listenForSettingChangesV3<bool>("directional-mode", [&](bool v) {
        handler.setupDirectionalMode(v);
    });
    handler.setupDirectionalMode(true);
}