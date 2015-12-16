#include <common.h>
#include <part.h>
#include <sparse_format.h>
#include "sparse_crc32.h"


typedef struct _emmc_handle
{
	u8  cardPartiton;
	u32 startBlock;
	u32 offset;
	disk_partition_t info;
	block_dev_desc_t *pdev;
}T_Emmc_Handle;

#define COPY_BUF_SIZE (1024*1024)
#define EMMC_BLOCK_SIZE 512
u8 copybuf[COPY_BUF_SIZE];

static unsigned int g_buf_index = 0;
static T_Emmc_Handle g_emmc_handle;

#define SPARSE_HEADER_MAJOR_VER 1
#define SPARSE_HEADER_LEN       (sizeof(sparse_header_t))
#define CHUNK_HEADER_LEN (sizeof(chunk_header_t))


/* modify for download very big size ext4 image */
typedef enum EXT4_DL_STATUS_DEF
{
	START = 0,
	END
} EXT4_DL_STATUS_E;

static sparse_header_t sparse_header;
static EXT4_DL_STATUS_E download_status = END;
static unsigned long current_chunks = 0;
static u32 total_blocks = 0;
static unsigned int gbufindex = 0;

static int read_all(void *buf_src, u32 src_index, void *buf_dest, size_t len)
{
	/*if (gbufindex)
		printf("%s g_buf_index = %d len = %d\n", __FUNCTION__, gbufindex + g_buf_index, len);
	else
		printf("%s g_buf_index = %d len = %d\n", __FUNCTION__, g_buf_index, len);*/
	memcpy(buf_dest, (void*)((u32)buf_src + src_index), len);
	g_buf_index += len;
	return len;
}

static int preread_chunk(void *buf_src, u32 src_index, void *buf_dest, size_t len)
{
	memcpy(buf_dest, (void*)((u32)buf_src + src_index), len);

	return len;
}

#if 0
static int write_all(T_Emmc_Handle *p_handle, void *buf, size_t len)
{
	u32 blocknum;
	block_dev_desc_t *pdev = p_handle->pdev;

	if (len % EMMC_BLOCK_SIZE) {
		blocknum = len / EMMC_BLOCK_SIZE + 1;
	} else {
		blocknum = len / EMMC_BLOCK_SIZE;
	}

	/*printf("startBlock = %d offset = %d blocknum = %d\n", p_handle->startBlock, p_handle->offset, blocknum);*/
	if (pdev->block_write(pdev->dev, p_handle->startBlock + p_handle->offset, blocknum, (unsigned char *)buf) == blocknum) {
		p_handle->offset += blocknum;
		return len;	
	}else{
		return 0;
	}
}

int process_raw_chunk(void *buf, T_Emmc_Handle *p_handle, u32 blocks, u32 blk_sz, u32 *crc32)
{
	u64 len = (u64)blocks * blk_sz;
	int ret;
	int chunk;

	while (len) {
		chunk = (len > COPY_BUF_SIZE) ? COPY_BUF_SIZE : len;
		ret = read_all(buf, g_buf_index, copybuf, chunk);
		if (ret != chunk) {
			printf("read returned an error copying a raw chunk: %d %d\n",
					ret, chunk);
			return -1;
		}
		*crc32 = sparse_crc32(*crc32, copybuf, chunk);
		ret = write_all(p_handle, copybuf, chunk);
		if (ret != chunk) {
			printf("write returned an error copying a raw chunk\n");
			return -1;
		}
		len -= chunk;
	}

	return blocks;
}


int process_fill_chunk(void *buf, T_Emmc_Handle *p_handle, u32 blocks, u32 blk_sz, u32 *crc32)
{
	u64 len = (u64)blocks * blk_sz;
	int ret;
	int chunk;
	u32 fill_val;
	u32 *fillbuf;
	unsigned int i;

	/* Fill copy_buf with the fill value */
	ret = read_all(buf, g_buf_index, &fill_val, sizeof(fill_val));
	fillbuf = (u32 *)copybuf;
	for (i = 0; i < (COPY_BUF_SIZE / sizeof(fill_val)); i++) {
		fillbuf[i] = fill_val;
	}

	while (len) {
		chunk = (len > COPY_BUF_SIZE) ? COPY_BUF_SIZE : len;
		*crc32 = sparse_crc32(*crc32, copybuf, chunk);
		ret = write_all(p_handle, copybuf, chunk);
		if (ret != chunk) {
			printf("write returned an error copying a raw chunk\n");
			return -1;
		}
		len -= chunk;
	}

	return blocks;
}

int process_skip_chunk(T_Emmc_Handle *p_handle, u32 blocks, u32 blk_sz, u32 *crc32)
{
	/* len needs to be 64 bits, as the sparse file specifies the skip amount
	 * as a 32 bit value of blocks.
	 */
	u64 len = (u64)blocks * blk_sz;
	p_handle->offset += len/EMMC_BLOCK_SIZE;

	return blocks;
}
#endif

int process_crc32_chunk(void *buf, u32 crc32)
{
	u32 file_crc32;
	int ret;

	ret = read_all(buf, g_buf_index, &file_crc32, 4);
	if (ret != 4) {
		errorf("read returned an error copying a crc32 chunk\n");
		return -1;
	}

	if (file_crc32 != crc32) {
		errorf("computed crc32 of 0x%8.8x, expected 0x%8.8x\n",
			 crc32, file_crc32);
		return -1;
	}

	return 0;
}

/* -1 : error; x : buf is finished to write */
int write_sparse_img(uchar * partname, u8* buf, unsigned long length)
{
	unsigned int i;
	unsigned int j;
	chunk_header_t chunk_header;
	u32 crc32 = 0;
	int ret;
	static uint64_t offset = 0;
	uint64_t chunk_len = 0;
	int chunk;
	u32 fill_val;
	u32 *fillbuf;

	g_buf_index = 0;

	if (download_status == END) {
		current_chunks = 0;
		total_blocks = 0;
		offset = 0;
		memset(&sparse_header, 0, sizeof(sparse_header_t));
		ret = read_all(buf, g_buf_index, &sparse_header, sizeof(sparse_header));
		debugf("sparse_header file_hdr_sz = %d chunk_hdr_sz = %d blk_sz = %d total_blks = %d total_chunks = %d\n", 
			sparse_header.file_hdr_sz, sparse_header.chunk_hdr_sz, sparse_header.blk_sz, 
			sparse_header.total_blks, sparse_header.total_chunks);
		if (ret != sizeof(sparse_header)) {
			errorf("Error reading sparse file header\n");
			goto fail;
		}
		if (sparse_header.magic != SPARSE_HEADER_MAGIC) {
			errorf("Bad magic\n");
			goto fail;
		}
		if (sparse_header.major_version != SPARSE_HEADER_MAJOR_VER) {
			errorf("Unknown major version number\n");
			goto fail;
		}
		if (sparse_header.file_hdr_sz > SPARSE_HEADER_LEN) {
			/* Skip the remaining bytes in a header that is longer than
		 	* we expected.
			 */
			g_buf_index += (sparse_header.file_hdr_sz - SPARSE_HEADER_LEN);
		}
		download_status = START;
	}

	for (i = current_chunks; i < sparse_header.total_chunks; i++) {
		memset(&chunk_header, 0, sizeof(chunk_header_t));
		if ((g_buf_index + sizeof(chunk_header)) > length) {
			debugf("bufferindex(%d) + sizeof(chunk_header)(%d) exceed length\n", g_buf_index, sizeof(chunk_header));
			current_chunks = i;
			break;
		}

		preread_chunk((void*)buf, g_buf_index, &chunk_header, sizeof(chunk_header));
		debugf("chunk_header.total_sz(%d)\n", chunk_header.total_sz);

		ret = 0;
		switch (chunk_header.chunk_type) {
		case CHUNK_TYPE_RAW:
		case CHUNK_TYPE_FILL:
		case CHUNK_TYPE_DONT_CARE:
		case CHUNK_TYPE_CRC32:
			if ((g_buf_index + chunk_header.total_sz) > length) {
				debugf("bufferindex(%d)+chunk_header.total_sz(%d) exceed  length(%d) \n", g_buf_index, chunk_header.total_sz, length);
				ret = 1;
			}
			break;
		default:
			errorf("Unknown chunk type 0x%4.4x\n", chunk_header.chunk_type);
		}

		if (ret == 1) {
			current_chunks = i;
			break;
		}

		memset(&chunk_header, 0, sizeof(chunk_header_t));
		ret = read_all((void*)buf, g_buf_index, &chunk_header, sizeof(chunk_header));

		if (ret != sizeof(chunk_header)) {
			errorf("Error reading chunk header\n");
			goto fail;
		}
 
		if (sparse_header.chunk_hdr_sz > CHUNK_HEADER_LEN) {
			/* Skip the remaining bytes in a header that is longer than
			 * we expected.*/
			g_buf_index += sparse_header.chunk_hdr_sz - CHUNK_HEADER_LEN;	
		}
		chunk_len = (uint64_t)chunk_header.chunk_sz * sparse_header.blk_sz;
		
		switch (chunk_header.chunk_type) {
		case CHUNK_TYPE_RAW:
			if (chunk_header.total_sz != (sparse_header.chunk_hdr_sz + chunk_len) ) {
				errorf("Bogus chunk size for chunk %d, type Raw\n", i);
				goto fail;
			}

			if (0 != common_raw_write(partname, chunk_len, 0, offset, buf + g_buf_index)) {
				errorf("Write raw chunk fail, trunk_len=0x%x!\n", chunk_len);
				goto fail;
			} else {
				crc32 = sparse_crc32(crc32, buf + g_buf_index, chunk_len);
				g_buf_index += chunk_len;
				offset += chunk_len;
				total_blocks += chunk_header.chunk_sz;
			}
			break;
		case CHUNK_TYPE_FILL:
			if (chunk_header.total_sz != (sparse_header.chunk_hdr_sz + sizeof(fill_val)) ) {
				errorf("Bogus chunk size for chunk %d, type Fill\n", i);
				goto fail;
			}
			/* Fill copy_buf with the fill value */
			ret = read_all(buf, g_buf_index, &fill_val, sizeof(fill_val));
			fillbuf = (u32 *)copybuf;
			for (j = 0; j < (COPY_BUF_SIZE / sizeof(fill_val)); j++)
				fillbuf[j] = fill_val;

			while (chunk_len) {
				chunk = (chunk_len > COPY_BUF_SIZE) ? COPY_BUF_SIZE : chunk_len;
				if (0 != common_raw_write(partname, chunk, 0, offset, copybuf)) {
					errorf("Write fill chunk fail, trunk_len=0x%x!\n", chunk_len);
					goto fail;
				} 
				crc32 = sparse_crc32(crc32, copybuf, chunk);
				offset += chunk;
				chunk_len -= chunk;
			}
			total_blocks += chunk_header.chunk_sz;
			break;
		case CHUNK_TYPE_DONT_CARE:
			if (chunk_header.total_sz != sparse_header.chunk_hdr_sz) {
				errorf("Bogus chunk size for chunk %d, type Dont Care\n", i);
				goto fail;
			}
			offset += chunk_len;
			total_blocks += chunk_header.chunk_sz;
			break;
		case CHUNK_TYPE_CRC32:
			if (process_crc32_chunk((void*)buf, crc32) != 0) {
				goto fail;
			}
			break;
		default:
			errorf("Unknown chunk type 0x%4.4x\n", chunk_header.chunk_type);
		}

	}
	//printf("offset = %d g_buf_index = %d  length = %d  Wrote %d blocks, expected to write %d blocks\n", g_emmc_handle.offset, g_buf_index, length, total_blocks, sparse_header.total_blks);
	if (sparse_header.total_blks != total_blocks) {
		return g_buf_index;
	} else {
		download_status = END;
		current_chunks = 0;
		offset = 0;
		total_blocks = 0;
		return 0;
	}

fail:
	return -1;
}


