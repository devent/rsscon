/*
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
package com.anrisoftware.rsscon.nativeimpl;

import java.io.IOException;

/**
 * The binding to the native rsscon driver library.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
class NativeBinding {

	public native long rssconCreate(String device, int baudrate)
			throws IOException;

	public native void rssconFree(long rsscon) throws IOException;

	public native void rssconInit(long rsscon) throws IOException;

	public native void rssconOpen(long rsscon) throws IOException;

	public native void rssconClose(long rsscon) throws IOException;

	public native boolean rssconIsOpen(long rsscon) throws IOException;

	public native boolean rssconSetBlocking(long rsscon, boolean block)
			throws IOException;

	public native boolean rssconGetBlocking(long rsscon) throws IOException;

	public native boolean rssconSetWait(long rsscon, boolean wait)
			throws IOException;

	public native boolean rssconGetWait(long rsscon) throws IOException;

	public native int rssconWrite(long rsscon, byte[] data, int length)
			throws IOException;

	public native int rssconRead(long rsscon, byte[] data, int length)
			throws IOException;

	public native int rssconGetLastError(long rsscon) throws IOException;

}
