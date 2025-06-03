#include <string>
#include "data.hpp"

bool isConflicted = false;
bool toggle = true;
bool isTrueScroll = false;

bool isGateOpen = false;
bool isClicking = true;

int interval = 0;
int cachedInterval = 0;
int intervalFrames = 0;
int scrollPasses = 0;
int frames = 0;

bool p1 = true;
bool p2 = true;
std::string keyStrP1 = "";
std::string keyStrP2 = "";