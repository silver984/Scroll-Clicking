#pragma once

template <typename T>
T setting(std::string name);

#define RETURN_IF_MOD_DISABLED(val) \
    if (!toggle) return val;