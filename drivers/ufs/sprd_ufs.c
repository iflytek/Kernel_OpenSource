#include <common.h>
#include <linux/sizes.h>
#include <sprd_ufs.h>
#include "protocol_util.h"
#include "hw_if.h"

/*
 * Global Variables shared across Driver
 */
uint32_t DescMemBase;
struct   ufs_driver_info drv_info;

/***************************************************************
 *
 * dwc_reset_hc
 * Description: resets the Host Controller. Enable's sending of
 *              DME_RESET and DME_ENABLE commands. Also polls for
 *              the completion of the initialization sequence
 *
 ***************************************************************/
int dwc_reset_hc()
{
	int retry = RETRY_CNT;

	FUNC_ENTER();
	/*
	 * Enable the Host Controller.
	 * DME_RESET and DME_ENABLE commands are sent automatically on DW_UFS_HCE write
	 */
	dwc_ufs_write_reg(DW_UFS_HCE, 0);

	udelay(1000); /* Platform Specific Implementation */

	dwc_ufs_write_reg(DW_UFS_HCE, HCE_RESET);
	

	/*
	 * poll untill (DW_UFS_HCE == 1) i'e initialization sequence has completed
	 */
	while (retry != 0)
	{
		if(1 == dwc_ufs_read_reg(DW_UFS_HCE))
			return UFS_SUCCESS;
		retry--;
		udelay(1); /* Platform Specific Implementation */
	}
	FUNC_EXIT();
	return UFS_TIMEOUT;
}

/***************************************************************
 *
 * send_uic_command
 * Description: Programs the Command Argument and the Command
 *     Register to send the DME_LINK_STARTUP command 
 *     to the device
 *
 ***************************************************************/
void send_uic_command(uint32_t command, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{

	dwc_ufs_write_reg(DW_UFS_UICCMDARG1,arg1);
	dwc_ufs_write_reg(DW_UFS_UICCMDARG2,arg2);
	dwc_ufs_write_reg(DW_UFS_UICCMDARG3,arg3);

	dwc_ufs_write_reg(DW_UFS_UICCMD,(command & 0xFF));
}

/***************************************************************
 *
 * send_dme_link_startup
 * Description: Programs the Command Argument and the Command
 *     Register to send the DME_LINK_STARTUP command 
 *     to the device
 *
 ***************************************************************/
void send_dme_link_startup(void)
{

	dwc_ufs_write_reg(DW_UFS_UICCMDARG1,0);
	dwc_ufs_write_reg(DW_UFS_UICCMDARG2,0);
	dwc_ufs_write_reg(DW_UFS_UICCMDARG3,0);

	dwc_ufs_write_reg(DW_UFS_UICCMD,(UIC_LINK_STARTUP_CMD & 0xFF));
}

/***************************************************************
 *
 * setup_snps_mphy_tc
 * Description: Programs the Unipro and Synopsys Mphy for 
 *
 ***************************************************************/
void setup_snps_mphy_tc()
{
	send_uic_command(0x00000002, 0x802b0000,0x00000000, 0x00000000);
	udelay(100000);
	send_uic_command(0x00000002, 0x80bf0000,0x00000000, 0x00000001);
	udelay(100000);
	send_uic_command(0x00000002, 0x80ea0000,0x00000000, 0x000000c0);
	udelay(100000);
	send_uic_command(0x00000002, 0x80f10000,0x00000000, 0x00000044);
	udelay(100000);

	send_uic_command(0x00000002, 0x00eb0000,0x00000000, 0x0000000d);
	udelay(100000);
	send_uic_command(0x00000002, 0x00eb0001,0x00000000, 0x0000000d);
	udelay(100000);
	send_uic_command(0x00000002, 0x00eb0004,0x00000000, 0x00000001);
	udelay(100000);
	send_uic_command(0x00000002, 0x00eb0005,0x00000000, 0x00000000);
	udelay(100000);

	send_uic_command(0x00000002, 0x00ec0000,0x00000000, 0x0000001b);
	udelay(100000);
	send_uic_command(0x00000002, 0x00ec0001,0x00000000, 0x0000001b);
	udelay(100000);
	send_uic_command(0x00000002, 0x00ec0004,0x00000000, 0x0000001b);
	udelay(100000);
	send_uic_command(0x00000002, 0x00ec0005,0x00000000, 0x0000001b);
	udelay(100000);

	send_uic_command(0x00000002, 0x00f00000,0x00000000, 0x00000012);
	udelay(100000);
	send_uic_command(0x00000002, 0x00f00001,0x00000000, 0x00000012);
	udelay(100000);
	send_uic_command(0x00000002, 0x00f00004,0x00000000, 0x00000002);
	udelay(100000);
	send_uic_command(0x00000002, 0x00f00005,0x00000000, 0x00000002);
	udelay(100000);

	send_uic_command(0x00000002, 0x00ba0004,0x00000000, 0x00000080);
	udelay(100000);
	send_uic_command(0x00000002, 0x00ba0005,0x00000000, 0x00000080);
	udelay(100000);

	send_uic_command(0x00000002, 0x80f30000,0x00000000, 0x00000064);
	udelay(100000);

	send_uic_command(0x00000002, 0x00f10000,0x00000000, 0x000000d6);
	udelay(100000);
	send_uic_command(0x00000002, 0x00f10001,0x00000000, 0x000000d6);
	udelay(100000);

	send_uic_command(0x00000002, 0x80fb0000,0x00000000, 0x00000009);
	udelay(100000);
	send_uic_command(0x00000002, 0x80f00000,0x00000000, 0x00000000);
	udelay(100000);

	send_uic_command(0x00000002, 0xd0850000,0x00000000, 0x00000001);
	udelay(100000);

}
/****************************************************************
 *
 * check_uic_link_startup_result
 * Description: The function returns the result of the sent 
 * UIC Command, Returns 0 on success , non zero number on failure
 *
 ***************************************************************/
static int check_uic_link_startup_result()
{
	return ((0xFF & dwc_ufs_read_reg(DW_UFS_UICCMDARG2)));
}


/***************************************************************
 * create_nop_out_upiu
 * Description: Fills the UPIU memory for NOP OUT command
 *
 ***************************************************************/
static void create_nop_out_upiu()
{
	struct dwc_ufs_nop_req_upiu *cmd_upiu_ptr;
	int i;
	void *tmp_ptr;

	FUNC_ENTER();
	tmp_ptr = drv_info.cmd_desc->command_upiu;
	cmd_upiu_ptr = (struct dwc_ufs_nop_req_upiu*)tmp_ptr;

	if (drv_info.dwc_ufs_version == DWC_UFS_HC_VERSION_2_0)
		drv_info.utrd_desc->ct_and_flags = (uint8_t)(UTP_NO_DATA_TRANSFER | UTP_UFS_STORAGE_COMMAND);
	else
		drv_info.utrd_desc->ct_and_flags = (UTP_NO_DATA_TRANSFER | UTP_DEVICE_MANAGEMENT_FUNCTION);
	drv_info.utrd_desc->resp_upiu_length = to_littleendian16(sizeof(struct dwc_ufs_nop_resp_upiu) >> 2);
	drv_info.utrd_desc->prdt_length = 0;

	cmd_upiu_ptr->trans_type        = 0x00;
	cmd_upiu_ptr->flags             = 0x00;
	cmd_upiu_ptr->reserved_1	= 0x00;
	cmd_upiu_ptr->task_tag          = 0x01;
	for(i = 0;i < 4; i++) {
		cmd_upiu_ptr->reserved_2[i]	= 0x00;
	}
	cmd_upiu_ptr->tot_ehs_len       = 0x00;
	cmd_upiu_ptr->reserved_3	= 0x00;
	cmd_upiu_ptr->data_seg_len      = 0x00;
	for(i = 0;i < 20; i++) {
		cmd_upiu_ptr->reserved_4[i] = 0x00;
	}

	FUNC_EXIT();
}

/***************************************************************
 *
 * handle_query_response
 * Description: The functon does the following 
 *              1. validates the query command's response received
 *              2.updates the ret argument with query data
 *              3. returns the status
 ***************************************************************/
static int handle_query_response(uint8_t *ret)
{
	struct dwc_ufs_query_upiu *resp_upiu;
	void *tmp_ptr;

	FUNC_ENTER();

	tmp_ptr = drv_info.cmd_desc->response_upiu;
	resp_upiu = (struct dwc_ufs_query_upiu *)tmp_ptr;


	/* Update the return value */
	if(ret) {
		ret[0] = resp_upiu->tsf[8];
		ret[1] = resp_upiu->tsf[9];
		ret[2] = resp_upiu->tsf[10];
		ret[3] = resp_upiu->tsf[11];
	}
	
	FUNC_EXIT();

	if(resp_upiu->query_resp == UFS_SUCCESS)
		return UFS_SUCCESS;

	return -(resp_upiu->query_resp);
}

/***************************************************************
 *
 * create_query_upiu
 * Description: Populates the UPIU memory for all query Operations
 *
 ***************************************************************/
static void create_query_upiu(uint8_t opcode, uint8_t query_func, uint8_t selector, uint8_t idn, uint8_t *val)
{
	struct dwc_ufs_query_upiu *cmd_upiu_ptr;
	void *tmp_ptr;
	int i;

	FUNC_ENTER();
	tmp_ptr = drv_info.cmd_desc->command_upiu;
	cmd_upiu_ptr = (struct dwc_ufs_query_upiu *)tmp_ptr;

	/* UTRD Descriptor Programming for processing command */
	if (drv_info.dwc_ufs_version == DWC_UFS_HC_VERSION_2_0)
		drv_info.utrd_desc->ct_and_flags = (uint8_t)(UTP_NO_DATA_TRANSFER | UTP_UFS_STORAGE_COMMAND);
	else
		drv_info.utrd_desc->ct_and_flags = (UTP_NO_DATA_TRANSFER | UTP_DEVICE_MANAGEMENT_FUNCTION);
	drv_info.utrd_desc->resp_upiu_length = to_littleendian16(sizeof(struct dwc_ufs_query_upiu) >> 2);
	drv_info.utrd_desc->prdt_length = 0;


	/* Command Descriptor Programming */
	cmd_upiu_ptr->trans_type        = 0x16;
	cmd_upiu_ptr->flags             = UPIU_CMD_FLAGS_NONE;
	cmd_upiu_ptr->reserved_1	= 0x00;
	cmd_upiu_ptr->task_tag          = 0x01;
	cmd_upiu_ptr->reserved_2	= 0x00;
	cmd_upiu_ptr->query_func        = query_func;
	cmd_upiu_ptr->query_resp        = 0x00;
	cmd_upiu_ptr->reserved_3        = 0x00;
	cmd_upiu_ptr->tot_ehs_len       = 0x00;
	cmd_upiu_ptr->data_seg_len      = 0x00;
	cmd_upiu_ptr->tsf[0]            = opcode;
	cmd_upiu_ptr->tsf[1]            = idn;
	cmd_upiu_ptr->tsf[2]            = 0x00;      /* index */
	cmd_upiu_ptr->tsf[3]            = selector; /* selector */
	cmd_upiu_ptr->tsf[4]            = 0x00;
	cmd_upiu_ptr->tsf[5]            = 0x00;
	cmd_upiu_ptr->tsf[6]            = 0x00;
	cmd_upiu_ptr->tsf[7]            = 0x00;

	/* Value/Flag Updation */
	cmd_upiu_ptr->tsf[8]            = val[0];
	cmd_upiu_ptr->tsf[9]            = val[1];
	cmd_upiu_ptr->tsf[10]           = val[2];
	cmd_upiu_ptr->tsf[11]           = val[3];

	for(i=12; i < 15 ;i++) {
		cmd_upiu_ptr->tsf[i]    = 0x00;
	}
	for(i=0; i < 3 ;i++)
		cmd_upiu_ptr->reserved_5[i] = 0x00;

	FUNC_EXIT();
}


/***************************************************************
 *
 * create_desc_upiu
 * Description: The function does a simple memcpy of the req_upiu
 *              content passed to UPIU memory for Read/Write 
 *              descriptor Operation 
 *
 ***************************************************************/
static void create_desc_upiu(void *req_upiu)
{
	FUNC_ENTER();

	if (drv_info.cmd_desc->command_upiu != req_upiu) {
		/* swap done since data_seg_len is in big endian format */
#ifdef HOST_BIG_ENDIAN
		memcpy(drv_info.cmd_desc->command_upiu, req_upiu,
			sizeof(struct dwc_ufs_query_upiu) + (((struct dwc_ufs_query_upiu *)req_upiu)->data_seg_len));

		SYNC_MEM(drv_info.cmd_desc, (offsetof(struct dwc_ufs_tcd, command_upiu)),
			sizeof(struct dwc_ufs_query_upiu) + (((struct dwc_ufs_query_upiu *)req_upiu)->data_seg_len));
#else
		memcpy(drv_info.cmd_desc->command_upiu, req_upiu,
			sizeof(struct dwc_ufs_query_upiu) + swap_16(((struct dwc_ufs_query_upiu *)req_upiu)->data_seg_len));

		SYNC_MEM(drv_info.cmd_desc, (offsetof(struct dwc_ufs_tcd, command_upiu)),
			(uint32_t)sizeof(struct dwc_ufs_query_upiu) + swap_16(((struct dwc_ufs_query_upiu *)req_upiu)->data_seg_len));
#endif
	}
	FUNC_EXIT();
}

/***************************************************************
 *
 * wait_for_cmd_completion
 * Description: Sets the DoorBell Register and waits for the 
 *              Doorbell to Clear. Returns Zero on Success or
 *		error number on Failure
 *
 ***************************************************************/
static int wait_for_cmd_completion()
{
	int retry = RETRY_CNT;

	FUNC_ENTER();
	/* Set the doorbell for processing the request */
	dwc_ufs_write_reg(DW_UFS_UTRLDBR, BIT(0));

	/* Wait for the DoorBell to clear */
	while(1)
	{
		if((dwc_ufs_read_reg(DW_UFS_UTRLDBR) & BIT(0)) == 0)
			break;
		retry--;
		if(retry == 0) {
			debugf("DoorBell Not Cleared and Timed Out\n");
			return UFS_TIMEOUT;
		}
		udelay(100); /* Platform Specific Implementation */
	}

	debugf("\n DoorBell Cleared");

	FUNC_EXIT();
	return UFS_SUCCESS;
}


/***************************************************************
 * do_flag_operation
 * Description: The function can be used for any operation on flags
                1. fills the query upiu memory content
		2. Handles the response of query command sent
		3. updates the flag return value
 ***************************************************************/
static int do_flag_operation(int opcode, int query_func, enum flags_id idn, uint8_t *flags_ret)
{
	int ret, i;
	uint8_t val[4];

	FUNC_ENTER();
	for(i=0;i < 3;i++)
		val[i] = 0x00;

	create_query_upiu((uint8_t)opcode, (uint8_t)query_func, (uint8_t)0, (uint8_t)idn, val);
	ret = wait_for_cmd_completion();
	if(ret != UFS_SUCCESS)
		return ret;

	ret = handle_query_response(val);
	*flags_ret = val[3];
	FUNC_EXIT();
	return ret;
}

/***************************************************************
 *
 * SetFlag
 * Description: The function invokes do_flag_operation for set flag
 *              operation
 ***************************************************************/
int SetFlag(enum flags_id idn, uint8_t *flags_ret)
{
	int ret;

	FUNC_ENTER();
	ret = do_flag_operation(SET_FLAG_OPCODE, STANDARD_WR_REQ, idn, flags_ret);
	FUNC_EXIT();
	return ret;
}

/***************************************************************
 *
 * ToggleFlag
 * Description: The function invokes do_flag_operation for Toggle
 *               flag operation
 *
 ***************************************************************/
int ToggleFlag(enum flags_id idn, uint8_t *flags_ret)
{
	int ret;

	FUNC_ENTER();
	ret = do_flag_operation(TOGGLE_FLAG_OPCODE, STANDARD_WR_REQ, idn, flags_ret);
	FUNC_EXIT();
	return ret;
}

/***************************************************************
 *
 * ReadFlag 
 * Description: The function invokes do_flag_operation for Read 
 *               flag operation
 *
 ***************************************************************/
int ReadFlag(enum flags_id idn, uint8_t *flags_ret)
{
	int ret;

	FUNC_ENTER();
	ret = do_flag_operation(READ_FLAG_OPCODE, STANDARD_RD_REQ, idn, flags_ret);
	FUNC_EXIT();
	return ret;
}



/***************************************************************
 *
 * ClearFlag
 * Description: The function invokes do_flag_operation for Clear
 *               flag operation
 *
 ***************************************************************/
int ClearFlag(enum flags_id idn, uint8_t *flags_ret)
{
	int ret;

	FUNC_ENTER();
	ret = do_flag_operation(CLEAR_FLAG_OPCODE, STANDARD_WR_REQ, idn, flags_ret);
	FUNC_EXIT();
	return ret;
}

/***************************************************************
 *
 * ReadAttribute
 * Description: The functon sends the query command to read an
 *              attribute and updates the ret_value with the 
 *              content of the attribute
 *
 ***************************************************************/
int ReadAttribute(enum attr_id idn, uint8_t *ret_value, uint8_t selector)
{
	int ret,i;

	FUNC_ENTER();

	for(i=0;i < 3;i++)
		ret_value[i] = 0x00;

	create_query_upiu(READ_ATTR_OPCODE, STANDARD_RD_REQ, selector, idn, ret_value);
	ret = wait_for_cmd_completion();
	if(ret != UFS_SUCCESS)
		return ret;

	ret = handle_query_response(ret_value);
	FUNC_EXIT();
	return ret;
}


/***************************************************************
 *
 * WriteAttribute
 * Description: The functon sends the query command to write an
 *              attribute with the value passed as second argument
 *
 ***************************************************************/
int WriteAttribute(enum attr_id idn, uint8_t *value, uint8_t selector)
{
	int ret;

	FUNC_ENTER();
	create_query_upiu(WRITE_ATTR_OPCODE, STANDARD_WR_REQ, selector, idn, value);
	ret = wait_for_cmd_completion();
	if(ret != UFS_SUCCESS)
		return ret;

	ret = handle_query_response(value);
	FUNC_EXIT();
	return ret;
}


/***************************************************************
 *
 * handle_scsi_completion
 * Description: The functon validates the SCSI command's 
 *              response received
 *
 ***************************************************************/
static int handle_scsi_completion()
{
	struct dwc_ufs_xfer_resp_upiu *resp_upiu;
	uint8_t status;
	void *tmp_ptr;
#ifdef DEBUG_MODE
	int i;
	uint8_t *ptr;
#endif

	FUNC_ENTER();
	tmp_ptr = drv_info.cmd_desc->response_upiu;
	resp_upiu = (struct dwc_ufs_xfer_resp_upiu *)tmp_ptr;

#ifdef DEBUG_MODE
	ptr = drv_info.cmd_desc->response_upiu;
	PRINTF("\n\n The SCSI Command Response is");
	for(i=0;(i < (sizeof(struct dwc_ufs_xfer_resp_upiu))); i+=4)
		PRINTF("\n0x%x 0x%x 0x%x 0x%x",ptr[i],ptr[i+1],ptr[i+2],ptr[i+3]);
#endif

	/* read the OCS value */
	if(drv_info.utrd_desc->ocs == UFS_SUCCESS) {
		status = resp_upiu->status;
		if(status == SAM_STAT_GOOD)
			return UFS_SUCCESS;
		/* 0x20 is added to sense key to return unique error code */
		if(status == SAM_STAT_CHECK_CONDITION)
			return -(0x20 + (drv_info.cmd_desc->response_upiu[SENSE_KEY_INDEX] & 0xf));
	} else {
#ifdef DEBUG_MODE
		PRINTF("\nThe OCS val returned is %x",drv_info.utrd_desc->ocs);
#endif
		return -(drv_info.utrd_desc->ocs);
	}
	FUNC_EXIT();
	return UFS_SUCCESS;
}

/***************************************************************
 *
 * read_nop_rsp
 * Description: The function reads and validates the response
 *              received for NOP command
 *
 ***************************************************************/
static int read_nop_rsp()
{
	struct dwc_ufs_nop_resp_upiu *resp_upiu;
	void   *tmp_ptr;

	tmp_ptr = drv_info.cmd_desc->response_upiu;
	resp_upiu = (struct dwc_ufs_nop_resp_upiu *)tmp_ptr;

	if((resp_upiu->response != UFS_SUCCESS) || (resp_upiu->trans_type != NOP_TRANS_TYPE))
		return UFS_INVALID_RESPONSE;

	return UFS_SUCCESS;
}

/***************************************************************
 *
 * send_nop_out_cmd()
 * Description: Invokes API for UPIU creation for nop out command
 * and sends it to the device by setting the DoorBell.
 *
 ****************************************************************/
int send_nop_out_cmd()
{
	int retry = RETRY_CNT;
	int ret;

	FUNC_ENTER();
	do {
		create_nop_out_upiu();
		udelay(100);
		retry--;
		ret = wait_for_cmd_completion();
		if(ret != UFS_SUCCESS) {
			dwc_ufs_write_reg(DW_UFS_UTRLCLR,0xfffffffe);
			continue;
		}

		ret = read_nop_rsp();
	} while(ret != UFS_SUCCESS && retry > 0);

	FUNC_EXIT();

	return ret;
}

/***************************************************************
 *
 * check_config_desclock
 * Description: The function reads the bConfigDescLock attribute
 *              and returns UFS_SUCCESS if unlocked.
 *
 ***************************************************************/
int check_config_desclock()
{
	uint8_t attr[4];

	/* read bConfigDescLock */
	ReadAttribute(B_CONFIG_DESC_LOCK,attr,0);
	if(attr[3] != 0)
		return UFS_CONFIG_DESC_LOCKED;
	return UFS_SUCCESS;
}

/***************************************************************
 *
 * SetConfigDescLock 
 * Description: The function programs the config lock attribute
 *              sets the Permanant Write Protect or/and the PowerOn
 *              Write Protect Flags if requested in the input args
 *
 ****************************************************************/
int SetConfigDescLock(void)
{
	int      ret;
	uint8_t  desc_lock_val[4] = {0x00,0x00,0x00,0x01};
	ret = WriteAttribute(B_CONFIG_DESC_LOCK, desc_lock_val, 0);
	return ret;
}

void prepare_read_desc_upiu(struct dwc_ufs_query_upiu *req_upiu, uint8_t idn, uint8_t index)
{
	int     i;

	FUNC_ENTER();

	/* Command Descriptor Programming */
	req_upiu->trans_type        = 0x16;
	req_upiu->flags             = UPIU_CMD_FLAGS_NONE;
	req_upiu->reserved_1	    = 0x00;
	req_upiu->task_tag          = 0x01;
	req_upiu->reserved_2	    = 0x00;

	req_upiu->query_func        = STANDARD_RD_REQ;

	req_upiu->query_resp        = 0x00;
	req_upiu->reserved_3        = 0x00;
	req_upiu->tot_ehs_len       = 0x00;

	/*in big endian*/
	req_upiu->data_seg_len      = 0x00;

	req_upiu->tsf[0]            = READ_DESC_OPCODE;
	req_upiu->tsf[1]            = idn;
	req_upiu->tsf[2]            = index; /* index */
	req_upiu->tsf[3]            = 0x0;                /* selector */
	req_upiu->tsf[4]            = 0x0;
	req_upiu->tsf[5]            = 0x0;

	/*0xFF larger than all types of desc, so the response will provide the exact desc size*/
	req_upiu->tsf[6]            = 0x0;
	req_upiu->tsf[7]            = 0xFF;

	for(i=8; i < 15 ;i++)
		req_upiu->tsf[i]    = 0x00;

	for(i=0; i < 3 ;i++)
		req_upiu->reserved_5[i] = 0x00;

	FUNC_EXIT();
}

/***************************************************************
 *
 * read_descriptor
 * Description: The functon populates the request upiu structure
 		with upiu info passed in 1st argument
 *
 ***************************************************************/
int write_read_descriptor(void *req_upiu)
{
	int ret;
	int     i;
	uint8_t *ptr;


	FUNC_ENTER();

	/* UTRD Descriptor Programming for processing command */
	if (drv_info.dwc_ufs_version == DWC_UFS_HC_VERSION_2_0)
		drv_info.utrd_desc->ct_and_flags = (uint8_t)(UTP_NO_DATA_TRANSFER | UTP_UFS_STORAGE_COMMAND);
	else
		drv_info.utrd_desc->ct_and_flags = (UTP_NO_DATA_TRANSFER | UTP_DEVICE_MANAGEMENT_FUNCTION);

	/*why use dwc_ufs_query_upiu size? there are always attached data follow the response upiu*/
	drv_info.utrd_desc->resp_upiu_length = ALIGNED_UPIU_SIZE;
	drv_info.utrd_desc->prdt_length = 0;

	/*maybe not necessary*/
	//create_desc_upiu(req_upiu);

	ret = wait_for_cmd_completion();
	if(ret != UFS_SUCCESS)
		return ret;

	ptr = drv_info.cmd_desc->response_upiu;

	debugf("\n\n The data segment of length %ld is",ALIGNED_UPIU_SIZE - sizeof(struct dwc_ufs_query_upiu));
	for(i=32; i < ALIGNED_UPIU_SIZE; i = i+4)
		debugf("\n %x %x %x %x",ptr[i],ptr[1+i],ptr[i+2],ptr[i+3]);

	ret = handle_query_response(NULL);
	FUNC_EXIT();
	return ret;
}




int get_descriptor_data(void ** desc_ptr, uint8_t idn, uint8_t index)
{
	int      ret;
	struct   dwc_ufs_query_upiu *req_upiu;
	void *   tmp_ptr;
	void     *resp_upiu = drv_info.cmd_desc->response_upiu; 

 	tmp_ptr = drv_info.cmd_desc->command_upiu;
	req_upiu = (struct dwc_ufs_query_upiu *)tmp_ptr;

	prepare_read_desc_upiu(req_upiu, idn, index);

	if((ret = write_read_descriptor(req_upiu)) != UFS_SUCCESS)
		return ret;

	*desc_ptr = (uint8_t *)resp_upiu + sizeof(struct dwc_ufs_query_upiu);
	return UFS_SUCCESS;

}

void preconfig_utrd_descriptor(void)
{
	drv_info.utrd_desc->ucdba            = cpu_to_le32(LOWER_32_BITS((ulong)drv_info.cmd_desc));
	drv_info.utrd_desc->ucdbau           = cpu_to_le32(UPPER_32_BITS((ulong)drv_info.cmd_desc));
	drv_info.utrd_desc->resp_upiu_offset = cpu_to_le16(offsetof(struct dwc_ufs_tcd, response_upiu) >> 2);
	drv_info.utrd_desc->prdt_offset      = cpu_to_le16(offsetof(struct dwc_ufs_tcd, prdt_table) >> 2);
	drv_info.utrd_desc->ocs = 0xf;
}


void preconfig_utm_descriptor()
{
	int i;
	
	for(i = 0; i < 3; i++)
		drv_info.utm_desc->reserved_1[i] = 0x0;
		
	drv_info.utm_desc->intr_flag  = 0x0;
	drv_info.utm_desc->reserved_2 = 0x0;
	drv_info.utm_desc->ocs = 0xf;

	for(i = 0; i < 3; i++)
		drv_info.utm_desc->reserved_3[i] = 0x0;

	drv_info.utm_desc->reserved_4 = 0x0;
	return;
}


/****************************************************************
 *
 * init_ufs_hostc
 * Description: The Function does the following
 *            1. Sets Clock Divider
 *            2. Resets the Host Controller
 *            3. Performs Loopback Configuration if enabled
 *            4. Sends the LinkStartUp command
 *            5. Allocates and configures UTRD's
 *
 ****************************************************************/
int init_ufs_hostc()
{
	int      ret    = 0;
	int      retry1 = RETRY_CNT;
	int      retry2 = RETRY_CNT;

	FUNC_ENTER();

	/* The function resets the UFS Host Controller */
	ret = dwc_reset_hc();
	if(ret != 0)
		return ret;

	/* Update the DWC UFSHCI specification that the controller implementation supports*/	
	drv_info.dwc_ufs_version = dwc_ufs_read_reg(DW_UFS_VER);

	/* Set the CLK Divider Register */
	dwc_ufs_write_reg(DW_UFS_HCLKDIV, CLK_DIV_DEFAULT);

#ifdef DWC_UFS_LOOPBACK_MODE /* For Debug Purpose Only */
	/* LOOP Back Configuration */
	dwc_ufs_write_reg(DW_UFS_LBMCFG, LOOPBACK_DEFAULT);
#else

	/*
	 * Send locally created DME_LINKSTARTUP UIC command to the start the
	 * Link Startup Procedure
	 * keep sending the DME_LINKSTARTUP command until the device is detected
	 */
	while(retry1 > 0) {
		if((dwc_ufs_read_reg(DW_UFS_HCS) & DW_UFS_HCS_DP))
			break;

		/* Do the HCE reset before sending next link startup command */
		dwc_reset_hc();
		
		/* Set the CLK Divider Register since it has been reset */
		dwc_ufs_write_reg(DW_UFS_HCLKDIV, CLK_DIV_DEFAULT);
		/* If working with Synopsys Mphy; this function is required */
#ifdef USE_SNPS_MPHY_TC
		setup_snps_mphy_tc();
#endif
		if(dwc_ufs_read_reg(DW_UFS_HCS) & DW_UFS_HCS_UCRDY)
			send_dme_link_startup();
		else
			continue;

		udelay(200000); /* Wait for 200ms before checking the Link status */

		/*
		 * wait for the UIC Command to complete by checking UCCS field
		 */
		retry2 = RETRY_CNT;
		while(retry2 > 0)
		{
			if((dwc_ufs_read_reg(DW_UFS_IS) & DW_UFS_IS_UCCS))
				break;
			udelay(100000); /* Total of 100ms delay before next LINK STARTUP */

			retry2--;
		}

		retry1--;

	}
	if(retry1 == 0)
		return UFS_TIMEOUT;

	if(0 != check_uic_link_startup_result())
		return UFS_INVALID_RESPONSE;
#endif

	drv_info.cmd_desc = (struct dwc_ufs_tcd *) memalign(SZ_128, sizeof(struct dwc_ufs_tcd));
	drv_info.utrd_desc = (struct dwc_ufs_utrd *) memalign(SZ_1K, sizeof(struct dwc_ufs_utrd));
	drv_info.utm_desc = (struct dwc_ufs_utmrd *) memalign(SZ_1K, sizeof(struct dwc_ufs_utmrd));

	/* Initialize the transfer request descriptor */
	preconfig_utrd_descriptor();

	/* 
	 * Write the descriptor base address in the UTRLBA register
	 * for the Transfer Requests
	 */
	dwc_ufs_write_reg(DW_UFS_UTRLBA,  LOWER_32_BITS((ulong)drv_info.utrd_desc));
	dwc_ufs_write_reg(DW_UFS_UTRLBAU, UPPER_32_BITS((ulong)drv_info.utrd_desc));

	/* Initialize the task management descriptor */
	preconfig_utm_descriptor();

	/* 
	 * Write the descriptor base address in the UTRLBA register
	 * for the Transfer Requests
	 */
	dwc_ufs_write_reg(DW_UFS_UTMRLBA,  LOWER_32_BITS((ulong)drv_info.utm_desc));
	dwc_ufs_write_reg(DW_UFS_UTMRLBAU, UPPER_32_BITS((ulong)drv_info.utm_desc));

	/*
	 * Enable UTP Transfer request List by writing to UTRLRSR
	 */
	dwc_ufs_write_reg(DW_UFS_UTRLRSR, 0x1);

	/*
	 * Enable UFS Task Managament List by writing to DW_UFS_UTMRLRSR
	 */
	dwc_ufs_write_reg(DW_UFS_UTMRLRSR, 0x1);
	FUNC_EXIT();

	return UFS_SUCCESS;
}


/***************************************************************
 *
 * init_ufs_device()
 * Description: The function does the following
 *            1. Sends the NOP Out command continuosly till it passes
 *            2. Sets the Device init Flag for UFS Device initialization
 *
 ****************************************************************/
int init_ufs_device()
{
	int ret;
	uint8_t flags_ret_val;
	int retry = RETRY_CNT;

	FUNC_ENTER();
	/* Send continuos NOP OUT Command unless Response is Valid */
	ret = send_nop_out_cmd();
	if(ret != UFS_SUCCESS)
		return ret;

	/* Set the Device Init Flag */
	ret = SetFlag(FDEVICE_INIT, &flags_ret_val);
	if(ret)
		return ret;
	do {
		retry--;
		ret = ReadFlag(FDEVICE_INIT, &flags_ret_val);
		PRINTF("\n Flags val %d",flags_ret_val);
		if((ret == UFS_SUCCESS) && (flags_ret_val == 0))
			break;
		else {
			dwc_ufs_write_reg(DW_UFS_UTRLCLR,0xfffffffe);
		}
	}while(retry > 0);

	if(flags_ret_val != 0)
		ret = UFS_FDEVICE_INIT_FAIL;

	FUNC_EXIT();
	return ret;
}

void ufs_init()
{
	init_ufs_hostc();
	init_ufs_device();
	ufs_update_native();
	//ufs_register_blk_device();
}

void update_native_lu_info(struct ufs_cfg_desc_tbl *cfg_desc)
{
	int lun_index = 0;
	int id = 0;
	uint32_t alloc_unit_num = 0;
	uint32_t blks_per_alloc_unit = 0;

	for (lun_index = 0; lun_index < UFS_MAX_LU_NUM; lun_index++)	{
		memset(&drv_info.lu_info[lun_index], 0 ,sizeof(struct lu_info_tbl));
		alloc_unit_num = be32_to_cpu(cfg_desc->unit_cfg[lun_index].dNumAllocUnits);
		if (0 != alloc_unit_num) {
			drv_info.lu_info[id].lu_index = lun_index;
			drv_info.lu_info[id].log2blksz = cfg_desc->unit_cfg[lun_index].bLogicalBlockSize;
			blks_per_alloc_unit = (drv_info.alloc_unit_sz * SZ_512) >> drv_info.lu_info[lun_index].log2blksz;
			drv_info.lu_info[id].blkcnt = alloc_unit_num * blks_per_alloc_unit;
			drv_info.lu_info[id].memory_type = cfg_desc->unit_cfg[lun_index].bMemoryType;
			drv_info.lu_info[id].bootable = cfg_desc->unit_cfg[lun_index].bBootLunID;
			id++;
		}
	}
	return;
}


uint32_t align_to_alloc_unit(uint64_t blk_cnt, uint32_t log2_blk_sz)
{
	uint32_t blks_per_alloc_unit = 0;
	uint32_t au_num = 0;
	blks_per_alloc_unit = (drv_info.alloc_unit_sz * SZ_512) >> log2_blk_sz;
	au_num = blk_cnt / blks_per_alloc_unit;
	if (0 != blk_cnt % blks_per_alloc_unit)
			au_num++;
	return au_num;
}

int compare_cfg_lu_table(struct lu_info_tbl * new_lu_cfg)
{
	int ret = LU_INFO_SAME;
	uint32_t aligned_au_new = 0;
	uint32_t aligned_au_old = 0;
	int lun_index = 0;
	if (0 == drv_info.lu_num)
		return ret;

	for (lun_index = 0; lun_index < UFS_MAX_LU_NUM; lun_index++) {
		if (new_lu_cfg[lun_index].lu_index != drv_info.lu_info[lun_index].lu_index
				||new_lu_cfg[lun_index].bootable != drv_info.lu_info[lun_index].bootable
				||new_lu_cfg[lun_index].memory_type != drv_info.lu_info[lun_index].memory_type
				||new_lu_cfg[lun_index].log2blksz != drv_info.lu_info[lun_index].log2blksz) {
			ret = LU_INFO_DIFF;
			break;
		}
		/*meet the end of the table or this lu extend to the maximum blk*/
		if (0 == new_lu_cfg[lun_index].blkcnt) {
			if (UFS_MAX_LU_NUM - 1 == lun_index)
				break;
			if (drv_info.lu_info[lun_index + 1].log2blksz != 0 && drv_info.lu_info[lun_index + 1].blkcnt != 0) {
				ret = LU_INFO_DIFF;
				break;
			}
		}
		
		aligned_au_new = align_to_alloc_unit(new_lu_cfg[lun_index].blkcnt, new_lu_cfg[lun_index].log2blksz);
		aligned_au_old = align_to_alloc_unit(drv_info.lu_info[lun_index].blkcnt, drv_info.lu_info[lun_index].log2blksz);
		if (aligned_au_new != aligned_au_old) {
			ret = LU_INFO_DIFF;
			break;
		}
	}

	return ret;
}

void prepare_write_desc_upiu(struct dwc_ufs_query_upiu *req_upiu, 
			struct ufs_cfg_desc_tbl *default_desc, struct lu_info_tbl *new_cfg)
{
	uint16_t data_len = 0;
	int lu_index = 0;
	uint32_t au_num = 0;
	uint32_t id = 0;
	struct ufs_cfg_desc_tbl *req_cfg_desc;
	FUNC_ENTER();

	/* Command Descriptor Programming */
	req_upiu->trans_type        = 0x16;
	req_upiu->flags             = UPIU_CMD_FLAGS_NONE;
	req_upiu->reserved_1	    = 0x00;
	req_upiu->task_tag          = 0x01;
	req_upiu->reserved_2	    = 0x00;

	req_upiu->query_func        = STANDARD_WR_REQ;

	req_upiu->query_resp        = 0x00;
	req_upiu->reserved_3        = 0x00;
	req_upiu->tot_ehs_len       = 0x00;

	data_len = (uint16_t) (sizeof(struct ufs_cfg_desc_tbl) & 0xFFFF);
	/*in big endian*/
	req_upiu->data_seg_len      = cpu_to_be16(data_len);

	req_upiu->tsf[0]            = WRITE_DESC_OPCODE;
	req_upiu->tsf[1]            = CONFIGURATION_DESC;  /*idn*/
	req_upiu->tsf[2]            = 0; /* index */
	req_upiu->tsf[3]            = 0x0;                /* selector */
	req_upiu->tsf[4]            = 0x0;
	req_upiu->tsf[5]            = 0x0;

	/*0xFF larger than all types of desc, so the response will provide the exact desc size*/
	req_upiu->tsf[6]            = 0x0;
	req_upiu->tsf[7]            = 0xFF;

	for(id = 8; id < 15 ;id++)
		req_upiu->tsf[id]    = 0x00;

	for(id = 0; id < 3 ;id++)
		req_upiu->reserved_5[id] = 0x00;
	req_cfg_desc = req_upiu + sizeof(struct dwc_ufs_query_upiu);
	if (req_cfg_desc != default_desc)
		memcpy(req_cfg_desc, default_desc, sizeof(struct ufs_cfg_desc_tbl));
	req_cfg_desc->bBootEnable = 0x01;
	id = 0;
	for (lu_index = 0; lu_index < UFS_MAX_LU_NUM; lu_index++) {
		if (lu_index == new_cfg[id].lu_index) {
			req_cfg_desc->unit_cfg[lu_index].bLUEnable = 0x01;
			req_cfg_desc->unit_cfg[lu_index].bBootLunID = new_cfg[id].bootable;
			req_cfg_desc->unit_cfg[lu_index].bMemoryType = new_cfg[id].memory_type;
			req_cfg_desc->unit_cfg[lu_index].bLogicalBlockSize = new_cfg[id].log2blksz;
			au_num = align_to_alloc_unit(new_cfg[id].blkcnt, new_cfg[id].log2blksz);
			req_cfg_desc->unit_cfg[lu_index].dNumAllocUnits = cpu_to_be32(au_num);
			id++;
		}
	}

	FUNC_EXIT();
}


int set_descriptor_data(struct ufs_cfg_desc_tbl *default_desc, struct lu_info_tbl *new_cfg)
{
	int      ret;
	struct   dwc_ufs_query_upiu *req_upiu;
	void *   tmp_ptr;

 	tmp_ptr = drv_info.cmd_desc->command_upiu;
	req_upiu = (struct dwc_ufs_query_upiu *)tmp_ptr;

	prepare_write_desc_upiu(req_upiu, default_desc, new_cfg);

	ret = write_read_descriptor(req_upiu);

	return ret;
}

int verify_lu_cfg(struct lu_info_tbl *lu_cfg)
{
	int ret = 0;
	int loop = 0;
	int multi = 0;
	int boota_occupied = 0;
	int bootb_occupied = 0;
	uint64_t assigned_size = 0;

	if (0 == lu_cfg[0].log2blksz)
		return LU_INFO_ILLEGAL;
	
	for (loop = 0; loop < UFS_MAX_LU_NUM; loop++) {
		if (MAIN_BOOT == lu_cfg[loop].bootable) {
			if (1 == boota_occupied)
				return LU_INFO_ILLEGAL;
			else
				boota_occupied = 1;
		}

		if (ALTERNATE_BOOT == lu_cfg[loop].bootable) {
			if (1 == bootb_occupied)
				return LU_INFO_ILLEGAL;
			else
				bootb_occupied = 1;
		}
		
		if (0 == lu_cfg[loop].blkcnt)
			break;
		
		if (loop != 0 && lu_cfg[loop].lu_index <= lu_cfg[loop-1].lu_index)
			return LU_INFO_ILLEGAL;
		
		if (0 == lu_cfg[loop].log2blksz)
			return LU_INFO_ILLEGAL;
		multi = (1 << lu_cfg[loop].log2blksz) / SZ_512;
		assigned_size += lu_cfg[loop].blkcnt * multi;
	}

	if (assigned_size > drv_info.dev_total_cap)
		return LU_INFO_ILLEGAL;
	
	return 0;
}


int ufs_lu_reconstruct(struct lu_info_tbl *lu_cfg)
{
	int ret = 0;
	struct ufs_cfg_desc_tbl *cfg_desc;

	ret = verify_lu_cfg(lu_cfg);
	if (LU_INFO_ILLEGAL == ret)
		return UFS_INVALID_PARTITIONING;
	ret = get_descriptor_data((void **)(&cfg_desc), CONFIGURATION_DESC, 0);
	/*maybe not necessary*/
	//update_native_lu_info(cfg_desc);

	ret = compare_cfg_lu_table(lu_cfg);
	if (LU_INFO_SAME == ret)
		return UFS_SUCCESS;

	if (UFS_SUCCESS != check_config_desclock())
		return UFS_CONFIG_DESC_LOCKED;

	ret = set_descriptor_data(cfg_desc, lu_cfg);
	if (UFS_SUCCESS != ret)
		return ret;
	
	ret = WriteAttribute(B_CONFIG_DESC_LOCK, LOCK_CONFIGURATION, 0);
	if (UFS_SUCCESS != ret)
		return ret;
	
	/*TODO*/
	reset_ufs_device();

	ufs_init();

	ret = compare_cfg_lu_table(lu_cfg);
	if (LU_INFO_SAME == ret)
		return UFS_SUCCESS;
	else
		return UFS_FATAL_ERROR;
}

static void create_cmd_upiu(uint32_t opcode, enum dma_data_direction direction, 
							struct lu_info_tbl *lu_info, void *buf_addr,
							lbaint_t start, lbaint_t blkcnt, uint8_t cmd_set_type)
{
	struct   dwc_ufs_cmd_upiu *cmd_upiu_ptr = NULL;
	uint32_t data_direction = 0;
	uint8_t  upiu_flags = 0;
	uint32_t prdt_buf_size = drv_info.prdt_entry_size;
	void     *tmp_ptr = NULL;
	uint32_t i = 0;
	uint32_t transfer_size = 0;
	uint8_t  *ptr = NULL;
	lbaint_t size = 0;

	FUNC_ENTER();
	tmp_ptr = drv_info.cmd_desc->command_upiu;
	cmd_upiu_ptr = (struct dwc_ufs_cmd_upiu *)(tmp_ptr);

	if (direction == DMA_FROM_DEVICE) {
		data_direction = UTP_DEVICE_TO_HOST;
		upiu_flags     = UPIU_CMD_FLAGS_READ;
	} else if (direction == DMA_TO_DEVICE) {
		data_direction = UTP_HOST_TO_DEVICE;
		upiu_flags     = UPIU_CMD_FLAGS_WRITE;
	} else {
		data_direction = UTP_NO_DATA_TRANSFER;
		upiu_flags     = UPIU_CMD_FLAGS_NONE;
	}

	/* Update cmd_type, flags and response upiu length for transfer requests */
	if (drv_info.dwc_ufs_version == DWC_UFS_HC_VERSION_2_0)
		drv_info.utrd_desc->ct_and_flags = (uint8_t)(data_direction | UTP_UFS_STORAGE_COMMAND);
	else
		drv_info.utrd_desc->ct_and_flags = (uint8_t)(data_direction | UTP_SCSI_COMMAND);
	drv_info.utrd_desc->resp_upiu_length = to_littleendian16((uint16_t)(sizeof(struct dwc_ufs_xfer_resp_upiu) >> 2));

	cmd_upiu_ptr->trans_type        = 0x01;
	cmd_upiu_ptr->flags             = upiu_flags;
	cmd_upiu_ptr->lun               = lu_info->lu_index;
	cmd_upiu_ptr->task_tag          = 0x1;
	cmd_upiu_ptr->cmd_set_type      = cmd_set_type;
	cmd_upiu_ptr->reserved_1[0]     = 0x0;
	cmd_upiu_ptr->reserved_1[1]     = 0x0;
	cmd_upiu_ptr->reserved_1[2]     = 0x0;
	cmd_upiu_ptr->tot_ehs_len       = 0x0;
	cmd_upiu_ptr->reserved_2        = 0x0;
	cmd_upiu_ptr->data_seg_len      = 0x0;

	if (opcode == UFS_OP_SECURITY_PROTOCOL_IN || opcode == UFS_OP_SECURITY_PROTOCOL_OUT) {
		/*TODO, for RPMB, size in unit of byte*/
		size = blkcnt << lu_info->log2blksz;
		get_cmnd(opcode, 0, size, cmd_upiu_ptr->cdb);
		prdt_buf_size = RPMB_FRAME_SIZE;
	} else {
		get_cmnd(opcode, start, blkcnt, cmd_upiu_ptr->cdb);
	}

	transfer_size = blkcnt << lu_info->log2blksz;
	cmd_upiu_ptr->exp_data_xfer_len = cpu_to_be32(transfer_size);

	/* Update PRDT Length */ 
	/*the caller must guarantee the transfer_size no more than max prdt capability*/
	drv_info.utrd_desc->prdt_length = (uint16_t)((0 == transfer_size % prdt_buf_size) ?
					(transfer_size /prdt_buf_size) : (transfer_size /prdt_buf_size + 1));


	/* Fill PRD Table Info */
	if (direction != DMA_NONE) {
		for (i = 0; transfer_size > 0; i++) {
			drv_info.cmd_desc->prdt_table[i].base_addr
				 = LOWER_32_BITS((ulong)(buf_addr + i * prdt_buf_size));
			drv_info.cmd_desc->prdt_table[i].upper_addr
				 = UPPER_32_BITS((ulong)(buf_addr + i * prdt_buf_size));

			drv_info.cmd_desc->prdt_table[i].reserved1  = 0x0;
			if (transfer_size > prdt_buf_size) {
				drv_info.cmd_desc->prdt_table[i].size = prdt_buf_size - 1;
				transfer_size -= prdt_buf_size;
			} else {
				drv_info.cmd_desc->prdt_table[i].size = transfer_size - 1;
				transfer_size = 0;
			}
		}
	}

	ptr = (uint8_t *)drv_info.utrd_desc;
	debugf("\n The byte array dump of  UTRD is");
	for(i=0;(i < (sizeof(struct dwc_ufs_utrd))); i+=4)
		debugf("\n0x%x 0x%x 0x%x 0x%x", ptr[i], ptr[i+1], ptr[i+2], ptr[i+3]);

	ptr = (uint8_t *)drv_info.cmd_desc->command_upiu;
	debugf("\n\n The byte array dump of cmd upiu is");
	for(i=0;(i < (sizeof(struct dwc_ufs_cmd_upiu))); i+=4)
		debugf("\n0x%x 0x%x 0x%x 0x%x", ptr[i], ptr[i+1], ptr[i+2], ptr[i+3]);

	FUNC_EXIT();
}


int send_scsi_cmd (uint32_t opcode, enum dma_data_direction direction, 
						struct lu_info_tbl *lu_info, void *buf_addr,
						lbaint_t start, lbaint_t blkcnt, uint8_t cmd_set_type)
{
	int ret;

	FUNC_ENTER();
	create_cmd_upiu(opcode, direction, lu_info, buf_addr, start, blkcnt, cmd_set_type);

	ret = wait_for_cmd_completion();
	FUNC_EXIT();
	return ret;
}

struct lu_info_tbl *find_lu_info(int lun)
{
	int id = 0;
	for (id = 0; id < UFS_MAX_LU_NUM; id++) {
		if (lun == drv_info.lu_info[id].lu_index)
			return &drv_info.lu_info[id];
	}

	return NULL;
}


static ulong ufs_bread(int dev_num, lbaint_t start, lbaint_t blkcnt, void *dst)
{
	int ret;
	lbaint_t blocks_todo = blkcnt;
	lbaint_t cur = 0;
	uint32_t max_read_size = 0;
	lbaint_t max_read_blk = 0;
	void * temp_buff;
	FUNC_ENTER();

	struct lu_info_tbl *lu_info = find_lu_info(dev_num);
	if ( NULL == lu_info)
		return 0;

	if (0 != (ulong)dst % 4) {
		errorf("UFS read destination addr not aligned to 4 bytes, it will harm the read efficiency!\n");
		max_read_size = drv_info.max_prdt_cap > SZ_1M ? SZ_1M : drv_info.max_prdt_cap;
		max_read_blk = max_read_size >> lu_info->log2blksz;
		/*malloc can guarantee 8 bytes aligned*/
		temp_buff = malloc(max_read_size);
		if (NULL == temp_buff)
			return 0;
		do {
			cur = (blocks_todo > max_read_blk) ? max_read_blk : blocks_todo;
			/* Send Read 10 Command and handle its completion */
			ret = send_scsi_cmd(UFS_OP_READ_10, DMA_FROM_DEVICE, lu_info, temp_buff, start, cur, 0);
			if (UFS_SUCCESS == ret)
				ret = handle_scsi_completion();
			if (ret != UFS_SUCCESS) {
				free(temp_buff);
				return 0;
			}
	
			blocks_todo -= cur;
			start += cur;
			memcpy(dst, temp_buff, cur << lu_info->log2blksz);
			dst += cur << lu_info->log2blksz;
		} while (blocks_todo > 0);
	} else {
		max_read_blk = drv_info.max_prdt_cap >> lu_info->log2blksz;
		do {
			cur = (blocks_todo > max_read_blk) ? max_read_blk : blocks_todo;
			/* Send Read 10 Command and handle its completion */
			ret = send_scsi_cmd(UFS_OP_READ_10, DMA_FROM_DEVICE, lu_info, dst, start, cur, 0);
			if (UFS_SUCCESS == ret)
				ret = handle_scsi_completion();
			if (ret != UFS_SUCCESS)
				return 0;
			
			blocks_todo -= cur;
			start += cur;
			dst += cur << lu_info->log2blksz;
		} while (blocks_todo > 0);
	}
	FUNC_EXIT();
	return blkcnt;

}

static ulong ufs_bwrite(int dev_num, lbaint_t start, lbaint_t blkcnt, const void *src)
{
	int ret;
	lbaint_t blocks_todo = blkcnt;
	lbaint_t cur = 0;
	uint32_t max_write_size = 0;
	lbaint_t max_write_blk = 0;
	void * temp_buff;
	FUNC_ENTER();

	struct lu_info_tbl *lu_info = find_lu_info(dev_num);
	if ( NULL == lu_info)
		return 0;
	
	if (0 != (ulong)src % 4) {
		errorf("UFS write source addr not aligned to 4 bytes, it will harm the write efficiency!\n");
		max_write_size = drv_info.max_prdt_cap > SZ_1M ? SZ_1M : drv_info.max_prdt_cap;
		max_write_blk = max_write_size >> lu_info->log2blksz;
		/*malloc can guarantee 8 bytes aligned*/
		temp_buff = malloc(max_write_size);
		if (NULL == temp_buff)
			return 0;
		do {
			cur = (blocks_todo > max_write_blk) ? max_write_blk : blocks_todo;
			memcpy(temp_buff, src, cur << lu_info->log2blksz);
			/* Send Write 10 Command and handle its completion */
			ret = send_scsi_cmd(UFS_OP_WRITE_10, DMA_TO_DEVICE, lu_info, temp_buff, start, cur, 0);
			if (UFS_SUCCESS == ret)
				ret = handle_scsi_completion();
			if (ret != UFS_SUCCESS) {
				free(temp_buff);
				return 0;
			}
	
			blocks_todo -= cur;
			start += cur;
			src += cur << lu_info->log2blksz;
		} while (blocks_todo > 0);
	} else {
		max_write_blk = drv_info.max_prdt_cap >> lu_info->log2blksz;
		do {
			cur = (blocks_todo > max_write_blk) ? max_write_blk : blocks_todo;
			/* Send Write 10 Command and handle its completion */
			ret = send_scsi_cmd(UFS_OP_WRITE_10, DMA_TO_DEVICE, lu_info, src, start, cur, 0);
			if (UFS_SUCCESS == ret)
				ret = handle_scsi_completion();
			if (ret != UFS_SUCCESS)
				return 0;
			
			blocks_todo -= cur;
			start += cur;
			src += cur << lu_info->log2blksz;
		} while (blocks_todo > 0);
	}
	FUNC_EXIT();
	return blkcnt;

}

/*TODO*/
static ulong ufs_erase(int dev_num, lbaint_t start, lbaint_t blkcnt)
{
	int ret;
	lbaint_t blocks_todo = blkcnt;
	lbaint_t cur = blkcnt;
	lbaint_t max_write_blk = 0;
	FUNC_ENTER();

	struct lu_info_tbl *lu_info = find_lu_info(dev_num);
	if ( NULL == lu_info)
		return 0;


	
	FUNC_EXIT();
	return blkcnt;

}

void ufs_register_blk_device(int lun_index, uint32_t log2_blk_sz, uint64_t blk_count, enum lu_bootable type)
{
	drv_info.block_dev[lun_index].if_type = IF_TYPE_UFS;
	drv_info.block_dev[lun_index].dev = lun_index;  /*consider each lun as one block device*/
	if (NONE_BOOT == type)
		drv_info.block_dev[lun_index].part_type = PART_TYPE_EFI;
	else
		drv_info.block_dev[lun_index].part_type = PART_TYPE_UNKNOWN;
	drv_info.block_dev[lun_index].target = 0xFF;
	drv_info.block_dev[lun_index].lun = lun_index;
	drv_info.block_dev[lun_index].type = DEV_TYPE_UNKNOWN;
	drv_info.block_dev[lun_index].removable = 1;
	drv_info.block_dev[lun_index].lba = blk_count;
	drv_info.block_dev[lun_index].blksz = 1 << log2_blk_sz;
	drv_info.block_dev[lun_index].log2blksz = log2_blk_sz;
	drv_info.block_dev[lun_index].block_read = ufs_bread;
	drv_info.block_dev[lun_index].block_write = ufs_bwrite;
	drv_info.block_dev[lun_index].block_erase = ufs_erase;
	return;
}

void ufs_update_native(void)
{
	int lun_index = 0;
	int ret = 0;
	int id = 0;
	uint64_t block_count = 0;
	uint32_t segment_size = 0;
	struct ufs_dev_desc_tbl *dev_desc;
	struct ufs_geo_desc_tbl *geo_desc;
	struct ufs_unit_desc_tbl *unit_desc;

	drv_info.prdt_entry_size = PRDT_BUFFER_SIZE;
	drv_info.max_prdt_cap =  TOTAL_PRDT_CAP;
	ret = get_descriptor_data((void **)(&dev_desc), DEVICE_DESC, 0);
	if (UFS_SUCCESS != ret) {
		errorf("get device descriptor data fail!\n");
		return;
	}
	drv_info.unit_offset = dev_desc->bUD0BaseOffset;
	drv_info.unit_length = dev_desc->bUDConfigPLength;
	drv_info.lu_num = dev_desc->bNumberLU;
	drv_info.wlu_num = dev_desc->bNumberWLU;
	drv_info.boot_enable = dev_desc->bBootEnable;
	drv_info.wSpecVersion = be16_to_cpu(dev_desc->wSpecVersion);

	ret = get_descriptor_data((void **)(&geo_desc), GEOMETRY_DESC, 0);
	if (UFS_SUCCESS != ret) {
		errorf("get geometry descriptor data fail!\n");
		return;
	}
	segment_size = be32_to_cpu(geo_desc->dSegmentSize);
	drv_info.alloc_unit_sz = geo_desc->bAllocationUnitSize * segment_size;
	drv_info.dev_total_cap = geo_desc->qTotalRawDeviceCapacity;
	
	for (lun_index = 0; lun_index < UFS_MAX_LU_NUM; lun_index++)	{
		memset(&drv_info.block_dev[lun_index], 0 ,sizeof(block_dev_desc_t));
		memset(&drv_info.lu_info[lun_index], 0 ,sizeof(struct lu_info_tbl));
		ret = get_descriptor_data((void **)(&unit_desc), UNIT_DESC, lun_index);
		if (UFS_SUCCESS != ret)
			continue;

		block_count = be64_to_cpu(unit_desc->qLogicalBlockCount);
		if (0 != block_count) {
			drv_info.lu_info[id].lu_index = lun_index;
			drv_info.lu_info[id].blkcnt = block_count;
			drv_info.lu_info[id].log2blksz = unit_desc->bLogicalBlockSize;
			drv_info.lu_info[id].memory_type = unit_desc->bMemoryType;
			drv_info.lu_info[id].bootable = unit_desc->bBootLunID;
			id++;
			ufs_register_blk_device(lun_index, unit_desc->bLogicalBlockSize, block_count, unit_desc->bBootLunID);
		}

		
	}
	
}


block_dev_desc_t *ufs_get_dev(int dev)
{
	if (dev >= UFS_MAX_DEV_NUM)
		return NULL;

	if (IF_TYPE_UFS == drv_info.block_dev[dev].type)
		return &drv_info.block_dev[dev];

	return NULL;
}

int ufs_select_hwpart(int dev_num, int hwpart)
{
	if (0 == dev_num)
		drv_info.active_lun = hwpart;
	else
		drv_info.active_lun = 2;
	return 0;

}


