dtb-y += chromebook_link.dtb \
	chromebox_panther.dtb \
	crownbay.dtb \
	galileo.dtb \
	minnowmax.dtb \
	qemu-x86_i440fx.dtb \
	qemu-x86_q35.dtb \
	isharkl.dtb \
	iwhale2-fpga-douchannel.dtb \
	iwhale2-fpga-sinchannel.dtb \
	iwhale2-fpga-vmm.dtb \
	iwhale2-vdk.dtb \
	iwhale2-vdk-4g-interleave.dtb \
	iwhale2-vdk-4g-noninterleave.dtb \
	iwhale2-vdk-4g-single.dtb \
	iwhale2-vdk-vmm.dtb \
	isharkl2-fpga.dtb \
	isharkl2-vdk.dtb \
	isharkl2-vdk-4g.dtb

targets += $(dtb-y)

DTC_FLAGS += -R 4 -p 0x1000

PHONY += dtbs
dtbs: $(addprefix $(obj)/, $(dtb-y))
	@:

clean-files := *.dtb
