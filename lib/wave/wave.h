//#pragma once
#ifndef _wave_h_
#define _wave_h_
#else
#error reinclude wave.h
#endif

/*****************************************************************************/
/* Wave.h                                 Copyright (c) Ladislav Zezula 2003 */
/*---------------------------------------------------------------------------*/
/* Header file for WAVe unplode functions                                    */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* 31.03.03  1.00  Lad  The first version of Wave.h                          */
/*****************************************************************************/

//-----------------------------------------------------------------------------
// Functions

int  CompressWave  (unsigned char * pbOutBuffer, int dwOutLength, short * pwInBuffer, int dwInLength, int nCmpType, int nChannels);
int  DecompressWave(unsigned char * pbOutBuffer, int dwOutLength, unsigned char * pbInBuffer, int dwInLength, int nChannels);
