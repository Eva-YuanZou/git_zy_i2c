/*!
 * \file lig_basei2c_master_c51.c
 *
 */

#include "lig_basei2c_master.h"
#include <reg52.h>
#include <intrins.h>

#define LIG_MI2C_C51_TRANSBUFFER_SIZE 			64

#define LIG_MI2C_C51_MODE_LARGE				1
#ifdef LIG_MI2C_C51_MODE_LARGE
	#define C51_LARGE		large
#else
	#define C51_LARGE		//nothing
#endif

#define LIG_MI2C_C51_PAGE_IGNORE_FLAG_VALUE		0xFF
//////////////////////////////////////////////////////////////
//Parameter
//General GPIO/Pin
sbit LIG_MI2C_C51_SCL=0xB3;
sbit LIG_MI2C_C51_SDA=0xB2;

//////////////////////////////////////////////////////////////
//Moduler Function
STATIC void lig_mi2c_nop(UINT8 nop_num)C51_LARGE;

STATIC void  lig_mi2c_SetSDA(void)C51_LARGE;
STATIC void  lig_mi2c_SetCLK(void)C51_LARGE;
STATIC void  lig_mi2c_ClrSDA(void)C51_LARGE;
STATIC void  lig_mi2c_ClrCLK(void)C51_LARGE;
STATIC UINT8 lig_mi2c_GetSDA(void)C51_LARGE;

STATIC void  lig_mi2c_IICStart(void)C51_LARGE;
STATIC void  lig_mi2c_IICStop(void)C51_LARGE;

STATIC UINT8 lig_mi2c_GetData(BOOL check_last_bit)C51_LARGE;
STATIC UINT8 lig_mi2c_PutData(UINT8 ucVal)C51_LARGE;		//return 1 is error, 0 is success

//////////////////////////////////////////////////////////////
//
void lig_bi2c_master_Init(void)
{
    return;
}

INT32 lig_bi2c_master_write_bytes(UINT8 slave_device_addr, UINT8* wbuffer, INT32 wlength, BOOL bover_closeiic)
{

	INT32 n;
	UINT8 wvalue;

	lig_mi2c_IICStart();
	if (lig_mi2c_PutData(slave_device_addr) > 0)
	{
	   lig_mi2c_IICStop();
	   return LIG_BI2C_MASTER_ERROR_NOACK;
	}

	if ((wbuffer == NULL) || (wlength < 1))
	{
	   if (bover_closeiic)
	      lig_mi2c_IICStop();
	   return 0;
	}

	for (n=0; n<wlength; n+=1)
	{
	    wvalue = *(wbuffer+n);
	    if (lig_mi2c_PutData(wvalue) > 0)
	    {
	       n = LIG_BI2C_MASTER_ERROR_NOACK;
	       break;
	    }
	}

        if (n<0)
  	   lig_mi2c_IICStop();
        else
        {
  	   if (bover_closeiic)
	      lig_mi2c_IICStop();
	}
        return n;
}

INT32 lig_bi2c_master_read_bytes(UINT8 slave_device_addr, UINT8* rbuffer, INT32 rlength, BOOL bover_closeiic)
{
	INT32 n;
	UINT8 rvalue;

	if ((rbuffer == NULL) || (rlength < 1))
	   return LIG_BI2C_MASTER_ERROR_PARAM;

	lig_mi2c_IICStart();
	if (lig_mi2c_PutData(slave_device_addr) > 0)
	{
	   lig_mi2c_IICStop();
	   return LIG_BI2C_MASTER_ERROR_NOACK;
	}

	for (n=0; n<(rlength-1); n+=1)
	{
	    rvalue = lig_mi2c_GetData(FALSE);
	    *(rbuffer+n) = rvalue;
	}         
        
        rvalue = lig_mi2c_GetData(TRUE);
	*(rbuffer+n) = rvalue;
        n+=1;
            
	if (bover_closeiic)
	   lig_mi2c_IICStop();
	return n;
}

INT32 lig_bi2c_master_send_bytes(UINT8 slave_device_addr, UINT8 page_id,
                            	 UINT8* reg_addr, UINT8 reg_addr_len, BOOL b_reg_addr_bigmode,
                            	 UINT8* send_buffer, INT32 send_len)
{
	UINT8 data_buffer[LIG_MI2C_C51_TRANSBUFFER_SIZE];
	INT32 data_length = send_len + reg_addr_len+1;
	INT32 i;

	if (data_length > LIG_MI2C_C51_TRANSBUFFER_SIZE)
	   return LIG_BI2C_MASTER_ERROR_OVERFLOW;

	if ((send_buffer == NULL) || (send_len < 1) || (reg_addr == NULL) || (reg_addr_len < 1))
	   return LIG_BI2C_MASTER_ERROR_PARAM;

	data_length = 0;
	if (page_id != LIG_MI2C_C51_PAGE_IGNORE_FLAG_VALUE)
	{
	   data_buffer[data_length] = page_id;
	   data_length += 1;
	}

	if (b_reg_addr_bigmode == TRUE)
        {
           for (i=0; i<reg_addr_len; i+=1)
           {
               data_buffer[data_length] = *(reg_addr + i);
               data_length += 1;
           }
        }
        else
        {
           for (i=0; i<reg_addr_len; i+=1)
           {
               data_buffer[data_length] = *(reg_addr + (reg_addr_len - i -1));
               data_length += 1;
           }
        }

        for (i=0; i<send_len; i+=1)
        {
	   data_buffer[data_length] = *(send_buffer + i);
	   data_length += 1;
        }

        slave_device_addr &= 0xFE;
        return (lig_bi2c_master_write_bytes(slave_device_addr,data_buffer, data_length, TRUE));
}

INT32 lig_bi2c_master_recv_bytes(UINT8 slave_device_addr, UINT8 page_id,
                            	 UINT8* reg_addr, UINT8 reg_addr_len, BOOL b_reg_addr_bigmode,
                            	 UINT8* recv_buffer, INT32 recv_len)
{
	UINT8 data_buffer[8];
	INT32 data_length = reg_addr_len+1;
        INT32 i;

	if (data_length > 8)
	   return LIG_BI2C_MASTER_ERROR_OVERFLOW;

	if ((recv_buffer == NULL) || (recv_len < 1) || (reg_addr == NULL) || (reg_addr_len < 1))
	   return LIG_BI2C_MASTER_ERROR_PARAM;

	data_length = 0;
	if (page_id != LIG_MI2C_C51_PAGE_IGNORE_FLAG_VALUE)
	{
	   data_buffer[data_length] = page_id;
	   data_length += 1;
	}

	if (b_reg_addr_bigmode == TRUE)
        {
           for (i=0; i<reg_addr_len; i+=1)
           {
               data_buffer[data_length] = *(reg_addr + i);
               data_length += 1;
           }
        }
        else
        {
           for (i=0; i<reg_addr_len; i+=1)
           {
               data_buffer[data_length] = *(reg_addr + (reg_addr_len - i -1));
               data_length += 1;
           }
        }

	slave_device_addr &= 0xFE;       
        i = lig_bi2c_master_write_bytes(slave_device_addr,data_buffer, data_length, FALSE);
	if ( i != data_length)
	{
	   lig_mi2c_IICStop();
	   return i;
	}
        
        slave_device_addr |= 0x01;
        return (lig_bi2c_master_read_bytes(slave_device_addr, recv_buffer, recv_len, TRUE));
}

//////////////////////////////////////////////////////////////
//Moduler Function
void lig_mi2c_nop(UINT8 nop_num)C51_LARGE
{
	for (; nop_num > 0; nop_num-=1)
	    _nop_();
	return;
}

void lig_mi2c_SetSDA(void)C51_LARGE
{
	lig_mi2c_nop(1);
	LIG_MI2C_C51_SDA = 1;
	return;
}

void lig_mi2c_SetCLK(void)C51_LARGE
{
	lig_mi2c_nop(4);
	LIG_MI2C_C51_SCL = 1;
	lig_mi2c_nop(3);
	return;
}

void lig_mi2c_ClrSDA(void)C51_LARGE
{
	lig_mi2c_nop(7);
	LIG_MI2C_C51_SDA = 0;
	return;
}

void  lig_mi2c_ClrCLK(void)C51_LARGE
{
	lig_mi2c_nop(7);
	LIG_MI2C_C51_SCL = 0;
	return;

}

UINT8 lig_mi2c_GetSDA(void)C51_LARGE
{
	UINT8 getval = 0;
	lig_mi2c_nop(4);
	getval = LIG_MI2C_C51_SDA;
	getval &= 0x01;
        return getval;
}

void lig_mi2c_IICStart(void)C51_LARGE
{
	lig_mi2c_SetSDA();
	lig_mi2c_SetCLK();
	lig_mi2c_ClrSDA();
	lig_mi2c_ClrCLK();
        return;
}

void lig_mi2c_IICStop(void)C51_LARGE
{
	lig_mi2c_ClrSDA();
	lig_mi2c_SetCLK();
	lig_mi2c_SetSDA();
	return;
}

UINT8 lig_mi2c_GetData(BOOL check_last_bit)C51_LARGE
{
	UINT8 ucRec=0,n=8;

	for(n=8; n>0; n-=1)
	{
	    lig_mi2c_SetCLK();

	    ucRec<<=1;
	    ucRec += lig_mi2c_GetSDA();

	    lig_mi2c_ClrCLK();
	}

	if (check_last_bit)
	    lig_mi2c_SetSDA();
	else
	    lig_mi2c_ClrSDA();

	lig_mi2c_SetCLK();
	lig_mi2c_ClrCLK();
	lig_mi2c_SetSDA();
	return ucRec;
}

UINT8 lig_mi2c_PutData(UINT8 ucVal)C51_LARGE
{
	UINT8 n=8;

	while (n--)
	{
	   if (ucVal&0x80)
	       lig_mi2c_SetSDA();
	   else
	       lig_mi2c_ClrSDA();

		lig_mi2c_SetCLK();
		lig_mi2c_ClrCLK();

		ucVal<<=1;
	}

	lig_mi2c_ClrSDA();
	lig_mi2c_SetSDA();
	lig_mi2c_SetCLK();

	n = lig_mi2c_GetSDA(); //1 is failed; 0 is success, slave backs to ACK;
	lig_mi2c_ClrCLK();
	return n;
}

//ending
