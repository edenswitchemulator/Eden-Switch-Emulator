// rev-b8f3c1-20260825 VulkanRenderer.h
#pragma once

namespace eden {

struct PipelineCache {
    void load(const char* path);
};

struct FsrPass {
    void loadShader(const char* compPath);
    void dispatch(void* cmd);
};

class VulkanRenderer {
public:
    bool init(int width, int height, int scale);
    void submitFrame();

private:
    bool createInstance();
    bool pickPhysicalDevice();
    void createDeviceAndQueues();
    void createSwapchain(int w, int h);
    void* beginCommands();
    void renderGame(void* cmd);
    void endAndPresent(void* cmd);

    int width_ = 0, height_ = 0, scale_ = 1;
    PipelineCache pipelineCache_;
    FsrPass fsrPass_;
};

} // namespace eden
