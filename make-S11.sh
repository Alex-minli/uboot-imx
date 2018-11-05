#!/bin/bash
# Add S11 config
# minli-port-181011

clear
export ARCH=arm
export CROSS_COMPILE=~/S700/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.9/bin/arm-linux-androideabi-

make distclean
make mx6qp_S11_android_config
#make mx6qsabresdandroid_config

make | tee u-boot.log
