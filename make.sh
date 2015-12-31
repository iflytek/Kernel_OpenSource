if [ "$1" = "" ]
then
	echo "Pleast input command : make 'board_name'"
fi

if [ "$1" = "sp9830a-5h10_ga1" ]
then
export PATH=$PATH:$(pwd)/../prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin
export make CROSS_COMPILE=arm-eabi-
export KBUILD_OUTPUT=./out
export UBOOT_DEBUG_FLAG=-DDEBUG

make distclean
make sp9830a-5h10_ga1_defconfig
make -j4
fi

if [ "$1" = "sp9830a-7h10_ga1" ]
then
export PATH=$PATH:$(pwd)/../prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin
export make CROSS_COMPILE=arm-eabi-
export KBUILD_OUTPUT=./out
export UBOOT_DEBUG_FLAG=-DDEBUG

make distclean
make sp9830a-7h10_ga1_defconfig
make -j4
fi

if [ "$1" = "iwhale2" ]
then
export PATH=$PATH:$(pwd)/../prebuilts/gcc/linux-x86/x86/x86_64-linux-android-4.8/bin
export make CROSS_COMPILE=x86_64-linux-android-
export KBUILD_OUTPUT=./out
export UBOOT_DEBUG_FLAG=-DDEBUG

make distclean
make iwhale2_defconfig
make -j4
fi

if [ "$1" = "iwhale2_vp" ]
then
export PATH=$PATH:$(pwd)/../prebuilts/gcc/linux-x86/x86/x86_64-linux-android-4.8/bin
export make CROSS_COMPILE=x86_64-linux-android-
export KBUILD_OUTPUT=./out
export UBOOT_DEBUG_FLAG=-DDEBUG

make distclean
make iwhale2_vp_defconfig
make -j4
fi
