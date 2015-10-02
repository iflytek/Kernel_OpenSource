/*
 * Copyright (C) 2015 Intel, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 *
 * Based on code from coreboot
 */
#define DEBUG

#include <common.h>
#include <cpu.h>
#include <dm.h>
#include <asm/cpu.h>
#include <asm/lapic.h>
#include <asm/mp.h>
#include <asm/msr.h>
#include <asm/bootparam.h>

#ifdef CONFIG_SMP
static int enable_smis(struct udevice *cpu, void *unused)
{
	return 0;
}

static struct mp_flight_record mp_steps[] = {
	MP_FR_BLOCK_APS(mp_init_cpu, NULL, mp_init_cpu, NULL),
	/* Wait for APs to finish initialization before proceeding. */
	MP_FR_BLOCK_APS(NULL, NULL, enable_smis, NULL),
};

static int detect_num_cpus(void)
{
	int ecx = 0;

	/*
	 * Use the algorithm described in Intel 64 and IA-32 Architectures
	 * Software Developer's Manual Volume 3 (3A, 3B & 3C): System
	 * Programming Guide, Jan-2015. Section 8.9.2: Hierarchical Mapping
	 * of CPUID Extended Topology Leaf.

	 */
	while (1) {
		struct cpuid_result leaf_b;

		leaf_b = cpuid_ext(0xb, ecx);

		/*
		 * iSharkL doesn't have hyperthreading so just determine the
		 * number of cores by from level type (ecx[15:8] == * 2)
		 */
		if ((leaf_b.ecx & 0xff00) == 0x0200)
			return leaf_b.ebx & 0xffff;
		ecx++;
	}
}

static int isharkl_init_cpus(void)
{
	struct mp_params mp_params;

	lapic_setup();

	mp_params.num_cpus = detect_num_cpus();
	debug("num of cpus detected %d\n", mp_params.num_cpus);

	mp_params.parallel_microcode_load = 0,
		mp_params.flight_plan = &mp_steps[0];
	mp_params.num_records = ARRAY_SIZE(mp_steps);
	mp_params.microcode_pointer = 0;

	if (mp_init(&mp_params)) {
		printf("Warning: MP init failure\n");
		return -EIO;
	}

	return 0;
}
#endif

int x86_init_cpus(void)
{
#ifdef CONFIG_SMP
	debug("Init additional CPUs class %d\n", UCLASS_CPU);
	isharkl_init_cpus();
#endif
	return 0;
}

static int cpu_x86_isharkl_probe(struct udevice *dev)
{
        debug("Init Buillhill core\n");

	gd->arch.hardware_subarch = X86_SUBARCH_SHARKL;

	return 0;
}

static unsigned bus_freq(void)
{
	msr_t clk_info = msr_read(MSR_BSEL_CR_OVERCLOCK_CONTROL);

	switch (clk_info.lo & 0x3) {
	case 0:
		return 266600000;
	case 1:
		return 133300000;
	case 2:
		return 200000000;
	case 3:
		return 166666666;
	default:
		return 0;
	}
}

static unsigned long tsc_freq(void)
{
	msr_t platform_info;
	ulong bclk = bus_freq();

	if (!bclk)
		return 0;

	platform_info = msr_read(MSR_PLATFORM_INFO);

	return bclk * ((platform_info.lo >> 8) & 0xff);
}

static int isharkl_get_info(struct udevice *dev, struct cpu_info *info)
{
        info->cpu_freq = tsc_freq();
        info->features = 1 << CPU_FEAT_L1_CACHE | 1 << CPU_FEAT_MMU;

        return 0;
}

static int cpu_x86_isharkl_bind(struct udevice *dev)
{
        struct cpu_platdata *plat = dev_get_parent_platdata(dev);

        plat->cpu_id = fdtdec_get_int(gd->fdt_blob, dev->of_offset,
                                      "intel,apic-id", -1);

        return 0;
}

static const struct cpu_ops cpu_x86_isharkl_ops = {
        .get_desc       = x86_cpu_get_desc,
        .get_info       = isharkl_get_info,
};

static const struct udevice_id cpu_x86_isharkl_ids[] = {
        { .compatible = "intel,bullhill" },
        { }
};

U_BOOT_DRIVER(cpu_x86_isharkl_drv) = {
        .name           = "cpu_x86_isharkl",
        .id             = UCLASS_CPU,
        .of_match       = cpu_x86_isharkl_ids,
        .bind           = cpu_x86_isharkl_bind,
        .probe          = cpu_x86_isharkl_probe,
        .ops            = &cpu_x86_isharkl_ops,
};
