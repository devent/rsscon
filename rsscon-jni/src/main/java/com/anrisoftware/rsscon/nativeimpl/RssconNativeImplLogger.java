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

import java.io.ByteArrayOutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;

import com.anrisoftware.globalpom.log.AbstractLogger;

/**
 * Logging messages for {@link RssconInputStream}.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
class RssconNativeImplLogger extends AbstractLogger {

	private static final int MAX_BUFFER_OUTPUT = 48;

	/**
	 * Create logger for {@link RssconInputStream}.
	 */
	RssconNativeImplLogger() {
		super(RssconInputStream.class);
	}

	void closeRsscon(RssconNativeImpl rsscon) {
		log.debug("Close {}.", rsscon);
	}

	void openDevice(RssconNativeImpl rsscon) {
		log.debug("Open {}.", rsscon);
	}

	void writeBuffer(RssconNativeImpl rsscon, byte[] data) {
		if (log.isTraceEnabled()) {
			log.trace("Write buffer ({} bytes) '{}' to {}.", new Object[] {
					data.length, toHexString(data), rsscon });
		}
	}

	private String toHexString(byte[] data) {
		return toHexString(data, data.length);
	}

	void readBuffer(RssconNativeImpl rsscon, byte[] data, int bytes) {
		if (log.isTraceEnabled()) {
			log.trace("Read data ({} bytes) '{}' to {}.", new Object[] { bytes,
					toHexString(data, bytes), rsscon });
		}
	}

	private String toHexString(byte[] array, int bytes) {
		ByteArrayOutputStream out = new ByteArrayOutputStream();
		PrintWriter writer = new PrintWriter(new OutputStreamWriter(out));
		for (int i = 0; i < Math.min(bytes, MAX_BUFFER_OUTPUT); i++) {
			for (int j = 0; j < 3; j++) {
				int index = j + i * 3;
				if (index >= bytes) {
					break;
				}
				writer.printf("%02x", array[index]);
			}
			writer.append(" ");
		}
		if (bytes > MAX_BUFFER_OUTPUT) {
			writer.append("...");
		}
		writer.flush();
		return out.toString().trim();
	}

	void blockingSet(RssconNativeImpl rsscon, boolean block) {
		log.trace("Set blocking to {} for {}.", block, rsscon);
	}

}
