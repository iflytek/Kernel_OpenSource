#include <common.h>

#include <libavb.h>
#include <avb/uboot_avb_pub.h>


AvbOps g_avb_ops;
AvbSlotVerifyData* avb_slot_data[2] = {NULL, NULL};

int avb_check_bootimage(void)
{
     AvbSlotVerifyResult verify_result;
     const char* slot_suffixes[2] = {"", ""};

    //const char* const* requested_partitions = {"boot"};
    const char* requested_partitions[] = {"boot", NULL};
    bool allow_verification_error = true;
    AvbSlotVerifyData * out_slot_data = NULL;

    //load data and check
      verify_result = avb_slot_verify(&g_avb_ops,
                                      requested_partitions,
                                      slot_suffixes[0],
                                      allow_verification_error,
                                      &avb_slot_data[0]);
	debug(">>>>>>>>>>>>>>>>  avb verify_bootimage result is %d.\n", verify_result);

	debug("slot_data[0] is 0x%x.\n", avb_slot_data[0]);
	debug("slot_data[1] is 0x%x.\n", avb_slot_data[1]);

	out_slot_data = avb_slot_data[0];

#ifdef VBOOT_DEBUG_DUMP
	/*add by jason, for debug print the cmdline info*/
	debug("Well, at the end, so we get the restult.\n");
	{
		int i = 0;

		char* tmp = out_slot_data->cmdline;
		int len = strlen(tmp);
		char tmp_buffer[128];

		if(!tmp)
		{
			debug("slot_data->cmdline is null\n");
			return -1;
		}

		debug("len is %d\n", len);
		memset(tmp_buffer, 0, sizeof(tmp_buffer));

		for(i=0;i<32;i++)
		{
		
			if(	(i*64) < len)
			{
				debug("i:%d\n", i);
				memset(tmp_buffer, 0, sizeof(tmp_buffer));
				memcpy(tmp_buffer, (tmp), 64);
				debug("%s", tmp_buffer);
			}
			else if(((i+1)*64) < len)
			{
				debug("i:%d\n", i);
				memset(tmp_buffer, 0, sizeof(tmp_buffer));
				memcpy(tmp_buffer, (tmp), (len - i*64));
				debug("%s", tmp_buffer);
			}
			else
			{
				break;
			}

			tmp = tmp + 64;
		}
	}
#endif
}
