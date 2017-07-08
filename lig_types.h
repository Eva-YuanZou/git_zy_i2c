/**
 * @file lig_types.h
 * @author Zou Yuan
 * @date 24-Apr-2013
 * @brief DATA TYPES defines
 */

#ifndef _LIG_TYPES_H_
#define _LIG_TYPES_H_

#define LIG_TYPES_CHIP_TYPE_C51				1
#define LIG_TYPES_CHIP_TYPE_LPC				2

#define LIG_TYPES_CHIP_TYPE 				LIG_TYPES_CHIP_TYPE_C51
#if (LIG_TYPES_CHIP_TYPE == LIG_TYPES_CHIP_TYPE_C51)

#else
     #include "lpc_types.h"
#endif

#define STATIC      static
#define INLINE      __inline
#define CONSTANT    const
#define EXTERNAL    extern
#define VOLATILE    volatile
#define PACKED
#define PACKED_STR  PACKED struct

/*=======================================
 * Data types
 *======================================*/

typedef char            CHAR;
typedef unsigned char   UCHAR;          /* unsigned 8-bit */
typedef unsigned char   UINT8;
typedef unsigned short  UINT16;         /* unsigned 16-bit */
typedef unsigned long   UINT32;         /* unsigned 32-bit */
typedef signed char     INT8;
typedef short int       INT16;
typedef long int        INT32;

#if (LIG_TYPES_CHIP_TYPE == LIG_TYPES_CHIP_TYPE_LPC)
    typedef unsigned long long UINT64 ;
#endif

typedef signed int      int_t;
typedef unsigned int 	uint_t;
typedef int             prefuint_t; // Platform-specific efficient integers

#define UINT8           UCHAR

#ifndef BOOL
#define BOOL            UCHAR
#endif

#ifndef NULL
#define NULL            ((void *)0)
#endif

#define CLOCK_TIME_MAX  0xffffffff       // Maximum value of time type

typedef unsigned long   clock_time_t;   // The clock type used for returning system ticks (1ms).
typedef unsigned short  time_ms_t;      // Time in milliseconds

// gcc -pedantic does not allow bit field operation, so to avoid the warning, we need the following trick
typedef unsigned char   bit_fld_t_t;    // bit field type used in structures
typedef unsigned short  bit_fld16_t_t;

#define bit_fld_t       bit_fld_t_t
#define bit_fld16_t     bit_fld16_t_t

/* Emulate C99/C++ bool type    */

#ifdef __cplusplus
typedef bool bool_t;
#else
typedef enum
{
	false   = 0,
	true    = !(false)
} bool_t;
#endif // __cplusplus

#if !defined ON
#define ON  true
#define OFF false
#endif

#if !defined TRUE
#define TRUE  true
#define FALSE false
#endif

#define SET_BITS    0xFF
#define CLEAR_BITS  0x00

#define ROM            // 8051 type of ROM memory
#define XDATA          // 8051 type of external memory

#define ABS_DIFF(A, B) ((A>B) ? (A-B) : (B-A))

//------------------------------------------------------------------------------
// Configuration defines used by hal_config.h
//------------------------------------------------------------------------------
#define BIT0        0x0001
#define BIT1        0x0002
#define BIT2        0x0004
#define BIT3        0x0008
#define BIT4        0x0010
#define BIT5        0x0020
#define BIT6        0x0040
#define BIT7        0x0080
#define BIT8        0x0100
#define BIT9        0x0200
#define BIT10       0x0400
#define BIT11       0x0800
#define BIT12       0x1000
#define BIT13       0x2000
#define BIT14       0x4000
#define BIT15       0x8000

//------------------------------------------------------------------------------
// Language extending macros
//------------------------------------------------------------------------------
//Debug Print Enable Flag
#define LIG_DBG_ENABLE          0x01

#endif
