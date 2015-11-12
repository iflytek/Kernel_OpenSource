#ifndef DL_CMD_PROC_H
#define DL_CMD_PROC_H
#include "packet.h"

#define PARTITION_SIZE_LENGTH          (4)
#define MAX_PARTITION_NAME_SIZE   (36)
#define REPARTITION_UNIT_LENGTH    (MAX_PARTITION_NAME_SIZE *2 + PARTITION_SIZE_LENGTH)

typedef enum OPERATE_STATUS {
	OPERATE_SUCCESS = 1,
	OPERATE_SYSTEM_ERROR,
	OPERATE_DEVICE_INIT_ERROR,
	OPERATE_INVALID_DEVICE_SIZE,
	OPERATE_INCOMPATIBLE_PART,
	OPERATE_INVALID_ADDR,
	OPERATE_INVALID_SIZE,
	OPERATE_WRITE_ERROR,
	OPERATE_CHECKSUM_DIFF
} OPERATE_STATUS;

int dl_cmd_write_start(dl_packet_t * packet, void *arg);
int dl_cmd_write_midst(dl_packet_t * packet, void *arg);
int dl_cmd_write_end(dl_packet_t * packet, void *arg);
int dl_cmd_read_start(dl_packet_t * packet, void *arg);
int dl_cmd_read_midst(dl_packet_t * packet, void *arg);
int dl_cmd_read_end(dl_packet_t * packet, void *arg);
int dl_cmd_erase(dl_packet_t * packet, void *arg);
int dl_cmd_repartition(dl_packet_t * pakcet, void *arg);
int dl_cmd_reboot (dl_packet_t *pakcet, void *arg);
int dl_cmd_read_mcptype(dl_packet_t * packet, void *arg);

#endif /*DL_CMD_PROC_H */
