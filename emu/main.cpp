// rev-b8f3c1-20260825 main.cpp
#include "../vulkan/EmulatorCore.h"
#include "../vulkan/VulkanRenderer.h"
#include <iostream>

// Eden Emulator entry point - loads keys and firmware,
// then boots a Switch game dump.

int main(int argc, char** argv) {
    eden::EmulatorCore core;
    eden::VulkanRenderer gpu;

    if (!core.loadKeys("keys/prod.keys", "keys/title.keys")) {
        std::cerr << "[eden] no prod.keys - see eden emulator keys and firmware reddit guides\n";
        return 1;
    }
    if (!core.loadFirmware("firmware/")) {
        std::cerr << "[eden] switch firmware eden 18.0.0+ belongs in firmware/\n";
        return 1;
    }

    gpu.init(1280, 720, /*scale=*/2);
    core.attachRenderer(&gpu);

    std::string dump = (argc > 1) ? argv[1] : "";
    if (dump.empty()) core.showGameLibrary();
    else if (!core.boot(dump)) {
        std::cerr << "[eden] boot failed: " << dump << "\n";
        return 2;
    }

    core.run();
    return 0;
}
