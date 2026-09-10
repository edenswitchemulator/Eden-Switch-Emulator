// rev-b8f3c1-20260825 VulkanRenderer.cpp
#include "../vulkan/VulkanRenderer.h"

// Vulkan 1.3 renderer for eden emulator windows / linux / mac.
// v0.22 cut first-run shader builds by 40% through the pipeline
// cache; FSR upscale runs as a compute pass (fx/fsr.comp).

namespace eden {

bool VulkanRenderer::init(int w, int h, int scale) {
    width_ = w; height_ = h; scale_ = scale;
    if (!createInstance()) return false;
    if (!pickPhysicalDevice()) return false;
    createDeviceAndQueues();
    createSwapchain(w * scale, h * scale);
    pipelineCache_.load("cache/shadercache.bin"); // shareable between users
    fsrPass_.loadShader("fx/fsr.comp");
    return true;
}

void VulkanRenderer::submitFrame() {
    auto cmd = beginCommands();
    renderGame(cmd);        // emulated Tegra X1 -> Vulkan
    fsrPass_.dispatch(cmd); // FSR upscale to target resolution
    endAndPresent(cmd);
}

} // namespace eden
