#include <common.h>

#include "uboot_sec_drv.h"
#include "mv_gal.h"
#include "mv_svc_hypercalls.h"


static int init_flag = 0;

int uboot_verify_img(uint64_t start_addr,uint64_t lenth)
{
	struct mv_shared_data *shared_data;
	unsigned int *ptr;
	unsigned int result = 0;
	imgToVerifyInfo *info = (imgToVerifyInfo *)&start_addr;

	if (!init_flag) {
		printf("init flag=%d\n", init_flag);
		mv_init();
		init_flag = 1;
	}

	printf("image addr=%p, size=%d\n", info->img_addr, info->img_len);
	printf("pubkey hash addr=%p, size=%d\n", info->pubkeyhash, info->hash_len);

	puts("## Start verify image ...\n");

	shared_data = mv_gal_get_shared_data();
	ptr = (unsigned int *)shared_data->pal_shared_mem_data;
	ptr[2] = result;
	ptr[3] = info->img_addr&0xFFFFFFFF;
	ptr[4] = info->img_len&0xFFFFFFFF;
	ptr[5] = info->pubkeyhash&0xFFFFFFFF;
	ptr[6] = info->hash_len&0xFFFFFFFF;
	ptr[7] = info->flag;
	printf("%s:%d---vm=%d,cpu=%d,result: %d\n", __func__, __LINE__,
	       ptr[0], ptr[1], ptr[2]);

    /* secvm id: 3 */
	if (mv_svc_ivmc_request(3) != 0) {
		printf("## Waiting for response ...\n");
		while (1) {
			/* check the return value of ptr[2] */
			if (ptr[2] != result) {
				printf("%s:%d---vm=%u,cpu=%u,result: %u\n",__func__, __LINE__,
				       ptr[0], ptr[1], ptr[2]);
				break;
			}
		}
	} else {
		printf("## Verify request failed");
	}
	return ptr[2];
}
