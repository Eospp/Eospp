# !/bin/bash

bximage

dd if=boot.bin of=boot.img bs=512 count=1 conv=notrunc

if [ ! -d "/bootloader" ]
then
mkdir /bootloader
fi

mount boot.img /bootloader -t vfat -o loop
cp loader.bin /bootloader
sync
umount /bootloader 