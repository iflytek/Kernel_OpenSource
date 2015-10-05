#include <config.h>

.section .text

.globl car_init

#define SRAM_START 0xFFFB0000

#define IA32_MTRRCAP 0x00FE
#define IA32_MTRR_DEF_TYPE 0x02FF
#define IA32_MTRR_PHYSBASE0 0x0200

car_init:

/* disable cache */

	movl	%cr0, %eax
	orl	$0x60000000, %eax
	movl	%eax, %cr0
	wbinvd

/* disable MTRR */

	xorl	%eax, %eax
	xorl	%edx, %edx
	movl	$IA32_MTRR_DEF_TYPE, %ecx
	wrmsr

/* get the physical address mask [63:32] */

	xorl	%edi, %edi
	movl	$0x80000000, %eax
	cpuid
	cmpl	$0x80000008, %eax
	jb	no_address_info
	movl	$0x80000008, %eax
	cpuid
	subb	$32, %al
	jb	no_address_info
	movb	%al, %cl
	movl	$1, %edi
	shll	%cl, %edi
	decl	%edi
no_address_info:
	movl	%edi, %ebx

/* get the number of MTRR */

	movl	$IA32_MTRRCAP, %ecx
	rdmsr
	andl	$0xFF, %eax
	movl	%eax, %edi

	movl	$IA32_MTRR_PHYSBASE0, %ecx
	xorl	%esi, %esi

/* Variable Range MTRR #0: FFFF0000-FFFFFFFF, WP */

	movl	$0, %edx
	movl	$0xFFFF0005, %eax
	wrmsr
	incl	%ecx

	movl	%ebx, %edx
	movl	$0xFFFF0800, %eax
	wrmsr
	incl	%ecx

	incl	%esi


/* Variable Range MTRR #1: xxxx0000-xxxxFFFF, WP (SRAM) */

	movl	$0, %edx
	movl	$(SRAM_START | 0x0005), %eax
	wrmsr
	incl	%ecx

	movl	%ebx, %edx
	movl	$0xFFFF0800, %eax
	wrmsr
	incl	%ecx

	incl	%esi

/* Variable Range MTRR #2: 00000000-7FFFFFFF, WB */

	movl	$0, %edx
	movl	$0x00000006, %eax
	wrmsr
	incl	%ecx

	movl	%ebx, %edx
	movl	$0x80000800, %eax
	wrmsr
	incl	%ecx

	incl	%esi

/* disable the rest of Variable Range MTRR */

	xorl	%eax, %eax
	xorl	%edx, %edx

_mtrr_next:
	cmpl	%edi, %esi
	jnb	_mtrr_enable

	wrmsr
	incl	%ecx
	wrmsr
	incl	%ecx

	incl	%esi
	jmp	_mtrr_next

_mtrr_enable:
	movl	$IA32_MTRR_DEF_TYPE, %ecx
	rdmsr
	orl	$(1 << 11), %eax
	wrmsr

/* clear CD and NW */

	movl	%cr0, %eax
	andl	$0x9FFFFFFF, %eax
	movl	%eax, %cr0

        jmp     car_init_ret
