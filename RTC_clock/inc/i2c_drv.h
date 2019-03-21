#ifndef _I2C_DRV_H
#define _I2C_DRV_H

#define F_MASTER_MHZ    2UL
#define F_MASTER_HZ     2000000UL 

//��������� ���������� �������� � i2c
typedef enum {
    I2C_SUCCESS = 0,
    I2C_TIMEOUT,
    I2C_ERROR
} t_i2c_status;

// ������������� I2C ����������                                   
extern void i2c_master_init(unsigned long f_master_hz, unsigned long f_i2c_hz);

// ������ �������� slave-����������
extern t_i2c_status  i2c_wr_reg(unsigned char address, 
                               unsigned char reg_addr,
                               char * data, 
                               unsigned char length);

// ������ �������� slave-����������
extern t_i2c_status  i2c_rd_reg(unsigned char address, 
                               unsigned char reg_addr,
                               char * data, 
                               unsigned char length);

#endif