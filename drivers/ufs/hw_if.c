/* ==========================================================================
 * The Synopsys DWC UFS Software Driver and documentation (hereinafter 
 * "Software") is an unsupported proprietary work of Synopsys, Inc. unless
 * otherwise expressly agreed to in writing between Synopsys and you.
 *
 * The Software IS NOT an item of Licensed Software or Licensed Product under
 * any End User Software License Agreement or Agreement for Licensed Product
 * with Synopsys or any supplement thereto.  Permission is hereby granted,
 * free of charge, to any person obtaining a copy of this software annotated
 * with this license and the Software, to deal in the Software without 
 * restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject 
 * to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software. 
 * 
 * THIS SOFTWARE IS BEING DISTRIBUTED BY SYNOPSYS SOLELY ON AN "AS IS" BASIS
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE HEREBY DISCLAIMED. IN NO EVENT SHALL SYNOPSYS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * ========================================================================== */
#include <common.h>
#include"sprd_ufs.h"
#include "hw_if.h"

extern uint64_t DescMemBase;


struct hwif_info
{
	uint32_t id;
	void *va;
	short used;
}info_list[MAX_ALLOCATIONS];

/* Byte swap u16*/
uint16_t swap_16(uint16_t val) 
{
	return (uint16_t)((val << 8) | (val >> 8));
}

/* Byte swap unsigned int */
uint32_t swap_32(uint32_t val)
{

	val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF); 
	return (val << 16) | (val >> 16);
}

uint32_t to_bigendian32(uint32_t val)
{
#ifdef HOST_BIG_ENDIAN
	return val;
#else
	return swap_32(val);
#endif
}

uint32_t to_littleendian32(uint32_t val)
{
#ifdef HOST_BIG_ENDIAN
	return swap_32(val);
#else
	return val;
#endif
}

uint16_t to_bigendian16(uint16_t val)
{
#ifdef HOST_BIG_ENDIAN
	return val;
#else
	return swap_16(val);
#endif
}

uint16_t to_littleendian16(uint16_t val)
{
#ifdef HOST_BIG_ENDIAN
	return swap_16(val);
#else
	return val;
#endif
}


uint32_t dwc_ufs_read_reg(uint32_t reg_offset)
{

	uint32_t value;

#ifdef HOST_32BIT
	value = (*(( volatile uint32_t *)(REG_BASE_ADDRESS + reg_offset)));
#endif
#ifdef HOST_64BIT
	value = (*(( volatile uint32_t *)((uint64_t)(REG_BASE_ADDRESS + reg_offset))));
#endif
	return value;
}

void dwc_ufs_write_reg(uint32_t reg_offset, uint32_t value)
{
#ifdef HOST_32BIT
	(*(( volatile uint32_t *)(REG_BASE_ADDRESS + reg_offset))) = value;
#endif
#ifdef HOST_64BIT
	(*((volatile uint32_t *)((uint64_t)(REG_BASE_ADDRESS + reg_offset)))) = value;
#endif
}

