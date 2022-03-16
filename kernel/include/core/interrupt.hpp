#pragma once
#include <array.hpp>
#include <type.hpp>
#include <tuple.hpp>
#include <core/arch.hpp>
namespace eospp::core {

struct IDT_FLAGS {
    estd::uint8_t TYPE    : 4;
    estd::uint8_t ZERO    : 1;
    estd::uint8_t DPL     : 2;
    estd::uint8_t PRESENT : 1;
} __attribute__((packed));

static_assert(sizeof(IDT_FLAGS) == 1, "sizeof IDT_FLAGS != 1 byte");

struct IDT_DSC {
    estd::uint16_t LOW_OFFSET;
    estd::uint16_t SELECTOR;
    estd::uint8_t  ZERO;
    IDT_FLAGS      FLAGS;
    estd::uint16_t MIDDLE_OFFSET;
    estd::uint32_t HIGH_OFFSET;
    estd::uint32_t RESERVED;
} __attribute__((packed));

static_assert(sizeof(IDT_DSC) == 16, "sizeof IDT_DSC != 16 byte");

struct IDT {
    estd::uint16_t LIMIT;
    estd::uint64_t BASE;
} __attribute__((packed));

static_assert(sizeof(IDT) == 10, "sizeof IDT != 10 byte");

using sse_128 = double __attribute__((vector_size(16)));

static_assert(sizeof(sse_128) == 16, "xmm registers are 16 bytes long");

struct fault_regs {
    estd::uint64_t rbp;
    estd::uint64_t error_no;
    estd::uint64_t error_code;
    estd::uint64_t rip;
    estd::uint64_t cs;
    estd::uint64_t rflags;
    estd::uint64_t rsp;
    estd::uint64_t ss;
} __attribute__((packed));

struct syscall_regs {
    estd::uint64_t rax;
    estd::uint64_t rbx;
    estd::uint64_t rcx;
    estd::uint64_t rdx;
    estd::uint64_t rsi;
    estd::uint64_t rdi;
    estd::uint64_t r8;
    estd::uint64_t r9;
    estd::uint64_t r10;
    estd::uint64_t r11;
    estd::uint64_t r12;
    estd::uint64_t r13;
    estd::uint64_t r14;
    estd::uint64_t r15;
    estd::uint64_t rbp;
    estd::uint64_t code;
    estd::uint64_t rip;
    estd::uint64_t cs;
    estd::uint64_t rflags;
    estd::uint64_t rsp;
    estd::uint64_t ss;
} __attribute__((packed)); 

using IrqHandler     =  void(*)(syscall_regs*,void*);
using SysCallHandler =  void(*)(syscall_regs*);


class Interrupt {
private:
    using  Handler        = void(*)();
    using  IrqHandleEntry = estd::tuple<IrqHandler,void*>;
public:
    static void Init();

    static void TriggerIrq(syscall_regs *regs);

    static void TriggerSyscall(syscall_regs *regs);

    static void Enable();

    static void Disable();
private:
    static void InstallIdt();
    
    static void InstallISrs();
    
    static void RemapIrqs();
    
    static void InstallIrqs();
    
    static void InstallSysCalls();

    static void setGate(estd::size_t gate,Handler handler,estd::uint16_t selector,IDT_FLAGS flags);

    static bool RegisterIrqHandler(estd::size_t irq,IrqHandler handler,void *data);

    static bool RegisterSysCall(estd::size_t syscall,SysCallHandler handler);
    
    static bool UnRegisterIrqHandler(estd::size_t irq,IrqHandler handler);

    static bool UnRegisterSysCall(estd::size_t syscall,SysCallHandler handler);
private:
    static constexpr estd::size_t                               size{256};
    static constexpr estd::size_t                               syscall_count{10};
    static constexpr estd::size_t                               irq_count{16};
    static inline    estd::array<IDT_DSC, size>                 interrupts_;
    static inline    estd::array<SysCallHandler,syscall_count>  syscalls_;
    static inline    estd::array<IrqHandleEntry,irq_count>      irqs_;
    static inline    IDT                                        idt_;
};

class IrqGuard{
public:
    IrqGuard(){
        Arch::DisableHwint(rflags);
    }

    ~IrqGuard(){
        Arch::EnableHwint(rflags);
    }
private:
    estd::size_t rflags;
};


} // namespace eospp::core

extern "C" {

    void _isr0();
    void _isr1();
    void _isr2();
    void _isr3();
    void _isr4();
    void _isr5();
    void _isr6();
    void _isr7();
    void _isr8();
    void _isr9();
    void _isr10();
    void _isr11();
    void _isr12();
    void _isr13();
    void _isr14();
    void _isr15();
    void _isr16();
    void _isr17();
    void _isr18();
    void _isr19();
    void _isr20();
    void _isr21();
    void _isr22();
    void _isr23();
    void _isr24();
    void _isr25();
    void _isr26();
    void _isr27();
    void _isr28();
    void _isr29();
    void _isr30();
    void _isr31();

    void _irq0();
    void _irq1();
    void _irq2();
    void _irq3();
    void _irq4();
    void _irq5();
    void _irq6();
    void _irq7();
    void _irq8();
    void _irq9();
    void _irq10();
    void _irq11();
    void _irq12();
    void _irq13();
    void _irq14();
    void _irq15();

    void _syscall0();
    void _syscall1();
    void _syscall2();
    void _syscall3();
    void _syscall4();
    void _syscall5();
    void _syscall6();
    void _syscall7();
    void _syscall8();
    void _syscall9();
} 
