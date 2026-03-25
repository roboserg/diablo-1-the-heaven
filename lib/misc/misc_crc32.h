//#pragma once
#ifndef _misc_crc32_h_
#define _misc_crc32_h_
#else
#error reinclude misc_crc32.h
#endif

/*****************************************************************************/
/* crc32.h                                Copyright (c) Ladislav Zezula 2007 */
/*---------------------------------------------------------------------------*/
/* Description:                                                              */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* 11.06.07  1.00  Lad  The first version of crc32.h                         */
/*****************************************************************************/

struct crc32_context
{
    unsigned long value;
};

void CRC32_Init(crc32_context * ctx);
void CRC32_Update(crc32_context * ctx, unsigned char *input, int ilen);
void CRC32_Finish(crc32_context * ctx, unsigned long * value);
