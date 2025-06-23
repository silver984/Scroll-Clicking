#include <Geode/Geode.hpp>
#include "util.hpp"

using namespace geode::prelude;

template <typename T>
T setting(std::string name) {
    static std::unordered_map<std::string, T> cache;
    static std::mutex mutex;

    {
        std::lock_guard lock(mutex);
        if (!cache.contains(name)) {
            T val = Mod::get()->getSettingValue<T>(name);
            cache[name] = val;

            listenForSettingChangesV3<T>(name, [name](T newVal) {
                std::lock_guard lock(mutex);
                cache[name] = newVal;
                });
        }
        return cache[name];
    }
}
template bool setting<bool>(std::string);
template int setting<int>(std::string);
template std::string setting<std::string>(std::string);