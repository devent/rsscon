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

typedef struct {

	char devicePath[256];

	int lastError;

	bool noblock;

	bool wait;

	long tvsec;

	long tvusec;

	struct t_private {
		int fd;
		struct termios oldtio, newtio;
		ssize_t err;
	} private;

} RssconlinuxPortdata;

static const char *
strerrno(int err) {
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

bool setup(RssconlinuxPortdata* pdata) {
#ifdef RSSCON_LOGING
	printf("%d: setup...\n", __LINE__);
#endif

	struct termios *tio1, *tio2;
	int ret = 0;
	tio1 = &(pdata->private.oldtio);
	tio2 = &(pdata->private.newtio);

	if (tcgetattr(pdata->private.fd, tio1))
		ret = -1;

	memcpy(tio2, tio1, sizeof(struct termios));
	cfmakeraw(tio2);
	cfsetospeed(tio2, RSSCON_BAUD_RATE);
	cfsetispeed(tio2, RSSCON_BAUD_RATE);
#if 0
	tio2->c_cflag |= CRTSCTS;
#endif
	tio2->c_cflag |= CREAD | CLOCAL;
	tcflush(pdata->private.fd, TCIOFLUSH);
	if (tcsetattr(pdata->private.fd, TCSANOW, tio2))
		ret = -1;
#if 0
	tcflow(pdata->private.fd, TCOON); tcflow(pdata->private.fd, TCION);
#endif
	if (ret == -1) {
		pdata->lastError = RSSLINUX_ERROR_SETUPDEVICE;
		return false;
	}
	return true;
}

bool rssconlinuxOpen(void* portdata) {
#ifdef RSSCON_LOGING
	printf("%d: rssconlinuxOpen...\n", __LINE__);
#endif

	assert(portdata != NULL);
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) portdata;
	assert(pdata->devicePath != NULL);

	pdata->lastError = 0;

	int flag = O_RDWR | O_NOCTTY;
	if (pdata->noblock)
		flag |= O_NONBLOCK;

	pdata->private.fd = open(pdata->devicePath, flag);
	if (pdata->private.fd == -1) {
		fprintf(stderr, "error open device: %s\n", strerrno(errno));
		pdata->lastError = RSSLINUX_ERROR_OPENDEVICE;
		return false;
	}

	bool ret = setup(pdata);
	if (!ret) {
		fprintf(stderr, "error setup device.\n");
		return false;
	}

	return true;
}

bool rssconlinuxClose(void* portdata) {
#ifdef RSSCON_LOGING
	printf("%d: rssconlinuxClose...\n", __LINE__);
#endif

	assert(portdata != NULL);
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) portdata;
	assert(pdata->private.fd != 0);

	pdata->lastError = 0;

	tcflush(pdata->private.fd, TCIOFLUSH);
	tcsetattr(pdata->private.fd, TCSANOW, &(pdata->private.oldtio));
	int ret = close(pdata->private.fd);
	if (ret != 0) {
		pdata->lastError = RSSLINUX_ERROR_CLOSEDEVICE;
		return false;
	}

	pdata->private.fd = 0;
	return true;
}

bool rssconlinuxWrite(void* portdata, const void* data, size_t length,
		size_t* writed) {
#ifdef RSSCON_LOGING
	printf("%d: rssconlinuxWrite...\n", __LINE__);
#endif

	assert(portdata != NULL);
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) portdata;
	assert(pdata->private.fd != 0);

	ssize_t ret = write(pdata->private.fd, data, length);
	tcdrain(pdata->private.fd);
	if (ret == -1) {
		pdata->lastError = RSSLINUX_ERROR_WRITE;
		return false;
	}
	*writed = ret;

	return true;
}

bool waittodata(RssconlinuxPortdata* pdata, fd_set* set, struct timeval* tv) {
#ifdef RSSCON_LOGING
	printf("%d: waittodata...\n", __LINE__);
#endif

	FD_ZERO(set);
	FD_SET(pdata->private.fd, set);
	tv->tv_sec = pdata->tvsec;
	tv->tv_usec = pdata->tvusec;
	int err = select(pdata->private.fd + 1, set, NULL, NULL, tv);
	if (err <= 0)
		return false;
	return true;
}

bool rssconlinuxRead(void* portdata, void* data, size_t length, size_t* readed) {
#ifdef RSSCON_LOGING
	printf("%d: rssconlinuxRead...\n", __LINE__);
#endif

	assert(portdata != NULL);
	RssconlinuxPortdata* pdata = (RssconlinuxPortdata*) portdata;
	assert(pdata->private.fd != 0);

	struct timeval tv;
	fd_set set;
	ssize_t ret = 0;
	if (pdata->wait) {
		if (!waittodata(pdata, &set, &tv)) {
			pdata->lastError = RSSLINUX_ERROR_READ;
			fprintf(stderr, "error wait device.\n");
			return false;
		}
	}
	while (ret == 0) {
		if (pdata->wait) {
			if (!waittodata(pdata, &set, &tv)) {
				pdata->lastError = RSSLINUX_ERROR_READ;
				fprintf(stderr, "error wait device.\n");
				return false;
			}
		}
		ret = read(pdata->private.fd, data, length);
		if (ret == -1)
			break;
	}
	if (ret == -1) {
		pdata->lastError = RSSLINUX_ERROR_READ;
		fprintf(stderr, "error read device.\n");
		return false;
	}

	*readed = ret;
	return true;
}

int rssconlinuxLastError(void* portdata) {
#ifdef RSSCON_LOGING
	printf("%d: rssconlinuxLastError...\n", __LINE__);
#endif

	RssconlinuxPortdata* data = (RssconlinuxPortdata*) portdata;
	return data->lastError;
}

bool setupPortdata(RssconlinuxPortdata* portdata) {
#ifdef RSSCON_LOGING
	printf("%d: rssconlinuxSetupPortdata...\n", __LINE__);
#endif
	assert(portdata != NULL);

	portdata->devicePath[0] = '\0';
	portdata->lastError = RSSLINUX_ERROR_NOERROR;
	if (portdata->tvsec < 0)
		portdata->tvsec = 5;
	if (portdata->tvusec < 1)
		portdata->tvusec = portdata->tvsec * 10E6;
	portdata->noblock = false;
	portdata->wait = true;

	return true;
}

bool rssconSetupInterface(Rsscon rsscon) {
#ifdef RSSCON_LOGING
	printf("%d: rssconlinuxSetupInterface...\n", __LINE__);
#endif
	RssconlinuxPortdata portdata;
	rsscon->portdata = &portdata;
	setupPortdata(rsscon->portdata);
	rsscon->rssconOpen = rssconlinuxOpen;
	rsscon->rssconClose = rssconlinuxClose;
	rsscon->rssconWrite = rssconlinuxWrite;
	rsscon->rssconRead = rssconlinuxRead;
	rsscon->rssconLastError = rssconlinuxLastError;
	return true;
}

#endif
