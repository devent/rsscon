/**
 * Copyright 2012 Erwin Müller <erwin.mueller@deventm.org>
 *
 * This file is part of rsscon-jni-shared.
 *
 * rsscon-jni-shared is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * rsscon-jni-shared is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rsscon-jni-shared. If not, see <http://www.gnu.org/licenses/>.
 */
#include "com_anrisoftware_rsscon_nativeimpl_NativeBinding.h"
#include <rsscon.h>
#include <logger.h>
#include <string.h>

/**
 * The name of java.io.IOException.
 */
#define JNU_CLASS_IOEXCEPTION "java/io/IOException"

/**
 * The name of com.anrisoftware.rsscon.api.RssconIOException.
 */
#define JNU_CLASS_RSSCONIOEXCEPTION "com/anrisoftware/rsscon/api/RssconIOException"

/**
 * Throws the java.io.IOException with a message.
 */
void throwIOException(JNIEnv *env, const char* msg) {
     jclass cls = (*env)->FindClass(env, JNU_CLASS_IOEXCEPTION);
     (*env)->ThrowNew(env, cls, msg);
}

/**
 * Throws the com.anrisoftware.rsscon.api.RssconIOException
 * with the device string.
 */
void throwIOExceptionDevice(JNIEnv *env, const char* format,
		const char* device, int lastError, int errorNumber, const char* error) {
    jclass cls = (*env)->FindClass(env, JNU_CLASS_RSSCONIOEXCEPTION);
    jmethodID ctor = (*env)->GetMethodID(env, cls, "<init>",
    		"(Ljava/lang/String;Ljava/lang/String;IILjava/lang/String;)V");
    jstring javaFormat = (*env)->NewStringUTF(env, (const char *)format);
    jstring javaDevice = (*env)->NewStringUTF(env, (const char *)device);
    jstring javaError = (*env)->NewStringUTF(env, (const char *)error);
    jobject obj = (*env)->NewObject(env, cls, ctor, javaFormat, javaDevice,
    		lastError, errorNumber, javaError);
	(*env)->Throw(env, (jthrowable)obj);
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
    return get_log("com.anrisoftware.rsscon.nativeimpl.RssconNativeImpl");
}

/**
 * Free the logger and throw IOException if the logger was not freed
 * successfully.
 */
void free_log_and_throw(JNIEnv *env) {
    if (!free_log()) {
        throwIOException(env, "free_log() failed.");
    }
}

/**
 * Method long rssconCreate(String device, int baudrate).
 */
JNIEXPORT jlong JNICALL Java_com_anrisoftware_rsscon_nativeimpl_NativeBinding_rssconCreate(
        JNIEnv *env, jobject this, jstring devicestr, jint baudratenumber) {
    LOG4C_CATEGORY log = get_rsscon_log();
    log_enter(log, "rssconCreate");

    int baudrate = translateBaudrate(baudratenumber);

    log_trace(log, "Convert device name to c string.");
    char device[256];
    jsize length = (*env)->GetStringUTFLength(env, devicestr);
    (*env)->GetStringUTFRegion(env, devicestr, 0, length, device);

    log_trace(log, "Create rsscon device '%s' %d.", device, baudrate);
    long rsscon = toJava(rssconCreate(device, baudrate));

    log_leave(log, "rssconCreate := %d", &rsscon);
    free_log_and_throw(env);
    return rsscon;
}

/**
 * Method void rssconFree(long rsscon).
 */
JNIEXPORT void JNICALL Java_com_anrisoftware_rsscon_nativeimpl_NativeBinding_rssconFree(
        JNIEnv *env, jobject this, jlong rssconaddr) {
    Rsscon *rsscon = (Rsscon*) toC(rssconaddr);
    bool ret = rssconFree(rsscon);
    if (!ret) {
    	throwIOExceptionDevice(env,
                "Error free rsscon driver for device %s: %s - %d (%s)",
                rssconGetDevice(rsscon),
                rssconGetLastError(rsscon),
                rssconGetErrorNumber(rsscon),
                rssconGetErrorNumberAsString(rsscon));
    }
}

/**
 * Method void rssconInit(long rsscon).
 */
JNIEXPORT void JNICALL Java_com_anrisoftware_rsscon_nativeimpl_NativeBinding_rssconInit(
        JNIEnv *env, jobject this, jlong rssconaddr) {
    Rsscon *rsscon = (Rsscon*) toC(rssconaddr);
    bool ret = rssconInit(rsscon);
    if (!ret) {
    	throwIOExceptionDevice(env,
                "Error initialize rsscon driver for device %s: %s - %d (%s)",
                rssconGetDevice(rsscon),
                rssconGetLastError(rsscon),
                rssconGetErrorNumber(rsscon),
                rssconGetErrorNumberAsString(rsscon));
    }
}

/**
 * Method void rssconOpen(long rsscon).
 */
JNIEXPORT void JNICALL Java_com_anrisoftware_rsscon_nativeimpl_NativeBinding_rssconOpen(
        JNIEnv *env, jobject this, jlong rssconaddr) {
    Rsscon *rsscon = (Rsscon*) toC(rssconaddr);
    bool ret = rssconOpen(rsscon);
    if (!ret) {
    	throwIOExceptionDevice(env,
                "Error open rsscon driver for device %s: %s - %d (%s)",
                rssconGetDevice(rsscon),
                rssconGetLastError(rsscon),
                rssconGetErrorNumber(rsscon),
                rssconGetErrorNumberAsString(rsscon));
    }
}

/**
 * Method void rssconClose(long rsscon).
 */
JNIEXPORT void JNICALL Java_com_anrisoftware_rsscon_nativeimpl_NativeBinding_rssconClose(
        JNIEnv *env, jobject this, jlong rssconaddr) {
    Rsscon *rsscon = (Rsscon*) toC(rssconaddr);
    bool ret = rssconClose(rsscon);
    if (!ret) {
    	throwIOExceptionDevice(env,
                "Error close rsscon driver for device %s: %s - %d (%s)",
                rssconGetDevice(rsscon),
                rssconGetLastError(rsscon),
                rssconGetErrorNumber(rsscon),
                rssconGetErrorNumberAsString(rsscon));
    }
}

/**
 * Method int rssconWrite(long rsscon, byte[] data, int length).
 */
JNIEXPORT jint JNICALL Java_com_anrisoftware_rsscon_nativeimpl_NativeBinding_rssconWrite(
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
    	throwIOExceptionDevice(env,
                "Error write to rsscon driver for device %s: %s - %d (%s)",
                rssconGetDevice(rsscon),
                rssconGetLastError(rsscon),
                rssconGetErrorNumber(rsscon),
                rssconGetErrorNumberAsString(rsscon));
    }
    return wrote;
}

/**
 * Method int rssconRead(long rsscon, byte[] data, int length).
 */
JNIEXPORT jint JNICALL Java_com_anrisoftware_rsscon_nativeimpl_NativeBinding_rssconRead(
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
    	throwIOExceptionDevice(env,
                "Error read data from rsscon driver for device %s: %s - %d (%s)",
                rssconGetDevice(rsscon),
                rssconGetLastError(rsscon),
                rssconGetErrorNumber(rsscon),
                rssconGetErrorNumberAsString(rsscon));
    }
    return red;
}

/**
 * Method boolean rssconIsOpen(long rsscon).
 */
JNIEXPORT jboolean JNICALL Java_com_anrisoftware_rsscon_nativeimpl_NativeBinding_rssconIsOpen(
        JNIEnv *env, jobject this, jlong rssconaddr) {
    Rsscon *rsscon = (Rsscon*) toC(rssconaddr);
    return rssconIsOpen(rsscon);
}

/**
 * Method boolean rssconSetBlocking(long rsscon, boolean block).
 */
JNIEXPORT jboolean JNICALL Java_com_anrisoftware_rsscon_nativeimpl_NativeBinding_rssconSetBlocking
  (JNIEnv *env, jobject this, jlong rssconaddr, jboolean block) {
    Rsscon *rsscon = (Rsscon*) toC(rssconaddr);
    return rssconSetBlocking(rsscon, block);
}

/**
 * Method boolean rssconGetBlocking(long rsscon).
 */
JNIEXPORT jboolean JNICALL Java_com_anrisoftware_rsscon_nativeimpl_NativeBinding_rssconGetBlocking
  (JNIEnv *env, jobject this, jlong rssconaddr) {
    Rsscon *rsscon = (Rsscon*) toC(rssconaddr);
    return rssconGetBlocking(rsscon);
}

/**
 * Method boolean rssconSetWait(long rsscon, boolean wait).
 */
JNIEXPORT jboolean JNICALL Java_com_anrisoftware_rsscon_nativeimpl_NativeBinding_rssconSetWait
  (JNIEnv *env, jobject this, jlong rssconaddr, jboolean wait) {
    Rsscon *rsscon = (Rsscon*) toC(rssconaddr);
    return rssconSetWait(rsscon, wait);
}

/**
 * Method boolean rssconGetWait(long rsscon).
 */
JNIEXPORT jboolean JNICALL Java_com_anrisoftware_rsscon_nativeimpl_NativeBinding_rssconGetWait
  (JNIEnv *env, jobject this, jlong rssconaddr) {
    Rsscon *rsscon = (Rsscon*) toC(rssconaddr);
    return rssconGetWait(rsscon);
}

/**
 * Method int rssconGetLastError(long rsscon).
 */
JNIEXPORT jint JNICALL Java_com_anrisoftware_rsscon_nativeimpl_NativeBinding_rssconGetLastError
  (JNIEnv *env, jobject this, jlong rssconaddr) {
    Rsscon *rsscon = (Rsscon*) toC(rssconaddr);
    return rssconGetLastError(rsscon);
}
