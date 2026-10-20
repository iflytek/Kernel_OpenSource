#ifndef __AW37504_H__
#define __AW37504_H__



#define READ_BUF_MAX_SIZE	64
#define WRITE_BUF_MAX_SIZE	64


struct aw37504 {
	struct device *dev;
	struct i2c_client *i2c_client; //需要

	struct workqueue_struct *wq;
	struct work_struct work;

	u8 i2c_wbuf[WRITE_BUF_MAX_SIZE]; //需要 i2c写缓冲区
	u8 i2c_rbuf[READ_BUF_MAX_SIZE]; //需要 i2c读缓冲区
};

#endif
