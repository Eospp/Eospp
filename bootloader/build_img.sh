# !/bin/bash

bximage
echo "write boot.bin to boot.img"
dd if=boot.bin of=boot.img bs=512 count=1 conv=notrunc

if [ ! -d "/bootloader" ]
then
echo "mkdir /bootloader"
mkdir /bootloader
fi

echo "mount boot.img"
mount boot.img /bootloader -t vfat -o loop
echo "write loader.bin to boot.img"
cp loader.bin /bootloader
sync
echo "umount boot.img"
umount /bootloader 