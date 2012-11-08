/*
 * Copyright 2011 Erwin Mueller <erwin.mueller@deventm.org>
 *
 * This file is part of rssconjava-native.
 *
 * rssconjava-native is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * rssconjava-native is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rssconjava-native. If not, see <http://www.gnu.org/licenses/>.
 */
package com.anrisoftware.rsscon.nativeimpl;

import java.io.IOException;
import java.io.InputStream;

import com.anrisoftware.rsscon.api.RssconNative;
import com.google.inject.Inject;
import com.google.inject.assistedinject.Assisted;

/**
 * Implements the {@link RssconInputStream} with the {@link RssconNative}, that
 * is the binding to the native library.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
class RssconInputStream extends InputStream {

	private final RssconNativeHelper rssconNative;

	@Inject
	RssconInputStream(@Assisted RssconNative rssconNative) {
		this.rssconNative = (RssconNativeHelper) rssconNative;
	}

	@Override
	public int read() throws IOException {
		byte[] buffer = new byte[1];
		read(buffer);
		return buffer[0];
	}

	@Override
	public int read(byte[] b) throws IOException {
		return read(b, 0, b.length);
	}

	@Override
	public int read(byte[] b, int off, int len) throws IOException {
		if (!rssconNative.isOpen()) {
			rssconNative.open();
		}
		int bytes = rssconNative.read(b, len);
		return bytes;
	}

	@Override
	public void close() throws IOException {
		rssconNative.close();
	}

	@Override
	public int available() throws IOException {
		return rssconNative.isOpen() ? Integer.MAX_VALUE : 0;
	}
}
