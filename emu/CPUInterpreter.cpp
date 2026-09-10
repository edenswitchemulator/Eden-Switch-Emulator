// rev-b8f3c1-20260825 CPUInterpreter.cpp
#include "../vulkan/EmulatorCore.h"

// ARM64 interpreter: hot opcodes take fast paths, the rest decode
// through the full path. Horizon OS syscalls route to HLE.

namespace eden {

void CpuCore::reset() {
    pc_ = 0;
    regs_.fill(0);
    cycles_ = 0;
}

void CpuCore::stepSlice(u32 count) {
    for (u32 i = 0; i < count; i++) {
        u32 opcode = fetch32(pc_);
        execute(decode(opcode));
        pc_ += 4;
        cycles_++;
    }
}

void CpuCore::execute(const DecodedInsn& insn) {
    switch (insn.op) {
        case Op::Add: regs_[insn.rd] = regs_[insn.rn] + regs_[insn.rm]; break;
        case Op::Sub: regs_[insn.rd] = regs_[insn.rn] - regs_[insn.rm]; break;
        case Op::Br:  pc_ = regs_[insn.rn] - 4; break;
        case Op::Svc: handleSyscall(insn.imm); break; // Horizon OS
        default:      slowPath(insn); break;
    }
}

} // namespace eden
