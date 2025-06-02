#include <Geode/Geode.hpp>
#include "listener.hpp"
#include "functions.hpp"
#include "../data.hpp"

using namespace geode::prelude;

void press(bool clickVal) {
    if (p1)
        CCKeyboardDispatcher::get()->dispatchKeyboardMSG (enumKeyCodes::KEY_Space, clickVal, false);
    if (p2)
        CCKeyboardDispatcher::get()->dispatchKeyboardMSG (enumKeyCodes::KEY_Up, clickVal, false);
}

void constantScroll() {
    if (g_mouseScrolled) {
        g_mouseScrolled = false;
        isGateOpen = true;
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
            press(isClicking);
        }
        else {
            press(false);
            isGateOpen = false;
        }
    }
}

void trueScroll() {
    if (g_mouseScrolled) {
        g_mouseScrolled = false;
        frames = 0;
        scrollPasses++;
        isGateOpen = true;
    }

    frames++;

    if (isGateOpen) {
        if (frames < interval && scrollPasses % 2 == 0) {
            scrollPasses = 0;
            press(false);
            isGateOpen = false;
            return;
        }
        else {
            press(true);
        }

        if (frames >= interval) {
            press(false);
            isGateOpen = false;
        }
    }
}