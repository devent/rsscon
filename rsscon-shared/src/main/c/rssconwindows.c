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
#ifdef RSSCON_WINDOWS

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <windows.h>

#include "rsscon.h"
#include "rssconwindows.h"
#include "logger.h"

#define LOG_CATEGORY "com.globalscalingsoftware.rsscon.rssconwindows"

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

bool rssconwindowsFree(Rsscon* rsscon) {
	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
	log_debug(log, "free all windows data structures for rsscon %d.", rsscon);

	assert(rsscon != NULL);
	assert(rsscon->portdata != NULL);
	free(rsscon->portdata);
	rsscon->portdata = NULL;
	return true;
}

bool rssconwindowsInit(Rsscon* rsscon) {
	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
	log_debug(log, "setup windows settings for rsscon %d.", rsscon);

	assert(rsscon != NULL);
	assert(rsscon->portdata != NULL);
	RssconwindowsPortdata* pdata = (RssconwindowsPortdata*) rsscon->portdata;

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
	log_debug(log, "open windows device for rsscon %d.", rsscon);

	rssconSetLastError(rsscon, RSSCON_ERROR_NOERROR);

	const char* device = rssconGetDevice(rsscon);
	log_debug(log, "create file for device %s.", device);
	if (!createFile(device, pdata)) {
		rssconSetLastError(rsscon, RSSCON_ERROR_OPENDEVICE);
		return false;
	}

	DCB port = {0};
	port.DCBlength = sizeof(DCB);

	log_debug(log, "set comm configuration for device %s.", device);
	if (!setCommConfiguration(device, pdata, &port)) {
		rssconSetLastError(rsscon, RSSCON_ERROR_SETUPDEVICE);
		return false;
	}

	log_debug(log, "set the comm state for the port handler %d.", pdata->portHandle);
	if (!setCommState(device, pdata, &port)) {
		rssconSetLastError(rsscon, RSSCON_ERROR_SETUPDEVICE);
		return false;
	}

	log_debug(log, "set the handshake for the port handler %d.", pdata->portHandle);
	if (!handshakeOff(pdata)) {
		rssconSetLastError(rsscon, RSSCON_ERROR_SETUPDEVICE);
		return false;
	}

	log_debug(log, "set the timeout blocking for the port handler %d.", pdata->portHandle);
	if (!readTimeoutBlocking(pdata)) {
		rssconSetLastError(rsscon, RSSCON_ERROR_SETUPDEVICE);
		return false;
	}

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
		fputs("Failed to get comm state.\n", stderr);
		pdata->lastError = GetLastError();
		return false;
	}
	return true;
}

bool setCommConfiguration(const char* device, RssconwindowsPortdata* pdata, DCB* port) {
	COMMCONFIG commConfig = {0};
	DWORD dwSize = sizeof(commConfig);
	commConfig.dwSize = dwSize;
	if (!GetDefaultCommConfig(device, &commConfig, &dwSize)) {
		fputs("Failed to get default port settings.\n", stderr);
		pdata->lastError = GetLastError();
		return false;
	}
	if (!SetCommConfig(pdata->portHandle, &commConfig, dwSize)) {
		fputs("Failed to set default port settings.\n", stderr);
		pdata->lastError = GetLastError();
		return false;
	}
	return true;
}

bool setCommState(const char* device, RssconwindowsPortdata* pdata, DCB* port) {
	if ( !GetCommState(pdata->portHandle, port) ) {
		fputs("Failed to get comm state.\n", stderr);
		pdata->lastError = GetLastError();
		return false;
	}

	port->BaudRate = pdata->baudRate;
	port->ByteSize = pdata->byteSize;
	port->fParity = pdata->parityon;
	port->Parity = pdata->parity;
	port->StopBits = pdata->stopBits;

	if (!SetCommState(pdata->portHandle, port)) {
		fputs("Failed to set port settings.\n", stderr);
		pdata->lastError = GetLastError();
		return false;
	}

	return true;
}

bool handshakeOff(RssconwindowsPortdata* pdata) {
	assert(pdata != NULL);

	DCB port = {0};
	port.DCBlength = sizeof(DCB);

	if (!GetCommState(pdata->portHandle, &port)) {
		fputs("Failed to get port settings.\n", stderr);
		pdata->lastError = GetLastError();
		return false;
	}

	port.fOutxCtsFlow = 0; // Disable CTS monitoring
	port.fOutxDsrFlow = 0;// Disable DSR monitoring
	port.fDtrControl = DTR_CONTROL_DISABLE;// Disable DTR monitoring
	port.fOutX = 0;// Disable XON/XOFF for transmission
	port.fInX = 0;// Disable XON/XOFF for receiving
	port.fRtsControl = RTS_CONTROL_DISABLE;// Disable RTS (Ready To Send)

	if ( !SetCommState(pdata->portHandle, &port) )
	{
		fputs("Failed to set port settings.\n", stderr);
		pdata->lastError = GetLastError();
		return false;
	}

	return true;
}

bool readTimeoutBlocking(RssconwindowsPortdata* pdata) {
	assert(pdata != NULL);

	COMMTIMEOUTS cto;
	if (!GetCommTimeouts(pdata->portHandle, &cto)) {
		fputs("Failed to get port timeouts.\n", stderr);
		pdata->lastError = GetLastError();
		return false;
	}
	cto.ReadIntervalTimeout = 0;
	cto.ReadTotalTimeoutConstant = 0;
	cto.ReadTotalTimeoutMultiplier = 0;
	if (!SetCommTimeouts(pdata->portHandle, &cto)) {
		fputs("Failed to set port timeouts.\n", stderr);
		pdata->lastError = GetLastError();
		return false;
	}

	return true;
}

bool readTimeoutNonblocking(RssconwindowsPortdata* pdata) {
	assert(pdata != NULL);

	COMMTIMEOUTS cto;
	if (!GetCommTimeouts(pdata->portHandle, &cto)) {
		fputs("Failed to get port timeouts.\n", stderr);
		pdata->lastError = GetLastError();
		return false;
	}
	cto.ReadIntervalTimeout = MAXDWORD;
	cto.ReadTotalTimeoutConstant = 0;
	cto.ReadTotalTimeoutMultiplier = 0;
	if (!SetCommTimeouts(pdata->portHandle, &cto)) {
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

	COMMTIMEOUTS timeouts = {0};
	timeouts.ReadIntervalTimeout = timeout->readInterval;
	timeouts.ReadTotalTimeoutMultiplier = timeout->readTotalMultiplier;
	timeouts.ReadTotalTimeoutConstant = timeout->readTotalConstant;
	timeouts.WriteTotalTimeoutMultiplier = timeout->writeTotalMultiplier;
	timeouts.WriteTotalTimeoutConstant = timeout->writeTotalConstant;

	if (!SetCommTimeouts(pdata->portHandle, &timeouts)) {
		fputs("Failed to set port timeout periods.\n", stderr);
		pdata->lastError = GetLastError();
		return true;
	}

	return false;
}

bool purge(RssconwindowsPortdata* pdata) {
	assert(pdata != NULL);

	if (!PurgeComm(pdata->portHandle, PURGE_TXCLEAR | PURGE_RXCLEAR)) {
		pdata->lastError = GetLastError();
		fprintf(stderr, "failed to purge the port.\n");
		return false;
	}

	return true;
}

bool rssconwindowsRead(Rsscon* rsscon, void* data, size_t length, size_t* readed) {
	assert(rsscon != NULL);
	RssconwindowsPortdata* pdata = (RssconwindowsPortdata*)rsscon->portdata;
	assert(pdata != NULL);
	assert(pdata->portHandle != INVALID_HANDLE_VALUE);

	rssconSetLastError(rsscon, RSSCON_ERROR_NOERROR);

	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
	log_debug(log, "read data for rsscon %d of size %d.", rsscon, length);

	DWORD r;
	if (!ReadFile(pdata->portHandle, data, length, &r, NULL)) {
		pdata->lastError = GetLastError();
		rssconSetLastError(rsscon, RSSCON_ERROR_READ);
		fprintf(stderr, "failed to read a byte from the port.\n");
		return false;
	}

	*readed = r;
	return true;
}

bool rssconwindowsClose(Rsscon* rsscon) {
	assert(rsscon != NULL);
	RssconwindowsPortdata* pdata = (RssconwindowsPortdata*) rsscon->portdata;
	assert(pdata != NULL);
	assert(pdata->portHandle != INVALID_HANDLE_VALUE);

	rssconSetLastError(rsscon, RSSCON_ERROR_NOERROR);

	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
	log_debug(log, "close windows device for rsscon %d.", rsscon);

	if (!CloseHandle(pdata->portHandle)) {
		fputs("Failed to close the handle from port.\n", stderr);
		rssconSetLastError(rsscon, RSSCON_ERROR_CLOSEDEVICE);
		pdata->lastError = GetLastError();
		return false;
	}
	return true;
}

bool rssconwindowsWrite(Rsscon* rsscon, const void* data, size_t length,
		size_t* wrote) {
	assert(rsscon != NULL);
	RssconwindowsPortdata* pdata = (RssconwindowsPortdata*) rsscon->portdata;
	assert(pdata != NULL);
	assert(pdata->portHandle != INVALID_HANDLE_VALUE);

	rssconSetLastError(rsscon, RSSCON_ERROR_NOERROR);

	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
	log_debug(log, "write data for rsscon %d with size %d.", rsscon, length);

	DWORD w;
	if (!WriteFile(pdata->portHandle, data, length, &w, NULL)) {
		fputs("rssconwindowsWrite: failed to write data to the port handler.\n", stderr);
		rssconSetLastError(rsscon, RSSCON_ERROR_WRITE);
		pdata->lastError = GetLastError();
		return false;
	}

	*wrote = w;
	return true;
}

bool rssconInit(Rsscon* rsscon) {
	assert(rsscon != NULL);

	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
	log_debug(log, "setup windows interface for rsscon %d.", rsscon);

	RssconwindowsPortdata* portdata = malloc(sizeof(RssconwindowsPortdata));
	rsscon->portdata = portdata;
	rsscon->rssconInit = rssconwindowsInit;
	rsscon->rssconFree = rssconwindowsFree;
	rsscon->rssconOpen = rssconwindowsOpen;
	rsscon->rssconClose = rssconwindowsClose;
	rsscon->rssconWrite = rssconwindowsWrite;
	rsscon->rssconRead = rssconwindowsRead;

	return rsscon->rssconInit(rsscon);
}

#endif
