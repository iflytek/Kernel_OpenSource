
#include <crypto/sprdrsa.h>
#include <secureboot/sprdsec_header.h>
#include <secureboot/sec_string.h>
#include <crypto/sprdsha.h>
#include <secureboot/sec_common.h>
#include <secureboot/sec_efuse_api.h>

#define SEC_DEBUG

#ifdef SEC_DEBUG
#define secf(fmt, args...) do { printf("%s(): ", __func__); printf(fmt, ##args); } while (0)
#else
#define secf(fmt, args...)
#endif

/*####for anti-rollback######*/
#define TRUSTED_VERSION_MAX  31
#define NON_TRUSTED_VERSION_MAX 223
#define TRUSTED_FIRMWARE 1
#define NON_TRUSTED_FIRMWARE 0
#define VERSION_INIT_VALUE 0xFFFF//to mark read efuse first time
static uint32_t Trusted_rollback_version = VERSION_INIT_VALUE;
static uint32_t Non_Trusted_rollback_version = VERSION_INIT_VALUE;
#define CHECK_BINARY_NUM	10
static uint32_t Trusted_update_version[CHECK_BINARY_NUM] = {0};
static uint32_t Non_Trusted_update_version[CHECK_BINARY_NUM] = {0};
static uint32_t Trusted_update_index = 0;
static uint32_t Non_Trusted_update_index = 0;

/*####for anti-rollback end######*/
uint8_t to[RSA_KEY_BYTE_LEN_MAX] = { 0 };	// store RSA_PubDec  result
#define MODEM_MAGIC		"SCI1"
#define MODEM_HDR_SIZE		12
#define MODEM_IMG_HDR		0x1
#define MODEM_LAST_HDR		0x100
#define MODEM_SHA1_HDR		0x400
#define MODEM_SHA1_SIZE		20

typedef struct {
	unsigned int type_flags;
	unsigned int offset;
	unsigned int length;
} data_block_header_t;

void dumpHex(const char *title, uint8_t * data, int len)
{
	int i, j;
	int N = len / 16 + 1;
	printf("%s\n", title);
	printf("dumpHex:%d bytes", len);
	for (i = 0; i < N; i++) {
		printf("\r\n");
		for (j = 0; j < 16; j++) {
			if (i * 16 + j >= len)
				goto end;
			printf("%02x", data[i * 16 + j]);
		}
	}
end:	printf("\r\n");
	return;
}

void cal_sha256(uint8_t * input, uint32_t bytes_num, uint8_t * output)
{

	if ((NULL != input) && (NULL != output)) {
		sha256_csum_wd(input, bytes_num, output, NULL);
	} else {
		secf("\r\tthe pointer is error,pls check it\n");
	}
}

uint32_t check_rollback_version(uint32_t version)
{
	uint32_t temp = 1;
	uint32_t i;
	for(i=0;i<32;i++)
	{
		temp*=2;
		if(version == temp-1)
			return 1;
	}
	return 0;
}

void update_sec_version(void)
{
	int i;
	uint32_t nosecure_update_version = 0;

	if(Non_Trusted_update_index)
	{
		nosecure_update_version = Non_Trusted_update_version[0];
		for(i=1;i<Non_Trusted_update_index;i++)
		{
			if(nosecure_update_version != Non_Trusted_update_version[i])
			{
				secf("\r\Nosec rollback version not unified\n");
			}
		}
		//if(check_rollback_version(nosecure_update_version))
		if(nosecure_update_version>Non_Trusted_rollback_version)
		sprd_set_version(SW_VERSION_COUNTER2,nosecure_update_version);
		Non_Trusted_update_index = 0;
		Non_Trusted_rollback_version = 0;
	}
}

bool sprd_anti_rollback(uint32_t certtype,uint32_t certversion)
{
	if(certtype == TRUSTED_FIRMWARE)
	{
		if(Trusted_rollback_version == VERSION_INIT_VALUE){
			sprd_get_version(SW_VERSION_COUNTER1,&Trusted_rollback_version);
			secf("Trusted_rollback_version %d\n", Trusted_rollback_version);
		}
		//version_efuse = version_convert_efuse(curcertptr->version);
		if((certversion < Trusted_rollback_version)||(certversion>TRUSTED_VERSION_MAX)){
			secf("SEC ANTI_ROLLBACK ERR version = %x\n",certversion);
			return false;
		}else{
			Trusted_update_version[Trusted_update_index] = certversion;
			Trusted_update_index++;
		}
	}
	else
	{
		if(certtype == NON_TRUSTED_FIRMWARE)
		{
			if(Non_Trusted_rollback_version == VERSION_INIT_VALUE){
			sprd_get_version(SW_VERSION_COUNTER2,&Non_Trusted_rollback_version);
			secf("Non_Trusted_rollback_version %d\n", Non_Trusted_rollback_version);
			}
			//version_efuse = version_convert_efuse(curcertptr->version);
			if((certversion < Non_Trusted_rollback_version)||(certversion>NON_TRUSTED_VERSION_MAX)){
			secf("NOSEC ANTI_ROLLBACK ERR version = %x\n",certversion);
			return false;
			}else{
			secf("version_efuse %d\n", certversion);
			Non_Trusted_update_version[Non_Trusted_update_index] = certversion;
			Non_Trusted_update_index++;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool sprd_verify_cert(uint8_t * hash_key_precert, uint8_t * hash_data, uint8_t * certptr)
{
	bool ret = false;

	uint8_t certtype = *certptr;

	uint8_t temphash_data[HASH_BYTE_LEN];
    secf("cert type: %d\n",certtype);
	if ((certtype == CERTTYPE_CONTENT) || (certtype == CERTTYPE_KEY)) {

		if (certtype == CERTTYPE_KEY) {

			sprd_keycert *curcertptr = (sprd_keycert *) certptr;

			cal_sha256((uint8_t *) & (curcertptr->pubkey), SPRD_RSAPUBKLEN, temphash_data);

			if (sec_memcmp(hash_data, curcertptr->hash_data, HASH_BYTE_LEN)
			    || sec_memcmp(hash_key_precert, temphash_data, HASH_BYTE_LEN)
			    ) {
				secf("cmp hash of pubk diffent\r\n");
				return false;
			}

			int ret = RSA_PubDec((uint8_t *) & (curcertptr->pubkey.e),
					     curcertptr->pubkey.mod, curcertptr->pubkey.keybit_len,
					     curcertptr->signature, to);

			if (!sec_memcmp(curcertptr->hash_data, to, KEYCERT_HASH_LEN)) {
				secf("\nRSA verify Success\n");
				return true;
			} else {
				secf("\nRSA verify err\n");
				return false;
			}
			ret = sprd_anti_rollback(curcertptr->type,curcertptr->version);
		}

		else if(certtype ==  CERTTYPE_CONTENT) {		//certtype is content

			sprd_contentcert *curcertptr = (sprd_contentcert *) certptr;

			cal_sha256((uint8_t *) & (curcertptr->pubkey), SPRD_RSAPUBKLEN, temphash_data);

			if (sec_memcmp(hash_data, curcertptr->hash_data, HASH_BYTE_LEN)
			    || sec_memcmp(hash_key_precert, temphash_data, HASH_BYTE_LEN)
			    ) {
				secf("cmp hash key diffent\r\n");
				return false;
			}

			int ret = RSA_PubDec((uint8_t *) & (curcertptr->pubkey.e),
					     curcertptr->pubkey.mod, curcertptr->pubkey.keybit_len,
					     curcertptr->signature, to);

			if (!sec_memcmp(curcertptr->hash_data, to, ret)) {
				secf("\nRSA verify Success\n");
				return true;
			} else {
				secf("\nRSA verify Failed\n");
				return false;
			}
			ret = sprd_anti_rollback(curcertptr->type,curcertptr->version);
		}

	} else {
		secf("invalid cert type %d!!", certtype);
		ret = false;
	}

	return ret;
}


/*
 *  this function compare the first four bytes in image and return 1 if equals to
 *  MODEM_MAGIC
 */
static int is_packed_modem_image(uint8_t *data)
{
	if (sec_memcmp(data, (uint8_t*)MODEM_MAGIC, sizeof(MODEM_MAGIC)))
		return 0;
	return 1;
}

/*
 *  this function parse new packed modem image and return modem code offset and length
 */
static void get_modem_info(uint8_t *data, uint32_t *code_offset, uint32_t *code_len)
{
	uint32_t offset = 0, hdr_offset = 0, length = 0;
	uint8_t hdr_buf[MODEM_HDR_SIZE << 3] = {0};
	uint8_t read_len;
	uint8_t result = 0; // 0:OK, 1:not find, 2:some error occur
	data_block_header_t *hdr_ptr = NULL;

	read_len = sizeof(hdr_buf);
	sec_memcpy(hdr_buf, data, read_len);

		do {
			if(!hdr_offset) {
				hdr_ptr = (data_block_header_t*)hdr_buf + 1;
				hdr_offset = MODEM_HDR_SIZE;
			} else {
				hdr_ptr = (data_block_header_t*)hdr_buf;
			}

			while(!(hdr_ptr->type_flags & MODEM_IMG_HDR)) {
				hdr_ptr ++;
				hdr_offset += MODEM_HDR_SIZE;
				if(read_len == ((unsigned char*)hdr_ptr - hdr_buf)) {
					result = 1;
					break;
				}

				if(hdr_ptr->type_flags & MODEM_LAST_HDR) {
					result = 2;
					secf("no modem image, error image header!!!\n");
					break;
				}
			};

			if(result != 1) {
				break;
			}

		} while(1);

		if(!result) {
			offset = hdr_ptr->offset;
			if(hdr_ptr->type_flags & MODEM_SHA1_HDR) {
				offset += MODEM_SHA1_SIZE;
			}
			length = hdr_ptr->length;
		}

	*code_offset = offset;
	*code_len = length;
}


/*
uint8_t *hash_key_precert: hash of of pub key in pre cert or OTP, used to verify the pub key in current image
uint8_t *imgbuf: current image need to verify
*/

uint8_t *sprd_get_sechdr_addr(uint8_t * buf)
{
	if (NULL == buf) {
		secf("\r\t input of get_sechdr_Addr err\n");
	}
	sys_img_header *imghdr = (sys_img_header *) buf;
	uint8_t *sechdr = buf + imghdr->mImgSize + sizeof(sys_img_header);
	return sechdr;
}

uint8_t *sprd_get_code_addr(uint8_t * buf)
{
	sprdsignedimageheader *sechdr_addr = (sprdsignedimageheader *) sprd_get_sechdr_addr(buf);
	uint8_t *code_addr = buf + sechdr_addr->payload_offset;
	return code_addr;
}

uint8_t *sprd_get_cert_addr(uint8_t * buf)
{
	sprdsignedimageheader *sechdr_addr = (sprdsignedimageheader *) sprd_get_sechdr_addr(buf);
	uint8_t *cert_addr = buf + sechdr_addr->cert_offset;
	return cert_addr;
}

bool sprd_verify_img(uint8_t * hash_key_precert, uint8_t * imgbuf)
{
	sprdsignedimageheader *imghdr = (sprdsignedimageheader *) sprd_get_sechdr_addr(imgbuf);
	uint8_t *code_addr = sprd_get_code_addr(imgbuf);
	if (is_packed_modem_image(code_addr)) {
		secf("the image is new packed modem image\n");
		uint32_t modem_offset = 0;
		uint32_t modem_len = 0;
		get_modem_info(code_addr, &modem_offset, &modem_len);
		secf("modem offset: %d, length: %d\n", modem_offset, modem_len);
		code_addr += modem_offset;
	}
	uint8_t soft_hash_data[HASH_BYTE_LEN] = {0};
	cal_sha256(code_addr, imghdr->payload_size, soft_hash_data);
	uint8_t *curcertptr = sprd_get_cert_addr(imgbuf);

	bool result = sprd_verify_cert(hash_key_precert, (uint8_t *) soft_hash_data, curcertptr);

	return result;
}

 void sprd_get_hash_key(uint8_t * load_buf,uint8_t *hash_key)
{
	sprd_keycert *certtype = (sprd_keycert *)sprd_get_cert_addr((uint8_t *)load_buf);
	sec_memcpy(hash_key, certtype->hash_key, HASH_BYTE_LEN);
}

void sprd_get_pubk(uint8_t *load_buf,uint8_t *pubk)
{
	sprd_keycert *certtype = (sprd_keycert*)sprd_get_cert_addr((uint8_t*)load_buf);
	sec_memcpy(pubk, (uint8_t*)&certtype->pubkey, sizeof(sprd_rsapubkey));
}
void sprd_secure_check(uint8_t * current_img_addr,uint8_t * data_header)
{

	/*get current image's hash key & verify the downloading img */
	uint8_t *hash_key_next = NULL;
	uint8_t hash_key[HASH_BYTE_LEN];
	sec_memset(hash_key, 0, HASH_BYTE_LEN);
	sprd_get_hash_key(current_img_addr,hash_key);    // becarefull!    in chipram the firt parameter is IRAM,in uboot the first is uboot load addr

	hash_key_next = hash_key;
#if 0
	sprd_keycert *certtype = (sprd_keycert *) sprd_get_cert_addr(data_header);

	if (CERTTYPE_KEY == certtype->certtype){
		sprd_get_hash_key(IRAM_BEGIN,hash_key);
		hash_key_next = hash_key;

	}else{
		hash_key_next = NULL;
	}
#endif

	if (false == sprd_verify_img(hash_key_next, data_header)) {
		secf("\r\t sprd_secure_check err\n");
		while (1) ;
	}
}

void sprd_dl_check(uint8_t * hash,uint8_t * data_header)
{

	/*get current image's hash key & verify the downloading img */
	uint8_t *hash_key_next = NULL;
	uint8_t hash_key[HASH_BYTE_LEN];
	sec_memset(hash_key, 0, HASH_BYTE_LEN);
	//sprd_get_hash_key(current_img_addr,hash_key);    // becarefull!    in chipram the firt parameter is IRAM,in uboot the first is uboot load addr

	hash_key_next = hash;

	if (false == sprd_verify_img(hash_key_next, data_header)) {
		secf("\r\t sprd_secure_check err\n");
		while (1) ;
	}
}


void sprd_dl_verify(uint8_t *name,uint8_t * header,uint8_t *code)
{
	uint8_t pubk[SPRD_RSAPUBKLEN] = {0};
	uint8_t hash[HASH_BYTE_LEN] = {0};

	printf("%s,name: %s enter\r\n",__FUNCTION__,name);

	secf("name: %s enter\r\n",name);
	if(name){/*splloader is spl partiton, uboot is uboot partition which img the pubk hash stored*/
		if(strcmp("splloader",name) == 0 || strcmp("fdl1",name) == 0){
		sprd_secure_check(IRAM_BEGIN,header); /*add first param*/
		}else if(strcmp("fdl2",name) == 0){
			sprd_secure_check((CONFIG_SYS_TEXT_BASE - 0x200),header);// first para is the addr
		}else if(strcmp("splloader0",name) == 0){
			/*if secboot is enable, the romcode will verify fdl1's pubk,sectool sign spl and fdl1 use the same pubkkey,so wo can use the pubk's hash to verify spl's pubk*/
			sprd_get_pubk(IRAM_BEGIN, pubk);
			cal_sha256(pubk, SPRD_RSAPUBKLEN, hash);
			sprd_dl_check(hash,header);
		}else if(strcmp("uboot",name) == 0){
			sprd_secure_check(CONFIG_SYS_TEXT_BASE - 0x200,header); //for fastboot
		}


	}
}
void sprd_fb_secure_verify(uint8_t *name,uint8_t * header,uint8_t *code)
{
	uint8_t pubk[SPRD_RSAPUBKLEN] = {0};
	uint8_t hash[HASH_BYTE_LEN] = {0};

	//printf("%s,name: %s enter\r\n",__FUNCTION__,name);

	secf("name: %s enter\r\n",name);
	if(name){/*splloader is spl partiton, uboot is uboot partition which img the pubk hash stored*/
		if(strcmp("splloader",name) == 0 || strcmp("fdl1",name) == 0){
		load_spl();
		#if defined (TOS_VERIFY_SEC)
		secboot_param_set(header,&img_verify_info);
		sprd_get_hash_key(IRAM_BEGIN,img_verify_info.pubkeyhash);
		uboot_verify_img(&img_verify_info,sizeof(imgToVerifyInfo));
		#else
		sprd_secure_check(IRAM_BEGIN,header);
		#endif
		}else if(strcmp("fdl2",name) == 0|| strcmp("uboot",name) == 0){
			load_uboot();
			#if defined (TOS_VERIFY_SEC)
			secboot_param_set(header,&img_verify_info);
			sprd_get_hash_key((CONFIG_SYS_TEXT_BASE - 0x200),img_verify_info.pubkeyhash);
			uboot_verify_img(&img_verify_info,sizeof(imgToVerifyInfo));
			#else
			sprd_secure_check((CONFIG_SYS_TEXT_BASE - 0x200),header);// first para is the addr
			#endif
		}else if(strcmp("splloader0",name) == 0){
			/*if secboot is enable, the romcode will verify fdl1's pubk,sectool sign spl and fdl1 use the same pubkkey,so wo can use the pubk's hash to verify spl's pubk*/
			load_spl();
			#if defined (TOS_VERIFY_SEC)
			secboot_param_set(header,&img_verify_info);
			sprd_get_pubk(IRAM_BEGIN, pubk);
			cal_sha256(pubk, SPRD_RSAPUBKLEN, img_verify_info.pubkeyhash);
			uboot_verify_img(&img_verify_info,sizeof(imgToVerifyInfo));
			#else
			sprd_get_pubk(IRAM_BEGIN, pubk);
			cal_sha256(pubk, SPRD_RSAPUBKLEN, hash);
			sprd_dl_check(hash,header);
			#endif
		}
	}
}

void sprd_calc_hbk(void)
{
	unsigned int t_readValue = 0;
	char  *boot_mode_type_str;

  	boot_mode_type_str = getenv("bootmode");
  	if (NULL != boot_mode_type_str)
  	{
		if(!strncmp(boot_mode_type_str, "cali", 4))
		{
		/**read spl image**/
		#ifdef TOS_TRUSTY
		img_verify_info_tos->img_addr= (uint8_t*)(ARG_START_ADRESS+PAGE_ALIGN);
		memcpy((unsigned char *)img_verify_info_tos->img_addr,IRAM_BEGIN,SPL_IMAGE_SIZE);
		//common_raw_read("splloader",SPL_IMAGE_SIZE, 0, (unsigned char *)img_verify_info_tos->img_addr);
		img_verify_info_tos->img_len = SPL_IMAGE_SIZE;
		img_verify_info_tos->flag = SPRD_FLAG;
		printf("sprd_calc_hbk p1:%x, p2:%x,P3:%x\n",img_verify_info_tos->img_addr,img_verify_info_tos->img_len,img_verify_info_tos->flag);
		uboot_get_hbk(img_verify_info_tos,sizeof(imgToVerifyInfo));
		#else
		#endif
		}
		else
		{
		printf("no calibration mode\n");
		}
  	}
}

