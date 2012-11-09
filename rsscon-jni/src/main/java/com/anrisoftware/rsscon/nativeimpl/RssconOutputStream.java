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

import static java.lang.System.arraycopy;

import java.io.IOException;
import java.io.OutputStream;

import com.anrisoftware.rsscon.api.RssconNative;
import com.google.inject.Inject;
import com.google.inject.assistedinject.Assisted;

/**
 * Implements the {@link RssconOutputStream} with the {@link RssconNative}, that
 * is the binding to the native library.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @version 2011
 */
class RssconOutputStream extends OutputStream {

	private final RssconNativeImpl helper;

	@Inject
	RssconOutputStream(@Assisted RssconNative rssconNative) {
		this.helper = (RssconNativeImpl) rssconNative;
	}

	@Override
	public void write(int b) throws IOException {
		byte[] buffer = new byte[] { (byte) b };
		write(buffer);
	}

	@Override
	public void write(byte[] b) throws IOException {
		write(b, 0, b.length);
	}

	@Override
	public void write(byte[] b, int off, int len) throws IOException {
		if (!helper.isOpen()) {
			helper.open();
		}
		byte[] buffer = new byte[len];
		arraycopy(b, off, buffer, 0, len);
		helper.write(buffer, len);
	}

	@Override
	public void flush() throws IOException {
	}

	@Override
	public void close() throws IOException {
		helper.close();
	}

}
