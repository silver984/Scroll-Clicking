#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "globals.hpp"
#include <cmath>

using namespace geode::prelude;

struct State {
    bool isGateOpen = false;
    bool isHolding = true;

    int phaseFrames = 0;
    int elapsedFrames = 0;

    void onScroll() {
        isGateOpen = true;
        elapsedFrames = 0;

        if (globals.scrollState.velocity < globals.settings.maxVelocity)
            globals.scrollState.velocity++;
    }

    void reset() {
        *this = State();
    }
};
State state;

class $modify(GJBaseGameLayer) {
    bool init() override {
        if (!GJBaseGameLayer::init()) return false;
        if (!globals.settings.isModEnabled) return true;

        state.reset();
        globals.scrollState.velocity = 0.f;

        return true;
    }

    void processCommands(float p0) {
        if (!globals.settings.isModEnabled) return GJBaseGameLayer::processCommands(p0);
        GJBaseGameLayer::processCommands(p0); // everything does NOT work unless you call early for some reason

        static auto& scroll = globals.scrollState;
        static auto& settings = globals.settings;

        if (scroll.velocity >= 0.5f)
            scroll.velocity *= std::pow(0.01f, p0);
        else scroll.velocity = 0.f;

        static float velocityDiff;
        static float velocityQuotient;
        velocityDiff = settings.maxVelocity - scroll.velocity;
        velocityQuotient = velocityDiff / settings.maxVelocity;

        static int hold;
        static int release;
        static int interval;
        static int sum;

        if (settings.isVelocityMode) {
            hold = (int)(globals.settings.holdInterval * velocityQuotient);
            if (hold == 0) hold = 1;

            release = (int)(globals.settings.releaseInterval * velocityQuotient);
            if (release == 0) release = 1;
        }
        else {
            hold = globals.settings.holdInterval;
            release = globals.settings.releaseInterval;
        }

        if (state.isHolding) interval = hold;
        else interval = release;

        sum = hold + release;

        if (scroll.scrolled) {
            state.onScroll();
            scroll.scrolled = false;
        }

        state.phaseFrames++;
        if (settings.isStableScroll) state.elapsedFrames++;

        if (!state.isGateOpen) return GJBaseGameLayer::processCommands(p0);

        if (settings.isStableScroll) {
            if (state.elapsedFrames <= sum) {
                if (state.phaseFrames >= interval) {
                    state.isHolding = !state.isHolding;
                    state.phaseFrames = 0;
                }
                globals.press(state.isHolding);
            }
            else {
                globals.press(false);
                state.isGateOpen = false;
            }
            return;
        }

        if (state.phaseFrames < hold) globals.press(true);
        else if (state.phaseFrames >= hold) {
            globals.press(false);
            state.phaseFrames = 0;
            state.isGateOpen = false;
        }
    }
};