# !/bin/bash
cd bootloader/
make clean
make 
cd ..
# create img file
dd if=/dev/zero of=hdd.img bs=516096c count=1000
(echo n;echo p;echo 1;echo "";echo "";echo t;echo c;echo a;echo w;) | fdisk -u -C1000 -S63 -H16 hdd.img
#

# wirte bootloader to img file
dd if=bootloader/boot.bin of=hdd.img conv=notrunc
dd if=bootloader/loader.bin of=hdd.img seek=1 conv=notrunc

# build virtual block device with file
sudo /sbin/losetup -o1048576 /dev/loop100 hdd.img 

# build fat32 file system
sudo mkdosfs -v -F32 /dev/loop100

# destroy virutal block device
sudo losetup -d /dev/loop100

mv hdd.img tools/