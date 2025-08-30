#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

struct Globals {
    struct {
        bool isModEnabled = true;
        bool isDirectionalMode = true;
        bool invertDirectionalMode = false;
        bool isVelocityMode = false;
        bool isStableScroll = true;
        bool affectP1 = true;
        bool affectP2 = true;
        int holdInterval = 1;
        int releaseInterval = 1;
        int maxVelocity = 8;
    } settings;

    struct {
        bool scrolledUp = false;
        bool scrolled = false;
        float velocity = 0.f;
    } scrollState;

    void press(bool clickVal);
};

inline Globals globals;