# usage : ./qemu_boot.sh /path/xxx.img
qemu-system-x86_64 -boot a -fda $1 -m 2048