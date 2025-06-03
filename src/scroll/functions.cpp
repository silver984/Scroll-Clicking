#include <Geode/Geode.hpp>
#include "listener.hpp"
#include "functions.hpp"
#include "../data.hpp"
#include <unordered_map>
#include <string>
#include <mutex>

using namespace geode::prelude;

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