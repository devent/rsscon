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
#ifdef __linux

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

#define LOG_CATEGORY "com.anrisoftware.rsscon.rssconlinux"

typedef struct {

	int errorNumber;

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
#ifdef EAGAIN
	case EAGAIN:
		return "EAGAIN";
#endif
#ifdef EBADMSG
	case EBADMSG:
		return "EBADMSG";
#endif
#ifdef EINTR
	case EINTR:
		return "EINTR";
#endif
#ifdef EIO
	case EIO:
		return "EIO";
#endif
#ifdef EOVERFLOW
	case EOVERFLOW:
		return "EOVERFLOW";
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
	rsscon->portdata = NULL;
	return true;
}

bool rssconlinuxInit(Rsscon* rsscon) {
	assert(rsscon != NULL);
	assert(rsscon->portdata != NULL);
	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconlinuxInit");
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) rsscon->portdata;

	rssconSetLastError(rsscon, RSSCON_ERROR_NOERROR, 0);
	if (pdata->tvsec < 0) {
		pdata->tvsec = 5;
	}
	if (pdata->tvusec < 1) {
		pdata->tvusec = pdata->tvsec * 10E6;
	}
	pdata->noblock = false;
	pdata->wait = false;

	log_leave(log, "rssconlinuxInit := true");
	free_log();
	return true;
}

bool setup(Rsscon* rsscon) {
	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
	log_enter(log, "setup");
	assert(rsscon != NULL);
	assert(rsscon->portdata != NULL);
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) rsscon->portdata;

	struct termios *tio1, *tio2;
	tio1 = &(pdata->oldtio);
	tio2 = &(pdata->newtio);

	if (tcgetattr(pdata->fd, tio1)) {
		rssconSetLastError(rsscon, RSSCON_ERROR_SETUPDEVICE, errno);
		log_leave(log, "rssconlinuxInit := false");
		free_log();
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
		rssconSetLastError(rsscon, RSSCON_ERROR_SETUPDEVICE, errno);
		log_leave(log, "rssconlinuxInit := false");
		free_log();
		return false;
	}
#if 0
	tcflow(pdata->fd, TCOON); tcflow(pdata->fd, TCION);
#endif

	log_leave(log, "rssconlinuxInit := true");
	free_log();
	return true;
}

bool rssconlinuxOpen(Rsscon* rsscon) {
	assert(rsscon != NULL);
	assert(rsscon->portdata != NULL);
	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconlinuxOpen");
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) rsscon->portdata;

	rssconSetLastError(rsscon, RSSCON_ERROR_NOERROR, 0);

	int flag = O_RDWR | O_NOCTTY;
	if (pdata->noblock) {
		flag |= O_NONBLOCK;
	}

	const char* device = rssconGetDevice(rsscon);
	log_debug(log, "open device '%s' with flag '%d'.", device, flag);
	pdata->fd = open(device, flag);
	if (pdata->fd == -1) {
		rssconSetLastError(rsscon, RSSCON_ERROR_OPENDEVICE, errno);
		log_error(log, "error open device '%s': %s", device, strerrno(errno));
		log_leave(log, "rssconlinuxOpen := true");
		free_log();
		return false;
	}

	log_debug(log, "setup device '%s' with flag '%d'.", device, flag);
	bool ret = setup(rsscon);
	if (!ret) {
		rssconSetLastError(rsscon, RSSCON_ERROR_OPENDEVICE, errno);
		log_error(log, "error setup device '%s': %s", device, strerrno(errno));
		log_leave(log, "rssconlinuxOpen := true");
		free_log();
		return false;
	}

	log_debug(log, "finish creating device '%s'.", device);
	log_leave(log, "rssconlinuxOpen := true");
	free_log();
	return true;
}

bool rssconlinuxClose(Rsscon* rsscon) {
	assert(rsscon != NULL);
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) rsscon->portdata;
	assert(pdata->fd != 0);

	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconlinuxClose");

	rssconSetLastError(rsscon, RSSCON_ERROR_NOERROR, 0);

	tcflush(pdata->fd, TCIOFLUSH);
	tcsetattr(pdata->fd, TCSANOW, &(pdata->oldtio));
	int ret = close(pdata->fd);
	if (ret != 0) {
		rssconSetLastError(rsscon, RSSCON_ERROR_CLOSEDEVICE, errno);
		const char* device = rssconGetDevice(rsscon);
		log_error(log, "Error closing device '%s': %s", device, strerrno(errno));
		log_leave(log, "rssconlinuxClose := false");
		free_log();
		return false;
	}

	pdata->fd = 0;
	log_leave(log, "rssconlinuxClose := true");
	free_log();
	return true;
}

bool rssconlinuxWrite(Rsscon* rsscon, const void* data, size_t length,
		size_t* wrote) {
	assert(rsscon != NULL);
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) rsscon->portdata;
	assert(pdata->fd != 0);

	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconlinuxWrite");

	ssize_t ret = write(pdata->fd, data, length);
	tcdrain(pdata->fd);
	if (ret == -1) {
		rssconSetLastError(rsscon, RSSCON_ERROR_WRITE, errno);
		const char* device = rssconGetDevice(rsscon);
		log_error(log, "error write to device '%s': %s", device, strerrno(errno));
		log_leave(log, "rssconlinuxWrite := false");
		free_log();
		return false;
	}
	*wrote = ret;

	log_leave(log, "rssconlinuxWrite := true");
	free_log();
	return true;
}

bool waittodata(RssconlinuxPortdata* pdata, fd_set* set, struct timeval* tv) {
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
	assert(rsscon != NULL);
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) rsscon->portdata;
	assert(pdata->fd != 0);

	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconlinuxRead");

	struct timeval tv;
	fd_set set;
	ssize_t ret = 0;
	if (pdata->wait) {
		if (!waittodata(pdata, &set, &tv)) {
			rssconSetLastError(rsscon, RSSCON_ERROR_READ, errno);
			const char* device = rssconGetDevice(rsscon);
			log_error(log, "Error wait device '%s': %s", device, strerrno(errno));
			log_leave(log, "rssconlinuxRead := false");
			free_log();
			return false;
		}
	}
	while (ret == 0) {
		if (pdata->wait) {
			if (!waittodata(pdata, &set, &tv)) {
				rssconSetLastError(rsscon, RSSCON_ERROR_READ, errno);
				const char* device = rssconGetDevice(rsscon);
				log_error(log, "Error wait device '%s': %s", device, strerrno(errno));
				log_leave(log, "rssconlinuxRead := false");
				free_log();
				return false;
			}
		}
		ret = read(pdata->fd, data, length);
		if (ret == -1) {
			break;
		}
	}
	if (ret == -1) {
		rssconSetLastError(rsscon, RSSCON_ERROR_READ, errno);
		const char* device = rssconGetDevice(rsscon);
		log_error(log, "Error read from device '%s': %s", device, strerrno(errno));
		log_leave(log, "rssconlinuxRead := false");
		free_log();
		return false;
	}

	*readed = ret;
	log_debug(log, "Read %d data.", ret);
	log_leave(log, "rssconlinuxRead := true");
	free_log();
	return true;
}

bool rssconlinuxSetBlocking(Rsscon* rsscon, bool block) {
	assert(rsscon != NULL);
	assert(rsscon->portdata != NULL);
	if (rssconIsOpen(rsscon)) {
		rssconSetLastError(rsscon, RSSCON_ERROR_DEVICE_OPENED, 0);
		return false;
	}
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) rsscon->portdata;
	pdata->noblock = !block;
	return true;
}

bool rssconlinuxGetBlocking(Rsscon* rsscon) {
	assert(rsscon != NULL);
	assert(rsscon->portdata != NULL);
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) rsscon->portdata;
	return !pdata->noblock;
}

bool rssconlinuxSetWait(Rsscon* rsscon, bool wait) {
	assert(rsscon != NULL);
	assert(rsscon->portdata != NULL);
	if (rssconIsOpen(rsscon)) {
		rssconSetLastError(rsscon, RSSCON_ERROR_DEVICE_OPENED, 0);
		return false;
	}
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) rsscon->portdata;
	pdata->wait = wait;
	return true;
}

bool rssconlinuxGetWait(Rsscon* rsscon) {
	assert(rsscon != NULL);
	assert(rsscon->portdata != NULL);
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) rsscon->portdata;
	return !pdata->wait;
}

bool rssconlinuxSetErrorNumber(Rsscon* rsscon, int errorNumber) {
	assert(rsscon != NULL);
	assert(rsscon->portdata != NULL);
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) rsscon->portdata;
	pdata->errorNumber = errorNumber;
	return true;
}

int rssconlinuxGetErrorNumber(Rsscon* rsscon) {
	assert(rsscon != NULL);
	assert(rsscon->portdata != NULL);
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) rsscon->portdata;
	return pdata->errorNumber;
}

bool rssconInit(Rsscon* rsscon) {
	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconInit");

	RssconlinuxPortdata* portdata = malloc(sizeof(RssconlinuxPortdata));
	rsscon->portdata = portdata;
	rsscon->rssconInit = rssconlinuxInit;
	rsscon->rssconFree = rssconlinuxFree;
	rsscon->rssconOpen = rssconlinuxOpen;
	rsscon->rssconClose = rssconlinuxClose;
	rsscon->rssconWrite = rssconlinuxWrite;
	rsscon->rssconRead = rssconlinuxRead;
	rsscon->rssconSetBlocking = rssconlinuxSetBlocking;
	rsscon->rssconGetBlocking = rssconlinuxGetBlocking;
	rsscon->rssconSetWait = rssconlinuxSetWait;
	rsscon->rssconGetWait = rssconlinuxGetWait;
	rsscon->rssconSetErrorNumber = rssconlinuxSetErrorNumber;
	rsscon->rssconGetErrorNumber = rssconlinuxGetErrorNumber;

	log_leave(log, "rssconInit");
	free_log();
	return rsscon->rssconInit(rsscon);
}

#endif
