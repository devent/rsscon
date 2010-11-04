/**
 * Copyright 2010 Erwin Müller <erwin.mueller@deventm.org>
 * Version 1.0
 *
 * This file is part of rsscon.
 *
 * rsscon is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * rsscon is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with
 * rsscon. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef RSSCON_H
#define RSSCON_H

#ifdef WINDOWS

#include <stdbool.h>
#include <windows.h>
#include "noiseread.h"

typedef struct
{
    // ##
    // ## Public Members ##
    // ##

    // the name of the port, like 'COM8', for ports greater than 9 the
    // name need to be '//./COMxx'.
    char portName[256];

    // ##
    // ## boudRate, parityon, parity, stopBits, byteSize
    // ## is all set to default vaules with
    // ## bool rssconwindowsSetupPortdata(RssconwindowsPortdata*), but you
    // ## can set them seperate.
    // ##

    // the baud rate, CBR_xxxxx (CBR_115200, ...).
    DWORD baudRate;

    // TRUE for parity, FALSE for no parity.
    DWORD parityon;

    // the parity, EVENTPARITY, MARKPARITY, NOPARITY, ODDPARITY, SPACEPARITY.
    BYTE parity;

    // the stop bits, ONESTOPBIT, ONESSTOPBITS, TWOSTOPBITS.
    BYTE stopBits;

    // the size of a byte.
    BYTE byteSize;

    // ##
    // ## Private Members ##
    // ##

    // the port handle.
    HANDLE portHandle;

    // the last error.
    DWORD lastError;
} RssconwindowsPortdata;

/**
* Opens a port with the parameters in portdata.
*
* @return true when no error eccur, false otherwise.
*/
bool rssconwindowsOpen(void* portdata);

/**
* Close the port.
*
* @return true when no error eccur, false otherwise.
*/
bool rssconwindowsClose(void* portdata);

/**
* Read some bytes from the port.
*
* @return true when no error eccur, false otherwise.
*/
bool rssconwindowsRead(void* portdata, void* data, size_t length, size_t* read);

/**
* Write some bytes into the port.
*
* @return true when no error eccur, false otherwise.
*/
bool rssconwindowsWrite(void* portdata, const void* data, size_t length, size_t* writed);

int rssconwindowsLastError(void* portdata);

/*
* Set default settings to portdata.
*
* @return true when no error eccur, false otherwise.
*/
bool rssconwindowsSetupPortdata(RssconwindowsPortdata* portdata);

bool rssconwindowsSetupInterface(NoiseRead* noiseread);

#endif

#endif
