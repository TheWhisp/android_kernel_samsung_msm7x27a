#!/bin/sh

mkdir output
make trebon05_defconfig
schedtool -B -n 1 -e ionice -n 1 make -j `cat /proc/cpuinfo | grep "^processor" | wc -l`
make >&1 | tee ./output/log.txt
mv ./arch/arm/boot/zImage ./output
find ./ -name "*.ko" -exec mv {} ./output \;
rm output/log.txt
echo "done, please check your output folder"
