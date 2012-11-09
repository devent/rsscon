/**
 * Copyright 2012 Erwin Müller <erwin.mueller@deventm.org>
 *
 * This file is part of rsscon-jni.
 *
 * rsscon-jni is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * rsscon-jni is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rsscon-jni. If not, see <http://www.gnu.org/licenses/>.
 */
#include "com_anrisoftware_rsscon_nativeimpl_RssconNativeHelper.h"
#include <rsscon.h>
#include <logger.h>
#include <string.h>

/**
 * The name of java.io.IOException.
 */
#define JNU_CLASS_IOEXCEPTION "java/io/IOException"

/**
 * Throws exception with the specified name.
 * Adopted from http://java.sun.com/docs/books/jni/html/exceptions.html#11202
 */
void JNU_ThrowByName(JNIEnv *env, const char *name, const char *msg) {
    jclass cls = (*env)->FindClass(env, name);
    /* if cls is NULL, an exception has already been thrown */
    if (cls != NULL) {
        (*env)->ThrowNew(env, cls, msg);
    }
    /* free the local ref */
    (*env)->DeleteLocalRef(env, cls);
}

/**
 * Throws IOException with the specified message.
 * Adopted from http://java.sun.com/docs/books/jni/html/exceptions.html#11202
 */
void JNU_ThrowIOException(JNIEnv *env, const char *msg) {
	JNU_ThrowByName(env, JNU_CLASS_IOEXCEPTION, msg);
}

/**
 * Converts the pointer to a Java long value.
 * Adopted from http://forum.java.sun.com/thread.jspa?threadID=657793&messageID=3865562
 */
jlong toJava(void* p) {
    jlong pointer;
    memset(&pointer, 0, sizeof(jlong)); // most likely 'pointer= 0;' works too
    memcpy(&pointer, &p, sizeof(p));
    return pointer;
}

/**
 * Converts a Java long value to a C pointer.
 * Adopted from http://forum.java.sun.com/thread.jspa?threadID=657793&messageID=3865562
 */
void* toC(jlong pointer) {
    void *p;
    memcpy(&p, &pointer, sizeof(p));
    return p;
}

/**
 * Translates the specified baud rate value to the baud rate constant.
 */
int translateBaudrate(int baudratenumber) {
    switch (baudratenumber) {
    case 57600:
        return RSSCON_BAUDRATE_57600;
    case 115200:
        return RSSCON_BAUDRATE_115200;
    case 230400:
        return RSSCON_BAUDRATE_230400;
    case 460800:
        return RSSCON_BAUDRATE_460800;
    case 500000:
        return RSSCON_BAUDRATE_500000;
    case 576000:
        return RSSCON_BAUDRATE_576000;
    case 921600:
        return RSSCON_BAUDRATE_921600;
    case 1000000:
        return RSSCON_BAUDRATE_1000000;
    case 1152000:
        return RSSCON_BAUDRATE_1152000;
    case 1500000:
        return RSSCON_BAUDRATE_1500000;
    case 2000000:
        return RSSCON_BAUDRATE_2000000;
    case 2500000:
        return RSSCON_BAUDRATE_2500000;
    case 3000000:
        return RSSCON_BAUDRATE_3000000;
    case 3500000:
        return RSSCON_BAUDRATE_3500000;
    case 4000000:
        return RSSCON_BAUDRATE_4000000;
    default:
        return -1;
    }
}

/**
 * Returns the Log4c logger for the native helper.
 */
LOG4C_CATEGORY get_rsscon_log() {
    return get_log("com.anrisoftware.rsscon.nativeimpl.RssconNativeHelper");
}

/**
 * Free the logger and throw IOException if the logger was not freed
 * successfully.
 */
void free_log_and_throw(JNIEnv *env) {
    if (!free_log()) {
        JNU_ThrowByName(env, JNU_CLASS_IOEXCEPTION, "free_log() failed.");
    }
}

/**
 * Method long rssconCreate(String device, int baudrate).
 */
JNIEXPORT jlong JNICALL Java_com_anrisoftware_rsscon_nativeimpl_RssconNativeHelper_rssconCreate(
        JNIEnv *env, jobject this, jstring devicestr, jint baudratenumber) {
    LOG4C_CATEGORY log = get_rsscon_log();
    log_enter(log, "enter rssconCreate()");

    int baudrate = translateBaudrate(baudratenumber);

    log_debug(log, "convert java string to c string...");
    char device[256];
    jsize length = (*env)->GetStringUTFLength(env, devicestr);
    (*env)->GetStringUTFRegion(env, devicestr, 0, length, device);

    log_debug(log, "call rssconCreate(%s, %d)", device, baudrate);
    long rsscon = toJava(rssconCreate(device, baudrate));

    log_leave(log, "leave rssconCreate():=%d", &rsscon);
    free_log_and_throw(env);
    return rsscon;
}

/**
 * Method void rssconFree(long rsscon).
 */
JNIEXPORT void JNICALL Java_com_anrisoftware_rsscon_nativeimpl_RssconNativeHelper_rssconFree(
        JNIEnv *env, jobject this, jlong rssconaddr) {
    Rsscon *rsscon = (Rsscon*) toC(rssconaddr);
    bool ret = rssconFree(rsscon);
    if (!ret) {
        JNU_ThrowByName(env, JNU_CLASS_IOEXCEPTION,
                "Could not free the allocated memory of the rsscon driver.");
    }
}

/**
 * Method void rssconInit(long rsscon).
 */
JNIEXPORT void JNICALL Java_com_anrisoftware_rsscon_nativeimpl_RssconNativeHelper_rssconInit(
        JNIEnv *env, jobject this, jlong rssconaddr) {
    Rsscon *rsscon = (Rsscon*) toC(rssconaddr);
    bool ret = rssconInit(rsscon);
    if (!ret) {
        JNU_ThrowByName(env, JNU_CLASS_IOEXCEPTION,
                "Could not setup rsscon driver.");
    }
}

/**
 * Method void rssconOpen(long rsscon).
 */
JNIEXPORT void JNICALL Java_com_anrisoftware_rsscon_nativeimpl_RssconNativeHelper_rssconOpen(
        JNIEnv *env, jobject this, jlong rssconaddr) {
    Rsscon *rsscon = (Rsscon*) toC(rssconaddr);
    bool ret = rssconOpen(rsscon);
    if (!ret) {
        JNU_ThrowByName(env, JNU_CLASS_IOEXCEPTION,
                "Could not open the port for read and write access.");
    }
}

/**
 * Method void rssconClose(long rsscon).
 */
JNIEXPORT void JNICALL Java_com_anrisoftware_rsscon_nativeimpl_RssconNativeHelper_rssconClose(
        JNIEnv *env, jobject this, jlong rssconaddr) {
    Rsscon *rsscon = (Rsscon*) toC(rssconaddr);
    bool ret = rssconClose(rsscon);
    if (!ret) {
        JNU_ThrowByName(env, JNU_CLASS_IOEXCEPTION, "Could not close the port.");
    }
}

/**
 * Method int rssconWrite(long rsscon, byte[] data, int length).
 */
JNIEXPORT jint JNICALL Java_com_anrisoftware_rsscon_nativeimpl_RssconNativeHelper_rssconWrite(
        JNIEnv *env, jobject this, jlong rssconaddr, jbyteArray b, jint len) {
    Rsscon *rsscon = (Rsscon*) toC(rssconaddr);

    jbyte *data = (*env)->GetByteArrayElements(env, b, NULL);
    if (data == NULL) {
        return -1; // exception occurred
    }

    size_t length = len;
    size_t wrote;
    bool ret = rssconWrite(rsscon, data, length, &wrote);
    (*env)->ReleaseByteArrayElements(env, b, data, 0);

    if (!ret) {
        JNU_ThrowByName(env, JNU_CLASS_IOEXCEPTION,
                "Could not write data to the port.");
    }
    return wrote;
}

/**
 * Method int rssconRead(long rsscon, byte[] data, int length).
 */
JNIEXPORT jint JNICALL Java_com_anrisoftware_rsscon_nativeimpl_RssconNativeHelper_rssconRead(
        JNIEnv *env, jobject this, jlong rssconaddr, jbyteArray b, jint len) {
    Rsscon *rsscon = (Rsscon*) toC(rssconaddr);

    jbyte *data = (*env)->GetByteArrayElements(env, b, NULL);
    if (data == NULL) {
        return -1; // exception occurred
    }

    size_t length = len;
    size_t red;
    bool ret = rssconRead(rsscon, data, length, &red);
    (*env)->ReleaseByteArrayElements(env, b, data, 0);

    if (!ret) {
        JNU_ThrowByName(env, JNU_CLASS_IOEXCEPTION,
                "Could not read data to the port.");
    }
    return red;
}

/**
 * Method boolean rssconIsOpen(long rsscon).
 */
JNIEXPORT jboolean JNICALL Java_com_anrisoftware_rsscon_nativeimpl_RssconNativeHelper_rssconIsOpen(
        JNIEnv *env, jobject this, jlong rssconaddr) {
    Rsscon *rsscon = (Rsscon*) toC(rssconaddr);
    return rssconIsOpen(rsscon);
}
