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
#ifdef LINUX

#include <stdbool.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include "rsscon.h"
#include "rssconlinux.h"
#include "logger.h"

#define LOG_CATEGORY "com.globalscalingsoftware.rsscon"

typedef struct {

	bool noblock;

	bool wait;

	speed_t baudrate;

	long tvsec;

	long tvusec;

	int fd;

	struct termios oldtio, newtio;

} RssconlinuxPortdata;

static const char * strerrno(int err) {
	switch (err) {
	case 0:
		return "OK";
#ifdef EACCES
	case EACCES:
		return "EACCES";
#endif
#ifdef EBADF
	case EBADF:
		return "EBADF";
#endif
#ifdef EEXIST
	case EEXIST:
		return "EEXIST";
#endif
#ifdef EFAULT
	case EFAULT:
		return "EFAULT";
#endif
#ifdef EINVAL
	case EINVAL:
		return "EINVAL";
#endif
#ifdef EISDIR
	case EISDIR:
		return "EISDIR";
#endif
#ifdef ENOENT
	case ENOENT:
		return "ENOENT";
#endif
#ifdef ENOTEMPTY
	case ENOTEMPTY:
		return "ENOTEMPTY";
#endif
#ifdef EBUSY
	case EBUSY:
		return "EBUSY";
#endif
	default:
		return "E??";
	}
}

speed_t translateBaudRate(unsigned int baudrate) {
	switch (baudrate) {
	case RSSCON_BAUDRATE_57600:
		return B57600;
	case RSSCON_BAUDRATE_115200:
		return B115200;
	case RSSCON_BAUDRATE_230400:
		return B230400;
	case RSSCON_BAUDRATE_460800:
		return B460800;
	case RSSCON_BAUDRATE_500000:
		return B500000;
	case RSSCON_BAUDRATE_576000:
		return B576000;
	case RSSCON_BAUDRATE_921600:
		return B921600;
	case RSSCON_BAUDRATE_1000000:
		return B1000000;
	case RSSCON_BAUDRATE_1152000:
		return B1152000;
	case RSSCON_BAUDRATE_1500000:
		return B1500000;
	case RSSCON_BAUDRATE_2000000:
		return B2000000;
	case RSSCON_BAUDRATE_2500000:
		return B2500000;
	case RSSCON_BAUDRATE_3000000:
		return B3000000;
	case RSSCON_BAUDRATE_3500000:
		return B3500000;
	case RSSCON_BAUDRATE_4000000:
		return B4000000;
	default:
		return -1;
	}
}

bool rssconlinuxFree(Rsscon* rsscon) {
	assert(rsscon != NULL);
	assert(rsscon->portdata != NULL);
	free(rsscon->portdata);
	return true;
}

bool rssconlinuxInit(Rsscon* rsscon) {
#ifdef RSSCON_LOGING
	printf("%d: rssconlinuxInit...\n", __LINE__);
#endif
	assert(rsscon != NULL);
	assert(rsscon->portdata != NULL);
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) rsscon->portdata;

	rssconSetLastError(rsscon, RSSCON_ERROR_NOERROR);
	if (pdata->tvsec < 0) {
		pdata->tvsec = 5;
	}
	if (pdata->tvusec < 1) {
		pdata->tvusec = pdata->tvsec * 10E6;
	}
	pdata->noblock = false;
	pdata->wait = true;

	return true;
}

bool setup(Rsscon* rsscon) {
#ifdef RSSCON_LOGING
	printf("%d: setup...\n", __LINE__);
#endif

	assert(rsscon != NULL);
	assert(rsscon->portdata != NULL);
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) rsscon->portdata;

	struct termios *tio1, *tio2;
	tio1 = &(pdata->oldtio);
	tio2 = &(pdata->newtio);

	if (tcgetattr(pdata->fd, tio1)) {
		rssconSetLastError(rsscon, RSSCON_ERROR_SETUPDEVICE);
		return false;
	}

	speed_t baudrate = translateBaudRate(rssconGetBaudRate(rsscon));
	memcpy(tio2, tio1, sizeof(struct termios));
	cfmakeraw(tio2);
	cfsetospeed(tio2, baudrate);
	cfsetispeed(tio2, baudrate);
#if 0
	tio2->c_cflag |= CRTSCTS;
#endif
	tio2->c_cflag |= CREAD | CLOCAL;
	tcflush(pdata->fd, TCIOFLUSH);
	if (tcsetattr(pdata->fd, TCSANOW, tio2)) {
		rssconSetLastError(rsscon, RSSCON_ERROR_SETUPDEVICE);
		return false;
	}
#if 0
	tcflow(pdata->fd, TCOON); tcflow(pdata->fd, TCION);
#endif

	return true;
}

bool rssconlinuxOpen(Rsscon* rsscon) {
	log4c_category_t *log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconlinuxOpen(%d)", rsscon);

	assert(rsscon != NULL);
	assert(rsscon->portdata != NULL);
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) rsscon->portdata;

	log_debug(log, "reset last error...");
	rssconSetLastError(rsscon, RSSCON_ERROR_NOERROR);

	int flag = O_RDWR | O_NOCTTY;
	if (pdata->noblock) {
		flag |= O_NONBLOCK;
	}

	const char* device = rssconGetDevice(rsscon);

	log_debug(log, "open device '%s' with flag '%d'...", device, flag);
	pdata->fd = open(device, flag);
	if (pdata->fd == -1) {
		fprintf(stderr, "error open device: %s\n", strerrno(errno));
		rssconSetLastError(rsscon, RSSCON_ERROR_OPENDEVICE);

		log_leave(log, "leave rssconCreate:=false");
		free_log();
		return false;
	}

	log_debug(log, "setup device...", device, flag);
	bool ret = setup(rsscon);
	if (!ret) {
		fprintf(stderr, "error setup device.\n");
		rssconSetLastError(rsscon, RSSCON_ERROR_OPENDEVICE);

		log_leave(log, "leave rssconCreate:=false");
		free_log();
		return false;
	}

	log_leave(log, "leave rssconCreate:=true");
	free_log();
	return true;
}

bool rssconlinuxClose(Rsscon* rsscon) {
#ifdef RSSCON_LOGING
	printf("%d: rssconlinuxClose...\n", __LINE__);
#endif

	assert(rsscon != NULL);
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) rsscon->portdata;
	assert(pdata->fd != 0);

	rssconSetLastError(rsscon, RSSCON_ERROR_NOERROR);

	tcflush(pdata->fd, TCIOFLUSH);
	tcsetattr(pdata->fd, TCSANOW, &(pdata->oldtio));
	int ret = close(pdata->fd);
	if (ret != 0) {
		rssconSetLastError(rsscon, RSSCON_ERROR_CLOSEDEVICE);
		return false;
	}

	pdata->fd = 0;
	return true;
}

bool rssconlinuxWrite(Rsscon* rsscon, const void* data, size_t length,
		size_t* wrote) {
#ifdef RSSCON_LOGING
	printf("%d: rssconlinuxWrite...\n", __LINE__);
#endif

	assert(rsscon != NULL);
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) rsscon->portdata;
	assert(pdata->fd != 0);

	ssize_t ret = write(pdata->fd, data, length);
	tcdrain(pdata->fd);
	if (ret == -1) {
		rssconSetLastError(rsscon, RSSCON_ERROR_WRITE);
		return false;
	}
	*wrote = ret;

	return true;
}

bool waittodata(RssconlinuxPortdata* pdata, fd_set* set, struct timeval* tv) {
#ifdef RSSCON_LOGING
	printf("%d: waittodata...\n", __LINE__);
#endif

	FD_ZERO(set);
	FD_SET(pdata->fd, set);
	tv->tv_sec = pdata->tvsec;
	tv->tv_usec = pdata->tvusec;
	int err = select(pdata->fd + 1, set, NULL, NULL, tv);
	if (err <= 0) {
		return false;
	} else {
		return true;
	}
}

bool rssconlinuxRead(Rsscon* rsscon, void* data, size_t length, size_t* readed) {
#ifdef RSSCON_LOGING
	printf("%d: rssconlinuxRead...\n", __LINE__);
#endif

	assert(rsscon != NULL);
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) rsscon->portdata;
	assert(pdata->fd != 0);

	struct timeval tv;
	fd_set set;
	ssize_t ret = 0;
	if (pdata->wait) {
		if (!waittodata(pdata, &set, &tv)) {
			rssconSetLastError(rsscon, RSSCON_ERROR_READ);
			fprintf(stderr, "error wait device.\n");
			return false;
		}
	}
	while (ret == 0) {
		if (pdata->wait) {
			if (!waittodata(pdata, &set, &tv)) {
				rssconSetLastError(rsscon, RSSCON_ERROR_READ);
				fprintf(stderr, "error wait device.\n");
				return false;
			}
		}
		ret = read(pdata->fd, data, length);
		if (ret == -1)
			break;
	}
	if (ret == -1) {
		rssconSetLastError(rsscon, RSSCON_ERROR_READ);
		fprintf(stderr, "error read device.\n");
		return false;
	}

	*readed = ret;
	return true;
}

bool rssconInit(Rsscon* rsscon) {
#ifdef RSSCON_LOGING
	printf("%d: rssconlinuxSetupInterface...\n", __LINE__);
#endif
	RssconlinuxPortdata* portdata = malloc(sizeof(RssconlinuxPortdata));
	rsscon->portdata = portdata;
	rsscon->rssconInit = rssconlinuxInit;
	rsscon->rssconFree = rssconlinuxFree;
	rsscon->rssconOpen = rssconlinuxOpen;
	rsscon->rssconClose = rssconlinuxClose;
	rsscon->rssconWrite = rssconlinuxWrite;
	rsscon->rssconRead = rssconlinuxRead;

	return rsscon->rssconInit(rsscon);
}

#endif
