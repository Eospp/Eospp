
GCCPARAMS = -m64 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -std=c++17
all: system
	objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary system Eospp.bin

system:	head.o main.o 
	ld -b elf64-x86-64 -o system head.o main.o -T Eospp.lds 

main.o:	main.cpp
	gcc  ${GCCPARAMS} -c main.cpp

head.o:	head.S
	gcc -E  head.S > head.s 
	as --64 -o head.o head.s
clean:
	rm -rf *.o *.s