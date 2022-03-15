#include <core/interrupt.hpp>
#include <io/io.hpp>
#include <io/vesa.hpp>
namespace eospp::core {

void Interrupt::Init() {
    io::printf("init Interrupt\n");
    InstallIdt();
    InstallISrs();
    RemapIrqs();
    InstallIrqs();
    InstallSysCalls();
    Enable();
}

void Interrupt::InstallIdt() {
    idt_.LIMIT = size * sizeof(IDT_DSC) - 1;
    idt_.BASE  = reinterpret_cast<estd::uint64_t>(interrupts_.data());

    irqs_.fill({nullptr, nullptr});
    syscalls_.fill(nullptr);

    asm volatile("lidt %0"
                 :
                 : "m"(idt_));
}
void Interrupt::setGate(estd::size_t gate, Handler handler, estd::uint16_t selector, IDT_FLAGS flags) {
    estd::uintptr_t handler_addr    = reinterpret_cast<estd::uintptr_t>(handler);
    interrupts_[gate].SELECTOR      = selector;
    interrupts_[gate].FLAGS         = flags;
    interrupts_[gate].RESERVED      = 0;
    interrupts_[gate].ZERO          = 0;
    interrupts_[gate].LOW_OFFSET    = handler_addr & 0xFFFF;
    interrupts_[gate].MIDDLE_OFFSET = (handler_addr >> 16) & 0xFFFF;
    interrupts_[gate].HIGH_OFFSET   = handler_addr >> 32;
}

void Interrupt::InstallISrs() {
    setGate(0, _isr0, 8, {14, 0, 0, 1});
    setGate(1, _isr1, 8, {14, 0, 0, 1});
    setGate(2, _isr2, 8, {14, 0, 0, 1});
    setGate(3, _isr3, 8, {14, 0, 0, 1});
    setGate(4, _isr4, 8, {14, 0, 0, 1});
    setGate(5, _isr5, 8, {14, 0, 0, 1});
    setGate(6, _isr6, 8, {14, 0, 0, 1});
    setGate(7, _isr7, 8, {14, 0, 0, 1});
    setGate(8, _isr8, 8, {14, 0, 0, 1});
    setGate(9, _isr9, 8, {14, 0, 0, 1});
    setGate(10, _isr10, 8, {14, 0, 0, 1});
    setGate(11, _isr11, 8, {14, 0, 0, 1});
    setGate(12, _isr12, 8, {14, 0, 0, 1});
    setGate(13, _isr13, 8, {14, 0, 0, 1});
    setGate(14, _isr14, 8, {14, 0, 0, 1});
    setGate(15, _isr15, 8, {14, 0, 0, 1});
    setGate(16, _isr16, 8, {14, 0, 0, 1});
    setGate(17, _isr17, 8, {14, 0, 0, 1});
    setGate(18, _isr18, 8, {14, 0, 0, 1});
    setGate(19, _isr19, 8, {14, 0, 0, 1});
    setGate(20, _isr20, 8, {14, 0, 0, 1});
    setGate(21, _isr21, 8, {14, 0, 0, 1});
    setGate(22, _isr22, 8, {14, 0, 0, 1});
    setGate(23, _isr23, 8, {14, 0, 0, 1});
    setGate(24, _isr24, 8, {14, 0, 0, 1});
    setGate(25, _isr25, 8, {14, 0, 0, 1});
    setGate(26, _isr26, 8, {14, 0, 0, 1});
    setGate(27, _isr27, 8, {14, 0, 0, 1});
    setGate(28, _isr28, 8, {14, 0, 0, 1});
    setGate(29, _isr29, 8, {14, 0, 0, 1});
    setGate(30, _isr30, 8, {14, 0, 0, 1});
    setGate(31, _isr31, 8, {14, 0, 0, 1});
}

void Interrupt::InstallIrqs() {
    setGate(32, _irq0, 8, {14, 0, 0, 1});
    setGate(33, _irq1, 8, {14, 0, 0, 1});
    setGate(34, _irq2, 8, {14, 0, 0, 1});
    setGate(35, _irq3, 8, {14, 0, 0, 1});
    setGate(36, _irq4, 8, {14, 0, 0, 1});
    setGate(37, _irq5, 8, {14, 0, 0, 1});
    setGate(38, _irq6, 8, {14, 0, 0, 1});
    setGate(39, _irq7, 8, {14, 0, 0, 1});
    setGate(40, _irq8, 8, {14, 0, 0, 1});
    setGate(41, _irq9, 8, {14, 0, 0, 1});
    setGate(42, _irq10, 8, {14, 0, 0, 1});
    setGate(43, _irq11, 8, {14, 0, 0, 1});
    setGate(44, _irq12, 8, {14, 0, 0, 1});
    setGate(45, _irq13, 8, {14, 0, 0, 1});
    setGate(46, _irq14, 8, {14, 0, 0, 1});
    setGate(47, _irq15, 8, {14, 0, 0, 1});
}

void Interrupt::InstallSysCalls() {
    setGate(50, _syscall0, 8, {14, 0, 3, 1});
    setGate(51, _syscall1, 8, {14, 0, 3, 1});
    setGate(52, _syscall2, 8, {14, 0, 3, 1});
    setGate(53, _syscall3, 8, {14, 0, 3, 1});
    setGate(54, _syscall4, 8, {14, 0, 3, 1});
    setGate(55, _syscall5, 8, {14, 0, 3, 1});
    setGate(56, _syscall6, 8, {14, 0, 3, 1});
    setGate(57, _syscall7, 8, {14, 0, 3, 1});
    setGate(58, _syscall8, 8, {14, 0, 3, 1});
    setGate(59, _syscall9, 8, {14, 0, 3, 1});
}

void Interrupt::RemapIrqs() {
    //Restart the both PICs
    io::out_byte(0x20, 0x11);
    io::out_byte(0xA0, 0x11);

    io::out_byte(0x21, 0x20); //Make PIC1 start at 32
    io::out_byte(0xA1, 0x28); //Make PIC2 start at 40

    //Setup cascading for both PICs
    io::out_byte(0x21, 0x04);
    io::out_byte(0xA1, 0x02);

    //8086 mode for both PICs
    io::out_byte(0x21, 0x01);
    io::out_byte(0xA1, 0x01);

    //Activate all IRQs in both PICs
    io::out_byte(0x21, 0x00);
    io::out_byte(0xA1, 0x00);
}

bool Interrupt::RegisterIrqHandler(estd::size_t irq, IrqHandler handler, void* data) {
    if (irq >= irqs_.size() || estd::get<0>(irqs_[irq])) {
        return false;
    }

    irqs_[irq] = {handler, data};

    io::printf("register %ud irq\n", irq);

    return true;
}
bool Interrupt::RegisterSysCall(estd::size_t syscall, SysCallHandler handler) {
    if (syscall >= syscalls_.size() || syscalls_[syscall]) {
        return false;
    }

    syscalls_[syscall] = handler;

    io::printf("register %ud syscall\n", syscall);

    return true;
}

bool Interrupt::UnRegisterIrqHandler(estd::size_t irq, IrqHandler handler) {
    if (irq >= irqs_.size() || estd::get<0>(irqs_[irq]) != handler) {
        io::printf("failed to unregister irq idx = %ud\n", irq);
        return false;
    }
    irqs_[irq] = {nullptr, nullptr};

    return true;
}

bool Interrupt::UnRegisterSysCall(estd::size_t syscall, SysCallHandler handler) {
    if (syscall >= syscalls_.size() || syscalls_[syscall] != handler) {
        io::printf("failed to unregister syscall idx = %ud\n", syscall);
        return false;
    }

    syscalls_[syscall] = nullptr;
    return true;
}
void Interrupt::Enable() {
    asm volatile(
        "sti" ::);
}
void Interrupt::Disable(){
    asm volatile("cli" : : );
}
void Interrupt::TriggerIrq(syscall_regs* regs) {
    auto&& handler = estd::get<0>(irqs_[regs->code]);
    auto&& data    = estd::get<1>(irqs_[regs->code]);
    io::printf("regs->code = %ul\n",regs->code);
    io::printf("regs->rip = %xl\n",regs->rip);
    if (handler) {
        handler(regs, data);
    }
}

void Interrupt::TriggerSyscall(syscall_regs* regs) {
    if (syscalls_[regs->code]) {
        syscalls_[regs->code](regs);
    }
}

extern "C" {

const char* exceptions[32]{
    "Division by zero",
    "Debugger",
    "NMI",
    "Breakpoint",
    "Overflow",
    "Bounds",
    "Invalid Opcode",
    "Coprocessor not available",
    "Double fault",
    "Coprocessor segment overrun",
    "Invalid Task State Segment",
    "Segment not present",
    "Stack Fault",
    "General protection fault",
    "Page Fault",
    "Reserved",
    "Math Fault",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Point Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"};

void _fault_handler(fault_regs regs) {
    io::printf("Exception %ul %s occured\n", regs.error_no, exceptions[regs.error_no]);
    io::printf("error_code = %ul rip = %xl rflags = %ul cs = %ul rsp = %xl ss = %ul\n",
               regs.error_code, regs.rip, regs.rflags, regs.cs, regs.rsp, regs.ss);
               while(1){}
   
}

void _irq_handler(syscall_regs* regs) {
    //If the IRQ is on the slave controller, send EOI to it
    if (regs->code >= 8) {
        io::out_byte(0xA0, 0x20);
    }
    //Send EOI to the master controller
    io::out_byte(0x20, 0x20);

    Interrupt::TriggerIrq(regs);

}

void _syscall_handler(syscall_regs* regs) {
    Interrupt::TriggerSyscall(regs);
}

} //end of extern "C"

} // namespace eospp::core
