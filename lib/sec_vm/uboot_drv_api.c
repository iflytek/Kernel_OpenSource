#include <common.h>

#include "uboot_sec_drv.h"
#include "mv_gal.h"
#include "mv_svc_hypercalls.h"

#define SEC_VERIFY_CMD 0x1
#define SEC_GETHBK_CMD 0x3

int uboot_verify_img(uint64_t start_addr,uint64_t lenth)
{
	unsigned int result = 0;
	imgToVerifyInfo *info = (imgToVerifyInfo *)start_addr;


	printf("image addr=%p, size=%d\n", info->img_addr, info->img_len);
	printf("pubkey hash addr=%p, size=%d\n", info->pubkeyhash, info->hash_len);
	volatile unsigned int *share_data;
	tee_call_request(&share_data);/*check tos status and send cmd to tos, tos return share memory point*/

	printf("##%s:%d---vm=%d,cpu=%d,result: %d\n", __func__, __LINE__,
	       share_data[0], share_data[1], share_data[2]);
	puts("## Start verify image ...\n");
	share_data[2] = result;
	share_data[3] = SEC_VERIFY_CMD; /*cmd, other feature add own cmd*/
	share_data[4] = info->img_addr&0xFFFFFFFF;
	share_data[5] = info->img_len&0xFFFFFFFF;
	share_data[6] = info->pubkeyhash&0xFFFFFFFF;
	share_data[7] = info->hash_len&0xFFFFFFFF;
	share_data[8] = info->flag;

	printf("##cmd:0x%x,img_addr:0x%x,img_len:0x%x,pubkhash:0x%x,hashlen:0x%x",share_data[3],share_data[4],share_data[5],share_data[6],share_data[7]);
    /* secvm id: 3 */
	if (mv_svc_ivmc_request(3) != 0) {
		printf("## Waiting for response ...\n");
		while (1) {
			/* check the return value of ptr[2] */
			if (share_data[2] != result) {
				printf("%s:%d---vm=%u,cpu=%u,result: %u\n",__func__, __LINE__,
				       share_data[0], share_data[1], share_data[2]);
				break;
			}
			mdelay(200);
		}
	} else {
		printf("## Verify request failed");
	}
	return share_data[2];
}

int uboot_get_hbk(uint64_t start_addr, uint64_t lenth){
	unsigned int result = 0;
	imgToVerifyInfo *info = (imgToVerifyInfo *)start_addr;

	printf("image addr=%p, size=%d\n", info->img_addr, info->img_len);
	volatile unsigned int *share_data;
	tee_call_request(&share_data);/*check tos status and send cmd to tos, tos return share memory point*/

	printf("##%s:%d---vm=%d,cpu=%d,result: %d\n", __func__, __LINE__,
			   share_data[0], share_data[1], share_data[2]);
	puts("## Start save hbk ...\n");
	share_data[2] = result;
	share_data[3] = SEC_GETHBK_CMD; /*cmd, other feature add own cmd*/
	share_data[4] = info->img_addr&0xFFFFFFFF;
	share_data[5] = info->img_len&0xFFFFFFFF;
	share_data[6] = info->pubkeyhash&0xFFFFFFFF;
	share_data[7] = info->hash_len&0xFFFFFFFF;
	share_data[8] = info->flag;

	printf("##cmd:0x%x,img_addr:0x%x,img_len:0x%x\n",share_data[3],share_data[4],share_data[5]);
	/* secvm id: 3 */
	if (mv_svc_ivmc_request(3) != 0) {
		printf("## Waiting for response ...\n");
		while (1) {
			/* check the return value of ptr[2] */
			if (share_data[2] != result) {
				printf("%s:%d---vm=%u,cpu=%u,result: %u\n",__func__, __LINE__,
						  share_data[0], share_data[1], share_data[2]);
				break;
			}
			mdelay(200);
		}
	} else {
		printf("## Save hbk request failed");
	}
	return share_data[2];
}
