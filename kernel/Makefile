
GCCPARAMS = -m64 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -std=c++17
ESTL_INCLUDE_PATH = -I ../estl/
KERNEL_INCLUDE_PATH = -I ./include/
src =  $(wildcard ./src/util/*.cpp)
src += $(wildcard ./src/io/*.cpp)
src += $(wildcard ./*.cpp)
src += $(wildcard ./src/core/*.cpp)

src_obj = main.o string.o veas.o interrupt.o physical_allocator.o
asm_obj = irqs.o head.o isrs.o syscall.o arch.o
all: system
	objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary system Eospp.bin
	

system:	head.o $(src_obj) $(asm_obj)
	ld -b elf64-x86-64 -o system head.o irqs.o isrs.o arch.o syscall.o $(src_obj) -T Eospp.lds --no-relax

$(src_obj): $(src)
	g++  $(src) ${GCCPARAMS} ${ESTL_INCLUDE_PATH} ${KERNEL_INCLUDE_PATH}  -c

irqs.o: src/core/irqs.s
	as --64 -o irqs.o src/core/irqs.s

isrs.o: src/core/isrs.s
	as --64 -o isrs.o src/core/isrs.s

syscall.o: src/core/syscall.s
	as --64 -o syscall.o src/core/syscall.s

arch.o: src/core/arch.s
	as --64 -o arch.o src/core/arch.s

head.o:	head.S
	gcc -E  head.S > head.s 
	as --64 -o head.o head.s
clean:
	rm -rf *.o *.s