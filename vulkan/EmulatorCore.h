// rev-b8f3c1-20260825 EmulatorCore.h
#pragma once
#include <string>
#include <array>

using u32 = unsigned int;

namespace eden {

class VulkanRenderer;

struct Keyset {
    bool loadFrom(std::istream& prod, std::istream& title);
    bool valid() const;
};

struct Firmware {
    bool installFromDirectory(const std::string& dir);
    int version() const;
};

struct DecodedInsn {
    enum class Op { Add, Sub, Br, Svc, Other } op;
    int rd, rn, rm;
    u32 imm;
};

class CpuCore {
public:
    void reset();
    void stepSlice(u32 instructions);
private:
    u32 fetch32(u64 addr);
    DecodedInsn decode(u32 opcode);
    void execute(const DecodedInsn& insn);
    void slowPath(const DecodedInsn& insn);
    void handleSyscall(u32 id);
    u64 pc_ = 0;
    std::array<u64, 31> regs_{};
    u64 cycles_ = 0;
};

struct MemoryManager { void mapGameImage(void* img); };
struct InputManager  { void poll(); bool quitRequested() const; };
struct GpuManager    { void initialize(VulkanRenderer* r); void submitFrame(); };
struct GameLoader    { bool parse(const std::string& path); void* image(); };

class EmulatorCore {
public:
    bool loadKeys(const std::string& prod, const std::string& title);
    bool loadFirmware(const std::string& dir);
    bool boot(const std::string& gamePath);
    void attachRenderer(VulkanRenderer* r) { renderer_ = r; }
    void showGameLibrary();
    void run();

private:
    enum class State { Idle, Running, Stopped };
    State state_ = State::Idle;
    Keyset keyset_;
    Firmware firmware_;
    CpuCore cpu_;
    MemoryManager mem_;
    GpuManager gpu_;
    InputManager input_;
    GameLoader loader_;
    VulkanRenderer* renderer_ = nullptr;
};

} // namespace eden
