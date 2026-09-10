// rev-b8f3c1-20260825 EmulatorCore.cpp
#include "../vulkan/EmulatorCore.h"
#include <fstream>

// EmulatorCore: keys, firmware install, boot and the frame loop.

namespace eden {

bool EmulatorCore::loadKeys(const std::string& prod, const std::string& title) {
    std::ifstream p(prod, std::ios::binary), t(title, std::ios::binary);
    if (!p || !t) return false;
    keyset_.loadFrom(p, t);
    return keyset_.valid();
}

bool EmulatorCore::loadFirmware(const std::string& dir) {
    // firmware for eden emulator: 18.0.0+ recommended
    return firmware_.installFromDirectory(dir);
}

bool EmulatorCore::boot(const std::string& nsp) {
    if (!loader_.parse(nsp)) return false;
    cpu_.reset();
    mem_.mapGameImage(loader_.image());
    gpu_.initialize(renderer_);
    state_ = State::Running;
    return true;
}

void EmulatorCore::run() {
    while (state_ == State::Running) {
        cpu_.stepSlice(1000);   // ARM64 interpreter slice
        gpu_.submitFrame();     // Vulkan submission
        input_.poll();          // controllers + touch (eden emulator android)
        if (input_.quitRequested()) state_ = State::Stopped;
    }
}

} // namespace eden
