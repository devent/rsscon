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
