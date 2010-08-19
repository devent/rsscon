#ifndef RSSCONLINUX_H
#define RSSCONLINUX_H

#ifdef LINUX

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "noiseread.h"
#include <termios.h>
#include <sys/types.h>

#define RSSLINUX_ERROR_OPENDEVICE -1
#define RSSLINUX_ERROR_CLOSEDEVICE -2
#define RSSLINUX_ERROR_SETUPDEVICE -3
#define RSSLINUX_ERROR_WRITE -4
#define RSSLINUX_ERROR_READ -5

#ifdef NOISEREAD_3
#define RSSCON_BAUD_RATE B921600
#else
#define RSSCON_BAUD_RATE B115200
#endif


typedef struct
{

    char devicePath[256];

    int lastError;

    bool noblock;

    bool wait;

    long tvsec;

    long tvusec;

    struct t_private
    {
        int        fd;
        struct termios    oldtio, newtio;
        ssize_t    err;
    } private;

} RssconlinuxPortdata;

bool rssconlinuxSetupPortdata(RssconlinuxPortdata* pdata);

bool rssconlinuxSetupInterface(NoiseRead* noiseread);

bool rssconlinuxOpen(void* portdata);

bool rssconlinuxClose(void* portdata);

bool rssconlinuxWrite(void* portdata, const void* data, size_t length, size_t* writed);

bool rssconlinuxRead(void* portdata, void* data, size_t length, size_t* readed);

int rssconlinuxLastError(void* portdata);

#endif

#endif
