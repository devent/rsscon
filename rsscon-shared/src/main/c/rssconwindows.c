/**
 * Copyright 2012 Erwin Müller <erwin.mueller@deventm.org>
 *
 * This file is part of rsscon-shared.
 *
 * rsscon-shared is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * rsscon-shared is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rsscon-shared. If not, see <http://www.gnu.org/licenses/>.
 */
#if defined (_WIN32) || defined (_WIN64)

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "rsscon.h"
#include "rssconwindows.h"
#include "logger.h"

#define LOG_CATEGORY "com.anrisoftware.rsscon.rssconwindows"

typedef struct {
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

	// the port handle.
	HANDLE portHandle;

	// the last error
	DWORD lastError;
}RssconwindowsPortdata;

typedef struct {
	DWORD readInterval;
	DWORD readTotalMultiplier;
	DWORD readTotalConstant;
	DWORD writeTotalMultiplier;
	DWORD writeTotalConstant;
}Timeout;

static const char* strerrno(DWORD error) {
    static char buff[22];
#ifdef UNICODE
#warning "Unicode specified cannot use format. strerrno returns the error code."
    snprintf(buff, sizeof(buff), "%d", error);
#else
    LPVOID formatbuff;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &formatbuff,
        0, NULL );
    strncpy(buff, formatbuff, sizeof(buff));
    LocalFree(formatbuff);
#endif
    return buff;
}

bool rssconwindowsFree(Rsscon* rsscon) {
	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconwindowsFree");

	assert(rsscon != NULL);
	assert(rsscon->portdata != NULL);
	free(rsscon->portdata);
	rsscon->portdata = NULL;
    
    log_leave(log, "rssconwindowsInit := true");
    free_log();
	return true;
}

bool rssconwindowsInit(Rsscon* rsscon) {
    LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
    log_enter(log, "rssconwindowsInit");

	assert(rsscon != NULL);
	assert(rsscon->portdata != NULL);
	RssconwindowsPortdata* pdata = (RssconwindowsPortdata*) rsscon->portdata;

    rssconSetLastError(rsscon, RSSCON_ERROR_NOERROR, 0);
    
	pdata->portHandle = INVALID_HANDLE_VALUE;
	pdata->baudRate = 921600;
	pdata->parityon = FALSE;
	pdata->parity = NOPARITY;
	pdata->stopBits = ONESTOPBIT;
	pdata->byteSize = 8;
    
    log_leave(log, "rssconwindowsInit := true");
    free_log();
	return true;
}

bool rssconwindowsOpen(Rsscon* rsscon) {
	bool createFile(const char* device, RssconwindowsPortdata* pdata);
	bool setCommConfiguration(const char*, RssconwindowsPortdata*, DCB*);
	bool setCommState(const char*, RssconwindowsPortdata*, DCB*);
	bool handshakeOff(RssconwindowsPortdata* portdata);
	bool readTimeoutBlocking(RssconwindowsPortdata* pdata);
	bool readTimeoutNonblocking(RssconwindowsPortdata* pdata);

	assert(rsscon != NULL);
	assert(rsscon->portdata != NULL);
	RssconwindowsPortdata* pdata = (RssconwindowsPortdata*) rsscon->portdata;

    LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
    log_enter(log, "rssconwindowsOpen");

	rssconSetLastError(rsscon, RSSCON_ERROR_NOERROR, 0);

	const char* device = rssconGetDevice(rsscon);
	log_debug(log, "create file for device '%s'.", device);
	if (!createFile(device, pdata)) {
        rssconSetLastError(rsscon, RSSCON_ERROR_OPENDEVICE, GetLastError());
        log_error(log, "error open device '%s': %s", device, strerrno(GetLastError()));
        log_leave(log, "rssconwindowsOpen := false");
        free_log();
		return false;
	}

	DCB port = {0};
	port.DCBlength = sizeof(DCB);

	log_debug(log, "set port configuration for device %s.", device);
	if (!setCommConfiguration(device, pdata, &port)) {
        rssconSetLastError(rsscon, RSSCON_ERROR_SETUPDEVICE, GetLastError());
        log_error(log, "error set port configuration for device '%s': %s", device, strerrno(GetLastError()));
        log_leave(log, "rssconwindowsOpen := false");
        free_log();
		return false;
	}

	log_debug(log, "set the port state for device '%s'.", device);
	if (!setCommState(device, pdata, &port)) {
        rssconSetLastError(rsscon, RSSCON_ERROR_SETUPDEVICE, GetLastError());
        log_error(log, "error set port state for device '%s': %s", device, strerrno(GetLastError()));
        log_leave(log, "rssconwindowsOpen := false");
        free_log();
		return false;
	}

	log_debug(log, "set the handshake for device '%s'.", device);
	if (!handshakeOff(pdata)) {
        rssconSetLastError(rsscon, RSSCON_ERROR_SETUPDEVICE, GetLastError());
        log_error(log, "error set port handshake for device '%s': %s", device, strerrno(GetLastError()));
        log_leave(log, "rssconwindowsOpen := false");
        free_log();
		return false;
	}

	log_debug(log, "set the timeout blocking for device '%s.'", device);
	if (!readTimeoutBlocking(pdata)) {
        rssconSetLastError(rsscon, RSSCON_ERROR_SETUPDEVICE, GetLastError());
        log_error(log, "error set port timeout blocking for device '%s': %s", device, strerrno(GetLastError()));
        log_leave(log, "rssconwindowsOpen := false");
        free_log();
		return false;
	}

    log_debug(log, "Opened device '%s'.", device);
    log_leave(log, "rssconwindowsOpen := true");
    free_log();
	return true;
}

bool createFile(const char* device, RssconwindowsPortdata* pdata) {
	pdata->portHandle = CreateFile(
			device,
			GENERIC_READ | GENERIC_WRITE, // Access Mode
			0,// Shared Mode (must be 0 = not shared)
			NULL,// Security attributes
			OPEN_EXISTING,// How to open (must be OPEN_EXISTING)
			0,// Port attributes (no overlapped IO)
			NULL);// Must be NULL
	if (pdata->portHandle == INVALID_HANDLE_VALUE) {
		return false;
	}
	return true;
}

bool setCommConfiguration(const char* device, RssconwindowsPortdata* pdata, DCB* port) {
	COMMCONFIG commConfig = {0};
	DWORD dwSize = sizeof(commConfig);
	commConfig.dwSize = dwSize;
	if (!GetDefaultCommConfig(device, &commConfig, &dwSize)) {
		return false;
	}
	if (!SetCommConfig(pdata->portHandle, &commConfig, dwSize)) {
		return false;
	}
	return true;
}

bool setCommState(const char* device, RssconwindowsPortdata* pdata, DCB* port) {
	if ( !GetCommState(pdata->portHandle, port) ) {
		return false;
	}

	port->BaudRate = pdata->baudRate;
	port->ByteSize = pdata->byteSize;
	port->fParity = pdata->parityon;
	port->Parity = pdata->parity;
	port->StopBits = pdata->stopBits;

	if (!SetCommState(pdata->portHandle, port)) {
		return false;
	}

	return true;
}

bool handshakeOff(RssconwindowsPortdata* pdata) {
	assert(pdata != NULL);

	DCB port = {0};
	port.DCBlength = sizeof(DCB);

	if (!GetCommState(pdata->portHandle, &port)) {
		return false;
	}

	port.fOutxCtsFlow = 0; // Disable CTS monitoring
	port.fOutxDsrFlow = 0;// Disable DSR monitoring
	port.fDtrControl = DTR_CONTROL_DISABLE;// Disable DTR monitoring
	port.fOutX = 0;// Disable XON/XOFF for transmission
	port.fInX = 0;// Disable XON/XOFF for receiving
	port.fRtsControl = RTS_CONTROL_DISABLE;// Disable RTS (Ready To Send)

	if (!SetCommState(pdata->portHandle, &port)) {
		return false;
	}

	return true;
}

bool readTimeoutBlocking(RssconwindowsPortdata* pdata) {
	assert(pdata != NULL);

	COMMTIMEOUTS cto;
	if (!GetCommTimeouts(pdata->portHandle, &cto)) {
		return false;
	}
	cto.ReadIntervalTimeout = 0;
	cto.ReadTotalTimeoutConstant = 0;
	cto.ReadTotalTimeoutMultiplier = 0;
	if (!SetCommTimeouts(pdata->portHandle, &cto)) {
		return false;
	}

	return true;
}

bool readTimeoutNonblocking(RssconwindowsPortdata* pdata) {
	assert(pdata != NULL);

	COMMTIMEOUTS cto;
	if (!GetCommTimeouts(pdata->portHandle, &cto)) {
		return false;
	}
	cto.ReadIntervalTimeout = MAXDWORD;
	cto.ReadTotalTimeoutConstant = 0;
	cto.ReadTotalTimeoutMultiplier = 0;
	if (!SetCommTimeouts(pdata->portHandle, &cto)) {
		return false;
	}

	return true;
}

bool timeouts(RssconwindowsPortdata* portdata, Timeout* timeout)
{
	assert(portdata != NULL && timeout != NULL);
	RssconwindowsPortdata* pdata = (RssconwindowsPortdata*)portdata;
	assert(pdata != NULL);

	COMMTIMEOUTS timeouts = {0};
	timeouts.ReadIntervalTimeout = timeout->readInterval;
	timeouts.ReadTotalTimeoutMultiplier = timeout->readTotalMultiplier;
	timeouts.ReadTotalTimeoutConstant = timeout->readTotalConstant;
	timeouts.WriteTotalTimeoutMultiplier = timeout->writeTotalMultiplier;
	timeouts.WriteTotalTimeoutConstant = timeout->writeTotalConstant;

	if (!SetCommTimeouts(pdata->portHandle, &timeouts)) {
		return true;
	}

	return false;
}

bool purge(RssconwindowsPortdata* pdata) {
	assert(pdata != NULL);

	if (!PurgeComm(pdata->portHandle, PURGE_TXCLEAR | PURGE_RXCLEAR)) {
		return false;
	}

	return true;
}

bool rssconwindowsClose(Rsscon* rsscon) {
	assert(rsscon != NULL);
	RssconwindowsPortdata* pdata = (RssconwindowsPortdata*) rsscon->portdata;
	assert(pdata != NULL);
	assert(pdata->portHandle != INVALID_HANDLE_VALUE);

    LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
    log_enter(log, "rssconwindowsClose");

	rssconSetLastError(rsscon, RSSCON_ERROR_NOERROR, 0);

	if (!CloseHandle(pdata->portHandle)) {
        rssconSetLastError(rsscon, RSSCON_ERROR_CLOSEDEVICE, GetLastError());
        const char* device = rssconGetDevice(rsscon);
        log_error(log, "Error closing device '%s': %s", device, strerrno(GetLastError()));
        log_leave(log, "rssconwindowsClose := false");
        free_log();
		return false;
	}
	
    log_debug(log, "Closed device %s.", rsscon);
    log_leave(log, "rssconwindowsClose := true");
    free_log();
	return true;
}

bool rssconwindowsWrite(Rsscon* rsscon, const void* data, size_t length,
        size_t* wrote) {
    assert(rsscon != NULL);
    RssconwindowsPortdata* pdata = (RssconwindowsPortdata*) rsscon->portdata;
    assert(pdata != NULL);
    assert(pdata->portHandle != INVALID_HANDLE_VALUE);

    LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
    log_enter(log, "rssconwindowsWrite");

    rssconSetLastError(rsscon, RSSCON_ERROR_NOERROR, 0);

    DWORD w;
    if (!WriteFile(pdata->portHandle, data, length, &w, NULL)) {
        rssconSetLastError(rsscon, RSSCON_ERROR_WRITE, GetLastError());
        const char* device = rssconGetDevice(rsscon);
        log_error(log, "Error write to device '%s': %s", device, strerrno(GetLastError()));
        log_leave(log, "rssconwindowsWrite := false");
        free_log();
        return false;
    }

    *wrote = w;

    log_leave(log, "rssconwindowsWrite := true");
    free_log();
    
    return true;
}

bool rssconwindowsRead(Rsscon* rsscon, void* data, size_t length, size_t* readed) {
    assert(rsscon != NULL);
    RssconwindowsPortdata* pdata = (RssconwindowsPortdata*)rsscon->portdata;
    assert(pdata != NULL);
    assert(pdata->portHandle != INVALID_HANDLE_VALUE);

    LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
    log_enter(log, "rssconwindowsRead");

    rssconSetLastError(rsscon, RSSCON_ERROR_NOERROR, 0);

    DWORD r;
    if (!ReadFile(pdata->portHandle, data, length, &r, NULL)) {
        rssconSetLastError(rsscon, RSSCON_ERROR_READ, GetLastError());
        const char* device = rssconGetDevice(rsscon);
        log_error(log, "Error read from device '%s': %s", device, strerrno(GetLastError()));
        log_leave(log, "rssconwindowsRead := false");
        free_log();
        return false;
    }

    *readed = r;
    
    log_debug(log, "Read %d data.", r);
    log_leave(log, "rssconwindowsRead := true");
    free_log();
    return true;
}

bool rssconInit(Rsscon* rsscon) {
	assert(rsscon != NULL);

	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
    log_enter(log, "rssconInit");

	RssconwindowsPortdata* portdata = malloc(sizeof(RssconwindowsPortdata));
	rsscon->portdata = portdata;
	rsscon->rssconInit = rssconwindowsInit;
	rsscon->rssconFree = rssconwindowsFree;
	rsscon->rssconOpen = rssconwindowsOpen;
	rsscon->rssconClose = rssconwindowsClose;
	rsscon->rssconWrite = rssconwindowsWrite;
	rsscon->rssconRead = rssconwindowsRead;


    log_leave(log, "rssconInit");
    free_log();
	return rsscon->rssconInit(rsscon);
}

#endif
