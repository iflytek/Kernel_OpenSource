
obj-y += sec_string.o
obj-y += sprd_verify.o
obj-y += efuse_operate.o
obj-y += sec_efuse_api.o
ifdef CONFIG_SC9833
obj-y += sec_efuse_sharkl2.o
endif
ifneq ($(CONFIG_SOC_IWHALE2)$(CONFIG_SPRD_SOC_SP9853I), )
obj-y += sec_iefuse.o
endif
