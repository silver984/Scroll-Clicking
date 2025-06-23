#include <Geode/Geode.hpp>
#include "functions.hpp"
#include "data.hpp"
#include "util.hpp"
#include <unordered_map>
#include <string>
#include <mutex>

using namespace geode::prelude;

bool hasClickedOnce;
bool isGateOpen;
bool isHolding;
int stateFrames;
int frames;

void press(bool clickVal) {
    static std::unordered_map<std::string, enumKeyCodes> keyMap = {
        { "A", enumKeyCodes::KEY_A },
        { "B", enumKeyCodes::KEY_B },
        { "C", enumKeyCodes::KEY_C },
        { "D", enumKeyCodes::KEY_D },
        { "E", enumKeyCodes::KEY_E },
        { "F", enumKeyCodes::KEY_F },
        { "G", enumKeyCodes::KEY_G },
        { "H", enumKeyCodes::KEY_H },
        { "I", enumKeyCodes::KEY_I },
        { "J", enumKeyCodes::KEY_J },
        { "K", enumKeyCodes::KEY_K },
        { "L", enumKeyCodes::KEY_L },
        { "M", enumKeyCodes::KEY_M },
        { "N", enumKeyCodes::KEY_N },
        { "O", enumKeyCodes::KEY_O },
        { "P", enumKeyCodes::KEY_P },
        { "Q", enumKeyCodes::KEY_Q },
        { "R", enumKeyCodes::KEY_R },
        { "S", enumKeyCodes::KEY_S },
        { "T", enumKeyCodes::KEY_T },
        { "U", enumKeyCodes::KEY_U },
        { "V", enumKeyCodes::KEY_V },
        { "W", enumKeyCodes::KEY_W },
        { "X", enumKeyCodes::KEY_X },
        { "Y", enumKeyCodes::KEY_Y },
        { "Z", enumKeyCodes::KEY_Z },
        { "UP", enumKeyCodes::KEY_Up },
        { "DOWN", enumKeyCodes::KEY_Down },
        { "LEFT", enumKeyCodes::KEY_Left },
        { "RIGHT", enumKeyCodes::KEY_Right },
        { "SPACE", enumKeyCodes::KEY_Space },
        { "ENTER", enumKeyCodes::KEY_Enter }
    };

    auto keyP1 = keyMap.find(keyStrP1);
    auto keyP2 = keyMap.find(keyStrP2);

    if (p1)
        CCKeyboardDispatcher::get()->dispatchKeyboardMSG(keyP1->second, clickVal, false);

    if (p2)
        CCKeyboardDispatcher::get()->dispatchKeyboardMSG(keyP2->second, clickVal, false);
}

void onScroll() {
    isGateOpen = true;
    isHolding = true;
    hasClickedOnce = false;
    stateFrames = 0;
    press(true);
    scrolled = false;
}

void resetState() {
    isGateOpen = false;
    isHolding = false;
    hasClickedOnce = false;
    stateFrames = 0;
    frames = 0;
}

void scrollFn() {
    if (scrolled)
        onScroll();

    if (isGateOpen) {
        stateFrames++;

        if (isHolding && stateFrames >= holdInterval) {
            press(false);
            isHolding = false;
            stateFrames = 0;
        }
        else if (!isHolding && stateFrames >= releaseInterval) {
            if (hasClickedOnce)
                isGateOpen = false;
            else {
                press(true);
                isHolding = true;
                stateFrames = 0;
                hasClickedOnce = true;
            }
        }
    }
}