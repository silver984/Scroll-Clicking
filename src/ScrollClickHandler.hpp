#pragma once

#include <Geode/Geode.hpp>
#include <string>

using namespace geode::prelude;

struct ScrollClickHandler {
    bool toggle = Mod::get()->getSettingValue<bool>("toggle");
    bool isTrueScroll = false;
    bool scrolled = false;
    bool scrolledUp = false;
    bool p1 = Mod::get()->getSettingValue<bool>("p1");;
    bool p2 = Mod::get()->getSettingValue<bool>("p2");;
    int holdInterval = 0;
    int releaseInterval = 0;
    std::string keyStrP1 = Mod::get()->getSettingValue<std::string>("key-p1");;
    std::string keyStrP2 = Mod::get()->getSettingValue<std::string>("key-p2");;

    bool hasClickedOnce = false;
    bool isGateOpen = false;
    bool isHolding = false;
    int stateFrames = 0;
    int frames = 0;

    void press(bool clickVal);
    void resetState();
    void scrollFn();
    void onScroll();
    void setupDirectionalMode(bool mode);

    static ScrollClickHandler& get();
};