#pragma once

#include "PlayerStrategy.hpp"
#include <memory>
#include <string>
#include <dlfcn.h>
#include <stdexcept>
#include <iostream>

namespace sevens {

/**
 * Utility class for loading player strategies from shared libraries.
 */
class StrategyLoader {
public:
    static std::shared_ptr<PlayerStrategy> loadFromLibrary(const std::string& libraryPath) {
        void* handle = dlopen(libraryPath.c_str(), RTLD_LAZY);
        if (!handle) {
            throw std::runtime_error("Could not open library: " + std::string(dlerror()));
        }
        
        // Reset errors
        dlerror();
        
        // Look for the createStrategy symbol
        typedef PlayerStrategy* (*CreateStrategyFn)();
        CreateStrategyFn createStrategy = (CreateStrategyFn) dlsym(handle, "createStrategy");
        
        const char* dlsym_error = dlerror();
        if (dlsym_error) {
            dlclose(handle);
            throw std::runtime_error("Could not find createStrategy: " + std::string(dlsym_error));
        }
        
        // Create the strategy
        PlayerStrategy* strategy = createStrategy();
        if (!strategy) {
            dlclose(handle);
            throw std::runtime_error("createStrategy returned nullptr");
        }
        
        // Custom deleter to handle cleanup of both the strategy and the library handle
        return std::shared_ptr<PlayerStrategy>(strategy, [handle](PlayerStrategy* s) {
            delete s;
            dlclose(handle);
        });
    }
};

} // namespace sevens