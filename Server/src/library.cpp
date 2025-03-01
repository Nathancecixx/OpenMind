#include "library.h"

#include <mutex>

static std::mutex printLock = std::mutex();

void library::print(const std::string& text) {
    std::lock_guard guard(printLock);
    std::cout << text;
}

