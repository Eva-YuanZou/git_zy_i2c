/**
 * @file lig_basei2c_master.h
 * @author Zou Yuan
 * @date Jun./05/2017
 * @brief header file
 * function: 使用普通GPIO模拟I2C主机发送和接收,对于不同芯片，
 *	          需要实现头文件中定义的5个基本函数;
 *
 */

#ifndef _LIG_BASEI2C_MASTER_H_
#define _LIG_BASEI2C_MASTER_H_

#include "lig_types.h"

#define LIG_BASEI2C_MASTER_FILE_VERSION         0x010000
//////////////////////////////////////////////////////////////
//Macro & Struct & ENUM
//------------------------------------------------------------
//ERROR Code
#define LIG_BI2C_MASTER_ERROR_UNKNOW            0xFF1
#define LIG_BI2C_MASTER_ERROR_PARAM             0xFF2
#define LIG_BI2C_MASTER_ERROR_NOACK             0xFF3
#define LIG_BI2C_MASTER_ERROR_OVERFLOW          0xFF4

//////////////////////////////////////////////////////////////
//Globe Parameter

//////////////////////////////////////////////////////////////
//Function
EXTERNAL void lig_bi2c_master_Init(void);

/*standard I2C read & write function
* paramter: UINT8 slave_device_addr - 从设备地址;
*           UINT8* wbuffer - 待写入从设备的数据缓冲区;
*           INT32  wlength - 待写入从设备数据长度;
*           UINT8* rbuffer - 待接收
*           BOOL   bover_closeiic - 是否在传输数据完成后，停止IIC过程，即调用IIC_STOP;
* return: 发送或接收数据的个数，如果为错误码或是负数，表示错误；          
*/
EXTERNAL INT32 lig_bi2c_master_write_bytes(UINT8 slave_device_addr, UINT8* wbuffer, INT32 wlength, BOOL bover_closeiic);
EXTERNAL INT32 lig_bi2c_master_read_bytes(UINT8 slave_device_addr, UINT8* rbuffer, INT32 rlength, BOOL bover_closeiic);

/* Send/Receive Data
* paramter: UINT8 slave_device_addr - 从设备地址;
*	    UINT8 page_id  - 寄存器页/段选； 如果page_id == 0xff, 表示忽略此值，不发送此值;
*	                     or 在实现函数的模块中，定义一个特殊值表示忽略；如定义LIG_I2C_PAGE_IGNORE_FLAG_VALUE为0xFF;
*           UINT8* reg_addr - 寄存器地址, 与reg_addr_len配合使用;
*           UINT8 reg_addr_len - 寄存器地址长度，1表示8位寄存器，2表示16位寄存器；
*           BOOL b_reg_addr_bigmode - 是否是大端模式，如果是则高位在前，否则低位在前；
*           UINT8* send_buffer / recv_buffer - 收发数据的缓存区；
*           INT32 send_len / recv_len - 数据缓存区长度，对于发送函数为发送数据长度，对于接收函数为想获取数据的长度；
* return: 发送或接收数据的个数，如果为错误码或是负数，表示错误；
*
* 寄存器地址说明：如设置寄存器0x1234
* reg_addr[2] = {0x12, 0x34};
* reg_addr_len = 2;
* if b_reg_addr_bigmode == TRUE
*    i2c_buffer[0] = reg_addr[0];
*    i2c_buffer[1] = reg_addr[1];
* else
*    i2c_buffer[0] = reg_addr[1];
*    i2c_buffer[1] = reg_addr[0];
* end
*/
EXTERNAL INT32 lig_bi2c_master_send_bytes(UINT8 slave_device_addr, UINT8 page_id,
                            		  UINT8* reg_addr, UINT8 reg_addr_len, BOOL b_reg_addr_bigmode,
                            		  UINT8* send_buffer, INT32 send_len);
EXTERNAL INT32 lig_bi2c_master_recv_bytes(UINT8 slave_device_addr, UINT8 page_id,
                            		  UINT8* reg_addr, UINT8 reg_addr_len, BOOL b_reg_addr_bigmode,
                            		  UINT8* recv_buffer, INT32 recv_len);


#endif //_LIG_BASEI2C_MASTER_H_



