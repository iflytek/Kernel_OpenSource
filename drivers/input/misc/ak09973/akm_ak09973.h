// Driver for AK09973
//
// SPDX-License-Identifier: GPL-2.0-only

#ifndef __AKM_AK09973_H__
#define __AKM_AK09973_H__

#define NV_NAME_LENGTH 8     /*NV name maximum length*/
#define NVE_NV_DATA_SIZE 104 /*NV data maximum length*/

struct nve_info_user {
	uint32_t nv_operation;
	uint32_t nv_number;
	char nv_name[NV_NAME_LENGTH];
	uint32_t valid_size;
	u_char nv_data[NVE_NV_DATA_SIZE];
};

#endif

