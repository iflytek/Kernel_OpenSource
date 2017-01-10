dtb-y += chromebook_link.dtb \
	chromebox_panther.dtb \
	crownbay.dtb \
	galileo.dtb \
	minnowmax.dtb \
	qemu-x86_i440fx.dtb \
	qemu-x86_q35.dtb \
	isharkl.dtb \
	sp9861e_1h10.dtb \
	sp9861e_1h10_vmm.dtb \
	sp9861-fpga-douchannel.dtb \
	sp9861-fpga-sinchannel.dtb \
	sp9861-fpga-vmm.dtb \
	sp9861-vdk.dtb \
	sp9861-vdk-4g-interleave.dtb \
	sp9861-vdk-4g-noninterleave.dtb \
	sp9861-vdk-4g-single.dtb \
	sp9861-vdk-vmm.dtb \
	sp9853i-fpga.dtb \
	sp9853i-vdk.dtb \
	sp9853i-vdk-4g.dtb \
	sp9853i-vmm.dtb \
	sp9853i_1h10.dtb

targets += $(dtb-y)

DTC_FLAGS += -R 4 -p 0x1000

PHONY += dtbs
dtbs: $(addprefix $(obj)/, $(dtb-y))
	@:

clean-files := *.dtb
