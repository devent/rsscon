#ifdef WINDOWS

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <windows.h>

#include "rsscon.h"

typedef struct
{
    DWORD readInterval;
    DWORD readTotalMultiplier;
    DWORD readTotalConstant;
    DWORD writeTotalMultiplier;
    DWORD writeTotalConstant;
} Timeout;

bool handshakeOff(RssconwindowsPortdata* portdata);

bool readTimeoutBlocking(RssconwindowsPortdata* portdata);

bool readTimeoutNonblocking(RssconwindowsPortdata* portdata);

bool purge(RssconwindowsPortdata* portdata);

// Sets timeouts for the port.
bool timeouts(RssconwindowsPortdata* portdata, Timeout* timeout);

// Set default settings to portdata.
bool rssconwindowsSetupPortdata(RssconwindowsPortdata* pdata)
{
    assert(pdata != NULL);

    pdata->portHandle = INVALID_HANDLE_VALUE;
#ifndef NOISEREAD_3
    pdata->baudRate = CBR_115200;
#endif
#ifdef NOISEREAD_3
    pdata->baudRate = 921600;
#endif
    pdata->parityon = FALSE;
    pdata->parity = NOPARITY;
    pdata->stopBits = ONESTOPBIT;
    pdata->byteSize = 8;
    return true;
}

bool rssconwindowsOpen(void* portdata)
{
    assert(portdata != NULL);
    RssconwindowsPortdata* pdata = (RssconwindowsPortdata*)portdata;
    assert(pdata != NULL);

#ifdef RSSCON_LOGING
    printf("create the file for port '%s'...\n", pdata->portName);
#endif

    pdata->portHandle = CreateFile(
        pdata->portName,
        GENERIC_READ | GENERIC_WRITE,   // Access Mode
        0,                              // Shared Mode (must be 0 = not shared)
        NULL,                           // Security attributes
        OPEN_EXISTING,                  // How to open (must be OPEN_EXISTING)
        0,                              // Port attributes (no overlapped IO)
        NULL);                          // Must be NULL

    DCB port = { 0 };
    port.DCBlength = sizeof(DCB);

     COMMCONFIG commConfig = {0};
    DWORD dwSize = sizeof(commConfig);
    commConfig.dwSize = dwSize;
    if ( GetDefaultCommConfig(pdata->portName, &commConfig, &dwSize) )
    {
        // Set the default communication configuration
        if ( !SetCommConfig(pdata->portHandle, &commConfig, dwSize) )
        {
            fputs("Failed to set default port settings.\n", stderr);
            pdata->lastError = GetLastError();
            return false;
        }
    }


    if ( !GetCommState(pdata->portHandle, &port) )
    {
        fputs("Failed to get port settings.\n", stderr);
        pdata->lastError = GetLastError();
        return false;
    }

    port.BaudRate = pdata->baudRate;
    port.ByteSize = pdata->byteSize;
    port.fParity = pdata->parityon;
    port.Parity =  pdata->parity;
    port.StopBits =  pdata->stopBits;

#ifdef RSSCON_LOGING
    printf("trying to set the comm state for the port handler %d...\n", pdata->portHandle);
#endif

    if ( !SetCommState(pdata->portHandle, &port) )
    {
        fputs("Failed to set port settings.\n", stderr);
        pdata->lastError = GetLastError();
        return false;
    }

    bool ok = handshakeOff(pdata);
    if (!ok) return false;
    ok = readTimeoutBlocking(pdata);
    if (!ok) return false;

    return true;
}

bool handshakeOff(RssconwindowsPortdata* pdata)
{
    assert(pdata != NULL);

    DCB port = { 0 };
    port.DCBlength = sizeof(DCB);

    if ( !GetCommState(pdata->portHandle, &port) )
    {
        fputs("Failed to get port settings.\n", stderr);
        pdata->lastError = GetLastError();
        return false;
    }

        port.fOutxCtsFlow = 0;                    // Disable CTS monitoring
        port.fOutxDsrFlow = 0;                    // Disable DSR monitoring
        port.fDtrControl = DTR_CONTROL_DISABLE;        // Disable DTR monitoring
        port.fOutX = 0;                            // Disable XON/XOFF for transmission
        port.fInX = 0;                            // Disable XON/XOFF for receiving
        port.fRtsControl = RTS_CONTROL_DISABLE;        // Disable RTS (Ready To Send)

    if ( !SetCommState(pdata->portHandle, &port) )
    {
        fputs("Failed to set port settings.\n", stderr);
        pdata->lastError = GetLastError();
        return false;
    }

    return true;
}

bool readTimeoutBlocking(RssconwindowsPortdata* pdata)
{
    assert(pdata != NULL);

    COMMTIMEOUTS cto;
    if ( !GetCommTimeouts(pdata->portHandle, &cto) )
    {
        fputs("Failed to get port timeouts.\n", stderr);
        pdata->lastError = GetLastError();
        return false;
    }
        cto.ReadIntervalTimeout = 0;
        cto.ReadTotalTimeoutConstant = 0;
        cto.ReadTotalTimeoutMultiplier = 0;
    if ( !SetCommTimeouts(pdata->portHandle, &cto) )
    {
        fputs("Failed to set port timeouts.\n", stderr);
        pdata->lastError = GetLastError();
        return false;
    }

    return true;
}

bool readTimeoutNonblocking(RssconwindowsPortdata* pdata)
{
    assert(pdata != NULL);

    COMMTIMEOUTS cto;
    if ( !GetCommTimeouts(pdata->portHandle, &cto) )
    {
        fputs("Failed to get port timeouts.\n", stderr);
        pdata->lastError = GetLastError();
        return false;
    }
        cto.ReadIntervalTimeout = MAXDWORD;
        cto.ReadTotalTimeoutConstant = 0;
        cto.ReadTotalTimeoutMultiplier = 0;
    if ( !SetCommTimeouts(pdata->portHandle, &cto) )
    {
        fputs("Failed to set port timeouts.\n", stderr);
        pdata->lastError = GetLastError();
        return false;
    }

    return true;
}

bool timeouts(RssconwindowsPortdata* portdata, Timeout* timeout)
{
    assert(portdata != NULL && timeout != NULL);
    RssconwindowsPortdata* pdata = (RssconwindowsPortdata*)portdata;
    assert(pdata != NULL);

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = timeout->readInterval;
    timeouts.ReadTotalTimeoutMultiplier = timeout->readTotalMultiplier;
    timeouts.ReadTotalTimeoutConstant = timeout->readTotalConstant;
    timeouts.WriteTotalTimeoutMultiplier = timeout->writeTotalMultiplier;
    timeouts.WriteTotalTimeoutConstant = timeout->writeTotalConstant;

    if ( !SetCommTimeouts(pdata->portHandle, &timeouts) )
    {
        fputs("Failed to set port timeout periods.\n", stderr);
        pdata->lastError = GetLastError();
        return true;
    }

    return false;
}

bool rssconwindowsRead(void* portdata, void* data, size_t length, size_t* read)
{
    assert(portdata != NULL);
    RssconwindowsPortdata* pdata = (RssconwindowsPortdata*)portdata;
    assert(pdata != NULL);

    assert(portdata != NULL);
    assert(data != NULL);

    DWORD r;
    if ( !ReadFile(pdata->portHandle, data, length, &r, NULL) )
    {
        pdata->lastError = GetLastError();
        fprintf(stderr, "failed to read a byte from the port.\n");
        return false;
    }

    *read = r;
    return true;
}

bool purge(RssconwindowsPortdata* pdata)
{
    assert(pdata != NULL);

    if ( !PurgeComm(pdata->portHandle, PURGE_TXCLEAR | PURGE_RXCLEAR) )
    {
        pdata->lastError = GetLastError();
        fprintf(stderr, "failed to purge the port.\n");
        return false;
    }

    return true;
}

bool rssconwindowsClose(void* portdata)
{
    assert(portdata != NULL);
    RssconwindowsPortdata* pdata = (RssconwindowsPortdata*)portdata;
    assert(pdata != NULL);

    if ( pdata->portHandle != INVALID_HANDLE_VALUE )
    {
#ifdef RSSCON_LOGING
        printf("closing the port handler %d...\n", pdata->portHandle);
#endif
        if ( !CloseHandle(pdata->portHandle) )
        {
            fputs("Failed to close the handle from port.\n", stderr);
            pdata->lastError = GetLastError();
            return false;
        }
    }
    return true;
}

bool rssconwindowsWrite(void* portdata, const void* data, size_t length, size_t* writed)
{
    assert(portdata != NULL);
    RssconwindowsPortdata* pdata = (RssconwindowsPortdata*)portdata;
    assert(pdata != NULL);

#ifdef RSSCON_LOGING
    printf("writing %d data to the port handler...\n", length);
#endif

    DWORD w;
    if ( !WriteFile(pdata->portHandle, data, length, &w, NULL) )
    {
        fputs("rssconwindowsWrite: failed to write data to the port handler.\n", stderr);
        pdata->lastError = GetLastError();
        return false;
    }

    *writed = w;
    return true;
}

int rssconwindowsLastError(void* portdata)
{
    assert(portdata != NULL);
    RssconwindowsPortdata* pdata = (RssconwindowsPortdata*)portdata;
    assert(pdata != NULL);

    return pdata->lastError;
}

bool rssconwindowsSetupInterface(NoiseRead* noiseread)
{
#ifdef RSSCON_LOGING
    printf("%d: rssconwindowsSetupInterface...\n", __LINE__);
#endif

    noiseread->rssconOpen = rssconwindowsOpen;
    noiseread->rssconClose = rssconwindowsClose;
    noiseread->rssconWrite = rssconwindowsWrite;
    noiseread->rssconRead = rssconwindowsRead;
    noiseread->rssconLastError = rssconwindowsLastError;

    return true;
}

#endif
