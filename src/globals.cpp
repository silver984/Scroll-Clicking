#include <geode.custom-keybinds/include/Keybinds.hpp>
#include <Geode/Geode.hpp>
#include "globals.hpp"

using namespace geode::prelude;
using namespace keybinds;

void Globals::press(bool clickVal) {
    static auto& settings = globals.settings;
    if (settings.affectP1)
        InvokeBindEvent("robtop.geometry-dash/jump-p1", clickVal).post();
    if (settings.affectP2)
        InvokeBindEvent("robtop.geometry-dash/jump-p2", clickVal).post();
}