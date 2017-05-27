#include <common.h>

#include "uboot_sec_drv.h"
#include "mv_gal.h"
#include "mv_svc_hypercalls.h"

#define SEC_VERIFY_CMD 0x1
#define SEC_GETHBK_CMD 0x8
/*add fastboot cmd for iwhale2*/
#define FUNCTYPE_GET_SOCID 0x03
#define FUNCTYPE_GET_LCS 0x0A
#define FUNCTYPE_SET_RMA 0x0B



int uboot_verify_img(unsigned long start_addr,uint32_t lenth)
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

/*add fastboot cmd for iwhale2*/
#ifdef CONFIG_SECBOOT
int get_lcs(uint32_t *p_lcs){
	unsigned int result = 0;

	volatile unsigned int *share_data;
	tee_call_request(&share_data);/*check tos status and send cmd to tos, tos return share memory point*/

	printf("##%s:%d---vm=%d,cpu=%d,result: %d\n", __func__, __LINE__,
			   share_data[0], share_data[1], share_data[2]);
	puts("## Start get life cycle status ...\n");
	share_data[2] = result;
	share_data[3] = FUNCTYPE_GET_LCS; /*cmd, other feature add own cmd*/
	share_data[4] = (unsigned int)p_lcs&0xFFFFFFFF;
	share_data[5] = (sizeof(uint32_t))&0xFFFFFFFF;


	printf("##cmd:0x%x,p_lcs:0x%x,p_lcs_len:0x%x\n",share_data[3],share_data[4],share_data[5]);
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
		printf("## Start get life cycle status request failed");
	}
	if(share_data[2] == 1)
		return 0;
	else
		return share_data[2];
}


int get_socid(uint64_t start_addr, uint64_t lenth){
	unsigned int result = 0;

	printf("start_addr=%p, lenth=%d\n", start_addr, lenth);
	volatile unsigned int *share_data;
	tee_call_request(&share_data);/*check tos status and send cmd to tos, tos return share memory point*/

	printf("##%s:%d---vm=%d,cpu=%d,result: %d\n", __func__, __LINE__,
			   share_data[0], share_data[1], share_data[2]);
	puts("## Start get socid...\n");
	share_data[2] = result;
	share_data[3] = FUNCTYPE_GET_SOCID; /*cmd, other feature add own cmd*/
	share_data[4] = (uint32_t)start_addr&0xFFFFFFFF;
	share_data[5] = lenth&0xFFFFFFFF;

	printf("##cmd:0x%x,start_addr:0x%x,lenth:0x%x\n",share_data[3],share_data[4],share_data[5]);
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
		printf("## get socid request failed");
	}
	if(share_data[2] == 1)
		return 0;
	else
		return share_data[2];
}

int set_rma(void){
	unsigned int result = 0;

	volatile unsigned int *share_data;
	tee_call_request(&share_data);/*check tos status and send cmd to tos, tos return share memory point*/

	printf("##%s:%d---vm=%d,cpu=%d,result: %d\n", __func__, __LINE__,
			   share_data[0], share_data[1], share_data[2]);
	puts("## Start set rma ...\n");
	share_data[2] = result;
	share_data[3] = FUNCTYPE_SET_RMA; /*cmd, other feature add own cmd*/
	//share_data[4] = info->img_addr&0xFFFFFFFF;
	//share_data[5] = info->img_len&0xFFFFFFFF;

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
		printf("## set rma request failed");
	}
	if(share_data[2] == 1)
		return 0;
	else
		return share_data[2];
}
#endif
