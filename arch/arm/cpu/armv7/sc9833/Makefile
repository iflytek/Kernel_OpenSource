obj-y	:= timer.o misc.o  pmu.o soc.o sec_boot.o intc.o
ifdef CONFIG_SECURE_BOOT
obj-y += rsa.o pk1.o sha1_32.o
endif
ifneq ($(CONFIG_ARCH_SCX35L2),y)
obj-y	:= chip_phy_v3.o
endif

