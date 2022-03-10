
GCCPARAMS = -m64 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -std=c++17
ESTL_INCLUDE_PATH = -I ../estl/
KERNEL_INCLUDE_PATH = -I ./include/
src =  $(wildcard ./src/util/*.cpp)
src += $(wildcard ./src/io/*.cpp)
src += $(wildcard ./*.cpp)

obj = main.o string.o veas.o

all: system
	objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary system Eospp.bin
	

system:	head.o $(obj)
	ld -b elf64-x86-64 -o system head.o $(obj) -T Eospp.lds 

$(obj): $(src)
	g++  $(src) ${GCCPARAMS} ${ESTL_INCLUDE_PATH} ${KERNEL_INCLUDE_PATH}  -c


head.o:	head.S
	gcc -E  head.S > head.s 
	as --64 -o head.o head.s
clean:
	rm -rf *.o *.s