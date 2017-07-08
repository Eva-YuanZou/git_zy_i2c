/**
 * @file lig_basei2c_master.h
 * @author Zou Yuan
 * @date Jun./05/2017
 * @brief header file
 * function: ʹ����ͨGPIOģ��I2C�������ͺͽ���,���ڲ�ͬоƬ��
 *	          ��Ҫʵ��ͷ�ļ��ж����5����������;
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
* paramter: UINT8 slave_device_addr - ���豸��ַ;
*           UINT8* wbuffer - ��д����豸�����ݻ�����;
*           INT32  wlength - ��д����豸���ݳ���;
*           UINT8* rbuffer - ������
*           BOOL   bover_closeiic - �Ƿ��ڴ���������ɺ�ֹͣIIC���̣�������IIC_STOP;
* return: ���ͻ�������ݵĸ��������Ϊ��������Ǹ�������ʾ����          
*/
EXTERNAL INT32 lig_bi2c_master_write_bytes(UINT8 slave_device_addr, UINT8* wbuffer, INT32 wlength, BOOL bover_closeiic);
EXTERNAL INT32 lig_bi2c_master_read_bytes(UINT8 slave_device_addr, UINT8* rbuffer, INT32 rlength, BOOL bover_closeiic);

/* Send/Receive Data
* paramter: UINT8 slave_device_addr - ���豸��ַ;
*	    UINT8 page_id  - �Ĵ���ҳ/��ѡ�� ���page_id == 0xff, ��ʾ���Դ�ֵ�������ʹ�ֵ;
*	                     or ��ʵ�ֺ�����ģ���У�����һ������ֵ��ʾ���ԣ��綨��LIG_I2C_PAGE_IGNORE_FLAG_VALUEΪ0xFF;
*           UINT8* reg_addr - �Ĵ�����ַ, ��reg_addr_len���ʹ��;
*           UINT8 reg_addr_len - �Ĵ�����ַ���ȣ�1��ʾ8λ�Ĵ�����2��ʾ16λ�Ĵ�����
*           BOOL b_reg_addr_bigmode - �Ƿ��Ǵ��ģʽ����������λ��ǰ�������λ��ǰ��
*           UINT8* send_buffer / recv_buffer - �շ����ݵĻ�������
*           INT32 send_len / recv_len - ���ݻ��������ȣ����ڷ��ͺ���Ϊ�������ݳ��ȣ����ڽ��պ���Ϊ���ȡ���ݵĳ��ȣ�
* return: ���ͻ�������ݵĸ��������Ϊ��������Ǹ�������ʾ����
*
* �Ĵ�����ַ˵���������üĴ���0x1234
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



